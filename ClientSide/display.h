#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Display
{
    public:
        Display(int, int, std::string);
        virtual ~Display();
        void update();
        bool isClosed();
        void set_viewport(float, float, float, float);
        void clear(float, float, float, float);
        static glm::mat2 get_2D_transform_matrix(float, float, float, float,
		float, float, float, float);
		int get_width();
		int get_height();
        static float get_delta();
        static bool isWindowClosed;
    protected:
    private:
        SDL_Window *window;
        SDL_GLContext glContext;
        const int MAX_FPS = 120;
        float one_frame_duration;
        long last_frame_time;
        static float delta;
        int width, height;
};

#endif // DISPLAY_H
