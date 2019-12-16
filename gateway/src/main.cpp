#include <Arduino.h>
#include <AutoConnect.h>
#include <WebServer.h>
#include <WiFi.h>

WebServer Server;  // Replace with WebServer for ESP32
AutoConnect Portal(Server);

void rootPage() {
  char content[] = "Hello, world";
  Server.send(200, "text/plain", content);
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();

  Server.on("/", rootPage);
  if (Portal.begin()) {
    Serial.println("WiFi connected: " + WiFi.localIP().toString());
  }
}

void loop() { Portal.handleClient(); }
