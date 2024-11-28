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
#include <QDebug>
#include <fstream>
#include <sstream>
#include "Student.h"
#include "HashTableDictionary.h"
#include "Histogram.h"
#include "UniformCriteria.h"
#include "Interval.h"

#include <QDialog>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QLabel>

class AddStudentDialog : public QDialog {
public:
    QLineEdit *firstNameEdit;
    QLineEdit *lastNameEdit;
    QLineEdit *dobEdit; // Формат: DD.MM.YYYY
    QLineEdit *yearEdit;

    explicit AddStudentDialog(QWidget *parent = nullptr) : QDialog(parent) {
        setWindowTitle("Add Student");
        auto *layout = new QVBoxLayout(this);

        auto *formLayout = new QFormLayout();
        firstNameEdit = new QLineEdit(this);
        lastNameEdit = new QLineEdit(this);
        dobEdit = new QLineEdit(this); // Поле ввода для даты
        yearEdit = new QLineEdit(this);

        formLayout->addRow("First Name:", firstNameEdit);
        formLayout->addRow("Last Name:", lastNameEdit);
        formLayout->addRow("Date of Birth (DD.MM.YYYY):", dobEdit);
        formLayout->addRow("Year of Study:", yearEdit);

        layout->addLayout(formLayout);

        auto *buttonLayout = new QHBoxLayout();
        QPushButton *okButton = new QPushButton("OK", this);
        QPushButton *cancelButton = new QPushButton("Cancel", this);

        buttonLayout->addWidget(okButton);
        buttonLayout->addWidget(cancelButton);
        layout->addLayout(buttonLayout);

        connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
        connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    }
};

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
    ShrdPtr<Sequence<Interval<int>>> generateYearIntervals() {
        int minYear = 1, maxYear = 5, numIntervals = 5;
        UniformCriteria<int> criteria;
        return criteria.generateIntervals(minYear, maxYear, numIntervals);
    }

    void addStudent() {
        AddStudentDialog dialog(this);
        if (dialog.exec() == QDialog::Accepted) {
            QString firstName = dialog.firstNameEdit->text();
            QString lastName = dialog.lastNameEdit->text();
            QString dobStr = dialog.dobEdit->text();
            int year = dialog.yearEdit->text().toInt();

            QStringList dobParts = dobStr.split('.');
            if (dobParts.size() != 3) {
                QMessageBox::warning(this, "Error", "Invalid date format. Use DD.MM.YYYY.");
                return;
            }

            int dob[3] = {dobParts[2].toInt(), dobParts[1].toInt(), dobParts[0].toInt()};
            Student student(firstName.toStdString(), lastName.toStdString(), students.GetCount() + 1, dob, year);

            students.Add(student.getID(), student);
            updateTable();
            visualize();
        }
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
        visualize();
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
                students.Add(id, student);
            }
        }

        file.close();
        updateTable();
        visualize();
    }


    void visualize() {
        auto intervals = generateYearIntervals(); // Генерация интервалов
        Histogram<int> histogram(intervals);      // Создание гистограммы

        // Сбор данных о годах обучения
        auto allItems = students.GetAllItems();
        ShrdPtr<Sequence<int>> years(new ArraySequence<int>());
        for (int i = 0; i < allItems->GetLength(); ++i) {
            years = years->Append(allItems->Get(i).second.getYearOfStudy());
        }

        histogram.add(years); // Добавление данных в гистограмму

        // Диагностика данных
        qDebug() << "Intervals and counts before visualization:";
        for (int i = 0; i < intervals->GetLength(); ++i) {
            Interval<int> interval = intervals->Get(i);
            int count = histogram.getData().ContainsKey(interval) ? histogram.getData().Get(interval) : 0;
            qDebug() << QString::fromStdString(interval.toString()) << ": " << count;
        }

        QChart *chart = new QChart();
        QBarSeries *series = new QBarSeries();
        QStringList categories;

        // Построение гистограммы
        for (int i = 0; i < intervals->GetLength(); ++i) {
            Interval<int> interval = intervals->Get(i);
            int count = 0;

            // Убедимся, что данные в гистограмме корректные
            if (histogram.getData().ContainsKey(interval)) {
                count = histogram.getData().Get(interval);
            }

            // Лог данных перед добавлением
            qDebug() << "Adding to QBarSet: Interval =" << QString::fromStdString(interval.toString()) << ", Count =" << count;

            auto *set = new QBarSet(QString::fromStdString(interval.toString()));
            *set << count; // Добавляем значение в набор

            series->append(set); // Добавляем набор в серию
            categories << QString::fromStdString(interval.toString());
        }

        // Проверка пустой серии
        if (series->count() == 0) {
            qDebug() << "Series is empty. No data to display.";
        }

        // Настройка графика
        chart->addSeries(series);
        chart->setTitle("Distribution by Year of Study");
        chart->setAnimationOptions(QChart::SeriesAnimations);

        // Настройка оси Y
        QValueAxis *axisY = new QValueAxis();
        axisY->setTitleText("Number of Students");

        // Устанавливаем диапазон динамически
        int maxValue = 10; // Максимум по умолчанию
        for (int i = 0; i < series->count(); ++i) {
            for (int j = 0; j < series->barSets().at(i)->count(); ++j) {
                maxValue = std::max(maxValue, static_cast<int>(series->barSets().at(i)->at(j)));
            }
        }
        axisY->setRange(0, maxValue);

        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);

        // Настройка оси X
        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->append(categories);
        axisX->setTitleText("Year Intervals");
        // chart->addAxis(axisX, Qt::AlignBottom);
        chart->createDefaultAxes();
        series->attachAxis(axisX);

        chartView->setChart(chart);
        chartView->update();

        qDebug() << "Visualization updated.";
    }


    void updateTable() {
        table->setRowCount(0);
        auto allItems = students.GetAllItems();
        for (int i = 0; i < allItems->GetLength(); ++i) {
            const auto &student = allItems->Get(i).second;
            int row = table->rowCount();
            table->insertRow(row);
            table->setItem(row, 0, new QTableWidgetItem(QString::number(student.getID())));
            table->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(student.getFirstName())));
            table->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(student.getLastName())));
            table->setItem(row, 3, new QTableWidgetItem(QString("%1.%2.%3").arg(student.getDateOfBirth()[2], 2, 10, QChar('0'))
                                                           .arg(student.getDateOfBirth()[1], 2, 10, QChar('0'))
                                                           .arg(student.getDateOfBirth()[0], 4, 10, QChar('0'))));
            table->setItem(row, 4, new QTableWidgetItem(QString::number(student.getYearOfStudy())));
        }
    }
};


