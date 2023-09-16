// Интерпретатор команд
#include "cmd_module.h"
#include "RemotePult.h"

extern RemotePult *remotePult;

void cmd_interpretate(CommandHandler_t* cmd);