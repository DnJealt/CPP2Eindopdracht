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
	std::shared_ptr<Socket> socket;
	int userInput = -1;
public:
	int activate();
	bool pick(std::string command);
	Choose(std::string description, std::vector<std::string> choices, std::shared_ptr<Socket> socket);
	~Choose();
};

