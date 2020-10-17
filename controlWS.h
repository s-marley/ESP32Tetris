#ifdef ESP32
#include <WiFi.h>
#include <ESPmDNS.h>
#elif
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#endif
#include <WebSocketsServer.h>
#include <HashMap.h>

#include "wifi.h"
// Create file with the following
// *************************************************************************
// #define SECRET_SSID "";  /* Replace with your SSID */
// #define SECRET_PSK "";   /* Replace with your WPA2 passphrase */
// *************************************************************************
const char ssid[] = SECRET_SSID;
const char passphrase[] = SECRET_PSK;

WebSocketsServer webSocket = WebSocketsServer(81);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  IPAddress ip = webSocket.remoteIP(num);
  switch (type) {

    case WStype_DISCONNECTED: {
        Serial.printf("[%u] Disconnected!\n", num);
      }
      break;

    case WStype_CONNECTED: {
        Serial.printf("[%u] Connection from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      }
      break;

    case WStype_TEXT: {
        Serial.printf("[%u] got Text: %s\n", num, payload);
        for ( size_t i = 0; i < length; i++ ) {
          char keyPress = (char) payload[i];
          switch (keyPress) {
            case 'w':
              currentInput = ROTATE;
              break;
            case 'a':
              currentInput = LEFT;
              break;
            case 's':
              currentInput = DOWN;
              break;
            case 'd':
              currentInput = RIGHT;
              break;
          }
          Serial.printf("Input %c\n", keyPress);
        }
      }
      break;

    case WStype_BIN:
      Serial.printf("[%u] got binary length: %u\n", num, length);
      break;
  }

}

void controlSetup() {
  // Make sure you're in station mode
  WiFi.mode(WIFI_STA);

  Serial.println("");
  Serial.print(F("Connecting to "));
  Serial.println(ssid);

  if (passphrase != NULL)
    WiFi.begin(ssid, passphrase);
  else
    WiFi.begin(ssid);

  Serial.print("Waiting on wifi ");
  int sanity = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("w");
    sanity++;
    if (sanity > 50) {
      Serial.println("ERROR: failed to connect to wifi");
      break;
    }
  }
  Serial.println("\nDone");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (!MDNS.begin("tetris")) {
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");

  // Add service to MDNS-SD
  MDNS.addService("http", "tcp", 80);

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  //  leds[0] = CRGB::Blue;
  FastLED.show();

}

void controlLoop() {
  webSocket.loop();
}
