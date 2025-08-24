#ifndef SHIP_MANAGER_H
#define SHIP_MANAGER_H

#include "Ship.h"
#include <vector>
#include <iostream>

class ShipManager {
public:
    ShipManager(const std::vector<int>& ship_limits);
    bool add_ship(const Ship& ship);
    bool all_ship_sunk() const;
    void take_damage(int ship_index, int segment_index);
    const Ship& get_ship(int index) const;
    int get_ship_count() const;
    ShipManager() : ships(), ship_count(), ship_limits() {}
    void serialize(std::ostream& out) const;
    void deserialize(std::istream& in);
    // friend std::ostream& operator<<(std::ostream& os, const ShipManager& manager);


private:
    std::vector<Ship> ships;
    std::vector<int> ship_count;
    std::vector<int> ship_limits;
    bool can_add_ship(int length) const;
};

#endif