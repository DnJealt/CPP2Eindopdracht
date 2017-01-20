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
		std::string cmd = command.get_cmd();
		if (cmd == "start") {
			if (players.size() < 2) {
				//send message start is not possible
				globalMessage("\nSorry " + command.get_player()->get_name() + ", maar je moet met 2 spelers zijn.");
			}
			else {
				//start game
				this->initGame();
				
				while (!gameOver) {
					startRound();
				}

				//count the winner
			}
		}
		else if (cmd == "hax") {
			cheats = true;
		}
		else if (cmd == "help") {
			playerMessage(showHelp(), command);
		}
		else if (std::all_of(cmd.begin(), cmd.end(), ::isdigit)) {
			command.get_player()->pickCommand(command.get_cmd());
		}
		else {
			command.get_socket() << command.get_player()->get_name() << ", you wrote: '" << command.get_cmd() << "', but I'll ignore that for now.\r\n";
		}
	}
	catch (const std::exception& ex) {
		std::cerr << "*** exception in handle thread for player " << command.get_player()->get_name() << '\n';
		if (command.get_socket().is_open()) {
			command.get_socket() << "ERROR: " << ex.what() << "\r\n";
		}
	}
	catch (...) {
		std::cerr << "*** exception in handle thread for player " << command.get_player()->get_name() << '\n';
		if (command.get_socket().is_open()) {
			command.get_socket() << "Sorry, something went wrong during handling of your request.\r\n";
		}
	}
}

void Game::globalMessage(std::string message)
{
	for each (auto player in players)
	{
		*player << message << "\r\n";
	}
}

void Game::playerMessage(std::string message, ClientCommand cmd)
{
	cmd.get_socket() << message;
}

void Game::startRound()
{
	globalMessage("Een nieuwe ronde staat op het punt te beginnen!\n");

	if (cheats) {
		iIzAH4x0r();
	}
	else {
		pickCharacters();
	}
	
	doTurn();
}

void Game::showCurrentStats(Socket & socket)
{
	for each (auto player in players)
	{
		socket << player->get_name() << ":\r\n";
		socket << "GoudBalans: " << std::to_string(player->get_gold()) << "\r\n";
		socket << "Gebouwen:\r\n";
		for each (auto building in player->getBuildedBuildings())
		{
			socket << building->getName() << ", " << building->getColor() << ", " << building->getPrice() << "\r\n";
		}
	}
}

std::vector<std::shared_ptr<Player>> Game::getPlayers()
{
	return this->players;
}

void Game::iIzAH4x0r()
{
	auto characters = reader->getCharactersShuffled();
	std::shared_ptr<Player> current = king;

	//reset each character
	for (auto character : characters) {
		character->setStolenFrom(false);
		character->setDead(false);
	}

	// Give each player 2 cards
	players[0]->addCharacterCard(characters[0]);
	players[0]->addCharacterCard(characters[1]);

	players[1]->addCharacterCard(characters[4]);
	players[1]->addCharacterCard(characters[5]);
}

void Game::doTurn()
{
	auto characters = reader->getCharactersInOrder();

	for each (auto character in characters) {
		//als character dood is moet die overslaan.
		if (!character->isDead()) {
			bool isPicked = false;
			//check welke player karakter is
			for each (auto currentPlayer in this->players) {
				if (currentPlayer->hasCharacter(character)) {

					//wachtende speler vertellen dat andere speler aan de beurt is.
					auto waitingPlayer = this->waitingPlayer(currentPlayer);
					*(waitingPlayer) << "Even geduld, " << waitingPlayer->get_name() + " is aan de beurt als " + character->getName() + ".\r\n";

					//speler beurt laten doen
					currentPlayer->turnWith(character);

					//zeggen dat character is gepicked
					isPicked = true;

					//als character koning is player koning maken
					if (character->getName() == "Koning") {
						king = currentPlayer;
					}
				}
			}

			//als kaart niet voorkomt bij spelers
			if (!isPicked) {
				globalMessage("\r\n" + character->getName() + " is door niemand gekozen\r\n");
			}
		}
		else {
			globalMessage("\r\n" + character->getName() + " komt niet aan de beurt omdat hij vermoord is\r\n");
			character->setDead(false);
		}
	}
}

