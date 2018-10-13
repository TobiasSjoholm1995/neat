#include <stdio.h>
#include <time.h>
#include <jansson.h>
#include "pmhelper.h"
#include <stdbool.h>
#include <stdlib.h>
#include "property.h"

typedef struct neat_policy_s {
    const char *uid;
    int priority;
    bool replace_matched;
    char *filename;
    time_t time;
    property_t *match;              //PropertyList
    property_list_t *properties;    //PropertyMultiList
} neat_policy_t;

time_t last_loaded_time;

//return NULL if file not found
json_t* 
load_file(char *filePath) 
{
    //ToDo: set last loaded time
    //last_loaded_time = time(0);

    json_t *json = load_json_file(filePath);
    return json;
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

int main() 
{
    neat_policy_t *policy = neat_policy_init("/home/free/Downloads/neat-TobiasSjoholm1995-patch-1/pm/JsonFiles/pib/default.profile");

    if(policy != NULL) {   
        printf("uid: %s\n", policy->uid);
        printf("priority: %d\n", policy->priority);
        printf("replace_matched: %d\n", policy->replace_matched);
        printf("time: %ld (weird format,lazy)\n", policy->time);
        printf("filename: %s\n", policy->filename);
        printf("match: \n");
        print_property(policy->match);
        printf("properties: \n");
        print_property_list(policy->properties);
    }
    else { printf("null error"); }
  
    return 0;
}


