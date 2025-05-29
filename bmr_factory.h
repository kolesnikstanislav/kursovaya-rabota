#pragma once
#include <string>
#include "mifflin_calculator.h"
#include "harris_calculator.h"
#include "katch_calculator.h"

class BMRCalculatorFactory {
public:
    static BMRCalculator* createCalculator(const std::string& type);
};
