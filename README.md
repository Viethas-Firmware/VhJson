# The Json Parser/Editor Arduino library

The easiest Arduino library Json parser, builder and editer.

VhJson is the easiest JSON manipulation library to parse or deserialize complex or nested JSON object and arrays.

Able to parse, create and edit the simple or complex (depth nested) JSON object by just specify the relative node/element path.

This library support any Arduino based MCU, ESP8266, ESP32, STM32F103C (Arm Cortext-M3).

## Tested Devices
* ESP8266
* ESP32
* STM32F103C
* STM32F407

## Usage
```C++
#include <Arduino.h>
#include <VhJson.h>

  
VhJson json;

// add "temperature" with value 30.5 and add "Humidity" with value "60.8"
json.set("temperature", "30.5");
json.set("Humidity", "60.8");
Serial.println(json.c_str());

/**
  This is a result above code.
  
  {"temperature": "30.5", "Humidity": "60.8"}
**/

// Add "age" to the array; its length is 6.
int temp[]  = {15, 20, 13, 45, 55, 16};
json.set("age", temp, 6);
Serial.println(json.c_str());

/**
  This is a result of the code above.
  {"temperature": "30.5", "Humidity": "60.8", "age": [ 15, 20, 13, 45, 55, 16 ]}
**/

// add JSON child.
VhJson child;
child.set("name", "John");
child.set("age", 20);
json.set("member", child);

/**
  This is a result of the code above.
  {"temperature": "30.5", "Humidity": "60.8", "age": [ 15, 20, 13, 45, 55, 16 ], "member": {"name": "John", "age": 20}}
**/

  
```
