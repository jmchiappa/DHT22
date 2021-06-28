# DHT22 library optimized for STM32duino framework

## Summary

This library aims to take benefit of powerful STM32 resources and architecture.
It drives the popular DHT22 digital thermometer sensor by using dedicated STM32 resources.
it is based on STM32DUINO framework on the latest version (>= 2.0.0).
This lib works under interrupt so no time is spending to wait for end of conversion.

## API references

### variable declaration

`DHT22 dht(pin , <timer instance>`);

`Timer instance` must be be a valid timer instance of the MCU. As STM32 MCUs have different configuration, refer to the datasheet to choose the right one.

for example : `DHT22 dht( D2, TIM17 );`

Any pin can be used to connect the DHT22 sensor.

### start an acquisition

As this library is working on interrupt only, it is based on "fire and forget" style.

`dht.startNewAcquisition();`

this fires a new sample acquisition

### check when a new sample is available

`dht.available();`

return `true` when a new sample is available. this flag is cleared  once data is read

### reading values

`dht.readTemperature();`

return a float value of current temperature expressed in °C. Once this value is read, the available flag is cleared.

`dht.readHumidity();`

return a float value of current relative humidity expressed in percent. Once this value is read, the available flag is cleared.

see example `basic.ino`
