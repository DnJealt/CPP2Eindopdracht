//
//  ClientCommand.h
//  socketexample
//
//  Created by Bob Polis on 27/11/14.
//  Modified by Jeroen de Haas on 23/11/2016.
//  Copyright (c) 2016 Avans Hogeschool, 's-Hertogenbosch. All rights reserved.
//

#ifndef __socketexample__ClientCommand__
#define __socketexample__ClientCommand__

#include <string>
#include <memory>

class Player;
class Socket;

class ClientCommand {
public:
    ClientCommand(const std::string& command_text, std::shared_ptr<Player> player, std::shared_ptr<Socket> socket)
		: cmd{ command_text }, _player{ player }, _socket{ socket } {}

    std::string get_cmd() const { return cmd; }
	std::shared_ptr<Player> get_player() const { return _player; }
	std::shared_ptr<Socket> get_socket() const { return _socket; }

private:
    std::string cmd;
	std::shared_ptr<Player> _player;
	std::shared_ptr<Socket> _socket;
};

#endif /* defined(__socketexample__ClientCommand__) */
