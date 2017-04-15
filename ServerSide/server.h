#ifndef SERVER_H
#define SERVER_H

#include <enet/enet.h>
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <glm/glm.hpp>

#define NUM_OF_PICKABLE_OBJECTS 18 //TODO hard-coded horribly, needs change in future
#define SYNC_RATE 60

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

struct ClientInfo {
    unsigned int ip, port;
};

struct UserInfo {
    PlayerInfo playerInfo;
    ClientInfo clientInfo;
};

class Server
{
    public:
        Server(int listeningPort);
        int initialize();
        void close();
        void loop();
        void send(ENetPeer *peer, char *buffer, size_t length);
        void sendReliable(ENetPeer *peer, char *buffer, size_t length);
        virtual ~Server();
    protected:
    private:
        int listeningPort;
        int nextUserId;
        ENetHost *thisServer;
        volatile bool isClosed;
        std::unordered_map<int, UserInfo*> users;
        const float SYNC_RATE_MS = 10000000/SYNC_RATE;

        void getGameStateBuffer(char *& buffer, size_t& len);
        void broadcastGameState();
};

#endif // SERVER_H
