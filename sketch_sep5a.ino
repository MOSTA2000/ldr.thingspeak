#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

WiFiClient client;
HTTPClient http;

// Replace with your network credentials
const char* ssid = "AliNet(03426440)";
const char* password = ""; // Add your Wi-Fi password here

// ThingSpeak API information
String thingSpeakAddress = "http://api.thingspeak.com/update?";
String writeAPIKey = "IDBLZDZS7B6TRB7J"; // Your ThingSpeak Write API Key
String tsfield1Name = "field1";
String request_string;

const int ldrPin = A0; // LDR connected to Analog Pin A0

void setup() {
  // Begin Serial Communication
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.disconnect();
  WiFi.begin(ssid, password);

  // Wait for Wi-Fi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  // Read LDR sensor value
  int ldrValue = analogRead(ldrPin);
  Serial.print("LDR Value: ");
  Serial.println(ldrValue);

  // Prepare request string to send data to ThingSpeak
  if (client.connect("api.thingspeak.com", 80)) {
    request_string = thingSpeakAddress;
    request_string += "key=";
    request_string += writeAPIKey;
    request_string += "&";
    request_string += tsfield1Name;
    request_string += "=";
    request_string += String(ldrValue);  // Add LDR value here

    // Send data to ThingSpeak
    http.begin(request_string);
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("Connection to ThingSpeak failed.");
  }

  delay(20000); // Delay between updates (ThingSpeak allows a minimum delay of 15 seconds)
}
