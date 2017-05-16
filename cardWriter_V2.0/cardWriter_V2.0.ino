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

#define RST_PIN         0           // Configurable, see typical pin layout above
#define SS_PIN          15          // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

byte blockNum = 7;
byte nameBlock = 4;
byte idBlock = 5;

byte staff_name[18];//16byte + 2byte CRC
byte staff_id[18];//16byte + 2byte CRC
byte size = sizeof(staff_name);

//the key value is ascii code of string "XCJSHA"(Xinchejian Shanghai)
const byte KeyVal[6] = {0x58,0x43,0x4A,0x53,0x48,0x41};

void setup() {
        Serial.begin(9600);        // Initialize serial communications with the PC
        while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
        SPI.begin();               // Init SPI bus
        mfrc522.PCD_Init();        // Init MFRC522 card
        Serial.println(F("Read & Write, waitting for a vaild card...."));
}

void loop() 
{
    // Look for new cards
    if ( ! mfrc522.PICC_IsNewCardPresent()) {
            return;
    }
    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial())  return;
    
    Serial.print(F("Card UID:"));    //Dump UID
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
    } 
    Serial.print(F(" PICC type: "));   // Dump PICC type
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    Serial.println(mfrc522.PICC_GetTypeName(piccType));


    byte combuffer[10];
    Serial.setTimeout(20000L) ; 
    Serial.println(F("Type 1 for READ, 2 for Write DFU, 3 for Write XCJ, ending with #"));

    byte len=Serial.readBytesUntil('#', (char *) combuffer, 10) ; // read number from serial
    for (byte i = len; i < 10; i++) combuffer[i] = '0';     // pad with 0
    

    if (combuffer[0]-'0' == 1){
        readCard();
    }

    else if (combuffer[0]-'0' == 2){
        writeCard();
    }

    else if (combuffer[0]-'0' == 3){
        writeOldCard();
    }

    
    Serial.println(" ");
    Serial.println(F("All done. "));
    Serial.println(" ");
    mfrc522.PICC_HaltA(); // Halt PICC
    mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
}

void readCard()
{
    MFRC522::MIFARE_Key newKey;
    for(int i=0;i<6;i++)
    {
        newKey.keyByte[i] = KeyVal[i];
    }
 
    MFRC522::StatusCode status;
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &newKey, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) 
    {
        Serial.print(F("Wrong key      "));
        Serial.println(mfrc522.GetStatusCodeName(status));   
    }
    else 
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
            String send_content = "staff_name=";
            for (i = 0; i<16; i++) 
            {
                if (staff_name[i] != 0x20)
                    send_content.concat((char)staff_name[i]);
            }
            send_content.concat("     staff_id=");
            for (i = 0; i<16; i++) 
            {
                if ((staff_id[i] != 0x20) && (staff_id[i] != 0x0d))
                    send_content.concat((char)staff_id[i]);
            }
            Serial.println(send_content);
        }
    }
}

