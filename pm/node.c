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


bool
contain(node_t *head, const char *file_path)
{
    if(file_path == NULL) { return NULL; }

    node_t *current = head;
    while(current != NULL) {
        if(strcmp(current->filename,file_path) == 0) { 
            return true; 
        }
        current = current->next;
    }
    return false;
}

//add at the start of the list
node_t*
add_node(node_t *head, node_t *node)
{
    if(node == NULL) { return head; }

    node_t *new_head = node; 
    new_head->next = head;
    return new_head;
}

void
remove_node(node_t **head, const char *file_path)
{
    if(head == NULL || *head == NULL || file_path == NULL) { return ; }

    node_t *current = *head;
    node_t *previous = NULL;

    while(current != NULL) {
        if(strcmp(current->filename,file_path) == 0) {
            if(previous == NULL) {
                *head = current->next;               
            } 
            else {
                previous->next = current->next;
            }   
            free(current);      
            break;
        }
        previous = current;
        current = current->next;
    }
}

node_t*
get_node(node_t *head, const char *file_path)
{
    if(file_path == NULL) { return NULL; }

    node_t *current = head;
    while(current != NULL) {
        if(strcmp(current->filename, file_path) == 0) { 
            return current;
        }
        current = current->next;
    }
    return NULL;
}

//testing
void print_node(node_t* head) 
{
    node_t *current = head;
    while(current != NULL) {   
        printf("--------NODE----------\n");
        if(current->uid != NULL) { printf("uid: %s\n", current->uid); }
        printf("priority: %d\n", current->priority);
        printf("replace_matched: %d\n", current->replace_matched);
        printf("last_updated: %s",ctime(&current->last_updated));
        if(current->filename != NULL) { printf("filename: %s\n", current->filename); }
        if(current->match != NULL) {
            printf("match: \n");
            print_property(current->match);
        }
        if(current->properties != NULL) {
            printf("properties: \n");
            print_property_list(current->properties);
        }
        printf("\n");
        current= current->next;
    }
}
