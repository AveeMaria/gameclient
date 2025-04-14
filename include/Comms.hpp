#pragma once 

#include "Network.hpp"
#include "PacketType.hpp"

#include <SDL_net.h>
#include <bitset>
#include <iostream>
#include <sstream>

class Comms {
private:
    IPaddress ip;
    UDPsocket sock;

    const char* host;
    Uint16 port;
public:
    Comms();
    Comms(const char* h, Uint16 p);
    ~Comms();

    bool resolveHost();
    bool openSocket();

    template<typename T>
    bool stack_send(T data);

    template<typename T>
    bool stack_send(T data, Uint8 gameID);

    template<typename T>
    bool stack_send(T data, IPaddress _ip);

    bool allocEmptyPacket(UDPpacket** packet, int size) const;
    
    bool recieve();
    bool recieve(UDPpacket** recvPacket);
    bool recieve(UDPpacket* recvPacket);
};

//////////////////////////
// POMEBNE STVARI KLELE //
//////////////////////////


static void printBytes(char data[], size_t size) {
    for (size_t i = 0; i < size; ++i) {
        std::cout << std::bitset<8>(data[i]) << " ";
    }
    std::cout << "\n";
}

static std::stringstream byteToString(char data[], size_t size) {
    std::stringstream ss;
    return ss;
}

template<typename T>
bool Comms::stack_send(T data) {
    Uint8 type = checkType(data);
    if (type == 255) return false;

    UDPpacket* sendPacket = SDLNet_AllocPacket(static_cast<int>(sizeof(T)) + 1);
    if (sendPacket == nullptr) {
        std::cout << "ERROR: No packet\n";
        return false;
    }

    sendPacket->len = (static_cast<int>(sizeof(T) + 1));

    sendPacket->address.host = ip.host;
    sendPacket->address.port = ip.port;

    //std::cout << "size of T: " << sizeof(T) << "\n";
    //std::cout << "Packet len: " << sendPacket->len << "\n";

    sendPacket->data[0] = type;
    std::memcpy(&sendPacket->data[1], &data, sizeof(T));
    //printBytes(reinterpret_cast<char*>(sendPacket->data), sizeof(T) + 1);

    if (sock == nullptr) {
        std::cout << "ERROR: null socket\n";
        return false;
    }

    if (SDLNet_UDP_Send(sock, -1, sendPacket) < 1) {
        std::cerr << "ERROR: SDLNet_UDP_Send error: " << byteToString((char*)SDLNet_GetError(), strlen(SDLNet_GetError())).str() << "\n";
        std::cerr << "ERROR: SDLNet_UDP_Send error: " << std::string(SDLNet_GetError()) << "\n";

        //printBytes((char*)SDLNet_GetError(), 200);
        SDLNet_FreePacket(sendPacket);
        return false;
    }
    //std::cout << "OK: poslan paket.\n";

    SDLNet_FreePacket(sendPacket);
    return true;
}


template<typename T>
bool Comms::stack_send(T data, Uint8 _gameID) {
    Uint8 type = checkType(data);
    if (type == 255) return false;
    if (_gameID == 0) return false;//default game id

    UDPpacket* sendPacket = SDLNet_AllocPacket(static_cast<int>(sizeof(T)) + 2);
    if (sendPacket == nullptr) {
        std::cout << "ERROR: No packet\n";
        return false;
    }

    sendPacket->len = (static_cast<int>(sizeof(T) + 2));

    sendPacket->address.host = ip.host;
    sendPacket->address.port = ip.port;

    //std::cout << "size of T: " << sizeof(T) << "\n";
    //std::cout << "Packet len: " << sendPacket->len << "\n";

    sendPacket->data[0] = type;
    sendPacket->data[1] = _gameID;

    std::memcpy(&sendPacket->data[2], &data, sizeof(T));
    //printBytes(reinterpret_cast<char*>(sendPacket->data), sizeof(T) + 2);

    if (sock == nullptr) {
        std::cout << "ERROR: null socket\n";
        return false;
    }

    if (SDLNet_UDP_Send(sock, -1, sendPacket) < 1) {
        std::cerr << "ERROR: SDLNet_UDP_Send error: " << byteToString((char*)SDLNet_GetError(), strlen(SDLNet_GetError())).str() << "\n";
        std::cerr << "ERROR: SDLNet_UDP_Send error: " << std::string(SDLNet_GetError()) << "\n";

        //printBytes((char*)SDLNet_GetError(), 200);
        SDLNet_FreePacket(sendPacket);
        return false;
    }
    //std::cout << "OK: poslan paket.\n";

    SDLNet_FreePacket(sendPacket);
    return true;
}

template<typename T>
bool Comms::stack_send(T data, IPaddress _ip) {
    Uint8 type = checkType(data);
    if (type == 255) return false;

    UDPpacket* sendPacket = SDLNet_AllocPacket(sizeof(T) + 1);

    sendPacket->address.host = _ip.host;
    sendPacket->address.port = _ip.port;

    sendPacket->len = (sizeof(T) + 1);

    sendPacket->data[0] = type;
    std::memcpy(&sendPacket->data[1], &data, sizeof(T));
    //printBytes(reinterpret_cast<char*>(sendPacket->data), sizeof(T) + 1);

    if (SDLNet_UDP_Send(sock, -1, sendPacket) < 1) {
        std::cerr << "ERROR: SDLNet_UDP_Send error: " << SDLNet_GetError() << "\n";
        SDLNet_FreePacket(sendPacket);
        return false;
    }
    //std::cout << "OK: poslan paket.\n";

    SDLNet_FreePacket(sendPacket);
    return true;
}