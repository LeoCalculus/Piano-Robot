#ifndef __WIFICONFIG_H
#define __WIFICONFIG_H

const char *ssid = "ELEC-391-Esp32";
const char *password = "UsuckBro";

// webpage code
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP32 Message Board</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { font-family: Arial; text-align: center; margin-top: 50px; }
    input[type=text] { padding: 10px; width: 80%; font-size: 16px; }
    input[type=submit] { padding: 10px 20px; font-size: 16px; cursor: pointer; background-color: #4CAF50; color: white; border: none; }
  </style>
</head>
<body>
  <h2>Send Message to ESP32</h2>
  <form action="/get" method="GET">
    <input type="text" name="message" placeholder="Type your message here...">
    <br><br>
    <input type="submit" value="Send to ESP32">
  </form>
</body>
</html>)rawliteral";

#endif