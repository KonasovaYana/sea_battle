#ifndef ISHIPSUNKOBSERVER_H
#define ISHIPSUNKOBSERVER_H

class IShipSunkObserver {
public:
    virtual void on_ship_sunk() = 0; 
    virtual ~IShipSunkObserver() = default;
};

#endif
