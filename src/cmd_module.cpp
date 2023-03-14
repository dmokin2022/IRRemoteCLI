// Команды, принимаемые от пользователя: get, set и пр.

#include <Arduino.h>
#include "cmd_module.h"
#include "Utils.h"

CommandHandler_t ucmd;

void cmd_init(CommandHandler_t* cmd)
{
	cmd->uart = &Serial;
	cmd->uart->begin(115200);

	cmd->state = COMMAND_IS_EXPECTED;
	cmd->symbolsCounter = 0;
	cmd_outputPromptMessage(cmd);
}


// Обработка пришедших от пользователя команд
void cmd_process(CommandHandler_t* cmd)
{
	while (cmd->uart->available()) {
		
		uint8_t newSymbol = cmd->uart->read();	// считывание символa
		
		// Если превышен размер командной строки
		if (cmd->symbolsCounter < MAX_COMMAND_STRING_SIZE) {
			cmd->commandString[cmd->symbolsCounter] = newSymbol;	// добавление символа в буфер
			cmd->symbolsCounter++;
		}
		
		// Если нажималась клавиша Enter (CR + LF = возврат каретки + перевод строки)
		if ((newSymbol == '\r') || (newSymbol == '\n')) {
			
			// Если команда не пустая (только нажатие Enter), то будет производиться её обработка
			if (cmd->symbolsCounter > 1) {
				cmd->state = COMMAND_IS_RECEIVED;					
			}

			cmd->commandString[cmd->symbolsCounter - 1] = 0;	// устанавливаем признак конца строки

			cmd->symbolsCounter = 0;
			cmd_outputMessage(cmd, &cmd->commandString[0]);
		}
	}


	switch (cmd->state) {
		case COMMAND_IS_RECEIVED: {

			//debugs("we are going to parse command");
			cmd_execute(cmd, cmd->commandString);
			//debugs("we have parsed command");

			cmd->state = COMMAND_IS_EXPECTED;
		} break;

		case COMMAND_IS_EXECUTED_OK: {
			/* Сообщение об успешном выполнении команды контроллером
			char str[50];
			sprintf(str,
					"Controller %d is in %s: OK",
					controller[controllerNum].nodeID,
					controller[controllerNum].state->textName);
	        sprintf(str, "OK");
			cmd_outputMessage(cmd, str);
			*/
			cmd_outputMessage(cmd, "OK");

			cmd->state = COMMAND_IS_EXPECTED;
		} break;

		/*
		case COMMAND_IS_EXECUTING: {
			// Сообщение о занятости контроллера выполнением предыдущей команды контроллером
			cmd_outputMessage(cmd, "Controller is busy by previous command. Wait...");

			cmd->state = COMMAND_IS_EXPECTED;
		} break;
		*/
	}
}

// Вывод пригласительного сообщения
void cmd_outputPromptMessage(CommandHandler_t* cmd)
{
	//cmd_outputMessage(cmd, ">");
	cmd->uart->print(">");
}

// Вывод произвольного сообщения
void cmd_outputMessage(CommandHandler_t* cmd, char* msg)
{
	cmd_outputPromptMessage(cmd);
	cmd->uart->println(msg);
}

void cmd_execute(CommandHandler_t* cmd, char* commandString)
{
	cmd_parse(cmd, commandString);
	cmd_interpretate(cmd);
}

/*!!! Внимание !!!
В Си++ нельзя использовать команду в формате:
 	cmd_parse(&ucmd, "get state one") - возникает неправильное преобразование к классу строки и
												происходит аппаратный сбой во время выполнения

Нужно использовать форму: 
	char str[] = "get state one";
	cmd_parse(&ucmd, &str[0]) - тогда работает нормально без проблем
*/
// Парсер команд
void cmd_parse(CommandHandler_t* cmd, char* commandString)
{
	// Очистка предыдущих слов команд
	FOR_EACH(cmd->words, i) { 
		cmd->words[i][0] = 0;
	}

    /* strtok — функция стандартной библиотеки языка программирования Си, для поиска лексем в строке. 
	Последовательность вызовов функции разбивает строку на лексемы, разделенные символами разделителями.

	Прототип, описанный в заголовочном файле string.h:

	char *strtok(char *string, const char *delim);
	string — указатель на строку, которую нужно разбить на лексемы. После вызова строка изменяется. Для последующего вызова можно передать NULL, тогда функция продолжит поиск в оригинальной строке.
	delim — указатель на строку, содержащую разделители.
	Функция возвращает указатель на первую найденную лексему в строке. Если лексем не найдено, то возвращается пустой указатель.
	*/
	char *word = strtok(commandString, " "); //во втором параметре указаны разделители (пробел, запятая, точка)
	
    // Разбиваем строку на отдельные слова, разделённые пробелом
    for (int i = 0; word != NULL; i++) {
        strcpy(cmd->words[i], word);
		word = strtok(NULL, " ");
    }
}

