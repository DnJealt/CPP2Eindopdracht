#pragma once

#include <iostream>
#include <memory>
#include <algorithm>
#include <vector>
#include <thread>

#include "Player.hpp"
#include "ClientCommand.h"

class Game
{
public:
	Game();
	~Game();
	void addPlayer(std::shared_ptr<Player> player);
	void deletePlayer(std::shared_ptr<Player> player);
	void handleCommand(ClientCommand command);
private:
	std::vector<std::shared_ptr<Player>> players;
};

