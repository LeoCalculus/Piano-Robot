#include <Arduino.h>
#include <WiFi.h>
#include <wifiConfig.hpp>
#include <WebServer.h>
#include <string>

// web server with port 80:
WebServer server(80);
String esp32wifiInfo;

// put function declarations here:


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  Serial.println();
  Serial.println(WiFi.softAPIP());

  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", index_html);
  }); // start the server with html

  server.on("/get", HTTP_GET, []() {
    if (server.hasArg("message")) {
      String msg = server.arg("message");
      
      Serial.println("-------------------------");
      Serial.print("New Message Received: ");
      Serial.println(msg);
      Serial.println("-------------------------");

      // Send a confirmation back to the phone
      server.send(200, "text/html", "<h1>Message Sent!</h1><p>Check the Serial Monitor.</p><br><a href='/'>Go Back</a>");
    } else {
      server.send(400, "text/plain", "No message received");
    }
  });
  server.begin();
  Serial.print("Server started");
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  esp32wifiInfo = "Server IP" + WiFi.softAPIP().toString();
  Serial.print(esp32wifiInfo);
  delay(1000);
}

// put function definitions here:
