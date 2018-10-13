#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <jansson.h>

#define FILENAME "ErrorLog.txt"

char*
 concat(const char *s1, const char *s2)
{
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2);
    char *result = malloc(len1 + len2 + 1); // +1 for the null-terminator
    
    if(result == NULL)
        return ""; //error, should never happen

    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1);
    return result;
}

int 
fileE_xist(char * filePath) 
{
   return access(filePath, F_OK) != -1; 
}

void 
write_log(const char* module, const char* func, char* desc) 
{
    char time_buffer[100];
    time_t now = time (0);
    strftime (time_buffer, 100, "%Y-%m-%d %H:%M:%S.000", localtime (&now));
    
    FILE *fp = fopen(FILENAME, "a");  
    if(fp != NULL) {
        fprintf(fp, "Time: %s  \nModule: %s\nFunction: %s\nDescription: %s\n\n", time_buffer, module, func, desc);  
        fclose(fp);
    }
}

void 
clear_log() 
{
    FILE *fp = fopen(FILENAME, "w");  
    if(fp != NULL) {
        fclose(fp);
    }
}

//Returns 0 if file is not found
int 
file_is_modified(const char *path, time_t oldTime) 
{
    struct stat file_stat;
    int err = stat(path, &file_stat);
    if (err != 0) {
        write_log("Pib.c" , "file_is_modified", "Error when  reading file " FILENAME);
    }
    return file_stat.st_mtime > oldTime;
}


json_t* 
load_json_file(char *filePath) 
{
    json_t *json = NULL;
    json_error_t error;

    json = json_load_file(filePath, 0, &error);

    if(!json) {
        write_log(__FILE__, __func__, error.text);
    }
    return json;
}

void 
write_json_file(char* filePath, json_t *json) 
{
    if(json_dump_file(json, filePath, JSON_INDENT(4)) == -1) {
        write_log(__FILE__, __func__, concat("Error: Unable to generate a Json file, ", filePath));
    }
}
                                                                                                                                                                                                                
