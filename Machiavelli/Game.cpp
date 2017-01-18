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

void Game::handleCommand(ClientCommand command)
{	
	//start the game
	try {
		std::cout << command.get_cmd();
		std::string cmd = command.get_cmd();
		if (cmd == "start") {
			if (players.size() < 2) {
				//send message start is not possible
				globalMessage("fucked. je bent maar in je eentje!");				
			}
			else {
				//start game
				this->initGame();
			}		
		}
		else if (cmd == "help") {

		}
		else if (std::all_of(cmd.begin(), cmd.end(), ::isdigit)) {
			//handle the command 
		}
		else {
			// moet zeggen tegen de compiler dat het een pointer is. 
			*command.get_socket() << command.get_player()->get_name() << ", you wrote: '" << command.get_cmd() << "', but I'll ignore that for now.\r\n";
		}
	}
	catch (const std::exception& ex) {
		std::cerr << "*** exception in consumer thread for player " << command.get_player()->get_name() << '\n';
		if (command.get_socket()->is_open()) {
			command.get_socket()->write("Sorry, something went wrong during handling of your request.\r\n");
		}
	}
	catch (...) {
		std::cerr << "*** exception in consumer thread for player " << command.get_player()->get_name() << '\n';
		if (command.get_socket()->is_open()) {
			command.get_socket()->write("Sorry, something went wrong during handling of your request.\r\n");
		}
	}
}

void Game::globalMessage(std::string message)
{
	for (auto player : players) {
		*player << message << "\r\n";
	}
}

void Game::initGame()
{
	characterCardReader = std::make_shared<CardReader>("character");
	buildingCardReader = std::make_shared<CardReader>("build");
}


