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
    QString sPath = QDir::homePath();
    curr_lhs_path= sPath;
    curr_rhs_path = sPath;

    model = new QFileSystemModel(this);
    model->setRootPath(sPath);
    model->setFilter(QDir::AllEntries | QDir::NoDot);
    ui->lhsView->setModel(model);
//    ui->lhsView->setShowGrid(false);
    ui->lhsView->setRootIndex(model->index(sPath));
    ui->lhsView->verticalHeader()->setVisible(false); // hide index column


    ui->rhsView->setModel(model);
    ui->rhsView->setRootIndex(model->index(sPath));
    ui->rhsView->verticalHeader()->setVisible(false); // hide index column

    ui->lhsView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->lhsView->horizontalHeader()->setStretchLastSection(true);
    ui->rhsView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->rhsView->horizontalHeader()->setStretchLastSection(true);

    ui->lhsView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->rhsView->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(ui->lhsView, SIGNAL(clicked(QModelIndex)), this, SLOT(on_click(QModelIndex)));
    connect(ui->rhsView, SIGNAL(clicked(QModelIndex)), this, SLOT(on_click(QModelIndex)));


    connect(ui->lhsView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customMenuRequested( QPoint)));
    connect(ui->rhsView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customMenuRequested(QPoint)));


    ui->lhsView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->rhsView->setContextMenuPolicy(Qt::CustomContextMenu);

    createActions();
    addMenu();
}


void MainWindow::exit(){

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Exit", "Are you sure? We will miss You.",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
       QApplication::instance()->quit();
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    emit exitAct->trigger();
    event->ignore();
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



void MainWindow::customMenuRequested(const QPoint &pos){
        QMenu *menu = new QMenu(this);
        QTableView* view = qobject_cast<QTableView *>(sender());
        selectedIndex =view->indexAt(pos);

        if( view->indexAt(pos).isValid())
        {
            menu->addAction(editAct);
            menu->addAction(deleteAct);
            menu->addSeparator();
            menu->addAction(moveAct);
            menu->addSeparator();
            menu->addAction(copyAct);
            menu->addAction(cutAct);
        }

        if (view ==  ui->rhsView) curr_context=context::rhs;
        else if (view ==  ui->lhsView) curr_context=context::lhs;

         menu->addAction(PasteAct);
        menu->popup(view->viewport()->mapToGlobal(pos));


    }


void MainWindow::editRecord() {

    QDir curr_dir = model->fileInfo(selectedIndex).dir();
        bool ok;
        QString oldName = model->fileName(selectedIndex);
        QString text = QInputDialog::getText(this, tr("Rename file"),
                                             tr("New name:"), QLineEdit::Normal,
                                             oldName, &ok);
        if (ok && !text.isEmpty()) {
            curr_dir.rename(oldName, text);
        }

}

void MainWindow::deleteFile() {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete", "Are you sure? This action cannot be undone.",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
          qDebug() << "Yes was clicked";
          if (!model->remove(selectedIndex)) {
              qDebug() << "not removed";
          }
        } else {
          qDebug() << "Yes was *not* clicked";
        }

}


void MainWindow::navigate(QTableView  *view, const QModelIndex &index, QString& curr_path ){
        QString sPath = model->fileInfo(index).absoluteFilePath();
        if (model->fileInfo(index).isDir()) {
            curr_path = sPath;
            model->index(sPath);
            view->setRootIndex(model->index(sPath));
        } else if (model->fileInfo(index).isFile()) {
            QDesktopServices desk;
            desk.openUrl(QUrl(sPath.prepend("file:///")));
        }
}

void MainWindow::on_lhsView_doubleClicked(const QModelIndex &index) {
    navigate(ui->lhsView, index,  curr_lhs_path);
}

void MainWindow::on_rhsView_doubleClicked(const QModelIndex &index) {
navigate(ui->rhsView, index,  curr_rhs_path);
}

void MainWindow::on_click(const QModelIndex &index)
{
     selectedIndex = index;
}
