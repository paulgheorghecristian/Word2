#include "game.h"
#include <iostream>

int main(int argc, char *argv[])
{
    Game* game = new Game(1280, 720, "World2-ver 0.00001", new Camera(glm::vec3(0, 20, 100), 0, 0, 0));
    game->run();
    delete game;
    return 0;
}
