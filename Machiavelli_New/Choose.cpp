#include "Choose.h"

Choose::Choose(std::string description, std::vector<std::string> choices, Socket& socket) : socket {socket}
{
	this->description = description;
	this->choices = choices;
}

int Choose::activate()
{
	socket << "\r\n" << description << "\r\n";

	for (int i = 0; i < this->choices.size(); ++i) {
		socket << i << ": " << this->choices[i] << "\r\n";
	}

	userInput = -1;
	while (true)
	{
		if (userInput >= 0 && userInput <= this->choices.size() - 1)
		{
			return userInput;
		}
	}
}

bool Choose::pick(std::string command)
{
	int test = std::stoi(command);
	if (test > choices.size() - 1) {
		socket << "You wrote: '" << command << "', but that option doesn't exist.\r\n";
		return false;
	}
	else {
		userInput = test;
		return true;
	}
}

Choose::~Choose()
{
}
