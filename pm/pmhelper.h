#ifndef HEADER_PMHELPER
#define HEADER_PMHELPER

char* concat(const char *s1, const char *s2);
int file_Exist(const char * filePath);
void write_log(const char* module, const char* func, char* desc);
time_t get_edit_time(const char *filename);
void clear_log();
int file_is_modified(const char *path, time_t oldTime);
json_t* load_json_file(const char *filePath);
int write_json_file(const char* filePath, json_t *json);

#endif
