#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "user.h"
#include "logic.h"
#include "bmr_factory.h"
#include "calorie_calculator.h"

#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QComboBox>
#include <QPushButton>
#include <QTextEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    ui->activityBox->addItems({
        "Малоподвижный",
        "Легкая активность",
        "Умеренная активность",
        "Активный образ жизни",
        "Очень активный"
    });

    ui->formulaBox->addItems({
        "Mifflin-St Jeor",
        "Harris-Benedict",
        "Katch-McArdle"
    });

    ui->goalBox->clear();
    ui->goalBox->addItems({
        "Похудение",
        "Поддержание",
        "Набор массы"
    });
    ui->goalBox->setCurrentIndex(1);

    connect(ui->goalBox, &QComboBox::currentTextChanged, this, &MainWindow::onGoalChanged);
    connect(ui->calculateButton, &QPushButton::clicked, this, &MainWindow::onCalculateClicked);
    connect(ui->mealCalcButton, &QPushButton::clicked, this, &MainWindow::onMealCalculateClicked);

    connect(ui->weightEdit, &QLineEdit::textChanged, this, [=] {
        validateNumericInput(ui->weightEdit, true);
    });
    connect(ui->heightEdit, &QLineEdit::textChanged, this, [=] {
        validateNumericInput(ui->heightEdit, true);
    });
    connect(ui->ageEdit, &QLineEdit::textChanged, this, [=] {
        validateNumericInput(ui->ageEdit, false);
    });
    connect(ui->leanMassEdit, &QLineEdit::textChanged, this, [=] {
        validateNumericInput(ui->leanMassEdit, true);
    });
    connect(ui->nameEdit, &QLineEdit::textChanged, this, [=] {
        validateNameInput(ui->nameEdit);
    });

    onGoalChanged(ui->goalBox->currentText());
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::validateNumericInput(QLineEdit *edit, bool allowDecimal) {
    QString text = edit->text();
    QRegularExpression regex(allowDecimal ? "^\\d*\\.?\\d*$" : "^\\d+$");

    if (!regex.match(text).hasMatch()) {
        edit->setStyleSheet("border: 1px solid red;");
    } else {
        edit->setStyleSheet("");
    }
}

void MainWindow::validateNameInput(QLineEdit *edit) {
    QString text = edit->text();
    QRegularExpression regex("^[А-Яа-яA-Za-z\\s\\-]+$");

    if (!regex.match(text).hasMatch()) {
        edit->setStyleSheet("border: 1px solid red;");
    } else {
        edit->setStyleSheet("");
    }
}

double MainWindow::getActivityFactor(int index) {
    static const double factors[5] = {1.2, 1.375, 1.55, 1.725, 1.9};
    return (index >= 0 && index < 5) ? factors[index] : 1.0;
}

void MainWindow::onGoalChanged(const QString &goal) {
    if (goal == "Похудение") {
        ui->adjustSpin->setEnabled(true);
        ui->adjustSpin->setValue(-20);
    } else if (goal == "Поддержание") {
        ui->adjustSpin->setValue(0);
        ui->adjustSpin->setEnabled(false);
    } else if (goal == "Набор массы") {
        ui->adjustSpin->setEnabled(true);
        ui->adjustSpin->setValue(10);
    }
}

void MainWindow::onCalculateClicked() {
    QString name = ui->nameEdit->text().trimmed();
    QString genderStr = ui->genderBox->currentText();
    QString formula = ui->formulaBox->currentText();
    QString goal = ui->goalBox->currentText();
    int percent = ui->adjustSpin->value();

    QRegularExpression nameRegex("^[А-Яа-яA-Za-z\\s\\-]+$");
    if (!nameRegex.match(name).hasMatch()) {
        QMessageBox::warning(this, "Ошибка", "Введите корректное имя (только буквы).");
        return;
    }

    bool ok1, ok2, ok3, ok4;
    double weight = ui->weightEdit->text().toDouble(&ok1);
    double height = ui->heightEdit->text().toDouble(&ok2);
    int age = ui->ageEdit->text().toInt(&ok3);
    double leanMass = ui->leanMassEdit->text().toDouble(&ok4);
    int activityIndex = ui->activityBox->currentIndex();

    if (!ok1 || !ok2 || !ok3 || (!ok4 && formula == "Katch-McArdle")) {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, введите корректные числовые данные.");
        return;
    }

    User user{name.toStdString(), genderStr.toStdString()[0], weight, height, age, activityIndex, leanMass};
    if (leanMass <= 0 && formula == "Katch-McArdle")
        user.leanMass = Settings::getInstance()->getDefaultLeanMass();

    std::string type;
    if (formula == "Mifflin-St Jeor") type = "mifflin";
    else if (formula == "Harris-Benedict") type = "harris";
    else if (formula == "Katch-McArdle") type = "katch";

    BMRCalculator* calculator = BMRCalculatorFactory::createCalculator(type);
    if (!calculator) {
        QMessageBox::critical(this, "Ошибка", "Не удалось создать калькулятор.");
        return;
    }

    CalorieCalculator calorieCalculator(calculator);
    double totalCalories = calorieCalculator.calculateTotal(user);

    if (goal == "Похудение") {
        totalCalories -= totalCalories * percent / 100.0;
    } else if (goal == "Набор массы") {
        totalCalories += totalCalories * percent / 100.0;
    }

    delete calculator;

    ui->resultLabel->setText("Суточная норма калорий: " +
                             QString::number(totalCalories, 'f', 1) + " ккал");
}

QMap<QString, int> MainWindow::loadCaloriesFromFile(const QString &filePath) {
    QMap<QString, int> table;
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            QStringList parts = line.split(':');
            if (parts.size() == 2) {
                QString food = parts[0].trimmed().toLower();
                int cal = parts[1].trimmed().toInt();
                table[food] = cal;
            }
        }
        file.close();
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл food_calories.txt");
    }
    return table;
}

int MainWindow::calculateMealCalories(const QStringList &meals, const QMap<QString, int> &table) {
    int total = 0;
    for (const QString &dish : meals) {
        QString clean = dish.trimmed().toLower();
        if (table.contains(clean)) {
            total += table[clean];
        }
    }
    return total;
}

void MainWindow::onMealCalculateClicked() {
    QString path = "food_calories.txt";
    QMap<QString, int> table = loadCaloriesFromFile(path);

    QStringList breakfast = ui->breakfastEdit->toPlainText().split(',', Qt::SkipEmptyParts);
    QStringList lunch = ui->lunchEdit->toPlainText().split(',', Qt::SkipEmptyParts);
    QStringList snack = ui->snackEdit->toPlainText().split(',', Qt::SkipEmptyParts);
    QStringList dinner = ui->dinnerEdit->toPlainText().split(',', Qt::SkipEmptyParts);

    int total = 0;
    total += calculateMealCalories(breakfast, table);
    total += calculateMealCalories(lunch, table);
    total += calculateMealCalories(snack, table);
    total += calculateMealCalories(dinner, table);

    ui->foodResultLabel->setText("Калорийность еды за день: " +
                                 QString::number(total) + " ккал");
}
