#include "server.h"

Server::Server(int listeningPort) : listeningPort(listeningPort)
{
    isClosed = true;
    nextUserId = 0;
}

int Server::initialize() {
    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = listeningPort;

    if (enet_initialize() != 0) {
        std::cout << "Unable to initialize enet!" << std::endl;
        return -1;
    }

    thisServer = enet_host_create(&address, 4, 2, 0, 0);
    if (thisServer == NULL) {
        std::cout << "Unable to create host!" << std::endl;
        return -1;
    }
    isClosed = false;
    return 0;
}

void Server::loop() {
    auto start = std::chrono::high_resolution_clock::now();
    long int msPassed = 0;
    ENetEvent event;
    while(!isClosed) {
        while(enet_host_service(thisServer, &event, 0) > 0) {
            switch(event.type) {
                case ENET_EVENT_TYPE_CONNECT:{
                    char *gameStateBuffer, *gameStateBufferWithId;
                    UserInfo *userInfo;
                    size_t len;

                    getGameStateBuffer(gameStateBuffer, len);
                    userInfo = new UserInfo();
                    userInfo->clientInfo.ip = event.peer->address.host;
                    userInfo->clientInfo.port = event.peer->address.port;
                    userInfo->playerInfo.id = nextUserId;
                    int *eventData = (int *)malloc(sizeof(int));
                    *eventData = nextUserId;
                    event.peer->data = eventData;
                    users[nextUserId] = userInfo;

                    gameStateBufferWithId = (char *)malloc(len + sizeof(int) + 1);
                    gameStateBufferWithId[0] = NEW_PLAYER;
                    memcpy(gameStateBufferWithId+1, &nextUserId, sizeof(int));
                    memcpy(gameStateBufferWithId + sizeof(int) + 1, gameStateBuffer, len);
                    free(gameStateBuffer);

                    sendReliable(event.peer, gameStateBufferWithId, len + sizeof(int) + 1);
                    free(gameStateBufferWithId);

                    nextUserId++;
                    break;
                }
                case ENET_EVENT_TYPE_RECEIVE:{
                    if (event.packet->data[0] == POS_AND_FWD) {
                        glm::vec3 pos, fwd;
                        memcpy(&pos, event.packet->data + 1, sizeof(float) * 3);
                        memcpy(&fwd, event.packet->data + 1 + sizeof(float)*3, sizeof(float) * 3);
                        UserInfo *info = users[*(int *)event.peer->data];
                        if (info) {
                            info->playerInfo.fwdX = fwd.x;
                            info->playerInfo.fwdY = fwd.y;
                            info->playerInfo.fwdZ = fwd.z;
                            info->playerInfo.posX = pos.x;
                            info->playerInfo.posY = pos.y;
                            info->playerInfo.posZ = pos.z;
                        }
                    }
                    break;
                }
                case ENET_EVENT_TYPE_DISCONNECT:{
                    break;
                }
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto dur = end - start;
        msPassed += std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();

        if (msPassed >= SYNC_RATE_MS) {
            std::cout << "sending gamestate..." << std::endl;
            broadcastGameState();
            start = std::chrono::high_resolution_clock::now();
            msPassed = 0;
        }
    }
}

void Server::broadcastGameState() {
    char *buffer;
    size_t len;
    getGameStateBuffer(buffer, len);
    char *bufferWithMessageType = (char *)malloc(len + 1);
    bufferWithMessageType[0] = GAMESTATE;
    memcpy(bufferWithMessageType+1, buffer, len);

    ENetPacket *packet = enet_packet_create(bufferWithMessageType, len+1, 0);
    enet_host_broadcast(thisServer, 0, packet);

    free(buffer);
    free(bufferWithMessageType);
}

void Server::getGameStateBuffer(char *& buffer, size_t& len) {
    int index = 0;
    int numOfUsers = users.size();
    len = sizeof(int) + sizeof(PlayerInfo) * numOfUsers;
    buffer = (char *)malloc(sizeof(int) + sizeof(PlayerInfo) * numOfUsers);
    memcpy(buffer, &numOfUsers, sizeof(int));
    for(auto it : users) {
        int offset = sizeof(int) + index * sizeof(PlayerInfo);
        UserInfo *userInfo = it.second;
        //std::cout << userInfo->playerInfo.posX << " " << userInfo->playerInfo.posY << " " << userInfo->playerInfo.posZ << std::endl;
        //std::cout << userInfo->playerInfo.fwdX << " " << userInfo->playerInfo.fwdY << " " << userInfo->playerInfo.fwdZ << std::endl;
        memcpy(buffer + offset, &userInfo->playerInfo, sizeof(PlayerInfo));
        index++;
    }
}

void Server::send(ENetPeer *peer, char *buffer, size_t length) {
    ENetPacket *packet = enet_packet_create(buffer, length, 0);
    enet_peer_send(peer, 0, packet);
}

void Server::sendReliable(ENetPeer *peer, char *buffer, size_t length) {
    ENetPacket *packet = enet_packet_create(buffer, length, ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
}

void Server::close() {
    isClosed = true;
}

Server::~Server()
{
    for(auto it : users) {
        delete it.second;
    }
    enet_deinitialize();
}
