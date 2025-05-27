#include <iostream>
#include <string>
#include <limits>

// ����� ��� �������� ��������
class Constants {
public:
    static constexpr double activityFactors[5] = { 1.2, 1.375, 1.55, 1.725, 1.9 };
};

// ����������� ����������� constexpr ����������
constexpr double Constants::activityFactors[5];

// ����� ������������
class User {
public:
    std::string name;
    char gender;
    double weight;
    double height;
    int age;
    int activityIndex;

    User() : gender('M'), weight(0), height(0), age(0), activityIndex(0) {}
};

// ������� ����������
class ActivityLevel {
public:
    static double getFactor(int index) {
        if (index >= 0 && index < 5)
            return Constants::activityFactors[index];
        return 1.0;
    }
};

// ����������� ����� BMR-������������ (������� Strategy)
class BMRCalculator {
public:
    virtual double calculate(const User& user) = 0;
    virtual ~BMRCalculator() {}
};

// ���������� ������� ��������-��� ����� (���������� ���������)
class MifflinStJeorCalculator : public BMRCalculator {
public:
    double calculate(const User& user) override {
        if (user.gender == 'M' || user.gender == 'm') {
            return 10 * user.weight + 6.25 * user.height - 5 * user.age + 5;
        }
        else {
            return 10 * user.weight + 6.25 * user.height - 5 * user.age - 161;
        }
    }
};

// ������� Factory Method � ��� ������ BMR-������������
class BMRCalculatorFactory {
public:
    static BMRCalculator* createCalculator(const std::string& type) {
        if (type == "mifflin") {
            return new MifflinStJeorCalculator();
        }

        // ����� ����� �������� ������ �������
        return nullptr;
    }
};

// ����������� �������
class CalorieCalculator {
    BMRCalculator* bmrCalculator;

public:
    CalorieCalculator(BMRCalculator* calculator) : bmrCalculator(calculator) {}

    double calculateTotal(const User& user) {
        double bmr = bmrCalculator->calculate(user);
        return bmr * ActivityLevel::getFactor(user.activityIndex);
    }
};

// ���� ������
class InputHandler {
public:
    void fillUser(User& user) {
        std::cout << "������� ���: ";
        std::getline(std::cin, user.name);

        std::cout << "������� ��� (M/W): ";
        std::cin >> user.gender;

        std::cout << "������� ��� (� ��): ";
        std::cin >> user.weight;

        std::cout << "������� ���� (� ��): ";
        std::cin >> user.height;

        std::cout << "������� �������: ";
        std::cin >> user.age;

        std::cout << "�������� ������� ���������� (0-4):\n";
        std::cout << "0. �������������\n1. ������ ����������\n2. ���������\n3. ��������\n4. ����� ��������\n";
        std::cin >> user.activityIndex;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
};

// ����� �����������
class OutputHandler {
public:
    void printResult(const User& user, double calories) {
        std::cout << "\n������, " << user.name << "!\n";
        std::cout << "���� �������� ����� �������: " << calories << " ����\n";
    }
};

// �������� ������������ �������� ������
class Validator {
public:
    static bool validateUser(const User& user) {
        return (user.weight > 0 && user.height > 0 && user.age > 0 &&
            (user.gender == 'M' || user.gender == 'F' ||
                user.gender == 'm' || user.gender == 'f') &&
            user.activityIndex >= 0 && user.activityIndex < 5);
    }
};

// ������� ����� ����������
class App {
    User user;
    InputHandler input;
    OutputHandler output;
    Validator validator;

public:
    void run() {
        input.fillUser(user);

        if (!validator.validateUser(user)) {
            std::cout << "������ ����� ������. ���������� �����.\n";
            return;
        }

        // ���������� ������� ��� ��������� ������ ���������
        BMRCalculator* bmr = BMRCalculatorFactory::createCalculator("mifflin");

        if (!bmr) {
            std::cout << "������ ��� �������� ������������ BMR.\n";
            return;
        }

        CalorieCalculator calculator(bmr);
        double totalCalories = calculator.calculateTotal(user);
        output.printResult(user, totalCalories);

        delete bmr;  // ������������ ������
    }
};

// ����� �����
int main() {
    App app;
    app.run();
    return 0;
}