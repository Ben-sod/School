// thermo_update.c: functions that read the thermometer temperature
// sensor, convert its value to degrees Celsius or Fahrenheit, and
// adjusts the display to show the temperature.

#include "thermo.h"

#include <stdio.h>                 // for debugging

#define MAX_SENSOR_VAL 64*10*100   // Max expected value in sensor value
#define MIN_CELSIUS    -500        // min/max values for celsius in tenths degrees
#define MAX_CELSIUS    +500
#define MIN_FAHRENHEIT -580        // min/max values for fahrenheit in tenths degrees
#define MAX_FAHRENHEIT +1220

// Uses the two global variables (ports) THERMO_SENSOR_PORT and
// THERMO_STATUS_PORT to set the temp structure. If THERMO_SENSOR_PORT
// is above its maximum trusted value, associated with +50.0 deg C,
// does not alter temp and returns 1.  Otherwise, sets fields of temp
// based on converting sensor value to degrees and checking whether
// Celsius or Fahrenheit display is in effect. Returns 0 on successful
// set. This function DOES NOT modify any global variables but may
// access global variables.
//
// CONSTRAINT: Uses only integer operations. No floating point
// operations are used as the target machine does not have a FPU.

int set_temp_from_ports(temp_t *temp){
  if(THERMO_SENSOR_PORT > 64000){  // error checking
    return 1;
  }
  short ctemp = THERMO_SENSOR_PORT / 64 - 500;
  short crem  = THERMO_SENSOR_PORT % 64;
  if(crem >= 32){
    ctemp += 1;                             // round up
  }

  if(!(THERMO_STATUS_PORT & 0b00000001)){   // celsius or fahrenheit?
    temp->is_fahrenheit = 0;                // celsius
    temp->tenths_degrees = ctemp;
  }
  else{                                     // convert to fahrenheit
    temp->is_fahrenheit = 1;
    //  int ftemp = ((int)THERMO_SENSOR_PORT * 9) / 5 / 64 + 320 - 500; // broken
    short ftemp = (ctemp * 9) / 5 + 320;
    // short frem  = (THERMO_SENSOR_PORT * 9) % 5;
    // if(frem > 2){                           // round up for positive
    //   ftemp += 1;
    // }
    // else if(frem < -2){                     // round down for negative
    //   ftemp -= 1;
    // }
    temp->tenths_degrees = ftemp;
  }
  return 0;
}


int digit_masks[10] = {         // bit masks representing each of the valid digits
  0b0111111,                    // zero
  0b0000110,                    // one
  0b1011011,                    // two
  0b1001111,                    // three
  0b1100110,                    // four
  0b1101101,                    // five
  0b1111101,                    // six
  0b0000111,                    // seven
  0b1111111,                    // eight
  0b1101111,                    // nine
};

#define BLANK    0b0000000      // blank
#define NEGATIVE 0b1000000      // negative sign
#define CEL_MASK (0x1 << 28)    // position of deg C bit
#define FAH_MASK (0x1 << 29)    // position of deg F bit


// Alters the bits of integer pointed to by display to reflect the
// temperature in struct arg temp.  If temp has a temperature value
// that is below minimum or above maximum temperature allowable or if
// an improper indication of celsius/fahrenheit is given, does nothing
// and returns 1. Otherwise, calculates each digit of the temperature
// and changes bits at display to show the temperature according to
// the pattern for each digit.  This function DOES NOT modify any
// global variables but may access global variables.
int set_display_from_temp(temp_t temp, int *display){
  if(   (temp.is_fahrenheit==0 && (temp.tenths_degrees < MIN_CELSIUS ||
                                   temp.tenths_degrees > MAX_CELSIUS))
     || (temp.is_fahrenheit==1 && (temp.tenths_degrees < MIN_FAHRENHEIT ||
                                   temp.tenths_degrees > MAX_FAHRENHEIT))
     || (temp.is_fahrenheit!=0 && temp.is_fahrenheit!=1))
  {
    return 1;                      // error conditions
  }

  int disp = 0;                    // transfer this to the display
  int quo = temp.tenths_degrees;   // initialize with temp, use in repeated division

  if(quo < 0){                     // check for negative
    disp |= NEGATIVE;              // add negative sign
    disp = disp << 7;              // shift
    quo = -quo;                    // negate quotient so remaining values are all positive
  }

                                   // CALCULATE EACH DIGIT
  int tenths = quo % 10;           // tenths place
  quo = quo / 10;
  int ones   = quo % 10;           // ones place
  quo = quo / 10;
  int tens  = quo % 10;            // tens place
  quo = quo / 10;
  int hundreds  = quo % 10;        // hundreds place

                                   // CREATE THE DISPLAY
  if(hundreds != 0){               // blank for hundreds?
    disp |= digit_masks[hundreds]; // nope
    disp = disp << 7;
  }
  if(hundreds !=0 || tens != 0){   // blank for tens?
    disp |= digit_masks[tens];     // nope
    disp = disp << 7;
  }
  disp |= digit_masks[ones];       // ones digit always there
  disp = disp << 7;
  disp |= digit_masks[tenths];     // thens digit always there
  // no shift for tenths digit

  if(temp.is_fahrenheit){          // set bits associated C/F
    disp |= FAH_MASK;
  }
  else{
    disp |= CEL_MASK;
  }

  *display = disp;                 // deref-set the pointed to integer
  return 0;
}

// Called to update the thermometer display.  Makes use of
// set_temp_from_ports() and set_display_from_temp() to access
// temperature sensor then set the display. Checks these functions and
// if they indicate an error, makes not changes to the display.
// Otherwise modifies THERMO_DISPLAY_PORT to set the display.
//
// CONSTRAINT: Does not allocate any heap memory as malloc() is NOT
// available on the target microcontroller.  Uses stack and global
// memory only. */
int thermo_update(){
  temp_t temp = {};
  int ret;
  ret = set_temp_from_ports(&temp);
  if(ret!=0){
    return 1;
  }
  int disp = 0;
  ret = set_display_from_temp(temp,&disp);
  if(ret != 0){
    return 1;
  }
  THERMO_DISPLAY_PORT = disp;
  return 0;
}
