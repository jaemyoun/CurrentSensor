/* git test
 * Internet of Disk Manager (IoDM) for Arduino Yun
 *
 * for Cold Storage project
 * by Jaemyoun in CPS Lab. @ Hanyang University
 * powered by NAVER Labs.
 */

#include <Wire.h>
#include <LiquidCrystal.h>
#include <Parse.h>
#include <Bridge.h>
#include <assert.h>
#include "INA226.h"

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
#define NUMOF_INA226					(5 * 2)
#define INA226_ADDR_S0_12V    (0x40)     // INA226 I2C Address (A1=GND, A0=GND)
#define INA226_ADDR_S0_5V     (0x41)     // INA226 I2C Address (A1=GND, A0=VS+)
#define INA226_ADDR_S1_12V    (0x42)     // INA226 I2C Address (A1=GND, A0=SDA)
#define INA226_ADDR_S1_5V     (0x43)     // INA226 I2C Address (A1=GND, A0=SCL)
#define INA226_ADDR_S2_12V    (0x44)     // INA226 I2C Address (A1=VS+, A0=GND)
#define INA226_ADDR_S2_5V     (0x45)     // INA226 I2C Address (A1=VS+, A0=VS+)
#define INA226_ADDR_S3_12V    (0x46)     // INA226 I2C Address (A1=VS+, A0=SDA)
#define INA226_ADDR_S3_5V     (0x47)     // INA226 I2C Address (A1=VS+, A0=SCL)
#define INA226_ADDR_S4_12V    (0x48)     // INA226 I2C Address (A1=SDA, A0=GND)
#define INA226_ADDR_S4_5V     (0x49)     // INA226 I2C Address (A1=SDA, A0=VS+)
const int 	INA226_ADDR[NUMOF_INA226] = {	INA226_ADDR_S0_12V, INA226_ADDR_S0_5V,
																					INA226_ADDR_S1_12V, INA226_ADDR_S1_5V,
																					INA226_ADDR_S2_12V, INA226_ADDR_S2_5V,
																					INA226_ADDR_S3_12V, INA226_ADDR_S3_5V,
																					INA226_ADDR_S4_12V, INA226_ADDR_S4_5V,};
INA226 ina[NUMOF_INA226];

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

//
// for Parse
//
void sendToParse(char *str) {
  ParseObjectCreate create;
  create.setClassName("IoDM");
  create.add("SID", "BULLH");
  create.addJSONValue("consumption", str);
  // create.addJSONValue("arrayField", "[30,\"s\"]");
  // create.addJSONValue("emptyField", "null");
  ParseResponse createResponse = create.send();
  assert(createResponse.getErrorCode() == 0);
  createResponse.close();
}

void setup()
{
	Bridge.begin();
  Serial.begin(115200); // setup Serial
	while (!Serial);
  setupSwitch();  // setup two switchs
  setupLED(); // setup LEDs
  lcd.begin(16, 2);  // setup a LCD

  // Initialize Parse
  Serial.println("Initialize Parse...");
  Parse.begin("6ZMrA6DGjcAHMIOFotmvexa1RenOOU3FVhzJu7kD", "RM4fO7CFGOXnETRpZosu5NNfFk1COwx6bG2yeSZt");
	Serial.println(Parse.getInstallationId());
  Serial.println(Parse.getSessionToken());
  if(Parse.startPushService()) {
    Serial.println("\nParse push started\n");
  }

  // setup INA226
  Serial.println("Initialize INA226...");
  for ( char idx = 0; idx < NUMOF_INA226; idx++ ) {
		Serial.print("INA226 "); Serial.print((int)idx); Serial.println(" Configure...");
    ina[idx].begin(INA226_ADDR[idx]);
    ina[idx].configure(INA226_AVERAGES_1, INA226_BUS_CONV_TIME_1100US, INA226_SHUNT_CONV_TIME_1100US, INA226_MODE_SHUNT_BUS_CONT);
    ina[idx].calibrate(0.002, 4);    // Calibrate INA226. Rshunt = 0.01 ohm, Max excepted current = 4A
  }
}

void loop()
{
  char idxINA226;
  float Power5v, Power12v, PowerSum;
  static char selectedINA226;
  static int timeout;
	String parseObjectValue = "[";
	char parseObjectChar[100];

	memset(parseObjectChar, NULL, 100);

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
  for ( idxINA226 = 0; idxINA226 < NUMOF_INA226; idxINA226 += 2 ) {
    Power12v = ina[idxINA226].readBusPower();
    Power5v = ina[idxINA226+1].readBusPower();
    PowerSum = Power5v + Power12v;

		parseObjectValue.concat(PowerSum);
		parseObjectValue.concat(",");

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

	parseObjectValue.toCharArray(parseObjectChar, parseObjectValue.length());
	parseObjectChar[parseObjectValue.length() - 1] = ']';
	Serial.println(parseObjectChar);
	sendToParse(parseObjectChar);
  delay(1000);
}
