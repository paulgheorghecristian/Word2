#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include "display.h"
#include <glm/glm.hpp>

class Input
{
    public:
        Input();
        void update(Display*);
        bool getKeyDown(int);
        bool getKeyUp(int);
        bool getKey(int);
        bool getMouseDown(int);
        bool getMouseUp(int);
        bool getMouse(int);
        glm::vec2 getMousePos();
        glm::vec2 getMouseDelta();
        virtual ~Input();
    protected:
    private:
        static const int NUM_KEYS = 100;
        static const int NUM_MOUSE = 10;
        bool inputs[NUM_KEYS];
        bool downKeys[NUM_KEYS];
        bool upKeys[NUM_KEYS];
        bool inputMouse[NUM_MOUSE];
        bool downMouse[NUM_MOUSE];
        bool upMouse[NUM_MOUSE];
        glm::vec2 mousePosition, lastMousePosition, delta;
};

#endif // INPUT_H
