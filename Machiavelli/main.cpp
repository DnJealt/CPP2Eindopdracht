//
//  main.cpp
//  socketexample
//
//  Created by Bob Polis on 16/09/14.
//  Revised by Jeroen de Haas on 22/11/2016
//  Copyright (c) 2014 Avans Hogeschool, 's-Hertogenbosch. All rights reserved.
//

#include <thread>
#include <iostream>
#include <exception>
#include <memory>
#include <utility>

#include "Socket.h"
#include "Sync_queue.h"
#include "ClientCommand.h"
#include "Player.hpp"
#include "Game.h"

namespace machiavelli {
    const int tcp_port {1080};	
	std::shared_ptr<Game> game = nullptr;
}

static Sync_queue<ClientCommand> queue;

void consume_command() // runs in its own thread
{
    try {
        while (true) {
            ClientCommand command {queue.get()}; // will block here unless there are still command objects in the queue

			std::shared_ptr<Player> player{ command.get_player() };
			std::shared_ptr<Socket> client{ command.get_socket() };			   

                try {
                    // TODO handle command here
					std::thread commandHandler { &Game::handleCommand, machiavelli::game, command };
					commandHandler.detach();					
                } catch (const std::exception& ex) {
					std::cerr << "*** exception in consumer thread for player " << player->get_name() << ": " << ex.what() << '\n';
                    if (client->is_open()) {
                        client->write("Sorry, something went wrong during handling of your request.\r\n");
                    }
                } catch (...) {
					std::cerr << "*** exception in consumer thread for player " << player->get_name() << '\n';
                    if (client->is_open()) {
                        client->write("Sorry, something went wrong during handling of your request.\r\n");
                    }
                }            
        }
    } catch (...) {
		std::cerr << "consume_command crashed\n";
    }
}


void handle_client(std::shared_ptr<Socket> client) // this function runs in a separate thread
{
    try {
		client->write("Welkom bij Machiavelli server! Om af te sluiten, typ 'quit'.\r\n");
		client->write("Wat is je naam?\r\n");
		std::string name{ client->readline() };

		client->write("\nHoe oud ben je?\r\n");

		bool valid = false;
		int age = 0;
		while (!valid) {
			std::string input{ client->readline() };
			if (isdigit(input[0])) {
				valid = true;
				age = std::stoi(input);
			}
		};

		//shared pointer omdat meerdere mensen het player object gaan gebruiken. 
		std::shared_ptr<Player> player{ new Player(name, age, client) };

		// add player to the game
		machiavelli::game->addPlayer(player);
		
		*client << "\nWelkom, " << player->get_name() << ", Typ 'start' om te beginnen!\r\n";

        while (true) { // game loop
            try {
                // read first line of request
				std::string cmd { client->readline() };

				std::cerr << '[' << client->get_dotted_ip() << " (" << client->get_socket() << ") " << player->get_name() << "] " << cmd << "\r\n";

                if (cmd == "quit") {
					client->write("Daaaag!\r\n");

					// TODO, remove player from game here
					machiavelli::game->deletePlayer(player);					

                    break; // out of game loop, will end this thread and close connection
                }
				else if (cmd == "quitall") {
					client->write("Houdoe!");

					auto players = machiavelli::game->getPlayers();

					for (auto i = 0; i < players.size(); ++i) {
						machiavelli::game->deletePlayer(players.at(i));
					}
					break;
				}
				

                ClientCommand command {cmd, player, client};
                queue.put(command);

            } catch (const std::exception& ex) {
				std::cerr << "EXCEPTION IN CLIENT HANDLER by player: " << player->get_name() << ": " << ex.what() << '\n';
				if (client->is_open()) {
					*client << "ERROR: " << ex.what() << "\r\n";
				}               
            } catch (...) {
				std::cerr << "EXCEPTION IN CLIENT HANDLER by player:" << player->get_name();
				if (client->is_open()) {
					client->write("ERROR: something went wrong during handling of your request. Sorry!\r\n");
				}
            }
        }
        // close weg
    } catch (...) {
		std::cerr << "handle_client crashed\n";
    }
	exit(0);
}

int main(int argc, const char * argv[])
{
    // start command consumer thread
	std::thread consumer {consume_command};

    // create a server socket
    ServerSocket server {machiavelli::tcp_port};

	// create the game.
	machiavelli::game = std::make_shared<Game>();

    while (true) {
        try {
            while (true) {
                // wait for connection from client; will create new socket
				std::cerr << "server listening" << '\n';
				std::unique_ptr<Socket> client {server.accept()};

                // communicate with client over new socket in separate thread
				std::thread handler {handle_client, std::move(client)};
				handler.detach(); // detaching is usually ugly, but in this case the right thing to do
            }
        } catch (const std::exception& ex) {
			std::cerr << ex.what() << ", resuming..." << '\n';
        } catch (...) {
			std::cerr << "problems, problems, but: keep calm and carry on!\n";
        }
    }
    //consumer.join();
    return 0;
}

