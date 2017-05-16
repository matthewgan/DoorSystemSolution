#include <ESP8266WiFi.h>
#include <SPI.h>
#include <MFRC522.h>

/*
 * PINOUT
 * +--------------------------------+---------------+
 * | WEMOS D1 ESP8266 BOARD         | CONECT TO PIN |
 * +--------------------------------+---------------+
 * | PIN  | FUCTION  | ESP-8266 PIN | RC522 | RELAY |
 * +------+----------+--------------+-------+-------+
 * | 3.3V | POWER    | 3.3V         | 3.3V  |       |
 * +------+----------+--------------+-------+-------+
 * | 5V   | POWER    | 5V           |       | VCC   |
 * +------+----------+--------------+-------+-------+
 * | GND  | GND      | GND          | GND   | GND   |
 * +------+----------+--------------+-------+-------+
 * | D13  | SCK      | GPIO-14      | SCK   |       |
 * +------+----------+--------------+-------+       +
 * | D12  | MISO     | GPIO-12      | MISO  |       |
 * +------+----------+--------------+-------+       +
 * | D11  | MOSI     | GPIO-13      | MOSI  |       |
 * +------+----------+--------------+-------+       +
 * | D10  | SS (SDA) | GPIO-15      | SDA   |       |
 * +------+----------+--------------+-------+       +
 * | D8   | IO       | GPIO-0       | RESET |       |
 * +------+----------+--------------+-------+-------+
 * | D2   | IO       | GPIO-16      |       | IN1   |
 * +------+----------+--------------+-------+-------+
 */
 
#define RST_PIN 0 // RST-PIN for RC522 - RFID - SPI - Module GPIO-0 
#define SS_PIN  15  // SDA-PIN for RC522 - RFID - SPI - Module GPIO-15
#define DEBUGMODE 1

#define DoorRelay D2
#define Buzzer D3

const char* ssid = "XinCheJian";  //the wifi network ssid
const char* password = "imakestuff";  //password to ssid
const char* host = "10.0.10.201";   //server ip address
const int httpPort = 80;  //server access address
const String serverPHPUrl = "/attendance.php";

int cardState = 0;

//the key value is ascii code of string "XCJSHA"(Xinchejian Shanghai)
const byte KeyVal[6] = {0x58,0x43,0x4A,0x53,0x48,0x41};

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

void setup() 
{
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

void loop() 
{
  CloseDoor();

  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) 
  {
    delay(50);
    return;
  }
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) 
  {
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
  byte size = sizeof(staff_name);
  MFRC522::MIFARE_Key newKey;
  for(int i=0;i<6;i++)
  {
      newKey.keyByte[i] = KeyVal[i];
  }
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockAddr, &newKey, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) 
  {
    if (DEBUGMODE) Serial.print(F("PCD_Authenticate() failed"));
    if (DEBUGMODE) Serial.println(mfrc522.GetStatusCodeName(status));
    mfrc522.PICC_HaltA(); // Halt PICC
    mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
    return;
  }
  else 
  {
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
  if (status != MFRC522::STATUS_OK) 
  {
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
  if (status != MFRC522::STATUS_OK) 
  {
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
  
  if (cardState == 1) 
  {    
    //if wifi is dead, reconnect to the wifi
    if (WiFi.status() != WL_CONNECTED) 
    {
      //need to change to multiWifi next version
      Connect2Wifi();
    }

    if (WiFi.status() != WL_CONNECTED) 
    {
      //ver 1.9 super card mode
      String card_id = "";
      for (int i=0;i<7;i++){
        card_id.concat((char)staff_id[i]);     
      }
      String masterCardID01 = "000001";
      String masterCardID02 = "000002";
      String masterCardID03 = "000003";
      if(card_id == masterCardID01 || card_id == masterCardID02 ||card_id == masterCardID03)
      {
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
    
    if (!client.connect(host, httpPort)) 
    {
      if (DEBUGMODE) Serial.println("connection failed");
      //ver 1.9 super card mode
      String card_id = "";
      for (int i=0;i<7;i++)
      {
        card_id.concat((char)staff_id[i]);     
      }
      card_id.concat("");
      Serial.println(card_id);    
      String masterCardID01 = "000001";
      String masterCardID02 = "000002";
      String masterCardID03 = "000003";
      if(card_id == masterCardID01 || card_id == masterCardID02 ||card_id == masterCardID03)
      {
        OpenDoor();
        if (DEBUGMODE) Serial.println("Super Card Authrization Mode");
        if (DEBUGMODE) Serial.println("Door Open");
      }
      Serial.println(masterCardID01);
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
    String url2 = serverPHPUrl + "?staff_name=" + send_content;

    if (DEBUGMODE) Serial.print("Requesting URL: ");
    if (DEBUGMODE) Serial.println(url2);
    String packet = String("GET ") + url2 + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n";
    if (DEBUGMODE) Serial.println(packet);

    // This will send the request to the server
    client.print(packet);

    //wait for response and set time out
    unsigned long timeout = millis();
    while (client.available() == 0) 
    {
      if (millis() - timeout > 3000) 
      {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }

    while (client.available()) 
    {
      String line = client.readStringUntil('\n');
      if (DEBUGMODE) Serial.println(line);
      if (line == "\r") 
      {
        if (DEBUGMODE) Serial.println("headers received");
      }
      if (line.startsWith("state:success")) 
      {
        //if (DEBUGMODE) Serial.println(line);
        if (DEBUGMODE) Serial.println("Receive response successfull!");
            OpenDoor();
        break;
      }
      if (line.startsWith("state:fail")) 
      {   
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

// buzzer notification

//di-di-di-di~
void warnForNoAuthCard() 
{
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

//di~--di~
void warnForNoWIFI() 
{
        digitalWrite(Buzzer,HIGH);
        delay (2000);  
        digitalWrite(Buzzer,LOW);
        delay (1000);
        digitalWrite(Buzzer,HIGH);
        delay (2000);  
        digitalWrite(Buzzer,LOW);
}

//di~-di--di~-di
void warnForNoServer() 
{
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
  while (WiFi.status() != WL_CONNECTED && timeLimit < 6000) 
  {
    delay(500);
    timeLimit += 500;
    if (DEBUGMODE) Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED)
  {
   if (DEBUGMODE) Serial.println("WiFi connected");
   if (DEBUGMODE) Serial.println("IP address: ");
   if (DEBUGMODE) Serial.println(WiFi.localIP());
  }

  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Wifi connection failed.");
    warnForNoWIFI();
  }
}

