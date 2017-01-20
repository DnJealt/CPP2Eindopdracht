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
#include <chrono>

#include "Socket.h"
#include "Sync_queue.h"
#include "ClientCommand.h"
#include "Player.hpp"
#include "Game.h"

namespace machiavelli {
    const int tcp_port {1080};
	std::shared_ptr<Game> game = nullptr;
}

static bool running = true;

static Sync_queue<ClientCommand> queue;

void consume_command() // runs in its own thread
{
    try {
        while (running) {
            ClientCommand command {queue.get()}; // will block here unless there are still command objects in the queue
			
			std::shared_ptr<Player> player{ command.get_player() };
			Socket& client{ command.get_socket() };

                try {
					// TODO handle command here
					std::thread commandHandler{ &Game::handleCommand, machiavelli::game, command };
					commandHandler.detach();
                    //// TODO handle command here
                    //client << player->get_name() << ", you wrote: '" << command.get_cmd() << "', but I'll ignore that for now.\r\n";
                } catch (const std::exception& ex) {
                    std::cerr << "*** exception in consumer thread for player " << player->get_name() << ": " << ex.what() << '\n';
                    if (client.is_open()) {
                        client.write("Sorry, something went wrong during handling of your request.\r\n");
                    }
                } catch (...) {
                    std::cerr << "*** exception in consumer thread for player " << player->get_name() << '\n';
                    if (client.is_open()) {
                        client.write("Sorry, something went wrong during handling of your request.\r\n");
                    }
               }
            
        }
    } catch (...) {
        std::cerr << "consume_command crashed\n";
    }
}


void handle_client(Socket client) // this function runs in a separate thread
{
    try {
		client.write("Welkom bij Machiavelli server! Om af te sluiten, typ 'quit'.\r\n");
		client.write("Wat is je naam?\r\n");

		std::string name;
		bool done{ false };
		while (!done) {
			done = client.readline([&name](std::string input) {
				name = input;
			});
		}

		client.write("\nHoe oud ben je?\r\n");

		std::string age;
		done = false;
		while (!done) {
			done = client.readline([&age](std::string input) {
				age = input;
			});
		}
		
		//create player
		std::shared_ptr<Player> player{ new Player(name, std::stoi(age), client) };

		//add player to the game
		machiavelli::game->addPlayer(player);

		client << "\nWelkom, " << player->get_name() << ", Typ 'start' om te beginnen!\r\n";

        while (running) { // game loop
            try {
                // read first line of request
                std::string cmd;
                if (client.readline([&cmd](std::string input) { cmd=input; })) {
                    std::cerr << '[' << client.get_dotted_ip() << " (" << client.get_socket() << ") " << player->get_name() << "] " << cmd << "\r\n";

                    if (cmd == "quit") {
						client.write("Bye!\r\n");
                        break; // out of game loop, will end this thread and close connection
                    }
                    else if (cmd == "quitall") {
                        running = false;
                    }

                    ClientCommand command {cmd, player, client};
                    queue.put(command);
                };

            } catch (const std::exception& ex) {
				client << "ERROR: " << ex.what() << "\r\n";
            } catch (...) {
				client.write("ERROR: something went wrong during handling of your request. Sorry!\r\n");
            }
        }
        // close weg
    } 
    catch(std::exception &ex) {
        std::cerr << "handle_client " << ex.what() << "\n";
    }
    catch (...) {
        std::cerr << "handle_client crashed\n";
    }
}

int main(int argc, const char * argv[])
{
    // start command consumer thread
    std::vector<std::thread> all_threads;
    all_threads.emplace_back(consume_command);

    // create a server socket
    ServerSocket server {machiavelli::tcp_port};

	// create the game
	machiavelli::game = std::make_shared<Game>();

    try {
        std::cerr << "server listening" << '\n';
        while (running) {
            // wait for connection from client; will create new socket
            server.accept([&all_threads](Socket client) {
                std::cerr << "Connection accepted from " << client.get_dotted_ip() << "\n";
                all_threads.emplace_back(handle_client, std::move(client));
            });
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << ", resuming..." << '\n';
    } catch (...) {
        std::cerr << "problems, problems, but: keep calm and carry on!\n";
    }

    for (auto &t : all_threads) {
        t.join();
    }

    return 0;
}

