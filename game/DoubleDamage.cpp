#include "DoubleDamage.h"
#include <iostream>

DoubleDamage::DoubleDamage(GameField& field) : gameField(field) {}

void DoubleDamage::attack(int x, int y, ShipManager& manager) {
    std::cout << "Double damage was applied." << std::endl;
    if (gameField[x][y]== GameField::ShipCell){
        gameField.attack(x, y, manager);
    }
}