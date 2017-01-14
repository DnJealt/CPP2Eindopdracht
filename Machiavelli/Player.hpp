//
//  Player.hpp
//  socketexample
//
//  Created by Bob Polis on 23-11-15.
//  Copyright © 2015 Avans Hogeschool, 's-Hertogenbosch. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include <string>
#include <memory>

#include "Socket.h"
#include "Game.h"

class Player {
public:
    Player() {}
	Player(const std::string& name, const int age, std::shared_ptr<Socket> socket) : name{ name }, age{ age }, socket{socket} {}

    std::string get_name() const { return name; }
	int get_age() const { return age; };
    void set_name(const std::string& new_name) { name = new_name; }

	//operator overloading player to write message to socket.
	const Player & operator<<(const std::string & message) const;
private:
    std::string name;
	int age;
	std::shared_ptr<Socket> socket;
};

#endif /* Player_hpp */
