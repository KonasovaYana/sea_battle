#include "Game.h"
using namespace std;
constexpr int FIELD_SIZE = 10;
Game::Game(InputManager* input_manager) :  user_field(FIELD_SIZE, FIELD_SIZE), enemy_field(FIELD_SIZE, FIELD_SIZE), user_manager({4, 3, 2, 1}), enemy_manager({4, 3, 2, 1}), ability_manager(enemy_field) {
    srand(static_cast<unsigned>(time(nullptr))); 
    output_manager = new OutputManager<Output>(out);
    this->input_manager = input_manager; 
    
}
void Game::start_new_game() {

    user_field = GameField(FIELD_SIZE, FIELD_SIZE);
    enemy_field = GameField(FIELD_SIZE, FIELD_SIZE);
    user_manager = ShipManager({4, 3, 2, 1});
    enemy_manager = ShipManager({4, 3, 2, 1});
    generate_enemy_field();
    output_manager->printMessage("New game started. Set up your ships.\n");
    generate_user_field();
    output_manager->showField(enemy_field, user_field);
    ability_manager = AbilityManager(enemy_field);
    enemy_field.register_observer(&ability_manager);
    is_created = true;
}

void Game::generate_enemy_field() {
    std::vector<int> ship_lengths = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};
    for (int len : ship_lengths) {
        bool placed = false;
        int attempts = 0;
        while (!placed) {
            int x = rand() % FIELD_SIZE;
            int y = rand() % FIELD_SIZE;
            Ship::Orientation orientation = static_cast<Ship::Orientation>(rand() % 2);
            Ship ship(len, orientation);
            try {
                placed = enemy_field.place_ship(ship, x, y, enemy_manager);
            } catch (const InvalidShipPlacementException& e) {
                placed = false;
            }
        }
    }
}
void Game::generate_user_field(){
    std::vector<int> ship_lengths = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};
    output_manager->printMessage("Your turn. You must enter data for 10 ships: from four-deck to single-deck.\n");
    output_manager->printMessage("Enter coordinates for place and orientation (x y orientation), (Horizontal = 0, Vertical = 1)\n");
    for (int len : ship_lengths) {
        bool placed = false;
        while (!placed) {
            output_manager->printMessage("Now you are placing the ship with length " + std::to_string(len) + "\n");
            std::vector<int> input = input_manager->inputXYOrient(FIELD_SIZE, FIELD_SIZE);
            int x = input[0];
            int y = input[1];
            int ori = input[2];
            if (x < 0 || x >= FIELD_SIZE || y < 0 || y >= FIELD_SIZE || (ori != 0 && ori != 1)){
                output_manager->printMessage("Incorrect parametrs\n");
            }else{
                Ship::Orientation orientation;
                if (ori == 0) {
                    orientation = Ship::Horizontal;
                } else {
                    orientation = Ship::Vertical;
                }
                try {
                    Ship ship(len, orientation);
                    placed = user_field.place_ship(ship, x, y, user_manager);
                } catch (const InvalidShipPlacementException& e) {
                    placed = false;
                    output_manager->printMessage("Your placement is incorrect\n");
                }
            }
        }
    }
}
void Game::attack() {
    bool user_wins = false;
    bool enemy_wins = false;
    make_user_move(ability_manager);
    if (enemy_manager.all_ship_sunk()) {
        user_wins = true;
    }else{
        make_enemy_move(ability_manager);
        if (user_manager.all_ship_sunk()) {
            enemy_wins = true;
        }
    }
    if (user_wins) {
        output_manager->printMessage("You win this round!\n");
        GameField previous_user_field = std::move(user_field);
        ShipManager previous_user_manager = std::move(user_manager);
        enemy_field = GameField(FIELD_SIZE, FIELD_SIZE);
        enemy_manager = ShipManager({4, 3, 2, 1});
        generate_enemy_field();
        user_field = std::move(previous_user_field);
        user_manager = std::move(previous_user_manager);
    } else if (enemy_wins) {
        output_manager->printMessage("You lose this round. Starting a new game.\n");
        start_new_game();
    }
}

void Game::make_user_move(AbilityManager& ability_manager){
    output_manager->printMessage("Your turn. Enter coordinates for attack and 1 if you want to use ability or 0 if wan't (x y 0/1): \n");
    std::vector<int> input = input_manager->inputXYOrient(FIELD_SIZE, FIELD_SIZE);
    int x = input[0];
    int y = input[1];
    int ability = input[2];
    if (x < 0 || x >= FIELD_SIZE || y < 0 || y >= FIELD_SIZE){
        output_manager->printMessage("Attempt to attack out of bounds.\n");
        return;
    }
    try {
        if (ability ==1){
            std::vector<Ability*> stack_ability = ability_manager.get_ability_stack();
            if (stack_ability.empty()) {
                throw NoAbilityException();
            }
            auto usable_ability = stack_ability.back();
            if (usable_ability->getType()=="Scanner"){
                ability_manager.use_ability(x, y, enemy_manager);
                output_manager->printMessage("Based on the scan, enter the coordinates for the attack.\n");
                std::pair<int, int> input = input_manager->inputXY(FIELD_SIZE, FIELD_SIZE);
                x = input.first;
                y = input.second;
            }else{
                ability_manager.use_ability(x, y, enemy_manager);
            }
        }
    } catch (const NoAbilityException& e) {
        std::cout << "Warning: "<< e.what() << "\n";
    } catch (const OutOfBoundsAttackException& e) {
        std::cout << e.what() << "\n";
    }try{
        enemy_field.attack(x, y, enemy_manager);
    }catch (const OutOfBoundsAttackException& e) {
        std::cout << e.what() << "\n";
    }
}

void Game::make_enemy_move(AbilityManager& ability_manager){
    int enemy_x = rand() % FIELD_SIZE;
    int enemy_y = rand() % FIELD_SIZE;
    output_manager->printMessage("Enemy attacks (" + std::to_string(enemy_x) +  ", " + std::to_string(enemy_y) + ").\n");
    try {
        user_field.attack(enemy_x, enemy_y, user_manager);
    } catch (const OutOfBoundsAttackException& e) {
        output_manager->printMessage("Enemy attack failed.\n");
    }
    output_manager->showField(enemy_field, user_field);
}
void Game::save_game(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file for saving.");
    }
    GameState state{user_field, enemy_field, user_manager, enemy_manager, ability_manager};
    file << state;
    file.close();
    output_manager->printMessage("Game saved to savefile.dat.\n");
    is_saved=true;
}

void Game::load_game(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        file.clear();  
        file.seekg(0);
        throw std::runtime_error("Failed to open file for loading.");
    }
    GameState state = GameState::load_state(filename);
    user_field = state.get_user_field();
    enemy_field = state.get_enemy_field();
    user_manager = state.get_user_manager();
    enemy_manager = state.get_enemy_manager();
    ability_manager = state.get_ability_manager();
    std::cout << "Game loaded from " << filename << ".\n";
    is_created=true;
}

OutputManager<Output>* Game::getOutputManager() {
	return output_manager;
}
void Game::quit() {
	exit(0);
}

bool Game::is_game_created(){
    return is_created;
}

bool Game::is_game_saved(){
    return is_saved;
}