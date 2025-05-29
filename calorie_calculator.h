#pragma once
#include "abstract_classes.h"
#include "logic.h"

class CalorieCalculator {
    BMRCalculator* bmrCalculator;

public:
    explicit CalorieCalculator(BMRCalculator* calc);
    double calculateTotal(const User& user);
};
