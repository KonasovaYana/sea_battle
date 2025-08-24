#ifndef GAME_H
#define GAME_H

#include "GameField.h"
#include "ShipManager.h"
#include "AbilityManager.h"
#include "NoAbilityException.h"
#include "InvalidShipPlacementException.h"
#include "OutOfBoundsAttackException.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "StateGame.h"
#include <fstream>  
#include <iostream>
#include "OutputManager.h"
#include "InputManager.h"
class Game {
private:
    GameField user_field;     
    GameField enemy_field;     
    ShipManager user_manager;   
    ShipManager enemy_manager; 
    AbilityManager ability_manager;
    

    void generate_enemy_field();
    void generate_user_field();

    Output out{ std::cout };
    InputManager* input_manager;
	OutputManager<Output>* output_manager;
    
public:
    Game(InputManager* input_manager);                 
    void start_new_game();
    void save_game(const std::string& filename);  
    void load_game(const std::string& filename);
    void attack(); 
    void make_user_move(AbilityManager& ability_manager);
    void make_enemy_move(AbilityManager& ability_manager);  
    void handle_user_command();
    OutputManager<Output>* getOutputManager();
    void quit();
    bool is_game_created();
    bool is_game_saved();
    bool is_saved = false;
    bool is_created =false;
};

#endif 
