/*
  Ctrl for CurrentSensor R3 Shield

  for Cold Storage project
  by Jae
  at CPS Lab. @ Hanyang University with NAVER Labs.
*/

/*
  Texas Instruments INA226 Control sample
  tomozh@gmail.com

  2031.09.03 fix power miscalculation bug
  2013.03.16 1st release
*/

/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <Wire.h>
#include <LiquidCrystal.h>

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
#define NELEMS(arg) (sizeof(arg) / sizeof((arg)[0]))

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
const word  INA226_CAL_VALUE_12V  = 2665;     // INA226 Calibration Register Value
const word  INA226_CAL_VALUE_5V   = 2580;     // INA226 Calibration Register Value

// INA226 Registers
#define INA226_REG_CONGIGURATION_REG            0x00    // Configuration Register (R/W)
#define INA226_REG_SHUNT_VOLTAGE                0x01    // Shunt Voltage (R)
#define INA226_REG_BUS_VOLTAGE                  0x02    // Bus Voltage (R)
#define INA226_REG_POWER                        0x03    // Power (R)
#define INA226_REG_CURRENT                      0x04    // Current (R)
#define INA226_REG_CALIBRATION                  0x05    // Calibration (R/W)
#define INA226_REG_MASK_ENABLE                  0x06    // Mask/Enable (R/W)
#define INA226_REG_ALERT_LIMIT                  0x07    // Alert Limit (R/W)
#define INA226_REG_DIE_ID                       0xFF    // Die ID (R)

// Operating Mode (Mode Settings [2:0])
#define INA226_CONF_MODE_POWER_DOWN             (0<<0)  // Power-Down
#define INA226_CONF_MODE_TRIG_SHUNT_VOLTAGE     (1<<0)  // Shunt Voltage, Triggered
#define INA226_CONF_MODE_TRIG_BUS_VOLTAGE       (2<<0)  // Bus Voltage, Triggered
#define INA226_CONF_MODE_TRIG_SHUNT_AND_BUS     (3<<0)  // Shunt and Bus, Triggered
#define INA226_CONF_MODE_POWER_DOWN2            (4<<0)  // Power-Down
#define INA226_CONF_MODE_CONT_SHUNT_VOLTAGE     (5<<0)  // Shunt Voltage, Continuous
#define INA226_CONF_MODE_CONT_BUS_VOLTAGE       (6<<0)  // Bus Voltage, Continuous
#define INA226_CONF_MODE_CONT_SHUNT_AND_BUS     (7<<0)  // Shunt and Bus, Continuous (default)

// Shunt Voltage Conversion Time (VSH CT Bit Settings [5:3])
#define INA226_CONF_VSH_140uS                   (0<<3)  // 140us
#define INA226_CONF_VSH_204uS                   (1<<3)  // 204us
#define INA226_CONF_VSH_332uS                   (2<<3)  // 332us
#define INA226_CONF_VSH_588uS                   (3<<3)  // 588us
#define INA226_CONF_VSH_1100uS                  (4<<3)  // 1.1ms (default)
#define INA226_CONF_VSH_2116uS                  (5<<3)  // 2.116ms
#define INA226_CONF_VSH_4156uS                  (6<<3)  // 4.156ms
#define INA226_CONF_VSH_8244uS                  (7<<3)  // 8.244ms

// Bus Voltage Conversion Time (VBUS CT Bit Settings [8:6])
#define INA226_CONF_VBUS_140uS                  (0<<6)  // 140us
#define INA226_CONF_VBUS_204uS                  (1<<6)  // 204us
#define INA226_CONF_VBUS_332uS                  (2<<6)  // 332us
#define INA226_CONF_VBUS_588uS                  (3<<6)  // 588us
#define INA226_CONF_VBUS_1100uS                 (4<<6)  // 1.1ms (default)
#define INA226_CONF_VBUS_2116uS                 (5<<6)  // 2.116ms
#define INA226_CONF_VBUS_4156uS                 (6<<6)  // 4.156ms
#define INA226_CONF_VBUS_8244uS                 (7<<6)  // 8.244ms

// Averaging Mode (AVG Bit Settings[11:9])
#define INA226_CONF_AVG_1                       (0<<9)  // 1 (default)
#define INA226_CONF_AVG_4                       (1<<9)  // 4
#define INA226_CONF_AVG_16                      (2<<9)  // 16
#define INA226_CONF_AVG_64                      (3<<9)  // 64
#define INA226_CONF_AVG_128                     (4<<9)  // 128
#define INA226_CONF_AVG_256                     (5<<9)  // 256
#define INA226_CONF_AVG_512                     (6<<9)  // 512
#define INA226_CONF_AVG_1024                    (7<<9)  // 1024

// Reset Bit (RST bit [15])
#define INA226_CONF_RESET_ACTIVE                (1<<15)
#define INA226_CONF_RESET_INACTIVE              (0<<15)

static void writeRegister(int addr, byte reg, word value)
{
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write((value >> 8) & 0xFF);
  Wire.write(value & 0xFF);
  Wire.endTransmission();
}

