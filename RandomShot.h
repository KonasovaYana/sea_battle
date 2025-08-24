#ifndef RANDOM_SHOT_H
#define RANDOM_SHOT_H
#include <random>
#include <ctime>
#include "Ability.h"
#include <cstdlib> 


class RandomShot : public Ability {
private:
    GameField& gameField;
    std::mt19937 generate; 
public:
    RandomShot(GameField& field);
    std::string getType() const override { return "RandomShot"; }
    void attack(int x, int y, ShipManager& manager) override;
};
#endif