void writeCard()
{
    MFRC522::MIFARE_Key key;
    for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
    MFRC522::StatusCode status;
    //new keyA set to "XCJSHA" in HEX
    byte newKey[16] = {KeyVal[0],KeyVal[1],KeyVal[2],KeyVal[3],KeyVal[4],KeyVal[5],0xFF,0x07,0x80,0x69,0x10,0x10,0x10,0x10,0x10,0x10};
    
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() use default key failed with key"));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;           
    }
    else {
        Serial.println(F("PCD_Authenticate() success: "));
    }

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

    //write personal name to sector 1 block 0
    delay (100);
    //the key is changed after write new key
    MFRC522::MIFARE_Key newKeyWritten;
    for(int i=0;i<6;i++)
    {
        newKeyWritten.keyByte[i] = KeyVal[i];
    }
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &newKeyWritten, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) 
    {
        Serial.print(F("PCD_Authenticate() use default key failed"));
        Serial.println(mfrc522.GetStatusCodeName(status));       
        return;
    }
    else 
    {
        Serial.println(F("PCD_Authenticate() success: "));
    }
    byte combuffer[16];
    Serial.setTimeout(200000L) ; 
    Serial.println(F("Type person name less than 16 chars, ending with #"));
    byte len=Serial.readBytesUntil('#', (char *) combuffer, 16) ; // read name from serial
    for (byte i = len; i < 16; i++) combuffer[i] = ' ';     // pad with spaces
    
    status = mfrc522.MIFARE_Write(nameBlock, combuffer, 16);
    if (status != MFRC522::STATUS_OK) 
    {
        Serial.print(F("MIFARE_Write() name failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }
    else 
    {
        Serial.println(F("MIFARE_Write() name success. "));
    }
    
    //write personal id to sector 1 block 1
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &newKeyWritten, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) 
    {
        Serial.print(F("PCD_Authenticate() use default key failed"));
        Serial.println(mfrc522.GetStatusCodeName(status));       
        return;
    }
    else 
    {
        Serial.println(F("PCD_Authenticate() success: "));
    }
    //byte combuffer[16];
    Serial.setTimeout(200000L) ; 
    Serial.println(F("Type person id less than 16 digits, ending with #"));
    len=Serial.readBytesUntil('#', (char *) combuffer, 16) ; // read id from serial
    for (byte i = len; i < 16; i++) combuffer[i] = ' ';     // pad with spaces
    
    status = mfrc522.MIFARE_Write(idBlock, combuffer, 16);
    if (status != MFRC522::STATUS_OK) 
    {
        Serial.print(F("MIFARE_Write() id failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }
    else 
    {
        Serial.println(F("MIFARE_Write() id success. "));
    }
}

void writeOldCard()
{
    //for cards that already write keyval before
    MFRC522::MIFARE_Key newKey;
    for(int i=0;i<6;i++)
    {
        newKey.keyByte[i] = KeyVal[i];
    }
    MFRC522::StatusCode status;
    //new keyA set to "XCJSHA" in HEX
    byte keyA[16] = {KeyVal[0],KeyVal[1],KeyVal[2],KeyVal[3],KeyVal[4],KeyVal[5],0xFF,0x07,0x80,0x69,0x10,0x10,0x10,0x10,0x10,0x10};
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &newKey, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) 
    {
        Serial.print(F("PCD_Authenticate() use default key failed with key"));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;           
    }
    else 
    {
        Serial.println(F("PCD_Authenticate() success: "));
    }

    // Write block
    status = mfrc522.MIFARE_Write(blockNum, keyA, 16);
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

    //write personal name to sector 1 block 0
    delay (100);
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &newKey, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) 
    {
        Serial.print(F("PCD_Authenticate() use default key failed"));
        Serial.println(mfrc522.GetStatusCodeName(status));       
        return;
    }
    else 
    {
        Serial.println(F("PCD_Authenticate() success: "));
    }
    byte combuffer[16];
    Serial.setTimeout(200000L) ; 
    Serial.println(F("Type person name less than 16 chars, ending with #"));
    byte len=Serial.readBytesUntil('#', (char *) combuffer, 16) ; // read name from serial
    for (byte i = len; i < 16; i++) combuffer[i] = ' ';     // pad with spaces
    
    status = mfrc522.MIFARE_Write(nameBlock, combuffer, 16);
    if (status != MFRC522::STATUS_OK) 
    {
        Serial.print(F("MIFARE_Write() name failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }
    else 
    {
        Serial.println(F("MIFARE_Write() name success. "));
    }
    
    //write personal id to sector 1 block 1
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &newKey, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) 
    {
        Serial.print(F("PCD_Authenticate() use default key failed"));
        Serial.println(mfrc522.GetStatusCodeName(status));       
        return;
    }
    else 
    {
        Serial.println(F("PCD_Authenticate() success: "));
    }
    //byte combuffer[16];
    Serial.setTimeout(200000L) ; 
    Serial.println(F("Type person id less than 16 chars, ending with #"));
    len=Serial.readBytesUntil('#', (char *) combuffer, 16) ; // read id from serial
    for (byte i = len; i < 16; i++) combuffer[i] = ' ';     // pad with spaces
    
    status = mfrc522.MIFARE_Write(idBlock, combuffer, 16);
    if (status != MFRC522::STATUS_OK) 
    {
        Serial.print(F("MIFARE_Write() id failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }
    else 
    {
        Serial.println(F("MIFARE_Write() id success. "));
    }
}


