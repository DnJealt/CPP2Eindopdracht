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
#include "ClientInfo.h"
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
	
            if (auto clientInfo = command.get_client_info().lock()) {
                auto &client = clientInfo->get_socket();
                auto &player = clientInfo->get_player();
                try {
                    // TODO handle command here
					client << player->get_name() << ", you wrote: '" << command.get_cmd() << "', but I'll ignore that for now.\r\n";
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
        }
    } catch (...) {
		std::cerr << "consume_command crashed\n";
    }
}

std::shared_ptr<ClientInfo> init_client_session(Socket client) {
    client.write("Welcome to Machiavelli server! To quit, type 'quit'.\r\n");
    client.write("What's your name?\r\n");
    std::string name {client.readline()};

	client.write("What's your age?\r\n");

	bool valid = false;
	int age = 0;
	while (!valid) {
		std::string input{ client.readline() };
		if (isdigit(input[0])) {
			valid = true;
			age = std::stoi(input);
		}
	};

	std::shared_ptr<Player> player{ new Player(name, age) };
		
    return std::make_shared<ClientInfo>(std::move(client), player);
}

void handle_client(Socket client) // this function runs in a separate thread
{
    try {
        auto client_info = init_client_session(std::move(client));

		// add player to the game
		machiavelli::game->addPlayer(client_info->get_player());

		client_info->get_socket() << "Welcome, " << client_info->get_player()->get_name() << ", Type 'start' to play the game!\r\n";

        while (true) { // game loop
            try {
                // read first line of request
				std::string cmd { client_info->get_socket().readline() };

				std::cerr << '[' << client_info->get_socket().get_dotted_ip() << " (" << client_info->get_socket().get_socket() << ") " << client_info->get_player()->get_name() << "] " << cmd << "\r\n";

                if (cmd == "quit") {
					client_info->get_socket().write("Bye!\r\n");

					// TODO, remove player from game here
					machiavelli::game->deletePlayer(client_info->get_player());

                    break; // out of game loop, will end this thread and close connection
                }

                ClientCommand command {cmd, client_info};
                queue.put(command);

            } catch (const std::exception& ex) {
				std::cerr << "EXCEPTION IN CLIENT HANDLER by player: " << client_info->get_player()->get_name() << ": " << ex.what() << '\n';
				if (client.is_open()) {
					client_info->get_socket() << "ERROR: " << ex.what() << "\r\n";
				}               
            } catch (...) {
				std::cerr << "EXCEPTION IN CLIENT HANDLER by player:" << client_info->get_player()->get_name();
				if (client.is_open()) {
					client_info->get_socket().write("ERROR: something went wrong during handling of your request. Sorry!\r\n");
				}
            }
        }
        // close weg
    } catch (...) {
		std::cerr << "handle_client crashed\n";
    }
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
                Socket client {server.accept()};

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
    consumer.join();
    return 0;
}

