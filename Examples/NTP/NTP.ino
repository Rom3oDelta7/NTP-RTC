/*
 * NTP.ino
 * Example showing time sync to NTP time source for ESP8266 WiFi
 * This example is based on https://github.com/PaulStoffregen/Time/tree/master/examples/TimeNTP_ESP8266WiFi
 *
 * This sketch uses the NTP-RTC library at https://github.com/Rom3oDelta7/NTP-RTC
 */

#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <NTPRTC.h>
#include <WorldTimezones.h>


const char ssid[] = "SSID";  				//  your network SSID (name)
const char pass[] = "PASSWORD";       	// your network password

void setup()
{
  Serial.begin(115200);
  while (!Serial) ; // Needed for Leonardo only
  delay(250);
  Serial.println("TimeNTP Example with NTP-RTC library");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("IP address assigned by DHCP is ");
  Serial.println(WiFi.localIP());
  // NTP initialization
  NTP_UTC_Timezone(UTC_MST);
  NTP_Init();
}

time_t prevDisplay = 0; // when the digital clock was displayed

void loop()
{
  if (timeStatus() != timeNotSet) {
    if (now() != prevDisplay) { //update the display only if time has changed
      prevDisplay = now();
      digitalClockDisplay();
    }
  }
}

void digitalClockDisplay()
{
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(".");
  Serial.print(month());
  Serial.print(".");
  Serial.print(year());
  Serial.println();
}

void printDigits(int digits)
{
  // utility for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
