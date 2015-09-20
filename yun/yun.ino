/*
 * Internet of Disk Manager (IoDM) for Arduino Yun
 *
 * for Cold Storage project
 * by Jaemyoun in CPS Lab. @ Hanyang University
 * powered by NAVER Labs.
 */

#include <Wire.h>
#include <LiquidCrystal.h>
#include <INA226.h>

//
// Port Setting
//
#define PIN_SELECT0       8
#define PIN_SELECT1       9
#define PIN_SELECT2       10
#define PIN_SELECT3       11
#define PIN_SELECT4       12
#define PIN_SELECT5       13

#define PIN_LCD_RS        A0
#define PIN_LCD_E         A1
#define PIN_LCD_DB4       4
#define PIN_LCD_DB5       5
#define PIN_LCD_DB6       6
#define PIN_LCD_DB7       7

#define PIN_LED0          A3
#define PIN_SWITCH0       3
#define PIN_SWITCH1       2


//
// for INA226
//
#define NUMOF_INA226													(5 * 2)
const int   INA226_ADDR_S0_12V    = 0x40;     // INA226 I2C Address (A1=GND, A0=GND)
const int   INA226_ADDR_S0_5V     = 0x41;     // INA226 I2C Address (A1=GND, A0=VS+)
const int   INA226_ADDR_S1_12V    = 0x42;     // INA226 I2C Address (A1=GND, A0=SDA)
const int   INA226_ADDR_S1_5V     = 0x43;     // INA226 I2C Address (A1=GND, A0=SCL)
const int   INA226_ADDR_S2_12V    = 0x44;     // INA226 I2C Address (A1=VS+, A0=GND)
const int   INA226_ADDR_S2_5V     = 0x45;     // INA226 I2C Address (A1=VS+, A0=VS+)
const int   INA226_ADDR_S3_12V    = 0x46;     // INA226 I2C Address (A1=VS+, A0=SDA)
const int   INA226_ADDR_S3_5V     = 0x47;     // INA226 I2C Address (A1=VS+, A0=SCL)
const int   INA226_ADDR_S4_12V    = 0x48;     // INA226 I2C Address (A1=SDA, A0=GND)
const int 	INA226_ADDR_S4_5V     = 0x49;     // INA226 I2C Address (A1=SDA, A0=VS+)
const int 	INA226_ADDR[NUMOF_INA226] = {	INA226_ADDR_S0_12V, INA226_ADDR_S0_5V,
																					INA226_ADDR_S1_12V, INA226_ADDR_S1_5V,
																					INA226_ADDR_S2_12V, INA226_ADDR_S2_5V,
																					INA226_ADDR_S3_12V, INA226_ADDR_S3_5V,
																					INA226_ADDR_S4_12V, INA226_ADDR_S4_5V,};
INA226 ina[NUMOF_INA226];

