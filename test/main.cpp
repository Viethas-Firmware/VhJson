#include <Arduino.h>
#include <json.h>

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
      json.set("temp", 30.5f);
      json.set("hum", 71.5f);

      Serial.println(json.c_str());

      float value = 0;
      json.get("temp", value);

      Serial.println(value);

      json.clear();

      manual = "";
    }
  }
}