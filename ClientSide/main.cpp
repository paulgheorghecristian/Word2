#include "game.h"
#include <iostream>

int main(int argc, char *argv[])
{
    Client *client = new Client("localhost", 1234);
    Game *game = NULL;
    #ifdef ONLINE
    if (client->initialize() == 0) {
        int rc = client->connect();
        if (rc != 0) {
            delete client;
            return 0;
        } else {
    #endif
            game = new Game("World2-ver 0.01", new Camera(glm::vec3(500, 20, 500), 0, 0, 0), client);
            game->run();
    #ifdef ONLINE
        }
    }
    #endif
    if (game) {
        delete game;
    }
    delete client;
    return 0;
}
