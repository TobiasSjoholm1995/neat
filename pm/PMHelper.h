#ifndef HEADER_FILE
#define HEADER_FILE

int FileExist(char * filePath);
void WriteLog(const char* module, const char* func, char* desc);
void ClearLog();
int file_is_modified(const char *path, time_t oldTime);
json_t* load_json_file(char *filePath);

#endif
