#include <Arduino.h>

const int batteryPin = 0 ; // GPIO1 (ADC input)

// Voltage divider resistors
const float R1 = 10000.0; // 10k
const float R2 = 3000.0;  // 2k + 1k
const float calibrationFactor = 1.025;

const int RELAY_PIN = 9;
const int RED_PIN = 6;
const int GREEN_PIN = 7;
const int BLUE_PIN = 8;

void setup() {
  Serial.begin(115200);
  analogReadResolution(12); // 0-4095

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, HIGH); // Start with charging OFF
  setRGB(LOW, LOW, LOW);         // All LEDs OFF
}

void setRGB(bool r, bool g, bool b) {
  digitalWrite(RED_PIN, r);
  digitalWrite(GREEN_PIN, g);
  digitalWrite(BLUE_PIN, b);
}

void loop() {
  int adcVal = analogRead(batteryPin);
  float vOut = (adcVal / 4095.0) * 3.3;
  float voltage = (vOut / (R2 / (R1 + R2))) * calibrationFactor;

  //Serial.print("Battery Voltage: ");
  //Serial.print(voltage, 2);
  //Serial.println(" V");

  if (voltage < 11.0) {
    digitalWrite(RELAY_PIN, LOW);  // Start charging
    setRGB(HIGH, LOW, LOW);        // Red
    //Serial.println("Charging: ON (Battery LOW)");
  } 
  else if (voltage >= 12.24) {
    digitalWrite(RELAY_PIN, HIGH); // Stop charging

    // Glow GREEN for 3000ms
    setRGB(LOW, HIGH, LOW);
    //Serial.println("Charging: OFF (Battery FULL)");
    delay(3000); // wait 3 seconds

    // Then switch to idle color
    //setRGB(HIGH, LOW, HIGH);       // Purple (Idle)
    //Serial.println("Status: Idle after full charge");
  } 
  else {
    setRGB(HIGH, LOW, HIGH);       // Purple
    //Serial.println("Charging: Idle");
  }

  delay(1000);
}
