#include "CardReader.h"

CardReader::CardReader(std::string readerName)
{
	if (readerName == "character")
		readCharacters();
	if (readerName == "build")
		readBuildings();
}

void CardReader::readCharacters()
{
	fileName = "karakterkaarten.csv";
	std::ifstream input_file(fileName);
	std::string line;

	std::vector<std::string> tokens;

	while (std::getline(input_file, line)) {
		std::string buffer;
		std::stringstream stringStream(line);
		
		while (std::getline(stringStream, buffer, ';'))
			tokens.push_back(buffer);
		
		std::shared_ptr<CharacterCard> character{ new CharacterCard() };
		character->setNumber(atoi(tokens[0].c_str()));
		character->setName(tokens[1]);
		
		characters.push_back(character);
	}
}

void CardReader::readBuildings()
{

}

std::vector<std::shared_ptr<CharacterCard>> CardReader::getCharactersInOrder()
{
	return characters;
}

std::vector<std::shared_ptr<CharacterCard>> CardReader::getCharactersShuffled()
{ 
	// return characters in shuffeled order
	std::vector<std::shared_ptr<CharacterCard>> shuffled{ characters };
	std::random_shuffle(shuffled.begin(), shuffled.end());

	return shuffled;
}

std::vector<std::shared_ptr<BuildingCard>> CardReader::getBuildings()
{
	return buildings;
}

CardReader::~CardReader()
{
}
