#include <vl53l0xTOFA.h>

/* This example shows how to use continuous mode to take
range measurements with the VL53L0X. It is based on
vl53l0x_ContinuousRanging_Example.c from the VL53L0X API.

The range readings are in units of mm. */
//CKH attempts to get AmbientRate and SignalRate amplitudes
//by modifying the pololu library but may just switch to Adafruit
//since they seem to have implemented the whole api
//CKH added some smoothing 4-5-20

#include <Wire.h>
#include <VL53L0xTOFA.h> //cindys new library just see if it crashes

VL53L0xTOFA sensor;
float smoothLength=0;
float signalrate=0;
float ambientrate=0;


//do some smoothing https://playground.arduino.cc/Main/RunningAverage/
float runningAverage(int M) { 
  #define LM_SIZE 10
  static int LM[LM_SIZE];      // LastMeasurements
  static byte index = 0;
  static float sum = 0;
  static byte count = 0;

  // keep sum updated to improve speed.
  sum -= LM[index];
  LM[index] = M;
  sum += LM[index];
  index++;
  index = index % LM_SIZE;
  if (count < LM_SIZE) count++;

  return sum / count;
}

void setup()
{
  Serial.begin(9600);
  Wire.begin();

  sensor.setTimeout(500);
  if (!sensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}
  }

  // Start continuous back-to-back mode (take readings as
  // fast as possible).  To use continuous timed mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)).
  sensor.startContinuous();
}

void loop()
{
  //smoothLength=runningAverage(sensor.readRangeContinuousMillimeters());
  sensor.readTOFA();
  smoothLength=runningAverage(sensor.tofa.distancemm);
  signalrate=sensor.tofa.signalrate;
  ambientrate=sensor.tofa.ambientrate;
  //calibLength=smoothLength/10*0.909-29.5 ; //uturn length in cm from fitting to two measurements only
  //Serial.print(smoothLength,2);//show it in cm
  Serial.print(smoothLength);
  Serial.print('\t');
  Serial.print(signalrate);
  Serial.print('\t');
  Serial.print(ambientrate);
  if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }

  Serial.println();
}
