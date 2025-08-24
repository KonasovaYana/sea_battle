#ifndef INVALIDSHIPPLACEMENTEXCEPTION_H
#define INVALIDSHIPPLACEMENTEXCEPTION_H

#include <exception>
#include <string>

class InvalidShipPlacementException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Invalid ship placement.";
    }
};

#endif