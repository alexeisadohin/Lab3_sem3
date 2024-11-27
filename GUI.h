#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QFileDialog>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QChart>
#include <QValueAxis>
#include <QBarCategoryAxis>
#include <fstream>
#include <sstream>
#include "Student.h"
#include "HashTableDictionary.h"
#include "Histogram.h"
#include "UniformCriteria.h"
#include "Interval.h"

class StudentManager : public QMainWindow {
private:
    QTableWidget *table;
    HashTableDictionary<int, Student> students;
    QChartView *chartView;

public:
    StudentManager(QWidget *parent = nullptr) : QMainWindow(parent) {
        setWindowTitle("Student Manager");
        resize(800, 600);

        auto *mainWidget = new QWidget(this);
        setCentralWidget(mainWidget);

        auto *layout = new QVBoxLayout(mainWidget);

        // Кнопки управления
        auto *buttonLayout = new QHBoxLayout();
        layout->addLayout(buttonLayout);

        QPushButton *addButton = new QPushButton("Add Student");
        QPushButton *deleteButton = new QPushButton("Delete Student");
        QPushButton *loadButton = new QPushButton("Load CSV");
        QPushButton *visualizeButton = new QPushButton("Visualize");

        buttonLayout->addWidget(addButton);
        buttonLayout->addWidget(deleteButton);
        buttonLayout->addWidget(loadButton);
        buttonLayout->addWidget(visualizeButton);

        // Таблица для отображения студентов
        table = new QTableWidget(0, 5, this);
        table->setHorizontalHeaderLabels({"ID", "First Name", "Last Name", "DOB", "Year"});
        layout->addWidget(table);

        // Виджет для гистограммы
        chartView = new QChartView(new QChart(), this);
        chartView->setRenderHint(QPainter::Antialiasing);
        layout->addWidget(chartView);

        // Подключение сигналов и слотов
        connect(addButton, &QPushButton::clicked, this, &StudentManager::addStudent);
        connect(deleteButton, &QPushButton::clicked, this, &StudentManager::deleteStudent);
        connect(loadButton, &QPushButton::clicked, this, &StudentManager::loadCSV);
        connect(visualizeButton, &QPushButton::clicked, this, &StudentManager::visualize);
    }

private slots:
    void addStudent() {
        Student student;
        int dob[3] = {2000, 1, 1};
        student = Student("New", "Student", students.GetCount() + 1, dob, 1);
        students.Add(student.getID(), student);
        updateTable();
    }

    void deleteStudent() {
        auto selectedItems = table->selectedItems();
        if (selectedItems.isEmpty()) {
            QMessageBox::warning(this, "Error", "Please select a student to delete.");
            return;
        }
        int row = selectedItems.first()->row();
        int id = table->item(row, 0)->text().toInt();
        students.Remove(id);
        updateTable();
    }

    void loadCSV() {
        QString fileName = QFileDialog::getOpenFileName(this, "Open Student File", "", "CSV Files (*.csv);;All Files (*)");

        if (fileName.isEmpty()) return;

        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::critical(this, "Error", "Failed to open file.");
            return;
        }

        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList fields = line.split(',');

            if (fields.size() == 5) {
                int id = fields[2].toInt();
                QString firstName = fields[0].trimmed();
                QString lastName = fields[1].trimmed();
                QStringList dobFields = fields[3].split('.');
                int dob[3] = {dobFields[2].toInt(), dobFields[1].toInt(), dobFields[0].toInt()};
                int year = fields[4].toInt();

                Student student(firstName.toStdString(), lastName.toStdString(), id, dob, year);
                try {
                    students.Add(id, student);
                } catch (const std::exception &) {}
            }
        }

        file.close();
        updateTable();
    }

    void visualize() {
        auto intervals = generateYearIntervals();
        Histogram<int> histogram(intervals);

        // Собираем данные из словаря студентов
        auto allItems = students.GetAllItems();
        auto years = ShrdPtr<Sequence<int>>(new ArraySequence<int>());
        for (int i = 0; i < allItems->GetLength(); ++i) {
            years = years->Append(allItems->Get(i).second.getYearOfStudy());
        }
        histogram.add(years);

        // Построение гистограммы
        QChart *chart = new QChart();
        QBarSeries *series = new QBarSeries();

        for (int i = 0; i < intervals->GetLength(); ++i) {
            Interval<int> interval = intervals->Get(i);
            int count = histogram.getData().ContainsKey(interval) ? histogram.getData().Get(interval) : 0;

            auto *set = new QBarSet(QString::fromStdString(interval.toString()));
            *set << count;
            series->append(set);
        }

        chart->addSeries(series);
        chart->setTitle("Distribution by Year of Study");
        chart->createDefaultAxes();

        chartView->setChart(chart);
    }

    ShrdPtr<Sequence<Interval<int>>> generateYearIntervals() {
        int minYear = 1, maxYear = 5, numIntervals = 5;
        UniformCriteria<int> criteria;
        return criteria.generateIntervals(minYear, maxYear, numIntervals);
    }

    void updateTable() {
        table->setRowCount(0);
        auto allItems = students.GetAllItems();
        for (int i = 0; i < allItems->GetLength(); ++i) {
            auto student = allItems->Get(i).second;
            int row = table->rowCount();
            table->insertRow(row);

            table->setItem(row, 0, new QTableWidgetItem(QString::number(student.getID())));
            table->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(student.getFirstName())));
            table->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(student.getLastName())));

            QString dob = QString("%1.%2.%3").arg(student.getDateOfBirth()[2], 2, 10, QChar('0'))
                                             .arg(student.getDateOfBirth()[1], 2, 10, QChar('0'))
                                             .arg(student.getDateOfBirth()[0], 4, 10, QChar('0'));
            table->setItem(row, 3, new QTableWidgetItem(dob));

            table->setItem(row, 4, new QTableWidgetItem(QString::number(student.getYearOfStudy())));
        }
    }
};
