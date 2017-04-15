#include "game.h"
#include <iostream>

int main(int argc, char *argv[])
{
    Client *client = new Client("localhost", 1234);
    Game *game = NULL;
    if (client->initialize() == 0) {
        int rc = client->connect();
        if (rc != 0) {
            delete client;
            return 0;
        } else {
            game = new Game(800, 600, "World2-ver 0.00001", new Camera(glm::vec3(500, 20, 500), 0, 0, 0), client);
            game->run();
        }
    }

    if (game) {
        delete game;
    }
    delete client;
    return 0;
}
