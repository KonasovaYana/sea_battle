#include <iostream>
#include "Scanner.h"

Scanner::Scanner(GameField& field) : gameField(field) {}

void Scanner::attack(int x, int y, ShipManager& manager) {
    if ((gameField.check_coords(x, y) && gameField[x][y] == GameField::ShipCell) || (gameField.check_coords(x+1, y) && gameField[x+1][y] == GameField::ShipCell) || (gameField.check_coords(x, y+1) && gameField[x][y+1] == GameField::ShipCell) ||(gameField.check_coords(x+1, y+1) && gameField[x+1][y+1] == GameField::ShipCell) ){
        std::cout << "There is a ship in this segment" << std::endl;
    }else{
        std::cout << "There is noting" << std::endl;
    }
    
}