#include "input.h"

Input::Input() : mousePosition(0.0)
{
    for(unsigned int i = 0; i < NUMKEYS; i++){
        downKeys[i] = false;
        upKeys[i] = false;
        inputs[i] = false;
    }
    for(unsigned int i = 0; i < NUMMOUSE; i++){
        downMouse[i] = false;
        upMouse[i] = false;
        inputs[i] = false;
    }
}

Input::~Input()
{
    //dtor
}

void Input::update(SDL_Window *window){
    SDL_Event event;

    for(unsigned int i = 0; i < NUMKEYS; i++){
        downKeys[i] = false;
        upKeys[i] = false;
    }
    for(unsigned int i = 0; i < NUMMOUSE; i++){
        downMouse[i] = false;
        upMouse[i] = false;
    }

    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_QUIT:{
                Display::isWindowClosed = true;
                break;
            }
            case SDL_KEYDOWN:{
                if(event.key.keysym.sym < 1024 && event.key.keysym.sym >= 0){
                    downKeys[event.key.keysym.sym] = true;
                    inputs[event.key.keysym.sym] = true;
                }else if(event.key.keysym.sym >= 1073741903){
                    int key = event.key.keysym.sym - 1073741903;
                    downKeys[key] = true;
                    inputs[key] = true;
                }

                break;
            }
            case SDL_KEYUP:{
                if(event.key.keysym.sym < 1024 && event.key.keysym.sym >= 0){
                    upKeys[event.key.keysym.sym] = true;
                    inputs[event.key.keysym.sym] = false;
                }else if(event.key.keysym.sym >= 1073741903){
                    int key = event.key.keysym.sym - 1073741903;
                    upKeys[key] = true;
                    inputs[key] = false;
                }
                break;
            }
            case SDL_MOUSEBUTTONDOWN:{
                downMouse[event.button.button] = true;
                inputMouse[event.button.button] = true;
                break;
            }
            case SDL_MOUSEBUTTONUP:{
                upMouse[event.button.button] = true;
                inputMouse[event.button.button] = false;
                break;
            }
            case SDL_MOUSEMOTION:{
                mousePosition = glm::vec2(event.motion.x, event.motion.y);
                delta = glm::vec2(540, 360) - mousePosition;
                break;
            }
        }
    }
    SDL_WarpMouseInWindow(window, 540, 360);
}

bool Input::GetKeyDown(int key){
    return downKeys[key];
}
bool Input::GetKeyUp(int key){
    return upKeys[key];
}
bool Input::GetKey(int key){
    return inputs[key];
}
bool Input::GetMouseDown(int key){
    return downMouse[key];
}
bool Input::GetMouseUp(int key){
    return upMouse[key];
}
bool Input::GetMouse(int key){
    return inputMouse[key];
}
glm::vec2 Input::GetMousePos(){
    return mousePosition;
}

glm::vec2 Input::GetMouseDelta(){
    return delta;
}


