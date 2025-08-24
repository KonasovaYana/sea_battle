#include "Ship.h"
#include <stdexcept>

Ship::Ship(int len, Orientation ori) {
    check_parameters(len);
    len_of_ship = len;
    orientation_of_ship = ori;
    segments = std::vector<SegmentCondition>(len_of_ship, Clear);
}

void Ship::check_parameters(int len) {
    if (len < 1 || len > 4) {
        throw std::out_of_range("Uncorrected data. The length of the ship is from 1 to 4");
    }
}

int Ship::get_len() const {
    return len_of_ship;
}

Ship::Orientation Ship::get_ori() const {
    return orientation_of_ship;
}

Ship::SegmentCondition Ship::get_cond(int segment_index)const{
    return segments[segment_index];
}

void Ship::take_damage(int segment_index) {
    if (segments[segment_index] == Clear) {
        segments[segment_index] = Damaged;
    } else if (segments[segment_index] == Damaged){
        segments[segment_index] = Killed;
    }
}

bool Ship::is_sunk() const {
    for (const auto& segment : segments) {
        if (segment != Killed) {
            return false;
        }
    }
    return true;
}
