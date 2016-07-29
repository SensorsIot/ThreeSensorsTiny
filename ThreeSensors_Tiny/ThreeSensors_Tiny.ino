

/*

This sketch runs on an ATtiny85 connected to an Arduino Mega running a receiver sketch
This sketch gets distance data from three ultrasonic sensors and transmits it to the Arduino when requested

Andreas Spiess, 2015

*/

#include <TinyWireS.h>       // Requires fork by Rambo with onRequest support
#include <NewPing.h>         // NewPing version 1.7
#include <avr/wdt.h>         // watchdog

#define SLEFT 0
#define SRIGHT 2
#define SMIDDLE 1


#define LEFT_T PB1
#define LEFT_E PB1

#define MIDDLE_T PB3
#define MIDDLE_E PB3

#define RIGHT_T PB4
#define RIGHT_E PB4

#define MAX_DISTANCE 270 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing SensorLeft (LEFT_T, LEFT_E, MAX_DISTANCE);       // Define left Sensor
NewPing SensorMiddle (MIDDLE_T, MIDDLE_E, MAX_DISTANCE);   // Define middle Sensor
NewPing SensorRight (RIGHT_T, RIGHT_E, MAX_DISTANCE);       // Define right Sensor


const int I2CSlaveAddress = 8;      // I2C Address.

int distance[3];                    // Where the Distance is stored (8 bit unsigned)
int place = 0;
unsigned long start;




void setup()
{
  TinyWireS.begin(I2CSlaveAddress);      // Begin I2C Communication
  TinyWireS.onRequest(transmit);         // When requested, call function transmit()

  wdt_enable(WDTO_500MS);               // Watchdog
}


void loop()
{
  readDistance();
  wdt_reset();                          // feed the watchdog
}


//-------------------------------------------------------------------

void transmit()
{
  byte hh;
  switch (place) {
    case 0:
      hh = 255;                       // start byte
      break;
    case 1:
      hh = distance[SLEFT];          // Send last recorded distance for current sensor
      break;
    case 2:
      hh = distance[SMIDDLE];       // Send last recorded distance for current sensor
      break;
    case 3:
      hh = distance[SRIGHT];       // Send last recorded distance for current sensor
      break;
  }
  TinyWireS.send(hh);
  place = place + 1;
  if (place > 3) place = 0;
}

void readDistance()
{
  distance[SLEFT] = SensorLeft.ping_cm();
  if (distance[SLEFT] > 254 ) {
    distance[SLEFT] = 254;
  }
  delay(20);
  distance[SMIDDLE] = SensorMiddle.ping_cm();
  if (distance[SMIDDLE] > 254 ) {
    distance[SMIDDLE] = 254;
  }
  delay(20);
  distance[SRIGHT] = SensorRight.ping_cm();
  if (distance[SRIGHT] > 254 ) {
    distance[SRIGHT] = 254;
  }
  delay(20);
}




