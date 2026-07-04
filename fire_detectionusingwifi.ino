#include <WiFi.h>
#include <HTTPClient.h>
#include <UrlEncode.h>

#define Buzzer 33
#define Sensor 32

const char* ssid = "zoom";//Enter your SSID
const char* password = "mor123@@";//Enter your password

String phoneNumber = "+201019972777"; //country_code + whats app phone number
String apiKey = "7051607";  //enter apikey of the whats app bot

void setup() {
  Serial.begin(115200);
  pinMode(Buzzer, OUTPUT);
  pinMode(Sensor, INPUT);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  // Send Message to WhatsAPP
  sendAlert("Your system is ready!");
}

void loop() {
  bool sensorValue = digitalRead(Sensor);
  if (sensorValue == 1) {
    digitalWrite(Buzzer, HIGH);
    sendAlert("Warning! Fire Detected...");
  } else {
    digitalWrite(Buzzer, LOW);
  }
}


void sendAlert(String message) {

  // Data to send with HTTP POST
  String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);
  HTTPClient http;
  http.begin(url);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  // Send HTTP POST request
  int httpResponseCode = http.POST(url);
  if (httpResponseCode == 200) {
    Serial.print("Message sent successfully");
  }
  else {
    Serial.println("Error sending the message");
    Serial.print("HTTP response code: ");
    Serial.println(httpResponseCode);
  }

  // Free resources
  http.end();
}