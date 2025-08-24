#include "InputManager.h"
#include <set>

std::pair<int, int> InputManager::inputXY(int width, int height) {
	int y, x;
	std::string input;
	while (true) {
		std::getline(is, input);
		std::stringstream ss(input);
		try {
			if (!(ss >> x >> y) || !ss.eof() || y < 0 || x < 0 || y > height-1 || x > width-1) {
				throw InvalidShipPlacementException();
			}
			break;
		}
		catch (InvalidShipPlacementException& e) {
			std::cout << e.what() << '\n';
		}
	}
	return std::make_pair(x, y);
}

std::vector<int> InputManager::inputXYOrient(int width, int height) {
	int x, y, orient;
	std::string input;
	while (true) {
		std::getline(is, input);
		std::stringstream ss(input);
		try {
			if (!(ss >> x >> y >> orient) || !ss.eof() || y < 0 || x < 0 ||
				y > height-1 || x > width-1 || (orient != 0 && orient != 1)) {
				throw InvalidShipPlacementException();
			}
			break;
		}
		catch (InvalidShipPlacementException& e) {
			std::cout << e.what() << '\n';
		}
	}

	std::vector <int> res = { x, y, orient };
	return res;
}

Commands InputManager::stringToCommand(std::string str) {
	if (str == "c") return CREATE_GAME;
	if (str == "s") return SAVE_GAME;
	if (str == "l") return LOAD_GAME;
	if (str == "q") return QUIT;
	if (str == "a") return ATTACK;
    if (str == "h") return HELP;
	throw std::invalid_argument("Invalid command: " + str);
}

void InputManager::validateCommandBindings(const json& data) {
    std::set<std::string> keys;
    std::set<std::string> commands;

    for (auto it = data.begin(); it != data.end(); ++it) {
        const std::string& key = it.key();
        const std::string& command = it.value();
        if (!keys.insert(key).second) {
            throw std::invalid_argument("Duplicate key found: " + key);
        }
        if (!commands.insert(command).second) {
            throw std::invalid_argument("Duplicate command found: " + command);
        }
    }
    std::set<std::string> requiredCommands = { "create_game", "save_game", "load_game", "exit", "attack", "help" };
    for (const std::string& required : requiredCommands) {
        if (commands.find(required) == commands.end()) {
            throw std::invalid_argument("Missing command binding for: " + required);
        }
    }
}

Commands InputManager::inputCommand() {
	std::string input;
	json data;
	std::ifstream input_file("commands.json");;
	if (input_file.is_open()) {
        input_file >> data;
        input_file.close();
        validateCommandBindings(data);
    } else {
        data = {
            {"c", "create_game"},
            {"s", "save_game"},
            {"l", "load_game"},
            {"q", "exit"},
            {"a", "attack"},
            {"h", "help"}
        };
    }
	while (true) {
		std::getline(is, input);
		try {
			if (data.contains(input)) {
				return stringToCommand(input);
			}else{
				throw std::invalid_argument("Enable command");
			}
		}
		catch (const std::invalid_argument& e) {
			std::ostringstream oss;
			oss << e.what() << '\n';
			Output out{ std::cout };
			out.printMessage(oss.str());
		}
	}
}