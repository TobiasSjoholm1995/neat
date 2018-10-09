#include <stdio.h>
#include <time.h>
#include <jansson.h>
#include "PMHelper.h"

time_t last_loaded_time;

void load_file(char *filePath) {
    last_loaded_time = time(0);
    json_t *json = load_json_file(filePath);

    if(!json) {
       //failed
    }

}

int main() { }


