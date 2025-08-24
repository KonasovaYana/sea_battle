#include "AbilityManager.h"
#include <cstdlib>
#include <ctime>

AbilityManager::AbilityManager(GameField& field) : gameField(field) {
    std::srand(static_cast<unsigned>(std::time(0))); 
    add_start_abilities(); 
}


void AbilityManager::use_ability(int x, int y, ShipManager& manager) {
    if (!abilities.empty()) {
        abilities.back()->attack(x, y, manager); 
        delete abilities.back(); 
        abilities.pop_back(); 
    } else {
        throw NoAbilityException();
    }
}

void AbilityManager::add_random_ability() {
    int ability_type = std::rand() % 3;
    switch (ability_type) {
        case 0:
            abilities.push_back(new DoubleDamage(gameField));
            break;
        case 1:
            abilities.push_back(new Scanner(gameField));
            break;
        case 2:
            abilities.push_back(new RandomShot(gameField));
            break;
    }
}

void AbilityManager::add_start_abilities() {
    if (!abilities.empty()) return;
    int indices[3] = {0, 1, 2};
    for (int i = 2; i > 0; --i) {
        int j = std::rand() % (i + 1); 
        std::swap(indices[i], indices[j]); 
    }

    for (int i = 0; i < 3; ++i) {
        switch (indices[i]) {
            case 0:
                abilities.push_back(new DoubleDamage(gameField));
                break;
            case 1:
                abilities.push_back(new Scanner(gameField));
                break;
            case 2:
                abilities.push_back(new RandomShot(gameField));
                break;
        }
    }
}

std::vector<Ability*> AbilityManager::get_ability_stack(){
    return abilities;
}

void AbilityManager::on_ship_sunk(){
    add_random_ability();
    std::cout << "A new ability was added because a ship was sunk!" << std::endl;
}

void AbilityManager::serialize(std::ostream& out) const {
    size_t size = abilities.size();
    out.write(reinterpret_cast<const char*>(&size), sizeof(size));

    for (const auto& ability : abilities) {
        std::string type = ability->getType(); 
        size_t type_size = type.size();
        out.write(reinterpret_cast<const char*>(&type_size), sizeof(type_size));
        out.write(type.c_str(), type_size);
    }
}

void AbilityManager::deserialize(std::istream& in, GameField& field) {
    size_t size;
    in.read(reinterpret_cast<char*>(&size), sizeof(size));
    abilities.clear(); 
    for (size_t i = 0; i < size; ++i) {
        size_t type_size;
        in.read(reinterpret_cast<char*>(&type_size), sizeof(type_size));
        std::string type(type_size, '\0');
        in.read(&type[0], type_size);
        Ability* new_ability = nullptr;
        if (type == "DoubleDamage") {
            new_ability = new DoubleDamage(field);  
        } else if (type == "Scanner") {
            new_ability = new Scanner(field);
        } else if (type == "RandomShot") {
            new_ability = new RandomShot(field);
        }
        if (new_ability) {
            abilities.push_back(new_ability);  
        }
    }
}
