#include <stdio.h>
#include <time.h>
#include <jansson.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dirent.h>

#include "pmhelper.h"
#include "node.h"

node_t *head_node = NULL;

void
json_to_node(node_t *node, json_t *json) 
{
    if(json != NULL && node != NULL) {
        node-> uid = json_string_value(json_object_get(json, "uid"));    
        node->priority = json_real_value(json_object_get(json, "priority"));
        node->replace_matched = json_boolean_value(json_object_get(json, "replace_matched"));
        node-> match = json_to_property_t(json_object_get(json, "match")); 
        node-> properties = json_to_property_list(json_object_get(json, "properties"));
    }
}

node_t * 
create_node(const char * file_path) 
{
    json_t *json = load_json_file(file_path);

    if(json == NULL) { return NULL; }

    node_t *node = malloc(sizeof(node_t));
    json_to_node(node, json);
    node->filename = file_path;
    node->last_updated = time(0);
    node->next = NULL;

    return node;
}

void
update_node(const char * file_path) 
{
    node_t *node = get_node(head_node, file_path);

    if(node != NULL) {
        if(node->last_updated <= file_edit_time(file_path)) {
            json_to_node(node, load_json_file(file_path)); //update file
            node->last_updated = time(0);
        }
    }
    else {  //node does not exist, create it
        head_node = add_node(head_node, create_node(file_path));
    }   
}


void
read_all_modified_files(const char *dir_path) 
{
    DIR *dir;
    struct dirent *ent;
    int file_type = 8;

    if ((dir = opendir (dir_path)) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            if (ent->d_type == file_type) {
                update_node(concat(concat(dir_path, "/"), ent->d_name));                
            }
        }
        closedir (dir);
    } else {
        write_log(__FILE__, __func__, concat("Error: Can't read the directoty ", dir_path));    
    }
}

//testing
int main() 
{
    read_all_modified_files("/home/free/Downloads/neat-TobiasSjoholm1995-patch-1/pm/JsonFiles/pib");
    print_node(head_node);
    
    
    //printf("\n----------------\nTime file list: \n");
    //print_list(time_file_list);
  
    return 0;
}


