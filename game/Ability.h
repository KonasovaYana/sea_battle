#ifndef ABILITY_H
#define ABILITY_H
#include <iostream>
#include <memory>
#include "ShipManager.h"
#include "Ship.h"
#include "GameField.h"

class Ability {
public:
    virtual void attack(int x, int y, ShipManager& manager) = 0;
    virtual std::string getType() const = 0;
    virtual ~Ability() {}
};

#endif
