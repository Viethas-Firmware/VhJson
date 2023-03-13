#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "json.h"

int main(int argc, char *argv[]) {
    Json json;
    if(argc > 0) {
        printf("%s\n", argv[1]);
        json.split(argv[1]);
        json.check_map();
    }
    return 0;
}