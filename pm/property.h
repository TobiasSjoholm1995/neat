#ifndef HEADER_PROPERTY
#define HEADER_PROPERTY

#include <jansson.h>

typedef enum type {
     INTEGER, DOUBLE, BOOLEAN, STRING, RANGE, NULL_VALUE,
}type_t;

typedef struct range
{
    unsigned int low_thresh;
    unsigned int high_thresh;
}range_t;

typedef union value { 
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


property_t *json_to_property_t(json_t *json);
property_list_t *json_to_property_list(json_t *json);
void print_property(property_t *head);              //testing
void print_property_list(property_list_t *head);    //testing

#endif