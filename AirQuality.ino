#include <LiquidCrystal_I2C.h>
#include "MQ135.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

int beep = 0;
int dt = 1000; // for buzzer

int buz = 3;       //buzzer connected to pin 3
int greenled = 8;  //green led connected to pin 8
int redled = 9;

//========== MQ135 Sensor variable
int gas_sensor = A0;  //Sensor pin
float m = -0.353;     //Slope
float c = 0.711;      //Y-Intercept
float R0 = 23.30;     //Sensor Resistance in fresh air from previous code 21.30

//========== MQ7 Sensor variable
int CO_sensor = A1;//Sensor pin
float m1 = -0.67;    //Slope
float c1 = 1.34;     //Y-Intercept
float R01 = 5.80;    //Sensor Resistance 4.80


void setup() {
  Serial.begin(9600);  // PC to Arduino Serial Monitor

  lcd.init();  // initialize the lcd
  lcd.backlight();
  lcd.print(" Air  Quality ");
  lcd.setCursor(0, 1);
  lcd.print(" Monitor System ");
  delay(4000);
  lcd.clear();


  pinMode(buz, OUTPUT);       // buzzer is connected as Output from Arduino
  pinMode(greenled, OUTPUT);  // green led is connected as output from Arduino
  pinMode(redled, OUTPUT);    // red led is connected as output from Arduino
  pinMode(gas_sensor, INPUT);
  pinMode(CO_sensor, INPUT);
}

void loop() {
 //========== MQ135 Sensor variable
  float sensor_volt;          //Define variable for sensor voltage 
  float RS_gas;               //Define variable for sensor resistance  
  float ratio;                //Define variable for ratio
  float sensorValue = analogRead(gas_sensor);   //Read analog values of sensor  
  sensor_volt = sensorValue*(5.0/1024.0);       //Convert analog values to voltage 
  RS_gas = ((5.0*10.0)/sensor_volt)-10.0;       //Get value of RS in a gas
  ratio = RS_gas/R0;                            // Get ratio RS_gas/RS_air
  double ppm_log = (log10(ratio)-c)/m;          //Get ppm value in linear scale according to the the ratio value  
  double ppm = pow(10, ppm_log);                //Convert ppm value to log scale 
  
  Serial.print("Our Air Qualit PPM = ");
  Serial.println(ppm);
  Serial.println(ratio);
  
    
  lcd.setCursor(0,0);             // set cursor of lcd to 1st row and 1st column
  lcd.print("CO2: ");             // print message on lcd
  lcd.print(ppm);                 // print value of MQ135
  
  //========== MQ7 Sensor variable
  float sensor_volt1;             //Define variable for sensor voltage 
  float RS_gas1;                  //Define variable for sensor resistance  
  float ratio1;                   //Define variable for ratio
  float sensorValue1 = analogRead(CO_sensor);   //Read analog values of sensor  
  sensor_volt1 = sensorValue1*(5.0/1024.0);     //Convert analog values to voltage 
  RS_gas1 = ((5.0*10.0)/sensor_volt1)-10.0;     //Get value of RS in a gas
  ratio1 = RS_gas1/R01;                         // Get ratio RS_gas/RS_air
  double ppm_log1 = (log10(ratio1)-c1)/m1;      //Get ppm value in linear scale according to the the ratio value  
  double ppm1 = pow(10, ppm_log1);              //Convert ppm value to log scale 
  
  Serial.print("CO PPM = ");
  Serial.println(ppm1);

  Serial.println(analogRead(gas_sensor));
  Serial.println(analogRead(CO_sensor));
  
  lcd.setCursor(0,1);             // set cursor of lcd to 1st row and 1st column
  lcd.print("CO PPM = ");         // print message on lcd
  lcd.print(ppm1);                // print value of MQ7
  delay(5000);

   //------Check condition for buzzer and LED--------//
    if (ppm >= 10 || ppm1 >= 10) {
      digitalWrite(greenled, LOW);
      digitalWrite(redled, HIGH);
      for(beep; beep < dt ; beep+=1){
        digitalWrite(buz, HIGH);
        delay(1);
        digitalWrite(buz, LOW);
        delay(1);
      }
      beep = 0;
      lcd.setCursor(0,1);
      lcd.clear();
      lcd.print("Polluted Air");
      Serial.println("Alert!!!");
      delay(2000); 
      lcd.clear();
    }
  
    else {
      digitalWrite(greenled, HIGH);
      digitalWrite(redled, LOW);
      digitalWrite(buz, LOW);
      lcd.setCursor(0,1);
      lcd.clear();
      lcd.print("Normal Air");
      Serial.println("Normal");
      delay(2000); 
      lcd.clear();
    }
}