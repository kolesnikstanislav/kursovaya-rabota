#include "katch_calculator.h"

double KatchMcArdleCalculator::calculate(const User& user) {
    return 370 + 21.6 * user.leanMass;
}
