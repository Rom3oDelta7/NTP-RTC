# "Shim" for Paul Stoffregen's Time library to use NTP on the ESP8266 
This "shim" library provides the initialization functions to use Network Time Protocol (NTP) on the ESP8266 using WiFi in the [Arduino IDE].
After initialization, all of the [Time library] functions will get their source time via NTP.
The motivation for this library was to provide a network-based real-time clock (RTC) for a [Blynk] application.
Currently, (Sept 2016) the Blynk RTC widgit will not compile in the latest Arduino IDE and even under older (1.6.9) versions of the IDE, the time returned is UTC even when offsets are set.
Also, the RTC widgit has no user interaction, so it unnecessarily occupies Blynk dashboard space which could better be utlized for widgits that interact with the user.
Nevertheless, this library is not limited to Blynk and can be used in the general case.


# Programming Interface
## Constructors
None

## Functions
```C++
void NTP_UTC_Timezone(const int offset);
```
This function sets the local timezone offset from UTC [UTC]. Note that this ___must___ be the first of these library functions called.
The header file ```WorldTimezones.h``` in this library has defines of the form ```UTC_*``` for timezone offsets across the world.
Note that fractional offsets are not supported. The offset must be a (positive or negative) integer.


```C++
void NTP_Init(void);
```
This function sets up the network connections to one of a set of NTP servers in the United States and defines the necessary functions
so that subsequent calls to any of the Time functions (e.g. ```hour()```) will return a time value based on the current network time (plus offset).
Essentially NTP is being used as a real-time clock (RTC).
For robustness, ```NTP_Init()``` will try all of the servers in the table of URLs until it receives a response.
If no response is ever received, then the time value will be 0.

## Header Files
```NTPRTC.h```
Declares the library functions. Mandatory.

```WorldTimezones.h```
C preprocessor defines for world timezones. Optional.


# Examples
None. <ToDo>

# Installation

Go to Releases and download the latest release .zip archive.
Install by using the "Sketch > Include Library > Add .ZIP Library..." menu commands in the Arduino IDE.

Of course, you may also clone this repository and manually install the files if you prefer.

# Copyright Notice

Copyright 2016 Rob Redford
This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
To view a copy of this license, visit [BY-NC-SA].

[Time library]: http://www.pjrc.com/teensy/td_libs_Time.html
[Blynk]: http://www.blynk.cc
[Arduino IDE]: http://www.arduino.cc
[BY-NC-SA]: http://creativecommons.org/licenses/by-nc-sa/4.0