// TODO: delete this func.
void checkConfig()
{
  Serial.print("Mode:                  ");
  switch (ina[0].getMode())
  {
    case INA226_MODE_POWER_DOWN:      Serial.println("Power-Down"); break;
    case INA226_MODE_SHUNT_TRIG:      Serial.println("Shunt Voltage, Triggered"); break;
    case INA226_MODE_BUS_TRIG:        Serial.println("Bus Voltage, Triggered"); break;
    case INA226_MODE_SHUNT_BUS_TRIG:  Serial.println("Shunt and Bus, Triggered"); break;
    case INA226_MODE_ADC_OFF:         Serial.println("ADC Off"); break;
    case INA226_MODE_SHUNT_CONT:      Serial.println("Shunt Voltage, Continuous"); break;
    case INA226_MODE_BUS_CONT:        Serial.println("Bus Voltage, Continuous"); break;
    case INA226_MODE_SHUNT_BUS_CONT:  Serial.println("Shunt and Bus, Continuous"); break;
    default: Serial.println("unknown");
  }

  Serial.print("Samples average:       ");
  switch (ina[0].getAverages())
  {
    case INA226_AVERAGES_1:           Serial.println("1 sample"); break;
    case INA226_AVERAGES_4:           Serial.println("4 samples"); break;
    case INA226_AVERAGES_16:          Serial.println("16 samples"); break;
    case INA226_AVERAGES_64:          Serial.println("64 samples"); break;
    case INA226_AVERAGES_128:         Serial.println("128 samples"); break;
    case INA226_AVERAGES_256:         Serial.println("256 samples"); break;
    case INA226_AVERAGES_512:         Serial.println("512 samples"); break;
    case INA226_AVERAGES_1024:        Serial.println("1024 samples"); break;
    default: Serial.println("unknown");
  }

  Serial.print("Bus conversion time:   ");
  switch (ina[0].getBusConversionTime())
  {
    case INA226_BUS_CONV_TIME_140US:  Serial.println("140uS"); break;
    case INA226_BUS_CONV_TIME_204US:  Serial.println("204uS"); break;
    case INA226_BUS_CONV_TIME_332US:  Serial.println("332uS"); break;
    case INA226_BUS_CONV_TIME_588US:  Serial.println("558uS"); break;
    case INA226_BUS_CONV_TIME_1100US: Serial.println("1.100ms"); break;
    case INA226_BUS_CONV_TIME_2116US: Serial.println("2.116ms"); break;
    case INA226_BUS_CONV_TIME_4156US: Serial.println("4.156ms"); break;
    case INA226_BUS_CONV_TIME_8244US: Serial.println("8.244ms"); break;
    default: Serial.println("unknown");
  }

  Serial.print("Shunt conversion time: ");
  switch (ina[0].getShuntConversionTime())
  {
    case INA226_SHUNT_CONV_TIME_140US:  Serial.println("140uS"); break;
    case INA226_SHUNT_CONV_TIME_204US:  Serial.println("204uS"); break;
    case INA226_SHUNT_CONV_TIME_332US:  Serial.println("332uS"); break;
    case INA226_SHUNT_CONV_TIME_588US:  Serial.println("558uS"); break;
    case INA226_SHUNT_CONV_TIME_1100US: Serial.println("1.100ms"); break;
    case INA226_SHUNT_CONV_TIME_2116US: Serial.println("2.116ms"); break;
    case INA226_SHUNT_CONV_TIME_4156US: Serial.println("4.156ms"); break;
    case INA226_SHUNT_CONV_TIME_8244US: Serial.println("8.244ms"); break;
    default: Serial.println("unknown");
  }

  Serial.print("Max possible current:  ");
  Serial.print(ina[0].getMaxPossibleCurrent());
  Serial.println(" A");

  Serial.print("Max current:           ");
  Serial.print(ina[0].getMaxCurrent());
  Serial.println(" A");

  Serial.print("Max shunt voltage:     ");
  Serial.print(ina[0].getMaxShuntVoltage());
  Serial.println(" V");

  Serial.print("Max power:             ");
  Serial.print(ina[0].getMaxPower());
  Serial.println(" W");
}


//
// for LCD
//
// initialize the library with the numbers of the interface pins
// lcd(RS, Enable, DB4, DB5, DB6, DB7);
LiquidCrystal lcd(PIN_LCD_RS, PIN_LCD_E, PIN_LCD_DB4, PIN_LCD_DB5, PIN_LCD_DB6, PIN_LCD_DB7);

//
// for Switch
//
#define TIMEOUT_COUNT   10
void setupSwitch() {
  pinMode(PIN_SWITCH0, INPUT);
  pinMode(PIN_SWITCH1, INPUT);
}

//
// for LED
//
void setupLED() {
  pinMode(PIN_SELECT0, OUTPUT);
  pinMode(PIN_SELECT1, OUTPUT);
  pinMode(PIN_SELECT2, OUTPUT);
  pinMode(PIN_SELECT3, OUTPUT);
  pinMode(PIN_SELECT4, OUTPUT);
  pinMode(PIN_SELECT5, OUTPUT);

  digitalWrite(PIN_SELECT0, false);
  digitalWrite(PIN_SELECT1, false);
  digitalWrite(PIN_SELECT2, false);
  digitalWrite(PIN_SELECT3, false);
  digitalWrite(PIN_SELECT4, false);
  digitalWrite(PIN_SELECT5, false);
}

