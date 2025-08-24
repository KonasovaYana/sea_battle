#ifndef STATEGAME_H
#define STATEGAME_H
#include "GameField.h" 
#include "AbilityManager.h"
#include <iostream>
#include <string>
class GameState {
public:
    GameState(const GameField& user_field, const GameField& enemy_field, const ShipManager& user_manager, const ShipManager& enemy_manager, const AbilityManager& ability_manager);
    void save_state(const std::string& filename) const;
    static GameState load_state(const std::string& filename);
    friend std::ostream& operator<<(std::ostream& os, const GameState& state);
    friend std::istream& operator>>(std::istream& is, GameState& state);
    const GameField& get_user_field() const;
    const GameField& get_enemy_field() const;
    const ShipManager& get_user_manager() const;
    const ShipManager& get_enemy_manager() const;
    const AbilityManager& get_ability_manager() const;

private:
    GameField user_field;
    GameField enemy_field;
    ShipManager user_manager;
    ShipManager enemy_manager;
    AbilityManager ability_manager;
};

#endif 