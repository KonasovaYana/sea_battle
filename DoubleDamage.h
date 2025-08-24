#ifndef DOUBLE_DAMAGE_H
#define DOUBLE_DAMAGE_H

#include "Ability.h"

class DoubleDamage : public Ability {
private:
    GameField& gameField;
public:
    DoubleDamage(GameField& field);
    std::string getType() const override { return "DoubleDamage"; }
    void attack(int x, int y, ShipManager& manager) override;
};
#endif