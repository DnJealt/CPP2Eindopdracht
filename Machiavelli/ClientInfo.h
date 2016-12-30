//
// ClientInfo.h
//
// Created by Jeroen de Haas on 22/11/2016.
// Copyright (c) 2016 Avans Hogeschool, 's-Hertogenbosch. All rights reserved.
//

#ifndef _CLIENT_INFO_H
#define _CLIENT_INFO_H

#include <string>
#include <utility>

#include "Socket.h"
#include "Player.hpp"

class ClientInfo {
    Socket _socket;
    std::shared_ptr<Player> _player;
public:
    ClientInfo(Socket socket, std::shared_ptr<Player> player)
        : _socket{std::move(socket)}, _player{std::move(player)} {}
    Socket& get_socket() { return _socket; }
    const Socket& get_socket() const { return _socket; }
    std::shared_ptr<Player> get_player() { return _player; }
    const std::shared_ptr<Player> get_player() const { return _player; }
};

#endif
