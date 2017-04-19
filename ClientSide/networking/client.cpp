#include "client.h"

Client::Client(const std::string& serverIP, unsigned int port) : serverIP(serverIP),
                                                                serverPort(port),
                                                                initialized(false),
                                                                server(NULL)
{

}

int Client::initialize() {
    if (enet_initialize() != 0) {
        std::cout << "Error initializing enet!" << std::endl;
        return -1;
    }

    initialized = true;

    client = enet_host_create(NULL, 1, 2, 0, 0);
    if (client == NULL) {
        std::cout << "Error creating the client!" << std::endl;
        return -1;
    }

    enet_address_set_host(&serverAddress, serverIP.c_str());
    serverAddress.port = serverPort;

    return 0;
}

int Client::connect() {
    ENetEvent event;

    server = enet_host_connect(client, &serverAddress, 2, 0);
    if (server == NULL) {
        std::cout << "Unable to connect to the server!" << std::endl;
        return -1;
    }

    if (enet_host_service(client, &event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT) {
        std::cout << "Connection to " << serverIP << " succeeded!" << std::endl;

        if (enet_host_service(client, &event, 5000) &&
            event.type == ENET_EVENT_TYPE_RECEIVE) {
            int numOfPlayers;

            if (event.packet->data[0] == NEW_PLAYER) {
                memcpy(&myId, event.packet->data+1, sizeof(int));
                memcpy(&numOfPlayers, event.packet->data + sizeof(int)+1, sizeof(int));

                if (numOfPlayers > 0) {
                    PlayerInfo *playersInfo = (PlayerInfo *)malloc(sizeof(PlayerInfo) * numOfPlayers);
                    memcpy(playersInfo, event.packet->data + sizeof(int) * 2 + 1, sizeof(PlayerInfo) * numOfPlayers);
                    for(int i = 0; i < numOfPlayers; i++){
                        std::cout << "other " << i << " id:" << playersInfo[i].id << std::endl;
                        otherPlayersInfo.push_back(playersInfo[i]);
                    }
                    free(playersInfo);
                }
                std::cout << "Id recv from server: " << myId << std::endl;
                return 0;
            } else {
                std::cout << "Didn't receive a new player message from server!" << std::endl;
                return -1;
            }
        } else {
            std::cout << "Didn't receive my id from server!" << std::endl;
            return -1;
        }
    } else {
        enet_peer_reset(server);
        std::cout << "Connection to " << serverIP << " failed!" << std::endl;
        return -1;
    }
}

void Client::send(char *buffer, size_t length) {
    ENetPacket *packet = enet_packet_create(buffer, length, 0);
    enet_peer_send(server, 0, packet);
}

void Client::sendReliable(char *buffer, size_t length) {
    ENetPacket *packet = enet_packet_create(buffer, length, ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(server, 0, packet);
}

void Client::sendPosAndFwd(const glm::vec3& position, const glm::vec3& fwd) {
    char posAndFwd[1 + 6 * sizeof(float)];
    posAndFwd[0] = POS_AND_FWD;
    memcpy(posAndFwd+1, &position, sizeof(float) * 3);
    memcpy(posAndFwd+1+sizeof(float)*3, &fwd, sizeof(float) * 3);
    send(posAndFwd, 1 + 6 * sizeof(float));
}

std::vector<PlayerInfo>& Client::getOtherPlayersInfo() {
    return otherPlayersInfo;
}

int Client::getMyId() {
    return myId;
}

int Client::receivePacketNonBlocking(char *&buffer, size_t& len) {
    ENetEvent event;
    int rc = enet_host_service(client, &event, 0);

    if (rc > 0) {
        if (event.type == ENET_EVENT_TYPE_RECEIVE) {
            buffer = (char *) malloc(event.packet->dataLength);
            len = event.packet->dataLength;
            memcpy(buffer, event.packet->data, len);
        }
    }

    return rc;
}

Client::~Client()
{
    if (server) {
        enet_peer_reset(server);
    }
    if (initialized) {
        enet_deinitialize();
    }
}