static void setupINA226Register(void)
{
	int indexINA226;

	for ( indexINA226 = 0; indexINA226 < NUMOF_INA226; indexINA226 += 2 ) {
		// for 12V
		writeRegister(INA226_ADDR[indexINA226], INA226_REG_CONGIGURATION_REG,
	        INA226_CONF_RESET_INACTIVE
	      | INA226_CONF_MODE_CONT_SHUNT_AND_BUS
	      | INA226_CONF_VSH_1100uS
	      | INA226_CONF_VBUS_1100uS
	      | INA226_CONF_AVG_64
	      );
	  writeRegister(INA226_ADDR[indexINA226], INA226_REG_CALIBRATION, INA226_CAL_VALUE_12V);

	  // for 5V
	  writeRegister(INA226_ADDR[indexINA226+1], INA226_REG_CONGIGURATION_REG,
	        INA226_CONF_RESET_INACTIVE
	      | INA226_CONF_MODE_CONT_SHUNT_AND_BUS
	      | INA226_CONF_VSH_1100uS
	      | INA226_CONF_VBUS_1100uS
	      | INA226_CONF_AVG_64
	      );
	  writeRegister(INA226_ADDR[indexINA226+1], INA226_REG_CALIBRATION, INA226_CAL_VALUE_5V);
	}
}

static word readRegister(int addr, byte reg)
{
  word res = 0x0000;

  Wire.beginTransmission(addr);
  Wire.write(reg);

  if(Wire.endTransmission() == 0) {
    if(Wire.requestFrom(addr, 2) >= 2) {
      res = Wire.read() << 8;
      res += Wire.read();
    }
  }

  return res;
}

#define CALCULATE_VOLTAGE(v)				(((long)((short)(v) * 1250L) + (1000/2)) / 1000)		// LSB=1.25mV
#define	CALCULATE_CURRENT(c)				((short)(c))
#define CALCULATE_POWER(p)					(((long)((short)(p) * 25000L) + (1000/2)) / 1000)		// LSB=25mW

//
// for LCD
//
// initialize the library with the numbers of the interface pins
// lcd(RS, Enable, DB4, DB5, DB6, DB7);
LiquidCrystal lcd(PIN_LCD_RS, PIN_LCD_E, PIN_LCD_DB4, PIN_LCD_DB5, PIN_LCD_DB6, PIN_LCD_DB7);

//
// for Switch
//
void setupSwitch() {
  pinMode(PIN_SWITCH0, INPUT);
  pinMode(PIN_SWITCH1, INPUT);
}

//
// for Selection LED
//
void setupSelectionLED() {
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
  case 0:
    digitalWrite(PIN_SELECT0, true);
    break;
  case 1:
    digitalWrite(PIN_SELECT1, true);
    break;
  case 2:
    digitalWrite(PIN_SELECT2, true);
    break;
  case 3:
    digitalWrite(PIN_SELECT3, true);
    break;
  case 4:
    digitalWrite(PIN_SELECT4, true);
    break;
  case 5:
    digitalWrite(PIN_SELECT5, true);
    break;
  }
}

//
// functions
//

void readINA226(long *voltage, short *current, long *power) {
	int indexINA226;

	for ( indexINA226 = 0; indexINA226 < NUMOF_INA226; indexINA226++ ) {
		voltage[indexINA226] = CALCULATE_VOLTAGE(readRegister(INA226_ADDR[indexINA226], INA226_REG_BUS_VOLTAGE));
		current[indexINA226] = CALCULATE_CURRENT(readRegister(INA226_ADDR[indexINA226], INA226_REG_CURRENT));
		power[indexINA226] = CALCULATE_POWER(readRegister(INA226_ADDR[indexINA226], INA226_REG_POWER));
	}
}

void printINA226(long *voltage, short *current, long *power) {
	int indexINA226;
	char buf[64];
	static int selectedINA226;

	// print by Serial Comm.
	for ( indexINA226 = 0; indexINA226 < NUMOF_INA226; indexINA226 += 2) {
  	// Serial.println();
		snprintf(buf, NELEMS(buf)
			, "S%d:%5ldmV%5dmA%5ldmW%5ldmV%5dmA%5ldmW%5ldmW"
			, (indexINA226 >> 1)
			, voltage[indexINA226]
			, current[indexINA226]
			, power[indexINA226]
			, voltage[indexINA226+1]
			, current[indexINA226+1]
			, power[indexINA226+1]
			, power[indexINA226] + power[indexINA226+1]
			);
	}

  // print on LCD.
  if ( digitalRead(PIN_SWITCH0) == HIGH ) {
    selectedINA226--;
    if ( selectedINA226 < 0 ) {
      selectedINA226 = 0;
    }
  } else if ( digitalRead(PIN_SWITCH1) == HIGH ) {
    selectedINA226++;
    if ( selectedINA226 >= (NUMOF_INA226 >> 1) ) {
      selectedINA226 = (NUMOF_INA226 >> 1) - 1;
    }
  }
  selectLED(selectedINA226);

  printToLCD(selectedINA226, power);

  delay(500);
}

void printToLCD(int selectedINA226, long *power) {
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("S");
  lcd.print(selectedINA226);

  lcd.setCursor(3, 0);
  lcd.print("Total");

  lcd.setCursor(9, 0);
  lcd.print(power[selectedINA226] + power[selectedINA226+1]);

  lcd.setCursor(0, 1);
  lcd.print("5V");

  lcd.setCursor(2, 1);
  lcd.print(power[selectedINA226+1]);

  lcd.setCursor(8, 1);
  lcd.print("12V");

  lcd.setCursor(11, 1);
  lcd.print(power[selectedINA226]);
}

void setup() {
  // for INA2226
  Wire.begin();
  // Serial.begin(9600);
  setupINA226Register();

  // for switch
  setupSwitch();

  // for selection LED
  setupSelectionLED();

  // for LCD
  lcd.begin(16, 2);  // set up the LCD's number of columns and rows:
}

void loop() {
	long voltage[NUMOF_INA226];
	short current[NUMOF_INA226];
	long power[NUMOF_INA226];

  readINA226(voltage, current, power);
  printINA226(voltage, current, power);
}
