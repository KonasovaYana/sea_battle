#ifndef SHIP_H
#define SHIP_H
#include <stdexcept>
#include <vector>

class Ship {
public:
    enum Orientation { Horizontal, Vertical };
    enum SegmentCondition { Clear, Damaged, Killed };

    Ship(int len, Orientation ori);
    int get_len() const;
    Orientation get_ori() const;
    SegmentCondition get_cond(int segment_index) const;
    void take_damage(int segment_index);
    bool is_sunk() const;
    SegmentCondition & operator[](int index) {
        if (index < 0 || index >= len_of_ship) {
            throw std::out_of_range("Index out of range in Ship");
        }
        return segments[index];
    }

private:
    int len_of_ship;
    Orientation orientation_of_ship;
    std::vector<SegmentCondition> segments;
    void check_parameters(int len);
};

#endif