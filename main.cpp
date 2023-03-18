#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "json.h"

int main(int argc, char *argv[]) {    
    Json json, child, child_1, child_2;

    int a[] = {1, 2, 3, 4, 5};
    const char* b[] = {"ab", "ba", "cd", "det"};
    const char* zips[]  = {"90000", "78000", "97000", "23000", "26000"};
    char* result = NULL;

    child_2.set("city", "Ho Chi Minh");
    child_2.set("country", "Viet Nam");
    child_2.set("zip", (char**)zips, 5);

    child.set("school", "Nguyen Van Cu");
    child.set("class", "12A3");
    child.set("teacher", "Nguyen Hoa Binh");
    child.set("region", child_2);
    child.set("timezone", "Asian/Ho Chi Minh");

    json.set("name", "Vu Minh Duc");
    json.set("student", "Good");
    json.set("age", 16);
    json.set("numbers", a, 5);
    json.set("character", (char**)b, 4);
    json.set("info", child);

    printf("%s\n", json.c_str());

    json.get("info", child_1);

    printf("child: %s\n", child_1.c_str());
    return 0;
}