/*
  basic.ino - DHT sensor example v1.0.0 - 2021-06-28
  Copyright (c) 2021 Jean-Marc Chiappa.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
*/

#include "dht22.h"

// use another timer if needed
// there's no rule to choose the "right" timer. It is depending on your application
// refer to the MCU datasheet for the list of timers
// this example is based on the P-NUCLEO-WB55RG (4 timers available)

const uint8_t PIN=D2;

// prototype of the callback

void sampleCompleted(DHT22::Result *);

// callback can be passed to the constructor or assign lazely with attach(callback);
DHT22 dht(PIN,TIM17,&sampleCompleted);

// callback when the sample is ready
// don't care to loop on an available sample
void sampleCompleted(DHT22::Result *res) {
    Serial.print("Temperature : ");
    Serial.print(res->temperature);
    Serial.print("\tHumidity : ");
    Serial.println(res->humidity);
    // just wait for a moment
    delay(500);
    // and start a new acquisition
    dht.startNewAcquisition();
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  dht.startNewAcquisition();
}

void loop() {

  // do your stuff...

}