void Game::initGame()
{
	gameOver = false;
	int age = 0;

	//read files
	reader = std::make_shared<CardReader>();

	//gold on the table
	goldCount = 30;

	for each (auto player in this->players)
	{
		if (player->get_age() > age) {
			this->king = player;
			age = player->get_age();
		}

		takeGold(player, 2);
		takeCard(player, 4);
		//give every player 2 gold, and 4 cards
	}

	globalMessage("De oudste speler is " + king->get_name() + ". Dus die begint en is koning.");
}

void Game::takeGold(std::shared_ptr<Player> player, int amount)
{
	goldCount -= amount;
	if (goldCount < 0) {
		amount += goldCount;
	}
	player->addGold(amount);
}

void Game::takeCard(std::shared_ptr<Player> player, const int amount)
{
	*(player) << "Je 2 goud gekregen en je hebt de volgende 4 bouwkaarten gepakt:\n";
	for (int i = 0; i < amount; i++)
	{
		auto card = reader->getBuildingCard();
		player->addBuildingCard(card);
		*(player) << "\r\n\t" << card->getName() << " (" << card->getColor() << ", " << std::to_string(card->getPrice()) << ")" << "\r\n";
	}
}

void Game::pickCharacters()
{
	// clear characters by every player
	for each (auto player in this->players)
	{
		player->getCharacters().clear();
	}

	auto characters = reader->getCharactersShuffled();
	std::shared_ptr<Player> current = king;

	//reset each character
	for (auto character : characters) {
		character->setStolenFrom(false);
		character->setDead(false);
	}

	while (characters.size() > 0) {
		//pick characters
		for each (auto player in players)
		{
			if (player != current) {
				// set next player to do the same and tell player other player is picking				
				*(player) << "Even geduld, " << current->get_name() << " is karakters aan het kiezen/wegleggen.";
			}
		}
		characters = current->pickCharacter(characters);

		//set other player to current player.
		current = waitingPlayer(current);
	}
}

std::string Game::showHelp()
{
	std::string str = "Verloop van een speelbeurt: \r\n";
	str += "\t- Inkomsten: Neem 2 goudstukken of pak 2 kaarten en leg er 1 af\r\n";
	str += "\t- Bouwen: Leg 1 bouwkaart neer en betaal de waarde\r\n";
	str += "\tKaraktereigenschap: Op elk moment te gebruiken\r\n\r\n";

	str += "1 - Moordenaar \r\n";
	str += "\tVermoordt een ander karakter\r\n";
	str += "2 - Dief \r\n";
	str += "\tSteelt van een andere speler\r\n";
	str += "3 - Magier \r\n";
	str += "\tRuilt bouwkaarten om\r\n";
	str += "4 - Koning \r\n";
	str += "\tBegint de volgende beurt\r\n";
	str += "\tOntvangt van monumenten\r\n";
	str += "5 - Prediker \r\n";
	str += "\tIs beschermd tegen de Condottiere\r\n";
	str += "\tOntvangt van kerkelijke gebouwen\r\n";
	str += "6 - Koopman \r\n";
	str += "\tOntvangt een extra goudstuk\r\n";
	str += "\tOntvangt van commerciele gebouwen\r\n";
	str += "7 - Bouwmeester \r\n";
	str += "\tTrekt twee extra kaarten\r\n";
	str += "\tMag drie gebouwen bouwen\r\n";
	str += "8 - Condottiere \r\n";
	str += "\tVernietigt een gebouw\r\n";
	str += "\tOntvangt van alle militaire gebouwen\r\n";

	return str;
}

std::shared_ptr<Player> Game::waitingPlayer(std::shared_ptr<Player> current)
{
	for each (auto player in players)
	{
		if (player != current)
			return player;
	}

	return nullptr;
}
