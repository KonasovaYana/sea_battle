#ifndef OUTPUTMANAGER_H
#define OUTPUTMANAGER_H
#include "Output.h"

template <typename OutputClass>
class OutputManager {
	OutputClass out;

public:
	OutputManager(OutputClass out) : out(out) {};

	void printMessage(std::string message) {
		out.printMessage(message);
	}

	void showField(GameField& user_field, GameField& enemy_field) {
		out.showField(user_field, enemy_field);
	}

	void help() {
		out.help();
	}

	~OutputManager() = default;
};

#endif