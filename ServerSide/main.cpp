#include <iostream>
#include <thread>
#include "server.h"

using namespace std;

int x;

void getInput(Server *server) {
    int x;
    while(1) {
        std::cin >> x;
        if (x == 0) {
            server->close();
            break;
        }
    }
}

int main()
{
    Server *server = new Server(1234);

    if (server->initialize() == 0) {
        std::thread keyboardInput(getInput, server);
        server->loop();
        keyboardInput.detach();
    }

    delete server;
    return 0;
}
