#pragma once

#include <iostream>
#include <memory>
#include <algorithm>
#include <vector>

#include "Player.hpp"


class Game
{
public:
	Game();
	~Game();
	void addPlayer(std::shared_ptr<Player> player);
	void deletePlayer(std::shared_ptr<Player> player);
private:
	std::vector<std::shared_ptr<Player>> players;
};

