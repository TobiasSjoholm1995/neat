#include <stdio.h>
#include <jansson.h>
#include <time.h>
#include "PMHelper.h"

time_t last_loaded_time;


//Task1    
json_t* load_file(char *filePath) {
    json_t *json = NULL;
    json_error_t error;
	
    last_loaded_time = time(0);

    json = json_load_file(filePath, 0, &error);

    if(!json) {
	printf("kk\n");
        WriteLog(__FILE__, __func__, error.text);
    }
    return json;
}

//Test functionality
int main()
{   
    //json_t *json = load_file("/home/free/Downloads/default.profile");

    return 0;
}
