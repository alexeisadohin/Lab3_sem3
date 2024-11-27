#include "Tests.h"
#include "GUI.h"
#include <QApplication>
int main(int argc, char *argv[]) {
    // test();
    QApplication app(argc, argv);

    StudentManager manager;
    manager.show();

    return app.exec();

    // return 0;
}