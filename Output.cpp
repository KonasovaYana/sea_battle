#include "Output.h"
#include <iomanip>

void Output::printMessage(std::string message) {
	 os << message;
}

char Output::conditionToChar(GameField::CellStatus symbol) {
	switch (symbol) {
	case GameField::Unknown:
        return '.';
	case GameField::Empty:
		return '-';
	case GameField::ShipCell:
		return 's';
    case GameField::Damaged:
		return 'o';
    case GameField::Killed:
		return 'x';
	}
}

char Output::conditionToCharS(GameField::CellStatus symbol) {
	switch (symbol) {
	case GameField::Unknown:
        return '.';
	case GameField::Empty:
		return '-';
	case GameField::ShipCell:
		return '.';
    case GameField::Damaged:
		return 'o';
    case GameField::Killed:
		return 'x';
	}
}

void Output::printFieldTwo(std::pair<int, int> sizes, std::vector<std::vector<GameField::CellStatus>> user_field, std::vector<std::vector<GameField::CellStatus>> enemy_field) {
	std::ostringstream oss;
	oss << std::setw(std::max((sizes.first + 1) * 3, 9)) << "User field" << "    " << std::setw(std::max((sizes.first + 1) * 3, 15)) << "Enemy field" << '\n';
	oss << std::setw(2) << "0" << "|";
	for (int j = 0; j < sizes.first; j++) {
		oss << std::setw(2) << j + 1 << " ";
	}
	oss << "    ";
	oss << std::setw(2) << "0" << "|";
	for (int j = 0; j < sizes.first; j++) {
		oss << std::setw(2) << j + 1 << " ";
	}

	oss << '\n' << "--|";
	for (int j = 0; j < sizes.first; j++) {
		oss << "---";
	}
	oss << "    " << "--|";
	for (int j = 0; j < sizes.first; j++) {
		oss << "---";
	}
	oss << '\n';

	for (int i = 0; i < sizes.second; i++) {
		oss << std::setw(2) << i + 1 << "|";
		for (int j = 0; j < sizes.first; j++) {
			oss << std::setw(2) << conditionToChar(user_field[i][j]) << " ";
		}
		oss << "    " << std::setw(2) << i + 1 << "|";
		for (int j = 0; j < sizes.first; j++) {
			oss << std::setw(2) << conditionToChar(enemy_field[i][j]) << " ";
		}
		oss << '\n';
	}
	oss << '\n';
	printMessage(oss.str());
}

void Output::showField(GameField& user_field, GameField& enemy_field) {
	std::pair<int, int> sizes = enemy_field.getSize();
	printFieldTwo(sizes, enemy_field.getView(), user_field.getView());
}

void Output::help() {
	os << "List of commands:\n" <<
		"[c] - create_game\n" <<
		"[s] - save_game\n" <<
		"[l] - load_game\n" <<
		"[q] - exit\n" <<
		"[a] - attack\n" <<
		"[h] - help\n";
}