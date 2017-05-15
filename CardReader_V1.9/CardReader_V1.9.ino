/*
Name:    NFC_ReadCard.ino
Created: 9/27/2016 10:59:24
 
********Version 1.5*********
-更改openDoor()命令位置
增加线上数据库二次验证, 刷卡模式由先开门再上传刷卡数据改为先上传验证数据再开门
对网络要求提高,安全性增加,不过同时可能造成部分时间段网络不稳定无法刷卡开门

********Version 1.6*********
-调整openDoor()命令位置
考虑到用户体验的流畅度, 将在成功读卡后首先判断网络状态,如无法连接网络,先进行开门操作。同时可以防止网络故障后无法开门的情况


********Version 1.7*********
-增加刷卡提示音


********Version 1.8*********
-增加网络故障提示音


********Version 1.9*********
-增加一张超级权限卡用户网络故障后开门
-增加服务器故障提示音
*/

#include <ESP8266WiFi.h>
#include <SPI.h>
#include <MFRC522.h>


#define RST_PIN 0 // RST-PIN for RC522 - RFID - SPI - Module GPIO-0 
#define SS_PIN  15  // SDA-PIN for RC522 - RFID - SPI - Module GPIO-15
#define DEBUGMODE 0

#define DoorRelay D2
#define Buzzer D3

const char* ssid = "XinCheJian";  //the wifi network ssid
const char* password = "imakestuff";  //password to ssid
const char* host = "192.168.1.100";   //server ip address
const int httpPort = 80;  //server access address
int cardState = 0;


MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

void setup() {

  Serial.begin(9600); // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card

  //set digital signal pin for the door
  pinMode(DoorRelay, OUTPUT);

  //ver1.7 buzzer
  pinMode(Buzzer, OUTPUT);
  CloseDoor();

  Connect2Wifi();
}

void loop() {
  CloseDoor();

  //prepare the key "xinfab" to the card
  MFRC522::MIFARE_Key newKey;
  newKey.keyByte[0] = 0x78;
  newKey.keyByte[1] = 0x69;
  newKey.keyByte[2] = 0x6e;
  newKey.keyByte[3] = 0x66;
  newKey.keyByte[4] = 0x61;
  newKey.keyByte[5] = 0x62;

  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) {
    delay(50);
    return;
  }
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }

  if (DEBUGMODE) Serial.print(F("Card UID:"));
  if (DEBUGMODE) Serial.println();
  if (DEBUGMODE) Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  if (DEBUGMODE) Serial.println(mfrc522.PICC_GetTypeName(piccType));

  MFRC522::StatusCode status;
  byte sector = 1;
  byte blockAddr = 7;
  byte nameBlock = 4;
  byte idBlock = 5;
  byte staff_name[18];  //16byte + 2byte CRC
  byte staff_id[18];
  String super_staff_id_1 = "";
  String super_staff_id_2 = "";
  String super_staff_id_3 = "";
  byte size = sizeof(staff_name);
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockAddr, &newKey, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    if (DEBUGMODE) Serial.print(F("PCD_Authenticate() use default key failed"));
    if (DEBUGMODE) Serial.println(mfrc522.GetStatusCodeName(status));
    mfrc522.PICC_HaltA(); // Halt PICC
    mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
    return;
  }
  else {
    cardState = 1;
    if (DEBUGMODE) Serial.println(F("PCD_Authenticate() success: "));
  }

  // Show the whole sector as it currently is
  //if (DEBUGMODE) Serial.println(F("Current data in sector:"));
  //mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &newKey, 1);
  //if (DEBUGMODE) Serial.println();

  // Read name data from the block
  //if (DEBUGMODE) Serial.print(F("Reading data from block ")); 
  //if (DEBUGMODE) Serial.print(nameBlock);
  //if (DEBUGMODE) Serial.println(F(" ..."));
  status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(nameBlock, staff_name, &size);
  if (status != MFRC522::STATUS_OK) {
    if (DEBUGMODE) Serial.print(F("MIFARE_Read() failed: "));
    if (DEBUGMODE) Serial.println(mfrc522.GetStatusCodeName(status));
    //cannot read name, don't open the door 
    mfrc522.PICC_HaltA(); // Halt PICC
    mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
    return;
  }

  // Read id data from the block
  //if (DEBUGMODE) Serial.print(F("Reading data from block "));
  //if (DEBUGMODE) Serial.print(idBlock);
  //if (DEBUGMODE) Serial.println(F(" ..."));
  status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(idBlock, staff_id, &size);
  if (status != MFRC522::STATUS_OK) {
    if (DEBUGMODE) Serial.print(F("MIFARE_Read() failed: "));
    if (DEBUGMODE) Serial.println(mfrc522.GetStatusCodeName(status));
    //cannot read id, don't open the door 
    mfrc522.PICC_HaltA(); // Halt PICC
    mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
    return;
  }
  
  delay(200);
  mfrc522.PICC_HaltA(); // Halt PICC
  mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
  
  if (cardState == 1) {    
    //if wifi is dead, reconnect to the wifi
    if (WiFi.status() != WL_CONNECTED) {
      //need to change to multiWifi next version
      Connect2Wifi();
    }

    if (WiFi.status() != WL_CONNECTED) {
      //ver 1.9 super card mode
      String card_id = "";
      for (int i=0;i<7;i++){
        card_id.concat((char)staff_id[i]);     
      }
      if(card_id == super_staff_id_auron || card_id == super_staff_id_alex ||card_id == super_staff_id_roxas){
        OpenDoor();
        if (DEBUGMODE) Serial.println("Super Card Authrization Mode");
        if (DEBUGMODE) Serial.println("Door Open");
      }
      return;
    }
    
    // Use WiFiClient class to create TCP connections
    if (DEBUGMODE) Serial.print("connecting to ");
    if (DEBUGMODE) Serial.println(host);
    WiFiClient client;
    
    if (!client.connect(host, httpPort)) {
      if (DEBUGMODE) Serial.println("connection failed");
      //ver 1.9 super card mode
      String card_id = "";
      for (int i=0;i<7;i++){
        card_id.concat((char)staff_id[i]);     
      }
      if(card_id == super_staff_id_auron || card_id == super_staff_id_alex ||card_id == super_staff_id_roxas){
        OpenDoor();
        if (DEBUGMODE) Serial.println("Super Card Authrization Mode");
        if (DEBUGMODE) Serial.println("Door Open");
      }
 
      warnForNoServer();
       return;
    }

    int i = 0;
    String send_content = "";
    for (i = 0; i<16; i++) {
      if (staff_name[i] != 0x20)
        send_content.concat((char)staff_name[i]);
    }
    send_content.concat("&staff_id=");
    for (i = 0; i<16; i++) {
      if ((staff_id[i] != 0x20) && (staff_id[i] != 0x0d))
        send_content.concat((char)staff_id[i]);
    }
    //0x0d is a mistake input when write the cards

    //create a URI for the request
    String url2 = "/AICP_YARD/attendance.php?staff_name=" + send_content;

    if (DEBUGMODE) Serial.print("Requesting URL: ");
    if (DEBUGMODE) Serial.println(url2);
    String packet = String("GET ") + url2 + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n";
    if (DEBUGMODE) Serial.println(packet);

    // This will send the request to the server
    client.print(packet);

    //wait for response and set time out
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 3000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }

    while (client.available()) {
      String line = client.readStringUntil('\n');
      if (DEBUGMODE) Serial.println(line);
      if (line == "\r") {
        if (DEBUGMODE) Serial.println("headers received");
      }
      if (line.startsWith("state:success")) {
        //if (DEBUGMODE) Serial.println(line);
        if (DEBUGMODE) Serial.println("Receive response successfull!");
            OpenDoor();
        break;
      }
      if (line.startsWith("state:fail")) {   
        warnForNoAuthCard();
        break;
      }
    }
    client.stop();
    if (DEBUGMODE) Serial.println();
    if (DEBUGMODE) Serial.println("closing connection");
    if (DEBUGMODE) Serial.println();
  }
  cardState = 0;
  delay(1000);
   
}

