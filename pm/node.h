#ifndef HEADER_MODIFIED_FILE
#define HEADER_MODIFIED_FILE

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#include "property.h"

typedef struct node {
    const char *filename;
    time_t last_updated;

    const char *uid;
    int priority;
    bool replace_matched;        
    property_t *match;              //PropertyList
    property_list_t *properties;    //PropertyMultiList

    struct node *next;
}node_t;


bool contain(node_t *head, const char *file_path);

//add at the start of the list
node_t* add_node(node_t *head, node_t *node);

void remove_node(node_t **head, const char *file_path);

node_t* get_node(node_t *head, const char *file_path);

//testing 
void print_node(node_t* head);

#endif

