#include "Choose.h"

Choose::Choose(std::string description, std::vector<std::string> choices, Socket& socket) : socket{ socket }, description{ description }, choices{ choices } {}

int Choose::createChoices()
{
	socket << "\r\n" << description << "\r\n";

	for (int i = 0; i < choices.size(); ++i) {
		socket << i << ": " << choices.at(i) << "\r\n";
	}

	userInput = -1;

	while (true)
	{
		if (userInput >= 0 && userInput <= choices.size() - 1)
		{
			return userInput;
		}
	}
}

bool Choose::pick(std::string command)
{
	int input = std::stoi(command);
	if (input > choices.size() - 1) {
		socket << "Je typte: '" << command << "', die optie bestaat niet...\r\n";
		return false;
	}
	else {
		userInput = input;
		return true;
	}
}