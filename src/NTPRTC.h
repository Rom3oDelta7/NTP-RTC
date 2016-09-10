/*
 NTPRTC - Shim for TimeLib.h library
 
 Uses Network Time Protocol (NTP) to provide a time seed for the normal time functions (hour, day, etc.)
 
   Copyright 2016 Rob Redford
   This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
   To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/4.0/.
 
*/
#ifndef NTPRTC_H
#define NTPRTC_H

#include <TimeLib.h>
#include <WorldTimezones.h>

void NTP_Init(void);
void NTP_UTC_Timezone(const int);

#endif