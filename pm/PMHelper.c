#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <jansson.h>

#define FILENAME "ErrorLog.txt"

int FileExist(char * filePath) {
   return access(filePath, F_OK) != -1; 
}

void WriteLog(const char* module, const char* func, char* desc) {
    char time_buffer[100];
    time_t now = time (0);
    strftime (time_buffer, 100, "%Y-%m-%d %H:%M:%S.000", localtime (&now));
    
    FILE *fp = fopen(FILENAME, "a");  
    if(fp != NULL) {
        fprintf(fp, "Time: %s  \nModule: %s\nFunction: %s\nDescription: %s\n\n", time_buffer, module, func, desc);  
        fclose(fp);
    }
}

void ClearLog() {
    FILE *fp = fopen(FILENAME, "w");  
    if(fp != NULL) {
        fclose(fp);
    }
}

//Returns 0 if file is not found
int file_is_modified(const char *path, time_t oldTime) {
    struct stat file_stat;
    int err = stat(path, &file_stat);
    if (err != 0) {
        WriteLog("Pib.c" , "file_is_modified", "Error when  reading file " FILENAME);
    }
    return file_stat.st_mtime > oldTime;
}


json_t* load_json_file(char *filePath) {
    json_t *json = NULL;
    json_error_t error;

    json = json_load_file(filePath, 0, &error);

    if(!json) {
        WriteLog(__FILE__, __func__, error.text);
    }
    return json;
}

