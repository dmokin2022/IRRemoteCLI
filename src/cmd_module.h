#pragma once
//#include "ControllerNet.h"
//#include "cmd.h"
//#include "main.h"
//#include "stm32f7xx_hal_uart.h"
#include <Arduino.h>

#define MAX_COMMAND_STRING_SIZE 100

// Массив для слова команды
#define SIMBOLS_IN_WORD 20
typedef char CmdWords[SIMBOLS_IN_WORD];

#define WORDS_IN_COMMAND 10

#define OE_ OPERATION_ENABLED
#define RSO_ READY_TO_SWITCH_ON

enum states {
	COMMAND_IS_EXPECTED,	// 0
	COMMAND_IS_RECEIVED, 	// 1
	COMMAND_IS_EXECUTING,	//
	COMMAND_IS_EXECUTED_OK 	// 
	
};

// Структура объекта для обработки команд от UART
// (структура использутся так как требуется 2 экземпляра одинаково рабаотающих приёмника
// команд через UART1 и UART2)
typedef struct _CommandHandler {
    //UART_HandleTypeDef huart;
	HardwareSerial* uart;
	
    char commandString[MAX_COMMAND_STRING_SIZE];    // буфер хранения пришедшей командной строки
    // Массив слов, входящих в строку команды
    CmdWords words[WORDS_IN_COMMAND];

    uint8_t state;  // состояние приёмника пользовательских команд

    uint8_t symbolsCounter;	// счётчик принятых символов

} CommandHandler_t;

extern CommandHandler_t ucmd;

void cmd_init(CommandHandler_t* cmd);

// Обработка пришедших от пользователя команд
void cmd_process(CommandHandler_t* cmd);

// Парсер команд
void cmd_parse(CommandHandler_t* cmd, char* commandString);

void cmd_interpretate(CommandHandler_t* cmd);

void cmd_execute(CommandHandler_t* cmd, char* commandString);

// Вывод произвольного сообщения для подтверждения/ошибки
void cmd_outputMessage(CommandHandler_t* cmd, char* msg);

// Вывод пригласительного сообщения
void cmd_outputPromptMessage(CommandHandler_t* cmd);