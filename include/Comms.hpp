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

    //to se klice v konstruktorjih
    bool resolveHost();
    bool openSocket();

    //part za sendat
    template <typename T>
    bool send(T);

    template<typename T>
    bool stack_send(T data);

    template<typename T>
    bool stack_send(T data, IPaddress _ip);

    bool stupidestSend(const char* data) const;
    bool stupidSend(Uint8* data, size_t size) const;

    bool allocPacket(UDPpacket** packet, const Uint8* data, int size) const;

    //part za recivevat
    bool allocEmptyPacket(UDPpacket** packet, int size) const;
    bool recieve();
    bool recieve(UDPpacket** recvPacket);
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


//smeti????
//template<typename T>
//Uint8 checkType(T data) {
//    // Implement type checking logic here
//    // Return a unique Uint8 value for each type
//    if (std::is_same<T, int>::value) {
//        return 1;
//    } else if (std::is_same<T, float>::value) {
//        return 2;
//    } else if (std::is_same<T, std::string>::value) {
//        return 3;
//    } else {
//        return 255; // Unknown type
//    }
//}

template<typename T>
std::unique_ptr<Uint8[]> prepareData(T data) {
    size_t packetSize = 1 + sizeof(data);
    std::cout << "data size: " << packetSize << "\n";

    std::unique_ptr<Uint8[]> buffer(new Uint8[packetSize]);
    Uint8 type = checkType(data);

    if (type == 255) {
        return nullptr;
    }
    else {
        buffer[0] = type;
    }

    std::memcpy(&buffer[1], &data, sizeof(data));

    printBytes(reinterpret_cast<char*>(buffer.get()), packetSize);

    return buffer;
}

template<typename T>
bool Comms::send(T data) {
    std::unique_ptr<Uint8[]> packet = prepareData(data);

    if (!packet) {
        std::cout << "ERROR: No packet\n";
        return false;
    }

    stupidSend(packet.get(), sizeof(data) + 1);
    return true;
}


// STACK ALLOCATION VERZIJA? A JE TO SPLOH KEJ HITREJ?
template<typename T>
bool Comms::stack_send(T data) {
    Uint8 type = checkType(data);
    if (type == 255) return false;


    UDPpacket* sendPacket = SDLNet_AllocPacket(static_cast<int>(sizeof(T)) + 1);
    if (sendPacket == nullptr) {
        std::cout << "ERROR: No packet\n";
        return false;
    }

    std::cout << "size of T: " << sizeof(T) << "\n";
    std::cout << "Packet len: " << sendPacket->len << "\n";

    //if (sendPacket->len == 0) {
        sendPacket->len = (static_cast<int>(sizeof(T) + 1));
    //}

    

    sendPacket->address.host = ip.host;
    sendPacket->address.port = ip.port;

    std::cout << "size of T: " << sizeof(T) << "\n";
    std::cout << "Packet len: " << sendPacket->len << "\n";

    sendPacket->data[0] = type;
    std::memcpy(&sendPacket->data[1], &data, sizeof(T));
    //    printBytes(reinterpret_cast<char*>(sendPacket->data), sizeof(T) + 1);

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

// STACK ALLOCATION VERZIJA? A JE TO SPLOH KEJ HITREJ?
template<typename T>
bool Comms::stack_send(T data, IPaddress _ip) {
    Uint8 type = checkType(data);
    if (type == 255) return false;

    UDPpacket* sendPacket = SDLNet_AllocPacket(sizeof(T) + 1);

    sendPacket->address.host = _ip.host;
    sendPacket->address.port = _ip.port;

    std::cout << "Packet size: " << sizeof(sendPacket) << "\n";

    if (sendPacket == nullptr) {
        std::cout << "ERROR: No packet\n";
    }

    sendPacket->len = (sizeof(T) + 1);

    sendPacket->data[0] = type;
    std::memcpy(&sendPacket->data[1], &data, sizeof(T));
    printBytes(reinterpret_cast<char*>(sendPacket->data), sizeof(T) + 1);

    if (SDLNet_UDP_Send(sock, -1, sendPacket) < 1) {
        std::cerr << "ERROR: SDLNet_UDP_Send error: " << SDLNet_GetError() << "\n";
        SDLNet_FreePacket(sendPacket);
        return false;
    }
    //std::cout << "OK: poslan paket.\n";

    SDLNet_FreePacket(sendPacket);
    return true;
}



//to je sexy af
/*
using ReturnType = std::variant<int, float, std::string, Coords>;
ReturnType myFunction(Uint32 option);
*/


///////////////////////
// recieve del kode? //
///////////////////////
