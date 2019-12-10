#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopServices>
#include <QUrl>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    QString sPath = QDir::currentPath();

    lhsmodel = new QFileSystemModel(this);
    lhsmodel->setRootPath(sPath);
    lhsmodel->setFilter(QDir::AllEntries | QDir::NoDot);
    ui->lhsView->setModel(lhsmodel);
    ui->lhsView->setRootIndex(lhsmodel->index(sPath));
    ui->lhsView->verticalHeader()->setVisible(false); // hide index column

    rhsmodel = new QFileSystemModel(this);
    rhsmodel->setRootPath(sPath);
    rhsmodel->setFilter(QDir::AllEntries | QDir::NoDot);
    ui->rhsView->setModel(rhsmodel);
    ui->rhsView->setRootIndex(rhsmodel->index(sPath));
    ui->rhsView->verticalHeader()->setVisible(false); // hide index column


    createActions();
    addMenu();

}


void MainWindow::createActions() {
    newFileAct = new QAction("New &File");
    newFileAct->setShortcuts(QKeySequence::New);
    newFileAct->setStatusTip(tr("Create a new file"));
    connect(newFileAct, &QAction::triggered, this, &MainWindow::newFile);

    newDirAct = new QAction("New &Dir");
//    newDirAct->setShortcuts(QKeySequence::New);
    newDirAct->setStatusTip(tr("Create a new dir"));
    connect(newDirAct, &QAction::triggered, this, &MainWindow::newDir);


    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the KCC's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    exitAct = new QAction(tr("&Exit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the KCC"));
    connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

}

void MainWindow::addMenu() {
    fileMenu = menuBar()->addMenu(tr("&File"));

    auto submenu = fileMenu->addMenu("Create");
    submenu->addAction(newDirAct);
    submenu->addAction(newFileAct);

    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
    menuBar()->addMenu(tr("&Help"))->addAction(aboutAct);

}

void MainWindow::newFile() {
    bool ok;

    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                         tr("File name:"), QLineEdit::Normal,
                                         "Новый файл", &ok);
    QFile file(QDir::currentPath() + '/' + text);

    if (ok && !text.isEmpty() && !file.exists())
        file.open(QIODevice::WriteOnly); // Or QIODevice::ReadWrite

    else
        return;
}

void MainWindow::newDir() {
    QDir dir = QDir::current();
    bool ok;
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                         tr("Dir name:"), QLineEdit::Normal,
                                         "Новая папка", &ok);
    if (ok && !text.isEmpty() && !dir.exists(text))
        dir.mkdir(text);
    else
        return;
}

void MainWindow::about() {
    QMessageBox::about(this, tr("About Recent Files"),
                       tr("The <b>Best</b> project ever - <i>Kai Camber Commander</i>."));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event) {

    QMenu *menu = new QMenu(this);

    QAction *editDevice = new QAction(("Edit"), this);
    QAction *deleteDevice = new QAction(("Delete"), this);

    connect(editDevice, SIGNAL(triggered()), this, SLOT(editRecord()));
    connect(deleteDevice, SIGNAL(triggered()), this, SLOT(deleteFile()));

    menu->addAction(editDevice);
    menu->addAction(deleteDevice);
    menu->exec(event->globalPos());

}

void MainWindow::editRecord() {
    int row = ui->lhsView->selectionModel()->currentIndex().row();
    QDir dir = QDir::current();
    if (row > 0) {
        bool ok;
        QString oldName = lhsmodel->fileName(ui->lhsView->selectionModel()->currentIndex());
        QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                             tr("New name:"), QLineEdit::Normal,
                                             oldName, &ok);
        if (ok && !text.isEmpty()) {
            dir.rename(oldName, text);

        }
    }
}

void MainWindow::deleteFile() {
    int row = ui->lhsView->selectionModel()->currentIndex().row();
    if (row > 0) {
        if (!lhsmodel->remove(ui->lhsView->selectionModel()->currentIndex())) {
            qDebug() << "not removed";
        }
    }
}


void MainWindow::on_lhsView_doubleClicked(const QModelIndex &index) {
    QString sPath = lhsmodel->fileInfo(index).absoluteFilePath();
    qDebug() << sPath;
    if (lhsmodel->fileInfo(index).isDir()) {
        lhsmodel->index(sPath);
        ui->lhsView->setRootIndex(lhsmodel->index(sPath));
    } else if (lhsmodel->fileInfo(index).isFile()) {
        QDesktopServices desk;
        desk.openUrl(QUrl(sPath.prepend("file:///")));
    }
}


void MainWindow::on_rhsView_doubleClicked(const QModelIndex &index) {
    QString sPath = rhsmodel->fileInfo(index).absoluteFilePath();
    qDebug() << sPath;
    if (rhsmodel->fileInfo(index).isDir()) {
        rhsmodel->index(sPath);
        ui->rhsView->setRootIndex(rhsmodel->index(sPath));
    } else if (rhsmodel->fileInfo(index).isFile()) {
        QDesktopServices desk;
        desk.openUrl(QUrl(sPath.prepend("file:///")));
    }
}
