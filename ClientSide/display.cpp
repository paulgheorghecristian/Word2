#include "display.h"

long Display::delta = 0;
bool Display::isWindowClosed;

Display::Display(float &WIDTH, float &HEIGHT, std::string title, bool fullscreen)
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
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_GL_SetSwapInterval(0);
    SDL_ShowCursor(0);

    if(fullscreen){
        SDL_DisplayMode dm;

        if (SDL_GetDesktopDisplayMode(0, &dm) != 0){
            SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
        }

        this->width = dm.w;
        this->height = dm.h;

        WIDTH = this->width;
        HEIGHT = this->height;
    }else{
        this->width = WIDTH;
        this->height = HEIGHT;
    }

    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL | (fullscreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_RESIZABLE));
    glContext = SDL_GL_CreateContext(window);

    GLenum status = glewInit();

    if(status != GLEW_OK){
        std::cerr << "Glew failed to initialize !" << std::endl;
    }

    isWindowClosed = false;
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glViewport(0.0f, 0.0f, WIDTH, HEIGHT);
    lastFrameTime = SDL_GetTicks();

    oneFrameDuration = 1000.0f / (float)MAX_FPS;
}

Display::~Display()
{
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Display::clear(float r, float g, float b, float a){
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Display::setViewport(float x, float y, float width, float height){
    glViewport(x, y, width, height);
}

bool Display::isClosed(){
    return isWindowClosed;
}
void Display::update(){

    SDL_GL_SwapWindow(window);

    float frameDuration = SDL_GetTicks() - lastFrameTime;

    if(frameDuration < oneFrameDuration){
        SDL_Delay(oneFrameDuration - frameDuration);
    }

    delta = SDL_GetTicks() - lastFrameTime;
}

float Display::getWidth(){
    return width;
}

float Display::getHeight(){
    return height;
}

long Display::getDelta(){
    return delta;
}

SDL_Window* Display::getWindow(){
    return window;
}

void Display::close(){
    isWindowClosed = true;
}

void Display::setLastFrameTime(long lastFrameTime){
    this->lastFrameTime = lastFrameTime;
}
