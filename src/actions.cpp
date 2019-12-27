#include "inc/mainwindow.h"
#include "inc/ui_mainwindow.h"


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
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(pasteFile()));


    cutAct = new QAction(("Cut"), this);
    connect(cutAct, SIGNAL(triggered()), this, SLOT(cutFile()));


    extractToAct = new QAction(("Extract To"), this);
    connect(extractToAct, SIGNAL(triggered()), this, SLOT(extractArchiveTo()));


    extractAct = new QAction(("Extract"), this);
    connect(extractAct, SIGNAL(triggered()), this, SLOT(extractArchive()));


    moveAct = new QAction(("Move"), this);
    connect(moveAct, &QAction::triggered, this, &MainWindow::moveFile);

//      openAct = new QAction(("Open"), this);
//      connect(moveAct, &QAction::triggered, this, &MainWindow::openFile);

}
