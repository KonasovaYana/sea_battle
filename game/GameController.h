#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "Game.h"
#include "InputManager.h"
#include <sstream>
#include <stdexcept>

template <typename InputClass>
class GameController {
    Game& game;
    InputClass* input;

public:
    GameController(InputClass* input, Game& game)
        : input(input), game(game) {}

    void inputCommand() {
        while (true) {
            game.getOutputManager()->printMessage("Enter a command\n");
            Commands current_command = input->inputCommand();
            checkCommand(current_command);
        }
    }

    void checkCommand(Commands current_command) {
        OutputManager<Output>* output_manager = game.getOutputManager();
        std::ifstream file;
        switch (current_command) {
            case CREATE_GAME:
                game.start_new_game();
                break;

            case SAVE_GAME:
                try {
                    if (game.is_game_created()){
                        game.save_game("savefile.dat");
                    } else {
                        throw std::logic_error("Create a game first\n");
                    }
                }catch (const std::logic_error& e) {
                    output_manager->printMessage(e.what());
                }
                break;

            case LOAD_GAME:
                file.open("savefile.dat", std::ios::binary);
                try {
                    if (file){
                        game.load_game("savefile.dat");
                    } else {
                        throw std::logic_error("There is no previously saved game\n");
                    }
                }catch (const std::logic_error& e) {
                    output_manager->printMessage(e.what());
                }
                break;

            case QUIT:
                game.quit();
                break;

            case ATTACK:
                try {
                    if (game.is_game_created()){
                        game.attack();
                    } else {
                        throw std::logic_error("Create a game first\n");
                    }
                }catch (const std::logic_error& e) {
                    output_manager->printMessage(e.what());
                }
                break;

            case HELP:
                output_manager->help();
                break;

            default:
                output_manager->printMessage("Unknown command");
                break;
        }
    }
};

#endif 
