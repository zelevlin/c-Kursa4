#include "controls.h"
#include "character.h"
#include "../levels/levelManagement/level.h"
#include <ncurses.h>
#include "inputState.h"
#include <stdlib.h>
#include <chrono>
#include <thread>

extern InputState inputState;  // Глобальное состояние ввода

void handleInput(Character &chara, Level &level) {
    int ch;
    bool inputReceived = false;

    // Устанавливаем немедленный возврат из getch() при отсутствии ввода
    timeout(0);

    while ((ch = getch()) != ERR) {
        inputReceived = true;
        switch (ch) {
            case 'a': inputState.left = true; break;
            case 'd': inputState.right = true; break;
            case 'w': inputState.up = true; break;
            case 's': inputState.down = true; break;
            case 'p': inputState.attacking = true; break;
            case ' ': inputState.jumping = true; break;
            case 'i': inputState.inventory = true; break;
            case 'q': inputState.exit = true; break;
        }
    }

    if (inputReceived) {
        processCharacterMovement(chara, level, inputState);
        // Обновление состояния отпускания клавиш
        inputState = {}; // Сброс всех состояний
    }

    // Возвращаем стандартное время ожидания ввода
    timeout(-1);
}

