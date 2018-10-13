#ifndef HEADER_MODIFIED_FILE
#define HEADER_MODIFIED_FILE

typedef struct time_file {
    const char *filename;
    time_t last_modified;
}time_file_t;

typedef struct time_file_list
{
    time_file_t *time_file;
    struct time_file_list *next;
} time_file_list_t;


bool contain(time_file_list_t *head, const char *file_path);

time_file_t* create_time_file(const char *file_path);

//add at the start of the list
time_file_list_t* add_file(time_file_list_t *head, const char *file_path);

void remove_file(time_file_list_t **head, const char *file_path);

void update_time(time_file_list_t *head, const char *file_path);

//returns 0 if file does not exist in list
time_t get_time_file(time_file_list_t *head, const char *file_path);

void print_list(time_file_list_t *head); //testing

#endif

