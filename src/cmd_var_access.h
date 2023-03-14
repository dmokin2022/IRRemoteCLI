#include <stdint.h>

#define VAR(x) {#x, &(x), sizeof(x)}
#define STR(x) {#x, &(x), strlen(x) + sizeof(double) + 1}

#define STRLEN(x) x->size - sizeof(double) - 1
#define IS_STRING(x) (x->size > DOUBLE_T)


typedef struct _accessible_variable_t {
    char name[30];
    void* ptr;
    uint8_t size;
} accessible_variable_t;

typedef struct _var_access_handler {
    accessible_variable_t* table;
    uint16_t items_in_table;
    char text_buf[40];  // буфер для подготовки строки типа "var = xxx"


} var_access_handler;


#define    UINT8_T  1
#define    UINT16_T 2
#define    UINT32_T 4
#define    DOUBLE_T 8
#define	   STRING_T DOUBLE_T+1



/*
enum TYPE_BY_SYZE {
    UINT8_T =  1,
    UINT16_T = 2,
    UINT32_T = 4,
    DOUBLE_T = 8,
	STRING_T = DOUBLE_T + 1,
};
*/


char* getVar(char* name);

void setVar(char* name, char* value);

