#ifndef OUTOFBOUNDSATTACKEXCEPTION_H
#define OUTOFBOUNDSATTACKEXCEPTION_H

#include <exception>
#include <string>

class OutOfBoundsAttackException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Attempt to attack out of bounds.";
    }
};

#endif