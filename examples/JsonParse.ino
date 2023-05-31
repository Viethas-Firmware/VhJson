#include <json.h>

void setup() {
    // Initialize serial port
    Serial.begin(9600);
    while(!Serial) continue;

    // declare json object
    Json doc;

    char json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
    // parse char to json object
    doc.split(json);
    // get data in json object
    char*       sensor;
    int         time;
    float*      coord;
    int         length;

    doc.get("sensor", sensor);
    doc.get("time", time);
    doc.get("data", coord, length);

    Serial.print("Sensor: "); Serial.println(sensor);
    Serial.print("Time: "); Serial.println(time);
    Serial.print("Latitude: "); Serial.println(coord[0]);
    Serial.print("Longitude: "); Serial.println(coord[1]);
}

void loop() {   }
