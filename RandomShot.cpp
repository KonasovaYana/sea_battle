#include "RandomShot.h"
#include <iostream>
RandomShot::RandomShot(GameField& field) : gameField(field) {
    std::srand(static_cast<unsigned>(std::time(0))); 
}
void RandomShot::attack(int x, int y, ShipManager& manager) {
    int lower_bound_index = 0; 
    int upper_bound_index = 9;
    int random_index = lower_bound_index + std::rand() % (upper_bound_index - lower_bound_index + 1);
    const Ship& attacked_ship = manager.get_ship(random_index);

    int lower_bound_segment = 0;
    int upper_bound_segment = attacked_ship.get_len() - 1;

    int random_segment = lower_bound_segment + std::rand() % (upper_bound_segment - lower_bound_segment + 1);

    if (attacked_ship.get_cond(random_segment) == Ship::Killed) {
        attack(x, y, manager);
    }else{
        manager.take_damage(random_index, random_segment);
    }
    std::cout << "The random ship was attacked." << std::endl;
}


