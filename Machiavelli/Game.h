#pragma once

#include <iostream>
#include <memory>
#include <algorithm>
#include <vector>
#include <thread>

#include "Player.hpp"
#include "ClientCommand.h"
#include "Socket.h"

class Game
{
public:
	Game();
	~Game();
	void addPlayer(std::shared_ptr<Player> player);
	void deletePlayer(std::shared_ptr<Player> player);
	void handleCommand(ClientCommand command);
	void globalMessage(std::string message);

private:
	std::vector<std::shared_ptr<Player>> players;
};

