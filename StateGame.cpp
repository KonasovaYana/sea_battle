#include "StateGame.h"
#include <fstream>
#include <iostream>
#include <stdexcept>

GameState::GameState(const GameField& user_field, const GameField& enemy_field,
                     const ShipManager& user_manager, const ShipManager& enemy_manager, const AbilityManager& ability_manager)
    : user_field(user_field), enemy_field(enemy_field),
      user_manager(user_manager), enemy_manager(enemy_manager), ability_manager(ability_manager) {}

void GameState::save_state(const std::string& filename) const {
    std::ofstream out(filename, std::ios::binary);
    if (!out) {
        throw std::runtime_error("Failed to open file for saving.");
    }
    user_field.serialize(out);
    enemy_field.serialize(out);
    user_manager.serialize(out);
    enemy_manager.serialize(out);
    ability_manager.serialize(out);

    if (!out.good()) {
        throw std::runtime_error("Error occurred during saving.");
    }
}


GameState GameState::load_state(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) {
        throw std::runtime_error("Failed to open file for loading.");
    }
    GameField user_field, enemy_field;
    ShipManager user_manager, enemy_manager;


    user_field.deserialize(in);
    enemy_field.deserialize(in);
    user_manager.deserialize(in);
    enemy_manager.deserialize(in);
    
    AbilityManager ability_manager(enemy_field);
    ability_manager.deserialize(in, enemy_field);

    if (!in.good()) {
        throw std::runtime_error("Error occurred during loading.");
    }

    return GameState(user_field, enemy_field, user_manager, enemy_manager, ability_manager);
}


std::ostream& operator<<(std::ostream& os, const GameState& state) {
    state.user_field.serialize(os);
    state.enemy_field.serialize(os);
    state.user_manager.serialize(os);
    state.enemy_manager.serialize(os);
    state.ability_manager.serialize(os);
    return os;
}

std::istream& operator>>(std::istream& is, GameState& state) {
    state.user_field.deserialize(is);
    state.enemy_field.deserialize(is);
    state.user_manager.deserialize(is);
    state.enemy_manager.deserialize(is);
    state.ability_manager.deserialize(is, state.enemy_field);

    if (!is.good()) {
        throw std::runtime_error("Error occurred during deserialization.");
    }

    return is;
}

const GameField& GameState::get_user_field() const{
    return user_field;
}
const GameField& GameState::get_enemy_field() const{
    return enemy_field;
}
const ShipManager& GameState::get_user_manager() const{
    return user_manager;
}
const ShipManager& GameState::get_enemy_manager() const{
    return enemy_manager;
}
const AbilityManager& GameState::get_ability_manager() const{
    return ability_manager;
}
