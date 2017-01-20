#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "Socket.h"

class Choose
{
private:
	std::string description;
	std::vector<std::string> choices;
	Socket& socket;
	int userInput = -1;
public:
	int createChoices();
	bool pick(std::string command);
	Choose(std::string description, std::vector<std::string> choices, Socket& socket);
};
