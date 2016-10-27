#ifndef GAME_H
#define GAME_H

#include "camera.h"
#include "display.h"
#include "input.h"
#include "shader.h"
#include "entity.h"
#include <string>
#include <vector>

class Game
{
    public:
        Game(float, float, std::string, Camera*);
        Game(std::string, Camera*);
        Game(std::string);
        Game(float, float, std::string);
        void run();
        virtual ~Game();
    protected:
    private:
        float screenWidth, screenHeight;
        std::string title;
        Camera* camera;
        Display* display;
        Input* input;
        Shader* shader;
        std::vector<Entity*> entities;
        Mesh* boxMesh;
        void construct();
        void handleInput();
        void render();
};

#endif // GAME_H
