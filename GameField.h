#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include "ShipManager.h"
#include "InvalidShipPlacementException.h"
#include "OutOfBoundsAttackException.h"
#include "IShipSunkObserver.h"
#include <vector>

class GameField {
public:
    enum CellStatus { Unknown, Empty, ShipCell, Damaged, Killed };
    GameField(int width, int height);
    GameField(const GameField& other);
    GameField(GameField&& other) noexcept;
    GameField& operator=(const GameField& other);
    GameField& operator=(GameField&& other) noexcept;
    GameField() : width(0), height(0), field(), ship_segment_map(), observers() {}
    ~GameField() = default;

    const std::vector<CellStatus>& operator[](int index) const {
        return field[index];
    }

    bool place_ship(Ship& ship, int x, int y, ShipManager& manager);
    void attack(int x, int y, ShipManager& manager );
    void print_field(const ShipManager& manager) const;
    bool check_coords(int x, int y) const;
    void register_observer(IShipSunkObserver* observer);
    void notify_observers();
    
    void serialize(std::ostream& out) const;
    void deserialize(std::istream& in);
    std::pair <int, int> getSize();
    std::vector<std::vector<CellStatus>> getView();
private:
    int width, height;
    std::vector<std::vector<CellStatus>> field;
    std::vector<std::vector<std::pair<int, int>>> ship_segment_map;
    std::vector<IShipSunkObserver*> observers;

    bool check_placement(const Ship& ship, int x, int y) const;
    bool check_contact_with_ship(int x, int y) const;
};

#endif