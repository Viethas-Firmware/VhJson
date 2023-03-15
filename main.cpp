#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "json.h"

int main(int argc, char *argv[]) {
    Json json;
    char str[1024];
    const char* name[5] = {"vu duc", "duc vu", "vu minh", "minh duc", "duc minh"};
    int age[5] = {5, 10, 20, 14, 23};

    const char* exam = "[1,2,3,4,5,6]";
    const char* exam_2 = "[\"vu duc\", \"duc vu\", \"vu minh\", \"minh duc\", \"duc minh\"]";
    int* array   = NULL;
    char** array_2 = NULL;
    uint16_t length = 0;
    

    json.add(str, "ages", age, 5);
    json.add(str, "names", name, 5);
    json.add(str);

    printf("%s\n", str);

    json.split((const char*)str);

    json.get("names", array_2, length);


    return 0;
}