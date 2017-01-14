//
//  Player.cpp
//  socketexample
//
//  Created by Bob Polis on 23-11-15.
//  Copyright © 2015 Avans Hogeschool, 's-Hertogenbosch. All rights reserved.
//

#include "Player.hpp"

const Player & Player::operator<<(const std::string & message) const
{
	socket->write(message);
	return *this;
	// TODO: write message to socket, and return it.
}
