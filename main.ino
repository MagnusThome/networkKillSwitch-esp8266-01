#include <ESP8266WiFi.h>
#include <aREST.h>
#include <aREST_UI.h>

const char* ssid = "your wifi network name";
const char* password = "wifi password";

int remoteprev = LOW;
const byte turnon[] = {0xA0, 0x01, 0x01, 0xA2};   // THIS SPECIFIC RELAY BOARD FOR ESP8266-01 IS CONTROLLED ON SERIAL WITH
const byte turnoff[] = {0xA0, 0x01, 0x00, 0xA1};  // THESE TWO STRINGS. THE BOARD WAITS FOR THESE TO TURN RELAY OFF OR ON
WiFiServer server(80);
aREST_UI rest = aREST_UI();

void setup(void)
{ 
  Serial.begin(9600);
  pinMode(12,OUTPUT);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
  rest.title("Relay Control");
  rest.button(remotepin);
  rest.set_id("1");
  rest.set_name("relayboard");
  server.begin();
}

void loop() { 
  WiFiClient client = server.available();
  if (!client) {
    int remote = digitalRead(12);
    if (remote != remoteprev) {
      if (remote == HIGH) {
        Serial.write(turnon, sizeof(turnon));
      }
      if (remote == LOW) {
        Serial.write(turnoff, sizeof(turnoff));
      }
      remoteprev = remote;
    }
    return;
  } 
  while(!client.available()){
    delay(1);
  }
  rest.handle(client);
}
