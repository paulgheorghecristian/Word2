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
        void set_viewport(float, float, float, float);
        void clear(float, float, float, float);
        static glm::mat2 get_2D_transform_matrix(float, float, float, float,
		float, float, float, float);
		int get_width();
		int get_height();
        static long get_delta();
        static bool isWindowClosed;
        SDL_Window* getWindow();
        void close();
        void setLastFrameTime(long);
    protected:
    private:
        SDL_Window *window;
        SDL_GLContext glContext;
        const int MAX_FPS = 300;
        float one_frame_duration;
        long last_frame_time;
        static long delta;
        int width, height;
};

#endif // DISPLAY_H
