#include <max6675.h>
#include <Servo.h>

// ######## MAX6675 TC pins ########
int thermoCLK = 13;
int thermoCS  = 10;
int thermoDO  = 12;
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

// ######## Motor ########
Servo Motor;
const int motorPin = 9;
const int MAX_FORWARD = 2000;
const int NEUTRAL     = 1500;
const int MAX_REVERSE  = 1000;
const int MAX_RPM = 6000;

// ######## Potentiometer ########
const int potPin = A0;

// ######## Temperature threshold (Celsius) ########
const double TEMP_THRESHOLD = 80.0;

// ######## Button (Pulled down when pressed) ########
const int buttonPin = 2;

// ######## Control flag: true = motor allowed, false = motor locked out ########
bool motorEnabled = true;

void setup() {
  Serial.begin(9600);

  pinMode(buttonPin, INPUT_PULLUP);

  Motor.attach(motorPin, MAX_REVERSE, MAX_FORWARD);
  Motor.writeMicroseconds(NEUTRAL);
  delay(6000);

  Serial.println("System ready");
}

void loop() {
  // ######## Stop command when button pressed ########
  if (digitalRead(buttonPin) == LOW) {
    motorEnabled = false;
  } 

  // ######## Read knob -> target RPM ########
  int potValue = analogRead(potPin);
  int outputValue = map(potValue, 0, 1023, 0, 500);

  // ######## Read temperature ########
  double tempC = thermocouple.readCelsius();

  if (isnan(tempC)) {
    Serial.println("Error: TC NOT CONNECTED");
    motorEnabled = false;
  } else {
    Serial.print("Temp: ");
    Serial.print(tempC, 2);
    Serial.print(" C  /  ");
    Serial.print(tempC * 9.0 / 5.0 + 32.0, 2);
    Serial.println(" °F  /  ");

  // ######## Stop command when overheat ########
    if (tempC >= TEMP_THRESHOLD) {
      motorEnabled = false;
      Serial.println(">>> OVER TEMP: motor disabled");
    }
  }

  // ######## Command motor ########
  if (motorEnabled) {
    Motor.writeMicroseconds(NEUTRAL + outputValue);
  } else {
    Motor.writeMicroseconds(NEUTRAL);
    Serial.println(">>> Motor DISABLED: ");
  }

  delay(10);
}