#ifndef ABILITYMANAGER_H
#define ABILITYMANAGER_H

#include <functional>
#include <vector>
#include "Ability.h"
#include "GameField.h"
#include "ShipManager.h"
#include "DoubleDamage.h"
#include "Scanner.h"
#include "RandomShot.h"
#include "NoAbilityException.h"
#include "IShipSunkObserver.h"

class AbilityManager: public IShipSunkObserver {
public:
    explicit AbilityManager(GameField& field);
    AbilityManager() = delete;
    void use_ability(int x, int y, ShipManager& manager);
    void add_random_ability();
    std::vector<Ability*> get_ability_stack();
    void on_ship_sunk();
    void serialize(std::ostream& out) const;
    void deserialize(std::istream& in, GameField& field);


private:
    std::reference_wrapper <GameField> gameField;
    std::vector<Ability*> abilities;
    void add_start_abilities();

};

#endif