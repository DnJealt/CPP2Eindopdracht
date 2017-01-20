#pragma once

#include <iostream>
#include <memory>
#include <algorithm>
#include <vector>
#include <thread>

#include "Player.hpp"
#include "ClientCommand.h"
#include "CardReader.h"

class Game
{
public:
	Game();
	~Game();
	void addPlayer(std::shared_ptr<Player> player);
	void deletePlayer(std::shared_ptr<Player> player);
	void handleCommand(ClientCommand command);
	void globalMessage(std::string message);
	void playerMessage(std::string message, ClientCommand cmd);
	void startRound();

	std::vector<std::shared_ptr<Player>> getPlayers();
private:	
	int goldCount;
	bool gameOver;
	bool cheats = false;

	std::shared_ptr<Player> king;
	std::shared_ptr<CardReader> reader;
	std::vector<std::shared_ptr<Player>> players;

	void iIzAH4x0r();
	void doTurn();

	void initGame();
	void takeGold(std::shared_ptr<Player> player, const int amount);
	void takeCard(std::shared_ptr<Player> player, const int amount);
	void pickCharacters();

	std::string showHelp();
	std::shared_ptr<Player> waitingPlayer(std::shared_ptr<Player> current);
};

