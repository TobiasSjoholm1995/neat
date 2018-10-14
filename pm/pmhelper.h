#ifndef HEADER_PMHELPER
#define HEADER_PMHELPER

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <jansson.h>

#define FILENAME "ErrorLog.txt"

char* concat(const char *s1, const char *s2);
int file_exist(const char * file_path);
void write_log(const char* module, const char* func, const char* desc);
time_t file_edit_time(const char *file_path);
void clear_log();
int file_is_modified(const char *path, time_t oldTime);
json_t* load_json_file(const char *file_path);
void write_json_file(const char* filePath, json_t *json);

#endif
