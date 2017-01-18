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
		it->get()->socket->close();
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
		/*else if (cmd == "quit") {
			command.get_socket()->close();
		}*/
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

void Game::playerMessage(std::string message, ClientCommand cmd) {
	*(cmd.get_socket()) << message;
}

void Game::initGame()
{
	characterCardReader = std::make_shared<CardReader>("character");
	buildingCardReader = std::make_shared<CardReader>("build");
}
std::string Game::showHelp() {

	std::string str = "Verloop van een speelbeurt: \r\n";
	str += "\t- Inkomsten: Neem 2 goudstukken of neem 2 kaarten en leg er 1 af\r\n";
	str += "\t- Bouwen: Leg 1 bouwkaart neer en betaal de waarde\r\n";
	str += "\tKaraktereigenschap: Op elk moment te gebruiken\r\n";
	str += "\r\n";

	str += "1 - Moordenaar\r\n";
	str += "\tVermoordt een ander karakter\r\n";
	str += "2 - Dief\r\n";
	str += "\tSteelt van een andere speler\r\n";
	str += "3 - Magier\r\n";
	str += "\tRuilt bouwkaarten om\r\n";
	str += "4 - Koning (geel)\r\n";
	str += "\tBegint de volgende beurt\r\n";
	str += "\tOntvangt van monumenten\r\n";
	str += "5 - Prediker (blauw)\r\n";
	str += "\tIs beschermd tegen de Condottiere\r\n";
	str += "\tOntvangt van kerkelijke gebouwen\r\n";
	str += "6 - Koopman (groen)\r\n";
	str += "\tOntvangt een extra goudstuk\r\n";
	str += "\tOntvangt van commerciele gebouwen\r\n";
	str += "7 - Bouwmeester\r\n";
	str += "\tTrekt twee extra kaarten\r\n";
	str += "\tMag drie gebouwen bouwen\r\n";
	str += "8 - Condottiere(rood)\r\n";
	str += "\tVernietigt een gebouw\r\n";
	str += "\tOntvangt van alle militaire gebouwen\r\n";

	return str;
}

std::vector<std::shared_ptr<Player>> Game::getPlayers() {
	return players;
}
