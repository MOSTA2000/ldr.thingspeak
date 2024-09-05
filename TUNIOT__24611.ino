#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "AliNet(03426440)";
const char* password = "1234567$"; // Your Wi-Fi password

// ThingSpeak API information
String thingSpeakReadAPIKey = "N35E4KW7T8IG70W4"; // Your ThingSpeak Read API Key
String thingSpeakChannelID = "2647234"; // Your ThingSpeak Channel ID

WiFiClient client;
HTTPClient http;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.disconnect();
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    // Form the request URL to get data from ThingSpeak
    String url = "http://api.thingspeak.com/channels/" + thingSpeakChannelID + "/feeds.json?api_key=" + thingSpeakReadAPIKey + "&results=2";

    http.begin(client, url); // Initialize HTTP
    int httpCode = http.GET(); // Make the GET request

    if (httpCode == 200) { // Check for successful response
      String payload = http.getString(); // Get the response payload
      Serial.println("Data from ThingSpeak:");
      Serial.println(payload); // Print the full JSON payload

      // Parse JSON data to extract the last two entries
      // Extract entry 1
      int startIndex = payload.indexOf("\"feeds\":[") + 9;
      int endIndex = payload.indexOf("]", startIndex);
      String feeds = payload.substring(startIndex, endIndex);

      // Find individual entries
      int pos = 0;
      int count = 0;
      while ((pos = feeds.indexOf("{", pos)) != -1 && count < 2) {
        int entryEnd = feeds.indexOf("}", pos) + 1;
        String entry = feeds.substring(pos, entryEnd);
        Serial.println("Entry:");
        Serial.println(entry);

        // Extract the LDR value from each entry
        int field1Start = entry.indexOf("\"field1\":\"") + 10;
        int field1End = entry.indexOf("\"", field1Start);
        String ldrValue = entry.substring(field1Start, field1End);
        Serial.print("LDR Value: ");
        Serial.println(ldrValue);

        pos = entryEnd;
        count++;
      }
    } else {
      Serial.println("Error on HTTP request");
    }

    http.end(); // Free resources
  } else {
    Serial.println("WiFi not connected.");
  }

  delay(60000); // Wait 60 seconds before the next request
}
