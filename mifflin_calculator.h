#pragma once
#include "abstract_classes.h"

class MifflinStJeorCalculator : public BMRCalculator {
public:
    double calculate(const User& user) override;
};

