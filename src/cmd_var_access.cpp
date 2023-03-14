#include "cmd_var_access.h"
#include <string.h>
#include <stdio.h>
#include "Utils.h"
#include <Arduino.h>

uint8_t var1 =  1;
uint16_t var2 = 22;
uint32_t var3 = 4444;
double  var4 =  88888888;
char str1[] =  "This is a test string";

#include "cmd_var_table.h"

var_access_handler vars;

accessible_variable_t* getVariableItemFromTableByName(char* name)
{
	Serial.println(name);

    // Проход по всей таблице переменных
    FOR_EACH(accessible_variables_table, i) {
        // Если строки совпали, то выходим из цикла поиска и функции
        if (strcmp(&(accessible_variables_table[i].name[0]), name) == 0) {
            return &accessible_variables_table[i];
			Serial.print("Найдено совпадение "); debug(i);
        }
		debug(i);
    }

    // Если совпадиний не найдено, возвращает нулевой указатель
    return 0;
}


char text_buf[40];

// Выдаёт строку формата "имя_переменной = значение \n"
char* getVar(char* name)
{
    // поиск записи переменной по имени
	debugs(Поиск переменных в таблице);

    accessible_variable_t* var_item = getVariableItemFromTableByName(name);

	debug((uint32_t)var_item);
    
	//debug(var_item->ptr);
	debug(var_item->name);
	debug(var_item->size);

	// Предварительная очистка текстового буфера от предыдущих данных
    text_buf[0] = 0;

	
    // Если переменная найдена в таблице
    if (var_item) {
        switch (var_item->size) {
            case UINT8_T:  sprintf (&text_buf[0], "%s = %u \n", var_item->name, *((uint8_t*)var_item->ptr)); break;
            case UINT16_T: sprintf (&text_buf[0], "%s = %u \n", var_item->name, *((uint16_t*)var_item->ptr)); break;
            case UINT32_T: sprintf (&text_buf[0], "%s = %u \n", var_item->name, *((uint32_t*)var_item->ptr)); break;
            case DOUBLE_T: sprintf (&text_buf[0], "%s = %f \n", var_item->name, *((double*)var_item->ptr)); break;
        }

		if (IS_STRING(var_item)) {
           sprintf (&text_buf[0], "%s = %s \n", var_item->name, ((char*)var_item->ptr));
		}
    }
	

	debug(text_buf[0]);
    
	return &text_buf[0];

}

void setVar(char* name, char* value)
{
    // поиск записи переменной по имени
    accessible_variable_t* var_item = getVariableItemFromTableByName(name);

    // Предварительная очистка текстового буфера от предыдущих данных
    vars.text_buf[0] = 0;

    // Если переменная найдена в таблице
    if (var_item) {
		// Присвоение простых типов
        switch (var_item->size) {
            case UINT8_T:  *((uint8_t*)var_item->ptr) = (uint8_t) atoi(value); break;
            case UINT16_T: *((uint16_t*)var_item->ptr) = (uint16_t) atoi(value); break;
            case UINT32_T: *((uint32_t*)var_item->ptr) = (uint32_t) atoi(value); break;
            case DOUBLE_T: *((double*)var_item->ptr) = (double) atof(value); break;
        }

		// Присвоиение строки
        if (IS_STRING(var_item)) {
			// Если новая строка по длине превосходит предыдущую (исходную), то 
			// копируется только до исходного размера (не более)
			int length_new = strlen(value);
			int length_old = STRLEN(var_item);
			if ((length_new) > var_item->size) length_new = length_old;
			memcpy(var_item->ptr, value, length_new);
			((char*)var_item->ptr)[length_new] = 0; // обозначение конца строки

			// Внимание!!! При таком алгоритме строка с каждым изменением может уменьшаться. Продумать этот момент
			// TODO
		}
    }
}


