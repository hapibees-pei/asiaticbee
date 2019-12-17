#include <Arduino.h>
#include <AutoConnect.h>
#include <WebServer.h>
#include <WiFi.h>

WebServer Server;
AutoConnect Portal(Server);
AutoConnectConfig Config("Gateway", "");

void rootPage() {
  char content[] = "Hello, world";
  Server.send(200, "text/plain", content);
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();

  Server.on("/", rootPage);

  Portal.config(Config);

  if (Portal.begin()) {
    Serial.println("WiFi connected: " + WiFi.localIP().toString());
  }
}

void loop() { Portal.handleClient(); }
