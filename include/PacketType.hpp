#pragma once

#include "Network.hpp"
#include <SDL_net.h>

//pazi kako to sranje includas!!!!! ful sketchy error je ce narobe
enum class PacketType : Uint8 {
    //networking
    PING = 0,
	PONG = 5,

    SYN = 10,
    SYN_ACK = 15,
    ACK = 20,

    //game data
    PLAYER_NAME = 30,//string
    ENTITY_POS = 40,

    //input data


    UNDEFINED = 255//smeti?
};



//template<typename T>
//Uint8 checkType(const T& data);

template<typename T>
Uint8 checkType(const T& data)
{
    /*if (std::is_same<T, int>::value) {
        std::cout << "The type is int.\n";
        return static_cast<Uint8>(PacketType::INT);
    }
    else if (std::is_same<T, float>::value) {
        std::cout << "The type is float.\n";
        return static_cast<Uint8>(PacketType::FLOAT);
    }*/

    if (std::is_same<T, Ping>::value) {
        std::cout << "The type is ping.\n";
        return static_cast<Uint8>(PacketType::PING);
    }
    else if (std::is_same<T, SYN>::value) {
        std::cout << "The type is syn.\n";
        return static_cast<Uint8>(PacketType::SYN);
    }
    else if (std::is_same<T, SYN_ACK>::value) {
        std::cout << "The type is syn_ack.\n";
        return static_cast<Uint8>(PacketType::SYN_ACK);
    }
    else if (std::is_same<T, ACK>::value) {
        std::cout << "The type is ack.\n";
        return static_cast<Uint8>(PacketType::ACK);
    }

    else if (std::is_same<T, std::string>::value) {
        std::cout << "The type is std::string.\n";
        return static_cast<Uint8>(PacketType::PLAYER_NAME);
    }
    else if (std::is_same<T, EntityPos>::value) {
        std::cout << "The type is std::string.\n";
        return static_cast<Uint8>(PacketType::ENTITY_POS);
    }
    else {
        std::cout << "The type is undefined.\n";
        return static_cast<Uint8>(PacketType::UNDEFINED);
    }
}