/*
  dht22.cpp - DHT sensor library v1.0.0 - 2021-06-28
  Copyright (c) 2021 Jean-Marc Chiappa.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
*/

#ifndef DHT22_H
#define DHT22_H

#include "Arduino.h"
 
static void DHT22read();

class DHT22
{
public:
  DHT22(uint8_t pin, TIM_TypeDef *instance);
  void begin();
  void startNewAcquisition();
  float readTemperature();
  float readHumidity();
  boolean available();
private:
  void update(void);
  TIM_TypeDef *htimer=NULL;
  uint8_t _pin;
  float _temperature;
  float rh;
  bool _newValue;
};

#endif // DHT22_H