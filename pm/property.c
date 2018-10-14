#include <stdio.h>
#include <jansson.h>
#include <stdbool.h>

typedef enum type 
{
     INTEGER, DOUBLE, BOOLEAN, STRING, RANGE, NULL_VALUE,
}type_t;

typedef struct range
{
    unsigned int low_thresh;
    unsigned int high_thresh;
}range_t;

typedef union value 
{ 
    int integer; double double_t; bool boolean; const char *string; range_t range;
}value_t;

typedef struct property
{
    const char *key;
    int precedence;
    int score;
    value_t *value;
    type_t type;
    struct property *next;
} property_t;

typedef struct property_list
{
    property_t *property;
    struct property_list *next;
} property_list_t;

property_t*
json_prop_to_propt_t(json_t *json_prop)
{
    return NULL;
}

type_t
json_to_type_t(json_t *json) 
{
    if(json_is_null(json))
        return NULL_VALUE;
    else if(json_is_integer(json)) 
        return INTEGER;
        else if(json_is_real(json)) 
        return DOUBLE;
    else if(json_is_boolean(json))
        return BOOLEAN;
    else if(json_is_string(json))
        return STRING;
    else if(json_is_object(json))
         return RANGE;      //assume range?

    return NULL_VALUE;  //unknown type, error handle?
}

value_t*
json_to_value_t(json_t *json) 
{
    value_t *my_value = malloc(sizeof(value_t));
    if(json_is_null(json)) {
        my_value = NULL;
    }
    else if(json_is_integer(json)) {
        my_value->integer = json_integer_value(json);
    }
    else if(json_is_real(json)) {
        my_value->double_t = json_real_value(json);
    }
    else if(json_is_boolean(json)) {
        my_value->boolean = json_boolean_value(json);
    }
    else if(json_is_string(json)) {
        my_value->string = json_string_value(json);
    }
    else if(json_is_object(json)) { //assume range
        range_t r;
        r.low_thresh = json_integer_value(json_object_get(json, "start"));
        r.high_thresh = json_integer_value(json_object_get(json, "end"));
        my_value->range = r;
    }
    //mo error handling, just return null
    return my_value;
}

property_t*
json_to_property_t(json_t * json)
{
    if(json == NULL || !json_is_object(json)) { return NULL; }

    property_t *head = malloc(sizeof(property_t));
    property_t *current = head;
    
    void *iter = json_object_iter(json);
    while(iter)
    {
            
        current->key = json_object_iter_key(iter);
        current->precedence = json_integer_value(json_object_get(json_object_iter_value(iter), "precedence"));
        current->score = json_integer_value(json_object_get(json_object_iter_value(iter), "score"));
        current->type = json_to_type_t(json_object_get(json_object_iter_value(iter), "value"));
        current->value = json_to_value_t(json_object_get(json_object_iter_value(iter), "value"));            

        if(json_object_iter_next(json,iter) != NULL) {    
            current-> next =  malloc(sizeof(property_t));
            current = current->next;    
        }    
        iter = json_object_iter_next(json, iter);       
    }
    current->next = NULL; //last item in the linked is null

    return head;
}

property_list_t*
json_array_to_property_list(property_list_t* current,json_t *json)
{
    size_t n = json_array_size(json);
    size_t index;
    json_t *value;
    
    json_array_foreach(json, index, value) {
        if(json_is_array(value)) {
            current = json_array_to_property_list(current, value);      
        }
        else {
            current->property = json_to_property_t(value);
        }
        if(index < n - 1) {
            current->next =  malloc(sizeof(property_list_t));
            current = current->next;
        }      
    }
    current->next = NULL;

    return current;    
}

property_list_t*
json_to_property_list(json_t *json) 
{
    if(json == NULL) { return NULL; }

    property_list_t *properties = NULL;
   
    if(json_is_object(json)) {
        properties = malloc(sizeof(property_list_t));
        properties->property = json_to_property_t(json);
        properties->next = NULL;
    }
    else if(json_is_array(json) && json_array_size(json) > 0) {
        properties = malloc(sizeof(property_list_t));
        json_array_to_property_list(properties, json);
    }

    return properties;
}



bool
has_property(property_t *list, property_t check)
{
    return true;
}

property_t *
add_property(property_t *list, property_t to_add)
{
    return NULL;
}

property_t *
overwrite_property_value(property_t *list, property_t check)
{
    return NULL;
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