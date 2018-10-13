#include <stdio.h>
#include <time.h>
#include <jansson.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dirent.h>
#include "pmhelper.h"
#include "property.h"
#include "modified_file.h"

typedef struct neat_policy_s {
    const char *uid;
    int priority;
    bool replace_matched;
    char *filename;
    time_t time;
    property_t *match;              //PropertyList
    property_list_t *properties;    //PropertyMultiList
} neat_policy_t;

time_file_list_t *time_file_list;

//return NULL if file not found
json_t* 
load_file(const char *file_path) 
{
    json_t *json = load_json_file(file_path);
    
    if(json != NULL) {
        if(!contain(time_file_list, file_path))
            time_file_list = add_file(time_file_list, file_path);
        else
            update_time(time_file_list,file_path);
    }
    return json;
}



void
read_all_modified_files(const char * dir_path) 
{
    DIR *dir;
    struct dirent *ent;
    int file_type = 8;

    if ((dir = opendir (dir_path)) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            if (ent->d_type == file_type) {
                const char * full_path = concat(concat(dir_path, "/"), ent->d_name);                                  
                if(get_time_file(time_file_list, full_path) <= get_edit_time(full_path)) {
                    load_file(full_path); //update file
                }      
            }
        }
        closedir (dir);
    } else {
        write_log(__FILE__, __func__, concat("Error: Can't read the directoty ", dir_path));    
    }
}

neat_policy_t*
json_to_neat_policy(json_t *json)
{
    neat_policy_t *policy = malloc(sizeof(neat_policy_t));

    policy-> uid = json_string_value(json_object_get(json, "uid"));    
    policy->priority = json_real_value(json_object_get(json, "priority"));
    policy->replace_matched = json_boolean_value(json_object_get(json, "replace_matched"));
    policy-> match = json_to_property_t(json_object_get(json, "match")); 
    policy-> properties = json_to_property_list(json_object_get(json, "properties")); 
    return policy;
}

neat_policy_t * 
neat_policy_init(char * filePath) 
{
    neat_policy_t *policy = NULL;
    json_t *json = load_file(filePath);

    if(json != NULL) {
        policy = json_to_neat_policy(json);
    }  
    if(policy  == NULL) {
        return NULL;  //error
    }
    
    policy->filename = filePath;
    policy->time = time(NULL);

    return policy;
}

//testing
void 
print_property(property_t *head) 
{
    property_t *current = head;
    while(current != NULL) {
        printf("\t%s:\n", current->key);
        printf("\t\tprecedence: %d, ", current-> precedence);
        printf("score: %d, ", current-> score);
        printf("type: %d, ", current-> type);

        if(current->type == INTEGER) printf("value: %d", current->value->integer);    
        if(current->type == DOUBLE) printf("value: %f", current->value->double_t);
        if(current->type == STRING) printf("value: %s", current->value->string);
        if(current->type == BOOLEAN) printf("value: %s", current->value->boolean ? "true": "false");
        if(current->type == RANGE) printf("value: range(%d,%d)", current->value->range.low_thresh, current->value->range.high_thresh); 
        if(current->type == NULL_VALUE) printf("value: %s", "null"); 

        printf("\n");
        current = current->next;
    }
}


//testing
void 
print_property_list(property_list_t *head) 
{
    property_list_t *current = head;
    while(current != NULL) {
        print_property(current->property);              
        current = current->next;
    }
}

//testing
int main() 
{
    neat_policy_t *policy = neat_policy_init("/home/free/Downloads/neat-TobiasSjoholm1995-patch-1/pm/JsonFiles/cib/enp0s3.cib");

    if(policy != NULL) {   
        printf("uid: %s\n", policy->uid);
        printf("priority: %d\n", policy->priority);
        printf("replace_matched: %d\n", policy->replace_matched);
        printf("time: %s \n",ctime(&policy->time));
        printf("filename: %s\n", policy->filename);
        printf("match: \n");
        print_property(policy->match);
        printf("properties: \n");
        print_property_list(policy->properties);
    }
    else { printf("null error"); }
    
    printf("\n----------------\nTime file list: \n");
    print_list(time_file_list);
  
    return 0;
}


