#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Display
{
    public:
        Display(float&, float&, std::string);
        virtual ~Display();
        void update();
        bool isClosed();
        void setViewport(float, float, float, float);
        void clear(float, float, float, float);
		float getWidth();
		float getHeight();
        static long getDelta();
        static bool isWindowClosed;
        SDL_Window* getWindow();
        void close();
        void setLastFrameTime(long);
    protected:
    private:
        SDL_Window *window;
        SDL_GLContext glContext;
        const int MAX_FPS = 300;
        float oneFrameDuration;
        long lastFrameTime;
        static long delta;
        float width, height;
};

#endif // DISPLAY_H
