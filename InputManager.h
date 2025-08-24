#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "InvalidShipPlacementException.h"
#include "json.hpp"
#include "Output.h"
using json = nlohmann::json;

enum Commands {
	CREATE_GAME,
	SAVE_GAME,
	LOAD_GAME,
	QUIT,
	ATTACK,
	HELP,
};

class InputManager {
private:
	std::istream& is;
	Commands stringToCommand(const std::string str);

public:
	InputManager(std::istream& is) : is(is) {};
	void validateCommandBindings(const json& data);
	std::pair<int, int> inputXY(int width, int height);
	std::vector<int> inputXYOrient(int width, int height);
	~InputManager() = default;
	Commands inputCommand();
};

#endif