#include "game.h"
#include <iostream>

int main(int argc, char *argv[])
{
    Game* game = new Game("World2-ver 0.00001", new Camera(glm::vec3(500, 20, 500), 0, 0, 0));
    game->run();
    delete game;
    return 0;
}
