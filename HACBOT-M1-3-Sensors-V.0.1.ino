// HACBOT-M!-3-Sensors-V.0.1

#include <MQ2.h>
#include <dht.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define gasSensor_Pin A3

////////////////////////////////////////////////////////////////////////

LiquidCrystal_I2C lcd(0x27, 16, 2);
MQ2 mq2(gasSensor_Pin);
dht DHT;

void setup()
{
  Serial.begin(9600);
  lcd.begin();

  mq2.begin();
}
 
void loop()
{
  Voltage_Sensor();
  delay(3000);

  MQ2_Sensor();
  delay(3000);

  Temperature_Sensor();
  delay(3000);


}

void Voltage_Sensor()
{
  #define voltageSensor_Pin A0

  float adc_voltage = 0.0;
  float in_voltage = 0.0;
  int adc_value = 0;

  adc_value = analogRead(voltageSensor_Pin);
  adc_voltage  = (adc_value * 5.0) / 1024.0;
  in_voltage = adc_voltage / (7500.0 / (30000.0 + 7500.0)) ; 

  Serial.println();
  Serial.println("---------------------");
  Serial.print("Battery: ");
  Serial.println(in_voltage, 2);
  Serial.println("---------------------");

  // LCD DISPLAY:

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("BATT:");
  lcd.print(in_voltage);
  lcd.print("V");

}

void MQ2_Sensor()
{
  int lpg, co, smoke;
  lpg = mq2.readLPG();
  co = mq2.readCO();
  smoke = mq2.readSmoke();

  Serial.print("LPG  : ");
  Serial.println(lpg);
  Serial.print("CO   : ");
  Serial.println(co);
  Serial.print("SMOKE: ");
  Serial.print((smoke*100)/1000000);
  Serial.print(" %");  

  // LCD DISPLAY:

  // lcd.setCursor(0,0);
  // lcd.print("LPG:");
  // lcd.print(lpg);
  lcd.print(" CO:");
  lcd.print(co);
  // lcd.setCursor(0,1);
  // lcd.print("SMOKE:");
  // lcd.print((smoke*100)/1000000);
  // lcd.print(" %");

}

void Temperature_Sensor()
{
  #define TempSensor_Pin 4
  
  int readData = DHT.read11(TempSensor_Pin);

	float temperature = DHT.temperature;
	float humidity = DHT.humidity;

  Serial.println();
  Serial.println("---------------------");
	Serial.print("Temperature: ");
	Serial.print(temperature);
	Serial.println(" C");
	// Serial.print((t*9.0)/5.0+32.0);
	// Serial.println("°F ");
	Serial.print("Humidity   : ");
	Serial.print(humidity);
	Serial.print("% ");

  // LCD DISPLAY:

  // lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("TEMP:");
  lcd.print(temperature);
  lcd.print("\xDF" "C");

  // lcd.setCursor(0,1);
  // lcd.print("Humid: ");
  // lcd.print(humidity); 
  // lcd.print("%");
}

