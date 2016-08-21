#include "display.h"

float Display::delta = 0;
bool Display::isWindowClosed;
Display::Display(int width, int height, std::string title)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetSwapInterval(0);

    SDL_ShowCursor(0);

    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    glContext = SDL_GL_CreateContext(window);

    GLenum status = glewInit();

    if(status != GLEW_OK){
        std::cerr << "Glew failed to initialize !" << std::endl;
    }
    isWindowClosed = false;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CLIP_DISTANCE0);
    //glEnable(GL_CLIP_DISTANCE1);
    glDepthFunc(GL_LEQUAL);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glViewport(0.0f, 0.0f, (float)width, (float)height);
    last_frame_time = SDL_GetTicks();

    one_frame_duration = 1000.0f / (float)MAX_FPS;
}

Display::~Display()
{
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Display::clear(float r, float g, float b, float a){
    last_frame_time = SDL_GetTicks();
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glScissor(viewport[0], viewport[1], viewport[2], viewport[3]);
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Display::set_viewport(float x, float y, float width, float height){
    glViewport(x, y, width, height);
}

bool Display::isClosed(){
    return isWindowClosed;
}
void Display::update(){

    SDL_GL_SwapWindow(window);

    float frame_duration = SDL_GetTicks() - last_frame_time;

    if(frame_duration < one_frame_duration){
        SDL_Delay(one_frame_duration - frame_duration);
    }

    delta = SDL_GetTicks() - last_frame_time;

    SDL_WarpMouseInWindow(window, 1080/2, 720/2);

}

glm::mat2 Display::get_2D_transform_matrix(float ferx, float fery, float widthfer, float heightfer,
		float viewx, float viewy, float widthview, float heightview){
		//transformarea fereastra-poarta
		float sx, sy, tx, ty;
		sx = (widthview-viewx) / (widthfer-ferx);
		sy = (heightview-viewy) / (heightfer-fery);
		tx = viewx - sx*ferx;
        ty = viewy - sy*fery;
		return glm::mat2(sx, tx, sy, ty);
}

float Display::get_delta(){
    return delta/1000.0f;
}
