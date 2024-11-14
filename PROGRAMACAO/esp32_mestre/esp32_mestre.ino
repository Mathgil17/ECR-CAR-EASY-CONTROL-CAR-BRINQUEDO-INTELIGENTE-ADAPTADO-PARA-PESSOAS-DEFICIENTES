#include <WiFi.h>

const char *ssid = "ESP32_AP";         
const char *password = "12345678";     

const char *host = "192.168.4.1";      
const uint16_t port = 80;              

const int buttonPin = 23;              

WiFiClient client;
bool lastButtonState = LOW;              // Estado anterior do botão

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(115200);

  // Conecta ao Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Conectando-se ao Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado ao Wi-Fi!");
}

void loop() {
  // Verifica o estado do botão
  bool buttonState = digitalRead(buttonPin);
  
  // Conecta-se ao servidor se necessário
  if (!client.connected()) {
    Serial.println("Tentando conectar ao servidor...");
    if (client.connect(host, port)) {
      Serial.println("Conectado ao servidor!");
    } else {
      Serial.println("Falha ao conectar ao servidor");
      delay(1000);  // Espera antes de tentar conectar novamente
      return;
    }
  }

  // Envia comando apenas se o estado do botão mudou
  if (buttonState != lastButtonState) {
    lastButtonState = buttonState;
    if (buttonState == HIGH) {
      client.println("ON");
      Serial.println("Comando 'ON' enviado");
    } else {
      client.println("OFF");
      Serial.println("Comando 'OFF' enviado");
    }
  }

  delay(100);  // Atraso para evitar leituras excessivas
}
