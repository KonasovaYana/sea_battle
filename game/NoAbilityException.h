#ifndef NOABILITYEXCEPTION_H
#define NOABILITYEXCEPTION_H

#include <exception>
#include <string>


class NoAbilityException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Attempt to use an ability when none are available.";
    }
};

#endif