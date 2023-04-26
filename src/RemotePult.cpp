
#include "RemotePult.h"
#include "Utils.h"

// Поиск совпадения кода нажатой кнопки со значениями в таблице
uint8_t RemotePult::findPositionInTable(uint32_t _buttonCode) {
    uint8_t i;

    // Проход по всей таблице в поиске совпадающего кода
    for (i = 0; i < pultButtonTableSize; i++) {
        if (_buttonCode == (pultButtonTable + i)->buttonCode) {
            break;
        }
    }

    // Если код обнаружен в таблице
    if (i < pultButtonTableSize) return i;
    else return NOT_FOUND;

}


// Базовая функция работы пульта
// приём команд с пульта и их интерпретация
void RemotePult::process() {
    uint8_t i;
    // Если таблица соответствия кодов не определена
    if (pultButtonTable == 0) return;
    
    // Если данные не пришли
    //if (!decode()) return;

        // Если данные пришли
        if (decode()) {
           
            newButtonCode = decodedIRData.decodedRawData;
            
            // Сохранение времени последнего прихода данных
            lastButtonTime = millis();

            if (state == KEY_NO) {
                buttonPositionInTable = findPositionInTable(newButtonCode);
                if (buttonPositionInTable == NOT_FOUND) {

                } else {
                    state = KEY_PRESSED;
                    //debug(state);
                    CALL_IF_NOT_NULL(pultButtonTable[buttonPositionInTable].onButtonPressed);

                    lastButtonCode = newButtonCode;
                }
            } else if (state == KEY_PRESSED) {
                if ((newButtonCode == 0x0)) {
                    state = KEY_AUTOREPEAT;
                    //debug(state);
                    
                    CALL_IF_NOT_NULL(pultButtonTable[buttonPositionInTable].onButtonAutorepeat);
                }
            } else if (state == KEY_AUTOREPEAT) {
                //debug(state);
                //debug(buttonPositionInTable);
                CALL_IF_NOT_NULL(pultButtonTable[buttonPositionInTable].onButtonAutorepeat);
            }

            resume(); // принимаем следующую команду
        // Если данные не приходили
        } else {
            if ((state == KEY_PRESSED) || (state == KEY_AUTOREPEAT)) {
                timeEllapsed = millis() - lastButtonTime;
                if (timeEllapsed > 200) {
                    state = KEY_RELEASED;
                    //debug(state);
                    //debug(timeEllapsed);
                    CALL_IF_NOT_NULL(pultButtonTable[buttonPositionInTable].onButtonReleased);
                    state = KEY_NO;
                    //debug(state);
                }
            }
        }
}