void warnForNoAuthCard() {
        digitalWrite(Buzzer,HIGH);
        delay (100);
        digitalWrite(Buzzer,LOW);
        delay (100);
        digitalWrite(Buzzer,HIGH);
        delay (100);
        digitalWrite(Buzzer,LOW);
        delay (100);
        digitalWrite(Buzzer,HIGH);
        delay (100);
        digitalWrite(Buzzer,LOW);
        delay (100);
        digitalWrite(Buzzer,HIGH);
        delay (300);
        digitalWrite(Buzzer,LOW);  
}

void warnForNoWIFI() {
        digitalWrite(Buzzer,HIGH);
        delay (2000);  
        digitalWrite(Buzzer,LOW);
        delay (1000);
        digitalWrite(Buzzer,HIGH);
        delay (2000);  
        digitalWrite(Buzzer,LOW);
}

void warnForNoServer() {
        digitalWrite(Buzzer,HIGH);
        delay (1000); 
        digitalWrite(Buzzer,LOW);
        delay (200);  
        digitalWrite(Buzzer,HIGH);
        delay (200);
        digitalWrite(Buzzer,LOW);
        delay (1000); 
        digitalWrite(Buzzer,HIGH);
        delay (1000); 
        digitalWrite(Buzzer,LOW);
        delay (200);  
        digitalWrite(Buzzer,HIGH);
        delay (200); 
        digitalWrite(Buzzer,LOW);       
}

void OpenDoor()
{
  digitalWrite(Buzzer,HIGH);  
  digitalWrite(DoorRelay, LOW);
  delay (350);
  digitalWrite(Buzzer,LOW);
  
}

void CloseDoor()
{
  digitalWrite(DoorRelay, HIGH);
}

void Connect2Wifi()
{
  if (DEBUGMODE) Serial.print("Connecting to ");
  if (DEBUGMODE) Serial.println(ssid);
  WiFi.begin(ssid, password);

  int timeLimit = 0;
  while (WiFi.status() != WL_CONNECTED && timeLimit < 6000) {
    delay(500);
    timeLimit += 500;
    if (DEBUGMODE) Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED){
   if (DEBUGMODE) Serial.println("WiFi connected");
   if (DEBUGMODE) Serial.println("IP address: ");
   if (DEBUGMODE) Serial.println(WiFi.localIP());
  }

  if (WiFi.status() != WL_CONNECTED){
    Serial.println("Wifi connection failed.");
    warnForNoWIFI();
  }
}

