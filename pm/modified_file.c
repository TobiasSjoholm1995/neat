#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
//#include <unistd.h>

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
contain(time_file_list_t *head, const char *filename)
{
    time_file_list_t *current = head;
    while(current != NULL) {
        if(current->time_file->filename == filename)
           { return true; }
        current = current->next;
    }
    return false;
}

time_file_t*
create_time_file(const char *filename) 
{
    time_file_t *tfile = malloc(sizeof(time_file_t));
    tfile->filename = filename;
    tfile->last_modified = time(0);
    return tfile;
}

//add at the start of the list
time_file_list_t*
add_file(time_file_list_t *head, const char *filename)
{
    time_file_list_t *new_head = malloc(sizeof(time_file_list_t));
    new_head->time_file = create_time_file(filename);

    new_head->next = head;
    return new_head;
}

void
update_time(time_file_list_t *head, const char *filename)
{
    time_file_list_t *current = head;
    while(current != NULL) {
        if(current->time_file->filename == filename) { 
            current->time_file->last_modified = time(0);
            return;
        }
        current = current->next;
    }
}

time_t
get_time_file(time_file_list_t *head, const char *filename)
{
    time_file_list_t *current = head;
    while(current != NULL) {
        if(current->time_file->filename == filename) {          
            return current->time_file->last_modified;
        }
        current = current->next;
    }
    return 0;
}

void
print_list(time_file_list_t *head) {
    time_file_list_t *current = head;
    while(current != NULL) {
        printf("File: %s\n", current->time_file->filename);
        printf("Time: %s\n", ctime(&current->time_file->last_modified));
        current = current->next;
    }
}


/*
int main() {

    time_file_list_t *head = NULL;
    head = add_file(head, "test1.c");
    head = add_file(head,"test2.c");
    print_list(head);
    printf("-------\n");
    usleep(10000000);
    update_time(head,"test2.c");
    print_list(head);

    return 0;
}*/
