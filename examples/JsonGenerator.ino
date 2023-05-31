#include <json.h>

void setup() {
    // initilize Serial port
    Serial.begin(9600);
    while(!Serial) continue;

    // declare json object
    Json doc;
    // add Value in the document
    doc.set("sensor", "gps");
    doc.set("time", 1351824120);
    // add array in the document
    float coord[2]  = {48.756080, 2.302038};
    doc.set("data", coord, 2);

    // Generate the minified JSON and send it to the Serial port.
    //
    Serial.println(doc.c_str());
    // The above line prints
    // {"sensor": "gps", "time": 1351824120, "data": [ 48.756081, 2.302038 ]}
}

void loop() {   }