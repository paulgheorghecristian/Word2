#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include "display.h"
#include <glm/glm.hpp>

class Input
{
    public:
        Input();
        void update(SDL_Window*);
        bool GetKeyDown(int);
        bool GetKeyUp(int);
        bool GetKey(int);
        bool GetMouseDown(int);
        bool GetMouseUp(int);
        bool GetMouse(int);
        glm::vec2 GetMousePos();
        glm::vec2 GetMouseDelta();
        virtual ~Input();
    protected:
    private:
        static const int NUMKEYS = 1024;
        static const int NUMMOUSE = 64;
        bool inputs[NUMKEYS];
        bool downKeys[NUMKEYS];
        bool upKeys[NUMKEYS];
        bool inputMouse[NUMMOUSE];
        bool downMouse[NUMMOUSE];
        bool upMouse[NUMMOUSE];
        glm::vec2 mousePosition, lastMousePosition, delta;
};

#endif // INPUT_H
