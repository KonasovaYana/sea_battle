#include "ShipManager.h"
#include <iostream>
#include <string>
ShipManager::ShipManager(const std::vector<int>& ship_limits)
    : ship_limits(ship_limits), ship_count(4, 0) {}

bool ShipManager::can_add_ship(int length) const {
    if (length < 1 || length > 4) return false;
    return ship_count[length - 1] < ship_limits[length - 1];
}

bool ShipManager::add_ship(const Ship& ship) {
    int len = ship.get_len();
    if (!can_add_ship(len)) {
        return false;
    }
    ships.push_back(ship);
    ship_count[len - 1]++;
    return true;
}

bool ShipManager::all_ship_sunk() const {
    for (const auto& ship : ships) {
        if (!ship.is_sunk()) {
            return false;
        }
    }
    return true;
}

void ShipManager::take_damage(int ship_index, int segment_index) {
    ships[ship_index].take_damage(segment_index);
}

const Ship& ShipManager::get_ship(int index) const{
    return ships[index];
}

int ShipManager::get_ship_count() const {
    return ships.size();
}

void ShipManager::serialize(std::ostream& out) const {
    size_t ship_count = ships.size();
    out.write(reinterpret_cast<const char*>(&ship_count), sizeof(ship_count)); 

    for (const auto& ship : ships) {
        int len = ship.get_len();
        int ori = static_cast<int>(ship.get_ori());
        out.write(reinterpret_cast<const char*>(&len), sizeof(len)); 
        out.write(reinterpret_cast<const char*>(&ori), sizeof(ori)); 

        if (len <= 0) {
            throw std::runtime_error("Invalid ship length: 0 or negative.");
        }
        for (int i = 0; i < len; ++i) {
            int segment_condition = static_cast<int>(ship.get_cond(i));
            out.write(reinterpret_cast<const char*>(&segment_condition), sizeof(segment_condition));
        }
    }
}
void ShipManager::deserialize(std::istream& in) {
    

    size_t ship_count;
    in.read(reinterpret_cast<char*>(&ship_count), sizeof(ship_count));

    ships.clear();
    for (size_t i = 0; i < ship_count; ++i) {
        int len, ori;
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        in.read(reinterpret_cast<char*>(&ori), sizeof(ori));

        Ship new_ship(len, static_cast<Ship::Orientation>(ori));

        for (int j = 0; j < len; ++j) {
            int segment_condition;
            in.read(reinterpret_cast<char*>(&segment_condition), sizeof(segment_condition));
            new_ship[j] = static_cast<Ship::SegmentCondition>(segment_condition);
        }

        ships.push_back(std::move(new_ship));
    }
}


// std::istream& operator>>(std::istream& is, ShipManager& ship_manager) {
//     int num_of_ships;
//     std::cout << "Enter the number of ships: ";
//     is >> num_of_ships;

//     for (int i = 0; i < num_of_ships; ++i) {
//         int ori, len;
//         std::cout << "Enter length for ship " << (i + 1) << ":\n";
//         is >> len;
//         std::cout << "Enter orientation for ship " << (i + 1) << ":\n";
//         is >> ori;
//         Ship::Orientation orientation;
//         if (ori == 0) {
//             orientation = Ship::Horizontal;
//         } else {
//             orientation = Ship::Vertical;
//         }
//         std::vector<int> condition(len, 2);
//         Ship ship(len, orientation);
//         ship_manager.add_ship(ship);
//     }
//     return is;
// }

// std::ostream& operator<<(std::ostream& os, const ShipManager& manager) {
//     os << manager.ships.size() << '\n';
//     for (const auto& ship : manager.ships) {
//         os << ship << '\n'; 
//     }
//     os << manager.ship_count.size() << '\n';
//     for (size_t i = 0; i < manager.ship_count.size(); ++i) {
//         os << manager.ship_count[i] << ' ' << manager.ship_limits[i] << '\n';
//     }
//     return os;
// }

