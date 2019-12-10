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

    ui->lhsView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->lhsView->horizontalHeader()->setStretchLastSection(true);
    ui->rhsView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->rhsView->horizontalHeader()->setStretchLastSection(true);

    ui->lhsView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->rhsView->setSelectionBehavior(QAbstractItemView::SelectRows);

    createActions();
    addMenu();
}




void MainWindow::createActions() {
    newFileAct = new QAction("New &File");
    newFileAct->setShortcuts(QKeySequence::New);
    newFileAct->setStatusTip(tr("Create a new file"));
    connect(newFileAct, &QAction::triggered, this, &MainWindow::newFile);

    newDirAct = new QAction("New &Dir");
    newDirAct->setStatusTip(tr("Create a new dir"));
    connect(newDirAct, &QAction::triggered, this, &MainWindow::newDir);

    exitAct = new QAction(tr("&Exit"), this);
    exitAct->setShortcuts(QKeySequence::Close);
    exitAct->setStatusTip(tr("Exit the KCC"));
    connect(exitAct, &QAction::triggered, this, &MainWindow::exit);

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the KCC's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    deleteAct = new QAction(tr("Delete"), this);
    deleteAct->setShortcuts(QKeySequence::Delete);
    deleteAct->setStatusTip(tr("Delete selected file"));
    connect(deleteAct, SIGNAL(triggered()), this, SLOT(deleteFile()));

    editAct = new QAction(tr("Rename"), this);
    editAct->setStatusTip(tr("Rename selected file"));
    connect(editAct, SIGNAL(triggered()), this, SLOT(editRecord()));

    copyAct = new QAction(("Copy"), this);
//    copyAct->setShortcuts(QKeySequence::Copy);

    connect(copyAct, SIGNAL(triggered()), this, SLOT(copyFile()));

    pasteAct = new QAction(("Paste"), this);
//    pasteAct->setShortcuts(QKeySequence::Paste);

    connect(pasteAct, SIGNAL(triggered()), this, SLOT(pasteFile()));

}


void MainWindow::copyFile() {
   copyInfo = lhsmodel->fileInfo(ui->lhsView->selectionModel()->currentIndex());
   qDebug() << copyInfo.filePath();

}


void MainWindow::pasteFile() {
    if(!QFile::copy(copyInfo.filePath(),lhsmodel->fileInfo(ui->lhsView->selectionModel()->currentIndex()).absolutePath()+"/"+copyInfo.fileName())){

        QFile::copy(copyInfo.filePath(),lhsmodel->fileInfo(ui->lhsView->selectionModel()->currentIndex()).absoluteFilePath()+"/"+copyInfo.fileName());
    }
}


void MainWindow::exit(){
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Exit", "Are you sure? We will miss You.",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
       qApp->closeAllWindows();
}


void MainWindow::addMenu() {
    fileMenu = menuBar()->addMenu(tr("&File"));

    auto submenu = fileMenu->addMenu("Create");
    submenu->addAction(newDirAct);
    submenu->addAction(newFileAct);

    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

}


QString MainWindow::getCurrentPath(){
    return lhsmodel->fileInfo(ui->lhsView->selectionModel()->currentIndex()).absoluteFilePath();
}

void MainWindow::newFile() {
    bool ok;

    QString text = QInputDialog::getText(this, tr("Create new file"),
                                         tr("File name:"), QLineEdit::Normal,
                                         "New File", &ok);
    QFile file(QDir::currentPath() + '/' + text);

    if (ok && !text.isEmpty() && !file.exists())
        file.open(QIODevice::WriteOnly);
    else
        return;
}

void MainWindow::newDir() {
    QDir dir = QDir::current();
    bool ok;
    QString text = QInputDialog::getText(this, tr("Create new directory"),
                                         tr("Dir name:"), QLineEdit::Normal,
                                         "New Folder", &ok);
    if (ok && !text.isEmpty() && !dir.exists(text))
        dir.mkdir(text);
    else
        return;
}

void MainWindow::about() {
    QMessageBox::about(this, tr("About KCc"),
                       tr("The <b>Best</b> project ever - <i>Kai Camber Commander</i>."));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event) {

    QMenu *menu = new QMenu(this);

    menu->addAction(editAct);
    menu->addAction(deleteAct);
    menu->addAction(copyAct);
    menu->addAction(pasteAct);

    menu->exec(event->globalPos());

}

void MainWindow::editRecord() {
    int row = ui->lhsView->selectionModel()->currentIndex().row();
    QDir dir = QDir::current();
    if (row > 0) {
        bool ok;
        QString oldName = lhsmodel->fileName(ui->lhsView->selectionModel()->currentIndex());
        QString text = QInputDialog::getText(this, tr("Rename file"),
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
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete", "Are you sure? This action cannot be undone.",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
          qDebug() << "Yes was clicked";
          if (!lhsmodel->remove(ui->lhsView->selectionModel()->currentIndex())) {
              qDebug() << "not removed";
          }
        } else {
          qDebug() << "Yes was *not* clicked";
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
