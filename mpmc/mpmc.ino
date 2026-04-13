#include <SoftwareSerial.h>

SoftwareSerial sim800(2, 3);
const int LDR_PIN  = 11;
const int BUZZ_PIN = 12;
const char* PHONE  = "+918454080146";

int lowCount       = 0;
bool alarmActive   = false;

void setup() {
  pinMode(LDR_PIN,  INPUT);
  pinMode(BUZZ_PIN, OUTPUT);
  digitalWrite(BUZZ_PIN, LOW);
  Serial.begin(9600);
  sim800.begin(9600);
  delay(3000);
  Serial.println("System ready.");
}

void loop() {
  int beam = digitalRead(LDR_PIN);
  Serial.print("Pin 11: ");
  Serial.println(beam);

  if (!alarmActive) {
    if (beam == HIGH) {             // HIGH = beam broken on this module
      lowCount++;
      Serial.print("Broken count: ");
      Serial.println(lowCount);

      if (lowCount >= 3) {
        alarmActive = true;
        lowCount = 0;
        digitalWrite(BUZZ_PIN, HIGH);
        Serial.println("Intrusion confirmed — buzzer ON, sending SMS...");
        sendSMS("ALERT: Intruder detected! Laser beam broken.");
      }
    } else {
      lowCount = 0;                 // LOW = beam intact, reset count
    }
  }

  if (alarmActive && beam == LOW) { // LOW = beam restored
    alarmActive = false;
    digitalWrite(BUZZ_PIN, LOW);
    Serial.println("Beam restored — system reset.");
  }

  delay(100);
}

void sendSMS(const char* message) {
  Serial.println("Sending SMS...");
  sim800.println("AT");
  delay(500);
  sim800.println("AT+CMGF=1");
  delay(500);
  sim800.print("AT+CMGS=\"");
  sim800.print(PHONE);
  sim800.println("\"");
  delay(500);
  sim800.print(message);
  delay(500);
  sim800.write(26);
  delay(5000);
  Serial.println("SMS sent.");
}