#ifndef OUTPUT_H
#define OUTPUT_H

#include <ostream>
#include <sstream>
#include "GameField.h"

class Output{
private:
	std::ostream& os;
	char conditionToCharS(GameField::CellStatus symbol);
	char conditionToChar(GameField::CellStatus symbol);

public:
	Output(std::ostream& os) : os(os) {};

	void printMessage(std::string message);

	void printFieldTwo(std::pair<int, int> sizes, std::vector<std::vector<GameField::CellStatus>> user_field, std::vector<std::vector<GameField::CellStatus>> enemy_field);

	void showField(GameField& user_field, GameField& enemy_field);

    void help();

	~Output() = default;
};

#endif