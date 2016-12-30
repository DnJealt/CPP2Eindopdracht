#include "Game.h"


Game::Game()
{
}

Game::~Game()
{
}

void Game::addPlayer(std::shared_ptr<Player> player)
{
	players.push_back(player);
}

void Game::deletePlayer(std::shared_ptr<Player> player)
{
	// using c++11 lambda for delete
	auto it = std::find_if(players.begin(), players.end(), [&player](const std::shared_ptr<Player>& checkPlayer) {return checkPlayer == player; });
		
	if (it != players.end())
	{
		players.erase(it);
	}
}
