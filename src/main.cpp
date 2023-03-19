#include <Arduino.h>
#include "json.h"

String manual = "";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0) {
    char chr = (char)Serial.read();
    if(chr != '\n') {
      manual += chr;
    }
    else {
      Json json;
      char* result = NULL;
      json.split(manual.c_str());
      Serial.println(json.c_str());

      json.get("msg", result);

      if(result) Serial.println(result);

      free(result);

      json.clear();

      manual = "";
    }
  }
}