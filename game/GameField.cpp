#include "GameField.h"
#include <iostream>

using namespace std;

GameField::GameField(int width, int height)
    : width(width), height(height), field(width, std::vector<CellStatus>(height, Unknown)), 
      ship_segment_map(width, std::vector<pair<int, int>>(height, {-1, -1})) {}

//copy
GameField::GameField(const GameField& other)
    : width(other.width), height(other.height), field(other.field), ship_segment_map(other.ship_segment_map) {}

//moving
GameField::GameField(GameField&& other) noexcept
    : width(other.width), height(other.height), field(std::move(other.field)), ship_segment_map(std::move(other.ship_segment_map)) {}


GameField& GameField::operator=(const GameField& other) {
    if (this == &other) {
        return *this;
    }
    width = other.width;
    height = other.height;
    field = other.field;
    ship_segment_map = other.ship_segment_map;
    return *this;
}

GameField& GameField::operator=(GameField&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    width = other.width;
    height = other.height;
    field = std::move(other.field);
    ship_segment_map= std::move(other.ship_segment_map);

    return *this;
}

bool GameField::check_coords(int x, int y) const{
    return x>=0 && x<width && y>=0 && y< height;
} 

bool GameField::check_contact_with_ship(int x, int y) const{
    for (int dx = -1; dx <=1; ++dx){
        for( int dy = -1; dy <=1; ++dy){
            int nx = x+dx;
            int ny = y+dy;
            if (check_coords(nx, ny)&& field[nx][ny]==ShipCell){
                return true;
            }
        }
    }
    return false;
}

bool GameField::check_placement(const Ship& ship, int x, int y)const{
    int len = ship.get_len();
    if (ship.get_ori() == Ship::Vertical){
        if (x+len>width) return false;
        for (int i=0; i<len; ++i){
            if (field[x+i][y] != Unknown|| check_contact_with_ship(x+i, y)) return false;
        }
    } else{
        if (y+len > height) return false;
        for (int i = 0; i<len; ++i){
            if (field[x][y+i]!= Unknown || check_contact_with_ship(x, y + i)) return false;
        }
    }
    return true;
}

bool GameField::place_ship(Ship& ship, int x, int y, ShipManager& manager){
    if(!check_coords(x, y) || !check_placement(ship, x, y)){
        throw InvalidShipPlacementException();
    }
    if (!manager.add_ship(ship)){
        cout<< "The limit of ships of this type has been exceeded." << endl;
        return false;
    }

    int len = ship.get_len();
    int ship_index = manager.get_ship_count()-1;
    if (ship.get_ori() == Ship::Vertical){
        for (int i = 0; i<len; ++i){
            field[x+i][y] = ShipCell;
            ship_segment_map[x + i][y] = {ship_index, i};
        }
    }else{
        for (int i=0; i<len; ++i){
            field[x][y+i] = ShipCell;
            ship_segment_map[x][y + i] = {ship_index, i};
        }
    }
    return true;
}

void GameField::attack(int x, int y, ShipManager& manager){
    if(!check_coords(x, y)){
        throw OutOfBoundsAttackException();
    }
    if (field[x][y] == ShipCell||field[x][y] == Damaged||field[x][y] == Killed){
        int ship_index = ship_segment_map[x][y].first;
        int segment_index = ship_segment_map[x][y].second;
        const Ship& ship = manager.get_ship(ship_index);
        const Ship::SegmentCondition condition = ship.get_cond(segment_index);
        if (condition == Ship:: Clear){
            cout<<"Got. A segment of the ship is injured."<<endl;
            manager.take_damage(ship_index, segment_index);
            field[x][y] = Damaged;
        }else if (condition == Ship:: Damaged){
            cout<<"Got. A segment of the ship has been destroyed."<<endl;
            manager.take_damage(ship_index, segment_index);
            field[x][y] = Killed;
        }else if (condition == Ship:: Killed){
            cout<<"You have already killed this ship."<<endl;
            return;
        }
        if (ship.is_sunk()) {
            notify_observers();
            cout << "The ship was sunk." << endl;
        }
    }else{
        field[x][y] = Empty;
        cout<<"Miss."<<endl;
    }
}

void GameField::print_field(const ShipManager& manager) const{
    for (const auto& row : field) {
        for (const auto& cell : row) {
            char symbol = '.';
            if (cell == ShipCell) {
                int ship_index = ship_segment_map[&row - &field[0]][&cell - &row[0]].first;
                int segment_index = ship_segment_map[&row - &field[0]][&cell - &row[0]].second;
                Ship::SegmentCondition condition = manager.get_ship(ship_index).get_cond(segment_index);
                if (condition == Ship::Clear) {
                    symbol = 'S';
                } else if (condition == Ship::Damaged) {
                    symbol = 'o';
                } else if (condition == Ship::Killed) {
                    symbol = 'x';
                }
            }
            if (cell == Empty){
                symbol = '-';
            }
            cout << symbol << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void GameField::register_observer(IShipSunkObserver* observer) {
    observers.push_back(observer);
}

void GameField::notify_observers() {
    for (IShipSunkObserver* observer : observers) {
        observer->on_ship_sunk();
    }
}

void GameField::serialize(std::ostream& out) const {
    out.write(reinterpret_cast<const char*>(&width), sizeof(width));
    out.write(reinterpret_cast<const char*>(&height), sizeof(height));
    for (const auto& row : field) {
        out.write(reinterpret_cast<const char*>(row.data()), row.size() * sizeof(CellStatus));
    }
    for (const auto& row : ship_segment_map) {
        for (const auto& cell : row) {
            out.write(reinterpret_cast<const char*>(&cell), sizeof(cell));
        }
    }
}

void GameField::deserialize(std::istream& in) {
    in.read(reinterpret_cast<char*>(&width), sizeof(width));
    in.read(reinterpret_cast<char*>(&height), sizeof(height));
    field.resize(height, std::vector<CellStatus>(width));
    for (auto& row : field) {
        in.read(reinterpret_cast<char*>(row.data()), row.size() * sizeof(CellStatus));
    }
    ship_segment_map.resize(width, std::vector<std::pair<int, int>>(height, {-1, -1}));
    for (auto& row : ship_segment_map) {
        for (auto& cell : row) {
            in.read(reinterpret_cast<char*>(&cell), sizeof(cell));
        }
    }
}

std::pair <int, int> GameField::getSize() {
	std::pair <int, int> sizes(width, height);
	return sizes;
}


std::vector<std::vector<GameField::CellStatus>> GameField::getView() {
	return this->field;
}