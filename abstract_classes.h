#pragma once
#include "user.h"

class BMRCalculator {
public:
    virtual double calculate(const User& user) = 0;
    virtual ~BMRCalculator() = default;
};
