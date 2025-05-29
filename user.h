#pragma once
#include <string>

class User {
public:
    std::string name;
    char gender;
    double weight;
    double height;
    int age;
    int activityIndex;
    double leanMass;

    // Конструктор по умолчанию
    User()
        : gender('M'), weight(0), height(0), age(0), activityIndex(0), leanMass(0) {}

    // Пользовательский конструктор с параметрами
    User(const std::string& name,
         char gender,
         double weight,
         double height,
         int age,
         int activityIndex,
         double leanMass)
        : name(name),
        gender(gender),
        weight(weight),
        height(height),
        age(age),
        activityIndex(activityIndex),
        leanMass(leanMass) {}
};

