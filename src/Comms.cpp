#include "../include/Comms.hpp"

Comms::Comms()
{
    host = "127.0.0.1";
    port = 12345;
    resolveHost();
    openSocket();
    std::cout << "----------------------------------\n";
}

Comms::Comms(Uint16 remotePort) {
    port = 0;
    host = "127.0.0.1";

    std::ifstream file("../../../server_ip.txt");

    std::string line;
    if (file.is_open()) {
        if (std::getline(file, line)) {
            std::cout << "[INFO]: ip addr from ip.txt: " << line.c_str() << "\n";
            if (!line.empty()) {
                host = line.c_str();
            }
        }
        else {
            std::cerr << "[ERROR]: Failed to read line from ip.txt\n";
        }
        file.close();
    }
    else {
        std::cout << "[ERROR]: cant open file ip.txt\n";
    }

    if (!line.empty()) {
        if (SDLNet_ResolveHost(&ip, host, remotePort) == -1) {
            std::cerr << "[ERROR]: SDLNet_ResolveHost: " << SDLNet_GetError() << "\n";
        }
        else {
            std::cout << "[INFO]: Server " << host << " resolved on remote port " << remotePort << "\n";
        }
    }
    else {
        if (SDLNet_ResolveHost(&ip, line.c_str(), remotePort) == -1) {
            std::cerr << "[ERROR]: SDLNet_ResolveHost: " << SDLNet_GetError() << "\n";
        }
        else {
            std::cout << "[INFO]: Server " << host << " resolved on remote port " << remotePort << "\n";
        }
    }

    if (!openSocket()) {
        std::cout << "[ERROR]: can't open socket\n";
    }

    std::cout << "[INFO]: Socket: " << sock << "\n";
    std::cout << "[INFO]: Sending to host: " << ip.host << ", port: " << ip.port << "\n";
}

Comms::~Comms()
{
    if (sock) {
        SDLNet_UDP_Close(sock);
    }
}

bool Comms::resolveHost() {
    if (SDLNet_ResolveHost(&ip, host, 12345) == -1) {
        std::cerr << "[ERROR]: SDLNet_ResolveHost: " << SDLNet_GetError() << "\n";
        return false;
    }

    std::cout << "[INFO]: Host " << host << " resolved.\n";
    return true;
}

bool Comms::openSocket() {
    sock = SDLNet_UDP_Open(port);//0 al NEEE(pa 12345)
    if (!sock) {
        std::cerr << "[ERROR]: SDLNet_UDP_Open: " << SDLNet_GetError() << "\n";
        return false;
    }

    if (sock == nullptr) {
        std::cerr << "[ERROR]: Socket null.\n";
        return false;
    }

    std::cout << "[INFO]: Socket opened on " << port << ".\n";
    return true;
}

bool Comms::allocEmptyPacket(UDPpacket** packet, int size) const {
    *packet = SDLNet_AllocPacket(size + 256);//udp overhead?

    if (!*packet) {
        std::cerr << "[ERROR]: SDLNet_AllocPacket: " << SDLNet_GetError() << "\n";
        return false;
    }

    return true;
}

/////////////////
//  recive del //
/////////////////

bool Comms::recieve()
{
    UDPpacket* recvPacket;
    if (!allocEmptyPacket(&recvPacket, 256)) {
        std::cerr << "[ERROR]: Failed to allocate memory for the packet." << std::endl;
        return false;
    }

    if (SDLNet_UDP_Recv(sock, recvPacket) <= 0) {
        SDLNet_FreePacket(recvPacket);
        return false;
    }

    size_t size = recvPacket->len;
    printBytes(reinterpret_cast<char*>(recvPacket->data), size);

    std::cout << "Received packet from: " << SDLNet_ResolveIP(&recvPacket->address) << "\n";

    std::cout << reinterpret_cast<char*>(recvPacket->data) << "\n";

    SDLNet_FreePacket(recvPacket);

    return true;
}

bool Comms::recieve(UDPpacket** recvPacket)
{
    if (SDLNet_UDP_Recv(sock, *recvPacket) <= 0) {
        return false;
    }
    //dont delete, samo za debugat
    //printBytes(reinterpret_cast<char*>((*recvPacket)->data), (*recvPacket)->len);

    return true;
}

bool Comms::recieve(UDPpacket* recvPacket)
{
    if (SDLNet_UDP_Recv(sock, recvPacket) <= 0) {
        return false;
    }
    //dont delete, samo za debugat
    //printBytes(reinterpret_cast<char*>((*recvPacket)->data), (*recvPacket)->len);

    return true;
}

/* IDEJE ZA POZNEJE
using ReturnType = std::variant<int, float, std::string, Coords>;
ReturnType myFunction(Uint32 option) {
    if (option == 1) {
        return 42;
    }
    else if (option == 2) {
        return 3.14f;
    }
    else if (option == 3) {
        return std::string("Hello, World!");
    }
    else {
        return Coords{ 1, 2 };
    }
}*/