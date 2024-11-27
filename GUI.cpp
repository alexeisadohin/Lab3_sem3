// #include "GUI.h"
// #include <QVBoxLayout>
// #include <QHBoxLayout>
// #include <QMessageBox>
// #include <QFileDialog>
// #include <QFile>
// #include <QTextStream>
// #include <QStringList>
// #include <QMap>
//
// #include <algorithm> // для std::min и std::max
// #include "UniformCriteria.h"
//
//
// MainWindow::MainWindow(QWidget *parent)
//     : QMainWindow(parent), studentHashTable(25), numIntervals(5), yearRange{std::numeric_limits<int>::max(), std::numeric_limits<int>::min()}, histogram(nullptr)  {
//     setupUI();
//     criteria = ShrdPtr<ICriteria<int>>(new UniformCriteria<int>());
//     // Устанавливаем специальное значение для placeholder
//     yearInput->setValue(0); // Или -1, в зависимости от ваших данных
// }
//
// MainWindow::~MainWindow() {
//     delete histogram; // Не забываем удалить гистограмму
// }
//
// void MainWindow::setupUI() {
//     QWidget *centralWidget = new QWidget(this);
//     setCentralWidget(centralWidget);
//
//     QVBoxLayout *mainLayout = new QVBoxLayout;
//
//     QTabWidget *tabWidget = new QTabWidget(this);
//
//     QWidget *studentsTab = new QWidget(this);
//     QWidget *histogramTab = new QWidget(this);
//
//     tabWidget->addTab(studentsTab, "Students");
//     tabWidget->addTab(histogramTab, "Histogram");
//
//     QVBoxLayout *studentsLayout = new QVBoxLayout;
//
//     tableWidget = new QTableWidget(this);
//     tableWidget->setColumnCount(4);
//     tableWidget->setHorizontalHeaderLabels({"First Name", "Last Name", "ID", "Year"});
//     tableWidget->setSelectionBehavior(QTableWidget::SelectRows);
//     tableWidget->setSelectionMode(QTableWidget::SingleSelection);
//     studentsLayout->addWidget(tableWidget);
//
//     QHBoxLayout *formLayout = new QHBoxLayout;
//
//     firstNameInput = new QLineEdit(this);
//     firstNameInput->setPlaceholderText("First Name");
//     formLayout->addWidget(firstNameInput);
//
//     lastNameInput = new QLineEdit(this);
//     lastNameInput->setPlaceholderText("Last Name");
//     formLayout->addWidget(lastNameInput);
//
//     idInput = new QLineEdit(this);
//     idInput->setPlaceholderText("ID");
//     formLayout->addWidget(idInput);
//
//     yearInput = new QSpinBox(this);
//     yearInput->setRange(1900, 2100);
//     // Подключаем сигнал valueChanged
//     connect(yearInput, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onYearValueChanged);
//
//     studentsLayout->addLayout(formLayout);
//
//     QHBoxLayout *buttonLayout = new QHBoxLayout;
//
//     addButton = new QPushButton("Add Student", this);
//     buttonLayout->addWidget(addButton);
//     connect(addButton, &QPushButton::clicked, this, &MainWindow::addStudent);
//
//     removeButton = new QPushButton("Remove Student", this);
//     buttonLayout->addWidget(removeButton);
//     connect(removeButton, &QPushButton::clicked, this, &MainWindow::removeStudent);
//
//     searchButton = new QPushButton("Search Student", this);
//     buttonLayout->addWidget(searchButton);
//     connect(searchButton, &QPushButton::clicked, this, &MainWindow::searchStudent);
//
//     loadButton = new QPushButton("Load Students", this);
//     buttonLayout->addWidget(loadButton);
//     connect(loadButton, &QPushButton::clicked, this, &MainWindow::loadStudentsFromFile);
//
//     studentsLayout->addLayout(buttonLayout);
//
//     studentsTab->setLayout(studentsLayout);
//
//
//     QVBoxLayout *histogramLayout = new QVBoxLayout;
//
//     criteriaComboBox = new QComboBox(this);
//     criteriaComboBox->addItem("By Year");
//     histogramLayout->addWidget(criteriaComboBox);
//     connect(criteriaComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onCriteriaChanged);
//
//     QHBoxLayout *intervalsLayout = new QHBoxLayout;
//     QLabel *intervalsLabel = new QLabel("Number of intervals:", this);
//     intervalsLayout->addWidget(intervalsLabel);
//     numIntervalsSpinBox = new QSpinBox(this);
//     numIntervalsSpinBox->setMinimum(1);
//     numIntervalsSpinBox->setValue(numIntervals);
//     intervalsLayout->addWidget(numIntervalsSpinBox);
//     histogramLayout->addLayout(intervalsLayout);
//     connect(numIntervalsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::generateIntervals);
//
//
//     chartView = new QChartView(new QChart(), this);
//     histogramLayout->addWidget(chartView);
//
//     generateHistogramButton = new QPushButton("Generate Histogram", this);
//     histogramLayout->addWidget(generateHistogramButton);
//     connect(generateHistogramButton, &QPushButton::clicked, this, &MainWindow::generateHistogram);
//
//     histogramTab->setLayout(histogramLayout);
//
//     mainLayout->addWidget(tabWidget);
//     centralWidget->setLayout(mainLayout);
//
//     setWindowTitle("Student Management System");
//     resize(1000, 700);
// }
//
// void MainWindow::addStudent() {
//     QString firstName = firstNameInput->text();
//     QString lastName = lastNameInput->text();
//     int id = idInput->text().toInt();
//     int year = yearInput->text().toInt();
//
//     if (firstName.isEmpty() || lastName.isEmpty() || idInput->text().isEmpty() || yearInput->text().isEmpty()) {
//         QMessageBox::warning(this, "Input Error", "All fields must be filled.");
//         return;
//     }
//
//     int dob[3] = {1, 1, 2000};
//     Student student(firstName.toStdString(), lastName.toStdString(), id, dob, year);
//     try {
//         studentHashTable.Add(id, student);
//         refreshTable();
//     } catch (const std::exception &e) {
//         QMessageBox::critical(this, "Error", e.what());
//     }
// }
//
// void MainWindow::removeStudent() {
//     int id = idInput->text().toInt();
//
//     if (idInput->text().isEmpty()) {
//         QMessageBox::warning(this, "Input Error", "Please enter an ID to remove.");
//         return;
//     }
//
//     try {
//         studentHashTable.Remove(id);
//         refreshTable();
//     } catch (const std::exception &e) {
//         QMessageBox::critical(this, "Error", e.what());
//     }
// }
//
// void MainWindow::searchStudent() {
//     int id = idInput->text().toInt();
//
//     if (idInput->text().isEmpty()) {
//         QMessageBox::warning(this, "Input Error", "Please enter an ID to search.");
//         return;
//     }
//
//     try {
//         Student student = studentHashTable.Get(id);
//         QMessageBox::information(this, "Student Found",
//                                  QString("Name: %1 %2\nYear: %3")
//                                  .arg(QString::fromStdString(student.getFirstName()))
//                                  .arg(QString::fromStdString(student.getLastName()))
//                                  .arg(student.getYearOfStudy()));
//     } catch (const std::exception &e) {
//         QMessageBox::critical(this, "Error", e.what());
//     }
// }
//
// void MainWindow::loadStudentsFromFile() {
//     QString fileName = QFileDialog::getOpenFileName(this, "Open Student File", "", "CSV Files (*.csv);;All Files (*)");
//
//     if (fileName.isEmpty()) return;
//
//     QFile file(fileName);
//     if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//         QMessageBox::critical(this, "Error", "Failed to open file.");
//         return;
//     }
//
//     QTextStream in(&file);
//     while (!in.atEnd()) {
//         QString line = in.readLine();
//         QStringList fields = line.split(',');
//
//         if (fields.size() == 4) {
//             QString firstName = fields[0].trimmed();
//             QString lastName = fields[1].trimmed();
//             int id = fields[2].toInt();
//             int year = fields[3].toInt();
//
//             int dob[3] = {1, 1, 2000};
//             Student student(firstName.toStdString(), lastName.toStdString(), id, dob, year);
//             try {
//                 studentHashTable.Add(id, student);
//             } catch (const std::exception &) {}
//         }
//     }
//
//     file.close();
//     refreshTable();
// }
//
// void MainWindow::refreshTable() {
//     auto items = studentHashTable.GetAllItems();
//
//     tableWidget->setRowCount(items->GetLength());
//
//     for (int i = 0; i < items->GetLength(); ++i) {
//         const auto& [key, student] = items->Get(i);
//         tableWidget->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(student.getFirstName())));
//         tableWidget->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(student.getLastName())));
//         tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(student.getID())));
//         tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(student.getYearOfStudy())));
//     }
// }
//
// void MainWindow::generateHistogram() {
//     QMap<int, int> yearCounts;
//
//     auto items = studentHashTable.GetAllItems();
//
//     for (int i = 0; i < items->GetLength(); ++i) {
//         const auto &[key, student] = items->Get(i);
//         int year = student.getYearOfStudy();
//         yearCounts[year]++;
//     }
//
//     createHistogram(yearCounts);
// }
//
//
//
// void MainWindow::createHistogram(const QMap<int, int>& counts) {
//     QChart *chart = new QChart();
//     QBarSeries *series = new QBarSeries();
//
//     QBarSet *set = new QBarSet("Students");
//     QStringList categories;
//
//     for (auto it = counts.begin(); it != counts.end(); ++it) {
//         *set << it.value();
//         categories << QString::number(it.key());
//     }
//
//     series->append(set);
//     chart->addSeries(series);
//
//     QBarCategoryAxis *axisX = new QBarCategoryAxis();
//     axisX->append(categories);
//     chart->addAxis(axisX, Qt::AlignBottom);
//     series->attachAxis(axisX);
//
//     QValueAxis *axisY = new QValueAxis();
//     chart->addAxis(axisY, Qt::AlignLeft);
//     series->attachAxis(axisY);
//
//     chart->setTitle("Year of Study Distribution");
//     chart->setAnimationOptions(QChart::SeriesAnimations);
//
//     chartView->setChart(chart);
// }