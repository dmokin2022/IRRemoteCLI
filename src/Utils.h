/**
  ******************************************************************************
  * @file    Utils.h
  * @author  Мокин Д. В. (dimamokin@yandex.ru)
  * @brief   Файл с макросами, используемыми в проекте
  *
  *   Макроопределения для удобного краткого написания некоторых определений и
  *   функций
  *
  ******************************************************************************
  */

#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include "stdint.h"
#include "string.h"

// Макросы для отладки (трассировки) через последовательный интерфейс (UART)

//#define DEBUG_ENABLE
//#define DEBUG_TARGET_MILANDR
//#define ARDUINO

#ifdef ARDUINO
    // Для Arduino
    #define debug(x) Serial.print(#x); Serial.print(" = "); Serial.println(x)
    #define debugs(x) Serial.println(#x)
    #define debugln(x) Serial.println(x)

	#define PRINT(x) Serial.print(x)
	#define PRINTLN(x) Serial.println(x)
	#define BUTTON_PRESSED(x) (digitalRead(x) == 1)

#endif

#ifdef ARM
    // Для контроллера Миландр (с библиотекой usart_driver)
    //#define debug(x) usart_print(uart1, #x); usart_print(uart1, " = "); usart_print_value(uart1,(uint32_t)x)
    //#define debugs(x) usart_println(uart1, #x)
    //#define debugln(x) usart_print_value(uart1, (uint32_t)x); usart_println(uart1, "")
    #define blink(pin, times) for (int i = 0; i < times; i++) {port_toggle_pin(pin);}

	//#include "usart/usart_driver.h"
	//extern usart_handle_s   usart2;
	//extern CommandHandler_t cmd;
	//#define CURRENT_USART cmd.usart
	#define OUTPUT_USART &usart3
	#define PRINT(x) usart_put_message(OUTPUT_USART, x, strlen(x))

	#define PRINTLN(x) do {usart_put_message(OUTPUT_USART, (const uint8_t*)x, strlen(x)); usart_put_message(OUTPUT_USART, (const uint8_t*)"\r\n", 2);}while(0)

	#define PIN(port_letter, pin_number) PORT##port_letter, pin_number
	//#define BUTTON_PRESSED(x) (digitalRead(x) == 1)

#endif

#ifdef NO_DEBUG
    // Отключение трассировки
    #define debug(x)
    #define debugs(x)
    #define debugln(x)
#endif


    
// Макрос для проверки указателя на функцию перед вызовом и вызова если он не нулевой
// нужно для безоспасности кода с указателями
#define CALL_IF_NOT_NULL(func) if (func) func();
//#define CALL_BY_POINTER(func) if (func) func();
// Макрос для передачи в качестве параметров указателя на массив (таблицу) с его размером
// (в байтах, то есть внутри принимающей функции должен быть
// пересчёт в строки sizeof(table)/sizeof(tableType) )
#define TABLE_WITH_SIZE(table) &table[0], sizeof(table)
#define TABLE(table) &table[0], sizeof(table)/sizeof(table[0])
#define OBJECT(table) &table, sizeof(table)
#define OBJECT0(table) &table[0], sizeof(table)
#define STR_WITH_LEN(string) &string, strlen((const char*)string)

// Макрос для вычисления количества строк в произвольной таблице (массиве)
// Нужен для прохождения по всем строкам таблицы в цикле
#define ITEMS_IN(table) sizeof(table)/sizeof(table[0])
#define LAST_ITEM_OF(table) &table[ITEMS_IN(table) - 1]
#define FIRST_ITEM_OF(table) &table[0]

#define FOR_(n, i)          for (int i = 0; i < n; i++)
#define FOR_EVERY(array, i) for (int i = 0; i < (sizeof(array)/sizeof(array[0])); i++)
#define FOR_EACH(array, i) for (int i = 0; i < (sizeof(array)/sizeof(array[0])); i++)

// Макрос для вставки в структуру символьного имени и его текстового строки
#define CODANTEX(code) code, #code

#define DELAY(time) for (uint32_t i = 0; i < time; i++) {__asm__ ("nop")}

#define NOT_FOUND -1

// Макрос поиска записи в таблице записей по выбранному полю = нужному значению
#define FIND_FIRST_item_IN_table_WHERE_field_EQUALS_value1(item, table, field, value)	\
	do {												\
		item = 0;										\
		int16_t index = NOT_FOUND;						\
		FOR_EACH(table, i) { 							\
			if (table[i].field == value) { 				\
				index = i;								\
				break; 									\
			} 											\
		}												\
		if (index != NOT_FOUND) item = &table[index]; 	\
	} while (0);


// item - куда возвращает указатель на строку таблицы
// table - имя самой таблицы
// field - по какому полю происходит поиск
// value - с каким значением сравнивается поле
#define FIND_FIRST_item_IN_table_WHERE_field_EQUALS_value(item, table, field, value)	\
	do {												\
		item = 0;										\
		int16_t index = NOT_FOUND;						\
		FOR_EACH(table, i) { if (table[i].field == value) { index = i; break; }	}		\
		if (index != NOT_FOUND) item = &table[index]; 	\
	} while (0);



// Макрос для оформления блоков функций в IDE, поддерживающий навигация Outline по функциям в модуле *.c/cpp
#define DELIMITER(title) static void ______________##title##________________();
#define DECOR(title) static void ______________##title##________________();
#define LDECOR(title) static void _##title##___________________();

typedef volatile union {
    uint32_t DWord;// Доступ к 4-байтному слову
    float Float; // Доступ к вещественному значению (4 байта)
    uint8_t Byte[4]; // Доступ к отдельным байтам
    uint16_t Word[2]; // Доступ к отдельным словам
} TVariant32;


// Структура для типового объекта (возвращается функциями, которые создают объекты)
typedef struct _object {
    void* ptr;
    uint16_t size;
    uint16_t type;
} object;




//extern void blink(int pin);

#endif // UTILS_H_INCLUDED
