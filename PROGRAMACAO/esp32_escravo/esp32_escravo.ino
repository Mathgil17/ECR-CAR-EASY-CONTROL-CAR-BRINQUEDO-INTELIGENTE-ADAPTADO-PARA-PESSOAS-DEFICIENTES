#include <WiFi.h>

const char *ssid = "ESP32_AP";         
const char *password = "12345678";     

WiFiServer server(80);                 

const int MotorL1_pin1 = 27;
const int MotorL1_pin2 = 14;
const int MotorR1_pin1 = 2;
const int MotorR1_pin2 = 4;
const int MotorL2_pin1 = 13; 
const int MotorL2_pin2 = 12;
const int MotorR2_pin1 = 5;
const int MotorR2_pin2 = 18;

const int trig1 = 25;
const int echo1 = 26;
const int trig2 = 33;
const int echo2 = 32;
const int trig3 = 23;
const int echo3 = 22;

int distance1;
int distance2;
int distance3;
long duration;

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  Serial.print("Iniciando Access Point... IP: ");
  Serial.println(WiFi.softAPIP());

  server.begin();
  
  pinMode(MotorL1_pin1, OUTPUT);
  pinMode(MotorL1_pin2, OUTPUT);
  pinMode(MotorR1_pin1, OUTPUT);
  pinMode(MotorR1_pin2, OUTPUT);
  pinMode(MotorL2_pin1, OUTPUT);
  pinMode(MotorL2_pin2, OUTPUT);
  pinMode(MotorR2_pin1, OUTPUT);
  pinMode(MotorR2_pin2, OUTPUT);

  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);
  pinMode(trig3, OUTPUT);
  pinMode(echo3, INPUT);
}

void Stop() {
  digitalWrite(MotorL1_pin1, LOW);
  digitalWrite(MotorL1_pin2, LOW);
  digitalWrite(MotorR1_pin1, LOW);
  digitalWrite(MotorR1_pin2, LOW);
  digitalWrite(MotorL2_pin1, LOW);
  digitalWrite(MotorL2_pin2, LOW);
  digitalWrite(MotorR2_pin1, LOW);
  digitalWrite(MotorR2_pin2, LOW);
}

void Forward() {
  digitalWrite(MotorL1_pin1, HIGH);
  digitalWrite(MotorL1_pin2, LOW);
  digitalWrite(MotorR1_pin1, HIGH);
  digitalWrite(MotorR1_pin2, LOW);
  digitalWrite(MotorL2_pin1, HIGH);
  digitalWrite(MotorL2_pin2, LOW);
  digitalWrite(MotorR2_pin1, HIGH);
  digitalWrite(MotorR2_pin2, LOW);
}

void Left() {
  digitalWrite(MotorL1_pin1, LOW);
  digitalWrite(MotorL1_pin2, HIGH);
  digitalWrite(MotorR1_pin1, HIGH);
  digitalWrite(MotorR1_pin2, LOW);
  digitalWrite(MotorL2_pin1, LOW);
  digitalWrite(MotorL2_pin2, HIGH);
  digitalWrite(MotorR2_pin1, HIGH);
  digitalWrite(MotorR2_pin2, LOW);
}

void Right() {
  digitalWrite(MotorL1_pin1, HIGH);
  digitalWrite(MotorL1_pin2, LOW);
  digitalWrite(MotorR1_pin1, LOW);
  digitalWrite(MotorR1_pin2, HIGH);
  digitalWrite(MotorL2_pin1, HIGH);
  digitalWrite(MotorL2_pin2, LOW);
  digitalWrite(MotorR2_pin1, LOW);
  digitalWrite(MotorR2_pin2, HIGH);
}

void Turn180() {
  digitalWrite(MotorL1_pin1, HIGH);
  digitalWrite(MotorL1_pin2, LOW);
  digitalWrite(MotorR1_pin1, LOW);
  digitalWrite(MotorR1_pin2, HIGH);
  digitalWrite(MotorL2_pin1, HIGH);
  digitalWrite(MotorL2_pin2, LOW);
  digitalWrite(MotorR2_pin1, LOW);
  digitalWrite(MotorR2_pin2, HIGH);
  delay(3000);
  Stop();
}

void readDistances() {
  digitalWrite(trig1, LOW);
  delayMicroseconds(2);
  digitalWrite(trig1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig1, LOW);
  duration = pulseIn(echo1, HIGH);
  distance1 = duration * 0.034 / 2;

  digitalWrite(trig2, LOW);
  delayMicroseconds(2);
  digitalWrite(trig2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig2, LOW);
  duration = pulseIn(echo2, HIGH);
  distance2 = duration * 0.034 / 2;

  digitalWrite(trig3, LOW);
  delayMicroseconds(2);
  digitalWrite(trig3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig3, LOW);
  duration = pulseIn(echo3, HIGH);
  distance3 = duration * 0.034 / 2;

  Serial.print("Distance1: "); Serial.println(distance1);
  Serial.print("Distance2: "); Serial.println(distance2);
  Serial.print("Distance3: "); Serial.println(distance3);
}

void handleMovement() {
  if (distance1 < 5 || distance2 < 5 || distance3 < 5) {
    Stop();
  } else if (distance1 < 10) {
    Turn180();
  } else if (distance2 < 10) {
    Right();
  } else if (distance3 < 10) {
    Left();
  }
}

void handleClient() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Cliente conectado");
    while (client.connected()) {
      if (client.available()) {
        String command = client.readStringUntil('\n');
        command.trim();
        Serial.print("Comando recebido: ");
        Serial.println(command); 
        if (command == "ON") {
          Forward();
        } else if (command == "OFF") {
          Stop();
        }
        client.println("Comando recebido: " + command);
      }
      readDistances();
      handleMovement();
    }
    client.stop();
    Serial.println("Cliente desconectado");
  }
}

void loop() {
  readDistances();
  handleMovement();
  handleClient();
}