void selectLED(int selected) {
  digitalWrite(PIN_SELECT0, false);
  digitalWrite(PIN_SELECT1, false);
  digitalWrite(PIN_SELECT2, false);
  digitalWrite(PIN_SELECT3, false);
  digitalWrite(PIN_SELECT4, false);
  digitalWrite(PIN_SELECT5, false);

  switch(selected) {
  case 0: digitalWrite(PIN_SELECT0, true); break;
  case 2: digitalWrite(PIN_SELECT1, true); break;
  case 4: digitalWrite(PIN_SELECT2, true); break;
  case 6: digitalWrite(PIN_SELECT3, true); break;
  case 8: digitalWrite(PIN_SELECT4, true); break;
  }
}

void setup()
{
  Serial.begin(115200); // setup Serial
  setupSwitch();  // setup two switchs
  setupLED(); // setup LEDs
  lcd.begin(16, 2);  // setup a LCD

  // setup INA226
  Serial.println("Initialize INA226");
  Serial.println("-----------------------------------------------");

  for ( char idx = 0; idx < NUMOF_INA226; idx++ ) {
    ina[idx].begin(INA226_ADDR[idx]);
    // Configure INA226
    ina[idx].configure(INA226_AVERAGES_1, INA226_BUS_CONV_TIME_1100US, INA226_SHUNT_CONV_TIME_1100US, INA226_MODE_SHUNT_BUS_CONT);
    // Calibrate INA226. Rshunt = 0.01 ohm, Max excepted current = 4A
    ina[idx].calibrate(0.002, 4);
  }

  // Display configuration
  checkConfig();

  Serial.println("-----------------------------------------------");
}

void loop()
{
  char idxINA226;
  float Power5v, Power12v, PowerSum;
  static char selectedINA226;
  static int timeout

  // Selecting an INA226 for printing on the LCD
  if ( digitalRead(PIN_SWITCH0) == HIGH ) {
    selectedINA226 = selectedINA226 <= 0 ? 0 : selectedINA226 - 2;
    timeout = TIMEOUT_COUNT;
  } else if ( digitalRead(PIN_SWITCH1) == HIGH ) {
    selectedINA226 = selectedINA226 >= (NUMOF_INA226 - 2) ? NUMOF_INA226 - 2 : selectedINA226 + 2;
    timeout = TIMEOUT_COUNT;
  } else if ( timeout <= 0 ) {
    selectedINA226 = selectedINA226 >= (NUMOF_INA226 - 2) ? 0 : selectedINA226 + 2;
  } else {
    timeout--;
  }
  selectLED(selectedINA226);

  // Printing all power consumption on the Serial and the LCD
  Serial.print("S");
  for ( idxINA226 = 0; idxINA226 < NUMOF_INA226; idxINA226 += 2 ) {
    Power12v = ina[idxINA226].readBusPower();
    Power5v = ina[idxINA226+1].readBusPower();
    PowerSum = Power5v + Power12v;
    Serial.print(PowerSum, 5);
    Serial.print(",");

    if ( idxINA226 == selectedINA226 ) {
      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print("Sen:"); lcd.print(selectedINA226);
      lcd.print("  Pow:"); lcd.print(PowerSum);

      lcd.setCursor(0, 1);
      lcd.print("5v"); lcd.print(Power5v);
      lcd.print(" 12v"); lcd.print(Power12v);
    }
  }
  // Serial.print("Bus voltage:   ");
  // Serial.print(ina.readBusVoltage(), 5);
  // Serial.println(" V");
  //
  // Serial.print("Bus power:     ");
  // Serial.print(ina.readBusPower(), 5);
  // Serial.println(" W");
  //
  //
  // Serial.print("Shunt voltage: ");
  // Serial.print(ina.readShuntVoltage(), 5);
  // Serial.println(" V");
  //
  // Serial.print("Shunt current: ");
  // Serial.print(ina.readShuntCurrent(), 5);
  // Serial.println(" A");
  Serial.println("");

  delay(1000);
}
