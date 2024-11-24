#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QMenuBar>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarCategoryAxis>
#include <QTabWidget>
#include <QPushButton>
#include "HashTableDictionary.h"
#include "Student.h"
#include "ICriteria.h"

class MainWindow : public QMainWindow {
    // Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    private slots:
        void addStudent();
    void removeStudent();
    void searchStudent();
    void showStatistics();
    void loadStudentsFromFile();
    void generateHistogram();
    void onCriteriaChanged(int index);
    void generateIntervals();

private:
    void setupUI();
    void refreshTable();
    void createHistogram(const QMap<int, int>& counts);

    QTableWidget *tableWidget;
    QLineEdit *firstNameInput, *lastNameInput, *idInput, *yearInput;
    QPushButton *addButton, *removeButton, *searchButton, *statsButton, *loadButton, *intervalButton;
    QComboBox *criteriaComboBox;

    QChartView *chartView;
    QBarSeries *barSeries;
    QValueAxis *valueAxis;
    QBarCategoryAxis *categoryAxis;

    HashTableDictionary<int, Student> studentHashTable;
    ShrdPtr<ICriteria<int>> criteria;
};

#endif // GUI_H
