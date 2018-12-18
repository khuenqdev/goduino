/**
 * Khue Q. Nguyen
 * December 18th, 2018
 * WIFI connection demo for ODRDOID-GO
 */

#include <SPI.h>
#include <WiFi.h>

char ssid[] = "Something"; // Your SSID
char pass[] = "12345678"; // Your password

int keyIndex = 0;
int status = WL_IDLE_STATUS;

WiFiClient client;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  connectToInternet();
}

void loop() {
  // put your main code here, to run repeatedly:

}

void connectToInternet()
{
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print("[INFO] Attempting Connection - WPA SSID:");
    Serial.println(ssid);
  }

  // Connection successful
  Serial.print("[INFO] Connection Successful");
  Serial.print("");
  printConnectionInformation();
  Serial.println("---------------------------------------");
  Serial.print("");
}

void printConnectionInformation()
{
  // Print Network SSID
  Serial.print("[INFO] SSID: ");
  Serial.println(WiFi.SSID());

  // Print received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("[INFO] Signal Strength (RSSI): ");
  Serial.println(rssi);
}
