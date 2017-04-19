#ifndef CLIENT_H
#define CLIENT_H

#include <enet/enet.h>
#include <string>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#define NUM_OF_PICKABLE_OBJECTS 18 //TODO hard-coded horribly, needs change in future

enum ActionFlag {
    NEW_PLAYERS = 1,
    DISCONNECTED_PLAYERS = 2,
    PICKED_OBJECTS = 4
};

enum MessageType {
    NEW_PLAYER = 1,
    GAMESTATE = 2,
    POS_AND_FWD = 3,
    PICKED_UP = 4,
    RELEASED = 5
};

struct PlayerInfo {
    float posX, posY, posZ;
    float fwdX, fwdY, fwdZ;
    int id;
};

struct GameState {
    int numOfPlayers;
    PlayerInfo *playersInfo;
};

class Client
{
    public:
        Client(const std::string& serverIP, unsigned int port);
        int initialize();
        int connect();
        std::vector<PlayerInfo>& getOtherPlayersInfo();
        int getMyId();
        void send(char *buffer, size_t length);
        void sendReliable(char *buffer, size_t length);
        void sendPosAndFwd(const glm::vec3& position, const glm::vec3& fwd);
        int receivePacketNonBlocking(char *&buffer, size_t& len);
        virtual ~Client();
    protected:
    private:
        std::string serverIP;
        unsigned int serverPort;
        ENetHost *client;
        ENetPeer *server;
        ENetAddress serverAddress;

        std::vector<PlayerInfo> otherPlayersInfo;
        int myId;
        bool initialized;
};

#endif // CLIENT_H
