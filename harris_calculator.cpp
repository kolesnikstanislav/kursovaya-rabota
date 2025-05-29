#include "harris_calculator.h"

double HarrisBenedictCalculator::calculate(const User& user) {
    return (user.gender == 'M' || user.gender == 'm')
    ? 66.47 + 13.75 * user.weight + 5.003 * user.height - 6.755 * user.age
    : 655.1 + 9.563 * user.weight + 1.850 * user.height - 4.676 * user.age;
}
