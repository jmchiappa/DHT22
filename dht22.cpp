/*
  dht22.cpp - DHT sensor library v1.0.0 - 2021-06-28
  Copyright (c) 2021 Jean-Marc Chiappa.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
*/

#include "dht22.h"

static struct Result {
  float temperature;
  float hygro;
  bool newValue;
} result;

// local variable
static uint8_t duration[42];
static uint8_t data[5];
static HardwareTimer *tim;
static uint8_t idx=0;
static uint16_t startTime;
static uint16_t endTime;
int32_t overflow;

static uint8_t _thisPin;
DHT22::DHT22(uint8_t pin, TIM_TypeDef *instance){
  htimer = instance;
  _pin = pin;
  _thisPin = pin;
}

void DHT22::begin() {
  tim = new HardwareTimer(htimer);
  TimerModes_t mode = tim->getMode(1);
  if( (mode==TIMER_DISABLED) || (mode==TIMER_OUTPUT_COMPARE)) {
    tim->setMode(1,TIMER_OUTPUT_COMPARE_ACTIVE);
    overflow = tim->getOverflow(MICROSEC_FORMAT); // return overflow depending on format provided
  }

  tim->resume();
}

/**
 * @brief main callback on attachinterrupt. Once an edge is detected,
 * grab only high level duration. Depending on this value, it should "1" (>50us) or "0"
 * Due to the "start transmission protocol", 2 pulses overheads the buffer. these must be discarded
 * 
 */
static void DHT22read() {
  
  if(digitalRead(_thisPin)) {
    // high level is beginging, catch the timer
    startTime = tim->getCount(MICROSEC_FORMAT);
  }else{
    // low level detected, end of high level calculate duration
    endTime=tim->getCount(MICROSEC_FORMAT);
    if(startTime>endTime) {
      duration[idx]=(int32_t)endTime-(int32_t)startTime+overflow;
    }else
      duration[idx]=endTime-startTime;

    idx++;
  }

  if(idx==42) {
    // transmission is over, convert to buffer
    for(uint8_t i=0; i<40;i++) {
      data[i/8] = data[i/8]<<1 | ( duration[2+i]>50) ;
      // Serial.println(duration[2+i]);
    }
    // for(uint8_t i=0;i<5;i++) {
    //   Serial.println(data[i]);
    // }
    // checksum is ok ?
    if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
      result.hygro = (((word)data[0]) << 8 | data[1])*0.1f;
      result.temperature = ((word)(data[2] & 0x7F)) << 8 | data[3];
      result.temperature *= 0.1;
      if (data[2] & 0x80) {
        result.temperature *= -1;
      }
      result.newValue = true;
    } else {
      // checksum error, no data
    }
    idx=0;
  }
}

bool DHT22::available() {
  return result.newValue;
}

float DHT22::readTemperature() {
  result.newValue = false;
  return result.temperature;
}

float DHT22::readHumidity() {
  result.newValue = false;
  return result.hygro;
}

void DHT22::startNewAcquisition() {
  idx=0;
  result.newValue = false;
  // clear temp buffer
  memset(data,4,0);
  PinName p = digitalPinToPinName(_pin);
  uint16_t pin = STM_GPIO_PIN(p);
  detachInterrupt(_pin);
  pinMode(_pin, INPUT_PULLUP);
  delay(2);
  pinMode(_pin,OUTPUT);
  digitalWrite(_pin,LOW);
  delay(2);
  // clear pending interrupt if any
  __HAL_GPIO_EXTI_CLEAR_IT(pin);
  attachInterrupt(_pin, DHT22read, CHANGE);
  pinMode(_pin,INPUT_PULLUP);
}