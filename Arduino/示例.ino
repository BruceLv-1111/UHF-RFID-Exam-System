//#include <deprecated.h>             // RFID
#include <MFRC522.h> //IC卡
//#include <MFRC522Extended.h>
//#include <require_cpp11.h>

#include <Wire.h>                  // I2C
#include <LiquidCrystal_PCF8574.h> // I2C液晶屏

#include <SoftwareSerial.h>

#define BYTE unsigned char
#define WORD unsigned short

/// LED /////////////////////////////////////////////////////////////////////////////////
#define PIN_LED_R 11 // 支持  PWM
#define PIN_LED_G 10 // 支持  PWM
#define PIN_LED_B 9  // 支持  PWM
BYTE gLedR = 255;
BYTE gLedG = 255;
BYTE gLedB = 255;

/// 数字输出信号 /////////////////////////////////////////////////////////////////////////////
#define PIN_DIGIOUT 12 // 最左边的
BYTE gDigiOut = 0;     //

/// 数字输入///////////////////////////////////////////////////////////////////////////////
#define PIN_DIGI_IN0 12
#define PIN_DIGI_IN1 11
#define PIN_DIGI_IN2 10
#define PIN_DIGI_IN3 9
#define PIN_DIGI_IN4 8
#define PIN_DIGI_IN5 7
#define PIN_DIGI_IN6 6
#define PIN_DIGI_IN7 5

/////ADC///////////////////////////////////////////////////////////////////////////
#define PIN_ADC A5
int gADC;

//////////////////////////////////////////////////////////////////////////////////
LiquidCrystal_PCF8574 gLCD(0x27); // 液晶屏对象
BYTE gLcdMsg = 0;                 // LCD 回显的信息内容

//////////////////////////////////////////////////////////////////////////////////
#define PIN_RFID_SS 10
#define PIN_RFID_RST 9
MFRC522 mfrc522(PIN_RFID_SS, PIN_RFID_RST); // Create MFRC522 instance.

//////////////////////////////////////////////////////////////////////////////////

// 初始化 ===================================================================================================================
void setup()
{
  randomSeed(analogRead(0));

  // RFID + LCD
  Wire.begin();
  Wire.beginTransmission(0x27);
  Wire.endTransmission();

  gLCD.begin(16, 2); // initialize the lcd
  gLCD.setBacklight(255);
  gLCD.home();
  gLCD.clear();
  gLCD.print("Init LCD...");

  SPI.begin();        // Initiate  SPI bus
  mfrc522.PCD_Init(); // Initiate MFRC522
}

// 主循环 ===============================================================
void loop()
{
  // Digital Out
  pinMode(PIN_DIGIOUT, OUTPUT);

  digitalWrite(PIN_DIGIOUT, LOW);

  // LCD1602
  int Key = 0;
  if (Key < 8)
  {
    switch (Key)
    {
    case 0:
      if (gLcdMsg > 0)
        gLcdMsg--;
      break;
    case 1:
      if (gLcdMsg < 9)
        gLcdMsg++;
      break;
    case 2:
      gLCD.scrollDisplayLeft();
      break;
    case 3:
      gLCD.scrollDisplayRight();
      break;
    case 4:
      gLCD.noBlink();
      break;
    case 5:
      gLCD.blink();
      break;
    case 6:
      gLCD.noCursor();
      break;
    case 7:
      gLCD.cursor();
      break;
    }

    if (Key < 2)
    {
      if (gLcdMsg % 2 == 0)
      {
        gLCD.clear();
        gLCD.setCursor(0, 0);
      }
      else
        gLCD.setCursor(0, 1);

      switch (gLcdMsg)
      {
      case 0:
        gLCD.print("Hello World ....");
        break;
      case 1:
        gLCD.print(" and hey U  !   ");
        break;
      case 2:
        gLCD.print("It's realy great");
        break;
      case 3:
        gLCD.print("2 B with U here.");
        break;
      case 4:
        gLCD.print("U Know that, U&I");
        break;
      case 5:
        gLCD.print("could be friends");
        break;
      case 6:
        gLCD.print(" as long as the ");
        break;
      case 7:
        gLCD.print(" battery is OK .");
        break;
      case 8:
        gLCD.print(" =____________= ");
        break;
      case 9:
        gLCD.print("So,  so long .. ");
        break;
      }
    }
    while (Key == 1)
      ; // 等待变化
  }
  delay(10);

  // RFID
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    gLCD.clear();
    gLCD.setCursor(0, 0);
    gLCD.print("Get an RFID card.");
  }

  if (!mfrc522.PICC_ReadCardSerial())
  {
    return;
  }

  gLCD.clear();
  gLCD.setCursor(0, 0);
  gLCD.print("UID=");
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    gLCD.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    gLCD.print(mfrc522.uid.uidByte[i], HEX);
  }

  {
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    gLCD.setCursor(0, 1);
    gLCD.print("Type= ");
    gLCD.print(mfrc522.PICC_GetTypeName(piccType));
    delay(1000);
  }
}
