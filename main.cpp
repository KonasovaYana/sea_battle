#include <iostream>
#include "GameController.h"

int main() {
    try {
        InputManager input_manager(std::cin);
        Game game{ &input_manager };
        GameController<InputManager> game_controller{ &input_manager, game };
        game_controller.inputCommand();

    } catch (const std::exception& e) {
        std::cout << "An unexpected error occurred: " << e.what() << std::endl;
    }
    return 0;
}