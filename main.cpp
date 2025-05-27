#include <iostream>
#include <string>
#include <limits>

// Класс для хранения констант
class Constants {
public:
    static constexpr double activityFactors[5] = { 1.2, 1.375, 1.55, 1.725, 1.9 };
};

// Определение статической constexpr переменной
constexpr double Constants::activityFactors[5];

// Класс пользователя
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

// Уровень активности
class ActivityLevel {
public:
    static double getFactor(int index) {
        if (index >= 0 && index < 5)
            return Constants::activityFactors[index];
        return 1.0;
    }
};

// Абстрактный класс BMR-калькулятора (паттерн Strategy)
class BMRCalculator {
public:
    virtual double calculate(const User& user) = 0;
    virtual ~BMRCalculator() {}
};

// Реализация формулы Миффлина-Сан Жеора (конкретная стратегия)
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

// Паттерн Factory Method — для выбора BMR-калькулятора
class BMRCalculatorFactory {
public:
    static BMRCalculator* createCalculator(const std::string& type) {
        if (type == "mifflin") {
            return new MifflinStJeorCalculator();
        }

        // Здесь можно добавить другие формулы
        return nullptr;
    }
};

// Калькулятор калорий
class CalorieCalculator {
    BMRCalculator* bmrCalculator;

public:
    CalorieCalculator(BMRCalculator* calculator) : bmrCalculator(calculator) {}

    double calculateTotal(const User& user) {
        double bmr = bmrCalculator->calculate(user);
        return bmr * ActivityLevel::getFactor(user.activityIndex);
    }
};

// Ввод данных
class InputHandler {
public:
    void fillUser(User& user) {
        std::cout << "Введите имя: ";
        std::getline(std::cin, user.name);

        std::cout << "Введите пол (M/W): ";
        std::cin >> user.gender;

        std::cout << "Введите вес (в кг): ";
        std::cin >> user.weight;

        std::cout << "Введите рост (в см): ";
        std::cin >> user.height;

        std::cout << "Введите возраст: ";
        std::cin >> user.age;

        std::cout << "Выберите уровень активности (0-4):\n";
        std::cout << "0. Малоподвижный\n1. Легкая активность\n2. Умеренная\n3. Активный\n4. Очень активный\n";
        std::cin >> user.activityIndex;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
};

// Вывод результатов
class OutputHandler {
public:
    void printResult(const User& user, double calories) {
        std::cout << "\nПривет, " << user.name << "!\n";
        std::cout << "Ваша суточная норма калорий: " << calories << " ккал\n";
    }
};

// Проверка корректности введённых данных
class Validator {
public:
    static bool validateUser(const User& user) {
        return (user.weight > 0 && user.height > 0 && user.age > 0 &&
            (user.gender == 'M' || user.gender == 'F' ||
                user.gender == 'm' || user.gender == 'f') &&
            user.activityIndex >= 0 && user.activityIndex < 5);
    }
};

// Главный класс приложения
class App {
    User user;
    InputHandler input;
    OutputHandler output;
    Validator validator;

public:
    void run() {
        input.fillUser(user);

        if (!validator.validateUser(user)) {
            std::cout << "Ошибка ввода данных. Попробуйте снова.\n";
            return;
        }

        // Используем фабрику для получения нужной стратегии
        BMRCalculator* bmr = BMRCalculatorFactory::createCalculator("mifflin");

        if (!bmr) {
            std::cout << "Ошибка при создании калькулятора BMR.\n";
            return;
        }

        CalorieCalculator calculator(bmr);
        double totalCalories = calculator.calculateTotal(user);
        output.printResult(user, totalCalories);

        delete bmr;  // освобождение памяти
    }
};

// Точка входа
int main() {
    App app;
    app.run();
    return 0;
}