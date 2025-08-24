#ifndef SCANNER_H
#define SCANNER_H

#include "Ability.h"
class Scanner : public Ability {
private:
    GameField& gameField;
public:
    std::string getType() const override { return "Scanner"; }
    Scanner(GameField& field);
    void attack(int x, int y, ShipManager& manager) override;
};
#endif
