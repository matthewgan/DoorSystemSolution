#include <SPI.h>
#include <MFRC522.h>


#define RST_PIN         0           // Configurable, see typical pin layout above
#define SS_PIN          15          // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

//block number to save the data
byte blockNum = 7;
byte nameBlock = 4;
byte idBlock = 5;
byte staff_name[18];//16byte + 2byte CRC
byte staff_id[18];//16byte + 2byte CRC
byte size = sizeof(staff_name);

String serialString = "";

//the key value is ascii code of string "XCJSHA"(Xinchejian Shanghai)
const byte KeyVal[6] = {0x58, 0x43, 0x4A, 0x53, 0x48, 0x41};
MFRC522::MIFARE_Key newKey;
MFRC522::MIFARE_Key factoryKey;
MFRC522::StatusCode status;



void setup() {

  Serial.begin(9600);        // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();               // Init SPI bus
  mfrc522.PCD_Init();        // Init MFRC522 card
}

void loop() {
  serialString = "";
  // put your main code here, to run repeatedly:
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())  return;
  Serial.println("Card Found");//****************************PC COMMAND
  //Dump UID
  Serial.print("UID:");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println("");
  Serial.print("TYPE:");   // Dump PICC type
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.print(mfrc522.PICC_GetTypeName(piccType));
  Serial.println("");

  if (authenticateWithFactoryKey())
  {
     Serial.println("New factory card");//****************************PC COMMAND
     byte combuffer[10];
     Serial.setTimeout(200000L) ; 
     serialString = Serial.readStringUntil('-');
     serialString.trim();
     byte len=Serial.readBytesUntil('#', (char *) combuffer, 10) ; // read number from serial
     for (byte i = len; i < 10; i++) combuffer[i] = '0';     // pad with 0
     if (combuffer[0]-'0' == 1)
     {
        writeNewKeyToCard();
        Serial.println("Fomat factory card success!");
     }
     else if (combuffer[0]-'0' == 2)
     {
        Serial.println("Process Abort, reset reder");
        resetReaderState();
     }      
  }
  else
  {
    resetReaderState();
    delay(100);
    mfrc522.PCD_Init();
    if ( ! mfrc522.PICC_IsNewCardPresent()) return;
    if ( ! mfrc522.PICC_ReadCardSerial())  return;
    
    if (authenticateWithNewKey())
    { 
      readCard();
      Serial.setTimeout(200000L) ; 
      serialString = Serial.readStringUntil('?');
      serialString.trim();
      serialString = Serial.readStringUntil('#');
      Serial.println(serialString);
      if (serialString == "START WRITE")
      {
        writeInfoToCard();
      }
      else if (serialString == "RESET")
      {
        delay(200);
        Serial.println("Reset card state.");
        while (Serial.available())
        {
          Serial.read();
        }
      }
    }    
    else
    {
      Serial.println("UNKONW CARD!");
      resetReaderState();
      delay(2000);
    }
  }
  resetReaderState();
}


bool authenticateWithNewKey()
{
  for (int i = 0; i < 6; i++)
  {
    newKey.keyByte[i] = KeyVal[i];
  }
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &newKey, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK)
  {
    Serial.println(F("Authenticate XCJKey Faild"));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return false;
  }
  else
  {
    Serial.println(F("Authenticate XCJKey Sucess"));
    return true;
  }
}

bool authenticateWithFactoryKey()
{
  for (byte i = 0; i < 6; i++) factoryKey.keyByte[i] = 0xFF;
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &factoryKey, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK)
  {
    Serial.println(F("Authenticate FactoryKey Faild"));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return false;
  }
  else
  {
    Serial.println(F("Authenticate FactoryKey Sucess"));
    return true;
  }
}

void resetReaderState()
{
  mfrc522.PICC_HaltA(); // Halt PICC
  mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
}

void writeNewKeyToCard()
{
  //new keyA set to "XCJSHA" in HEX
  byte newKey[16] = {KeyVal[0], KeyVal[1], KeyVal[2], KeyVal[3], KeyVal[4], KeyVal[5], 0xFF, 0x07, 0x80, 0x69, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10};

  // Write block
  status = mfrc522.MIFARE_Write(blockNum, newKey, 16);
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print(F("MIFARE_Write() KEY failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else
  {
    Serial.println(F("MIFARE_Write() KEY success. "));
  }
}

void readCard()
{
    Serial.println(F("An XCJ card found"));
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(nameBlock, staff_name, &size);
    (MFRC522::StatusCode) mfrc522.MIFARE_Read(idBlock, staff_id, &size);
    if (status != MFRC522::STATUS_OK)
    {
      Serial.print(F("Can not read name "));
      Serial.println(mfrc522.GetStatusCodeName(status));
    }
    else
    {
      int i = 0;
      String send_content_name = "STAFF_NAME=";
      for (i = 0; i < 16; i++)
      {
        if (staff_name[i] != 0x20)
          send_content_name.concat((char)staff_name[i]);
      }
      Serial.println(send_content_name);

      String send_content_id = "STAFF_ID=";
      for (i = 0; i < 16; i++)
      {
        if ((staff_id[i] != 0x20) && (staff_id[i] != 0x0d))
          send_content_id.concat((char)staff_id[i]);
      }
      Serial.println(send_content_id);
    }
}

void writeInfoToCard()
{
    serialString = "";
    Serial.println(serialString+"START TO WRITE");
    byte combufferName[16];
    byte combufferID[16];
    Serial.setTimeout(200000L) ;
    serialString = Serial.readStringUntil('#');
    serialString.trim();
    Serial.println(serialString);
    if (serialString == "?RESET")
    {
      delay(200);
      Serial.println("Reset card state.");
      while (Serial.available())
      {
        Serial.read();
      }
      return;
    }   
    Serial.println(serialString+"%%%%%%%%%%%%");
    byte lenName=Serial.readBytesUntil('&', (char *) combufferName, 16) ; // read name from serial
    for (byte i = lenName; i < 16; i++) combufferName[i] = ' ';     // pad with spaces
    status = mfrc522.MIFARE_Write(nameBlock, combufferName, 16);
        if (status != MFRC522::STATUS_OK) {
            Serial.print(F("MIFARE_Write() name failed: "));
            Serial.println(mfrc522.GetStatusCodeName(status));
            return;
        }
        else Serial.println(F("MIFARE_Write() name success. "));

        serialString = Serial.readStringUntil('-');
        serialString.trim();
        Serial.println(serialString);
        byte lenId=Serial.readBytesUntil('+', (char *) combufferID, 16) ; // read name from serial
        for (byte i = lenId; i < 16; i++) combufferID[i] = ' ';     // pad with spaces
        status = mfrc522.MIFARE_Write(idBlock, combufferID, 16);
        if (status != MFRC522::STATUS_OK) {
            Serial.print(F("MIFARE_Write() id failed: "));
            Serial.println(mfrc522.GetStatusCodeName(status));
            return;
        }
        else Serial.println(F("MIFARE_Write() id success. "));
}

/*
bool cardTest()
{  
     if ( ! mfrc522.PICC_IsNewCardPresent()) {
       return false;
     }
     if ( ! mfrc522.PICC_ReadCardSerial())  
     {
      Serial.println("CARD REMOVED");
      while (Serial.available())
      {
        Serial.read();
      }
      resetReaderState();
      return false;
     }
     return true;
}*/

