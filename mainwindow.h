#pragma once

#include <QMainWindow>
#include <QMap>
#include <QStringList>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onCalculateClicked();
    void onGoalChanged(const QString &);
    void onMealCalculateClicked();

    void validateNumericInput(QLineEdit *edit, bool allowDecimal);
    void validateNameInput(QLineEdit *edit);

private:
    Ui::MainWindow *ui;

    double getActivityFactor(int index);
    QMap<QString, int> loadCaloriesFromFile(const QString &filePath);
    int calculateMealCalories(const QStringList &meals, const QMap<QString, int> &table);
};
