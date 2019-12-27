#include "inc/mainwindow.h"

#include <QApplication>
#include <QVBoxLayout>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowIcon(QIcon("D:/kcc/kcc_logo.png"));
    w.setWindowTitle("Kai Camber Commander");
    w.show();
    return a.exec();
}
