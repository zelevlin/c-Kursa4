#ifndef INPUT_STATE_H
#define INPUT_STATE_H

struct InputState {
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool attacking = false;
    bool jumping = false;  
    bool inventory = false;  
    bool exit = false;

    //флаги для удержания клавиш
    bool upPressed = false;
    bool downPressed = false;
    bool leftPressed = false;
    bool rightPressed = false;
};

#endif // INPUT_STATE_H
