#include <Arduino.h>

int adcBat = A1;
int adcPSU = A2;
int PWM1 = 5;
int PWM2 = 6;
int dBat = 7;
int dPSU = 8;

int valBat = 0;
int valPSU = 0;

void setup() {
  Serial.begin(9600);
  pinMode(dBat, OUTPUT);
  pinMode(dPSU, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(dBat, LOW);
  digitalWrite(dPSU, LOW);
}

void loop() {
  valBat = analogRead(adcBat);
  valPSU = analogRead(adcPSU);

  if (valBat < 600) {
    digitalWrite(dBat, HIGH);
    analogWrite(PWM1, 150);
  } else {
    digitalWrite(dBat, LOW);
    analogWrite(PWM1, 0);
  }

  if (valPSU < 600) {
    digitalWrite(dBat, HIGH);
    analogWrite(PWM2, 150);
  } else {
    digitalWrite(dBat, LOW);
    analogWrite(PWM2, 0);
  }

  Serial.println("hallo");
}