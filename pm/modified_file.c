#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct time_file {
    const char *filename;
    time_t last_modified;
}time_file_t;

typedef struct time_file_list
{
    time_file_t *time_file;
    struct time_file_list *next;
} time_file_list_t;


bool
contain(time_file_list_t *head, const char *file_path)
{
    time_file_list_t *current = head;
    while(current != NULL) {
        if(strcmp(current->time_file->filename,file_path) == 0) { 
            return true; 
        }
        current = current->next;
    }
    return false;
}

time_file_t*
create_time_file(const char *file_path) 
{
    time_file_t *tfile = malloc(sizeof(time_file_t));
    tfile->filename = file_path;
    tfile->last_modified = time(0);
    return tfile;
}

//add at the start of the list
time_file_list_t*
add_file(time_file_list_t *head, const char *file_path)
{
    time_file_list_t *new_head = malloc(sizeof(time_file_list_t));
    new_head->time_file = create_time_file(file_path);
    new_head->next = head;
    return new_head;
}

void
update_time(time_file_list_t *head, const char *file_path)
{
    time_file_list_t *current = head;
    while(current != NULL) {
        if(strcmp(current->time_file->filename, file_path) == 0) { 
            current->time_file->last_modified = time(0);
            return;
        }
        current = current->next;
    }
}

//returns 0 if file does not exist in list
time_t
get_time_file(time_file_list_t *head, const char *file_path)
{
    time_file_list_t *current = head;
    while(current != NULL) {
        if(strcmp(current->time_file->filename,file_path) == 0) {                    
            return current->time_file->last_modified;
        }
        current = current->next;
    }
    return 0;
}

//testing
void
print_list(time_file_list_t *head) {
    time_file_list_t *current = head;
    while(current != NULL) {
        printf("File: %s\n", current->time_file->filename);
        printf("Time: %s\n", ctime(&current->time_file->last_modified));
        current = current->next;
    }
}
