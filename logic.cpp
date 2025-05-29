#include "logic.h"

Settings* Settings::instance = nullptr;

Settings::Settings() {}

Settings* Settings::getInstance() {
    if (!instance)
        instance = new Settings();
    return instance;
}

double Settings::getDefaultLeanMass() const {
    return 50.0;
}

bool Validator::validateUser(const User& user) {
    return (user.weight > 0 && user.height > 0 && user.age > 0 &&
            (user.gender == 'M' || user.gender == 'F' ||
             user.gender == 'm' || user.gender == 'f') &&
            user.activityIndex >= 0 && user.activityIndex < 5);
}

double ActivityLevel::getFactor(int index) {
    if (index >= 0 && index < 5)
        return Constants::activityFactors[index];
    return 1.0;
}

constexpr double Constants::activityFactors[5];
