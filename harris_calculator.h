#pragma once
#include "abstract_classes.h"

class HarrisBenedictCalculator : public BMRCalculator {
public:
    double calculate(const User& user) override;
};
