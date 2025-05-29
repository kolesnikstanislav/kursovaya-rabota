#pragma once
#include "abstract_classes.h"

class KatchMcArdleCalculator : public BMRCalculator {
public:
    double calculate(const User& user) override;
};

