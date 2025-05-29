#include "mifflin_calculator.h"

double MifflinStJeorCalculator::calculate(const User& user) {
    return (user.gender == 'M' || user.gender == 'm')
    ? 10 * user.weight + 6.25 * user.height - 5 * user.age + 5
    : 10 * user.weight + 6.25 * user.height - 5 * user.age - 161;
}
