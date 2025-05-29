#include "calorie_calculator.h"

CalorieCalculator::CalorieCalculator(BMRCalculator* calc)
    : bmrCalculator(calc) {}

double CalorieCalculator::calculateTotal(const User& user) {
    return bmrCalculator->calculate(user) * ActivityLevel::getFactor(user.activityIndex);
}
