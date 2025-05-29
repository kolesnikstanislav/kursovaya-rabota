#pragma once
#include "user.h"

class Validator {
public:
    static bool validateUser(const User& user);
};

class Settings {
private:
    static Settings* instance;
    Settings();

public:
    static Settings* getInstance();
    double getDefaultLeanMass() const;
};

class Constants {
public:
    static constexpr double activityFactors[5] = {1.2, 1.375, 1.55, 1.725, 1.9};
};

class ActivityLevel {
public:
    static double getFactor(int index);
};
