#ifndef HEADER_MODIFIED_FILE
#define HEADER_MODIFIED_FILE
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

typedef struct time_file {
    const char *filename;
    time_t last_modified;
}time_file_t;

typedef struct time_file_list
{
    time_file_t *time_file;
    struct time_file_list *next;
} time_file_list_t;

bool contain(time_file_list_t *head, const char *filename);
time_file_t* create_time_file(const char *filename);
time_file_list_t* add_file(time_file_list_t *head, const char *filename);
void update_time(time_file_list_t *head, const char *filename);
time_t get_time_file(time_file_list_t *head, const char *filename);
void print_list(time_file_list_t *head);

#endif
