#include <stdio.h>
#include <conio.h>
#include <json.h>


int main() {
    Json json;
    Json doc;

    json.set("sensor", "gps");
    json.set("time", 1351824120);
    
    float coordinates[2]  = {48.756080, 2.302038};
    
    json.set("data", coordinates, 2);
    printf("%s\n", json.c_str());
    // char str_json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";

    doc.split(json.c_str());

    char* name;
    int time;
    float *coord;
    int length;
    doc.get("sensor", name);
    doc.get("time", time);
    doc.get("data", coord, length);

    printf("Sensor: %s\n", name);
    printf("Time: %d\n", time);
    printf("Latitude: %f\n", coord[0]);
    printf("Longitude : %f\n", coord[1]);

    return 0;
}