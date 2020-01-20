#include "inc/mainwindow.h"
#include "inc/ui_mainwindow.h"
#include "inc/archivehandler.h"
#include <QDesktopServices>
#include <QUrl>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);

    QString sPath = QDir::homePath();
    curr_lhs_path = sPath;
    curr_rhs_path = sPath;

    model = new QFileSystemModel(this);
    model->setRootPath(sPath);
    model->setFilter(QDir::AllEntries | QDir::NoDot);


    ui->lhsView->setModel(model);
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

    ui->lhsView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->rhsView->setSelectionMode(QAbstractItemView::ExtendedSelection);


    connect(ui->lhsView, SIGNAL(clicked(QModelIndex)), this, SLOT(on_click(QModelIndex)));
    connect(ui->rhsView, SIGNAL(clicked(QModelIndex)), this, SLOT(on_click(QModelIndex)));


    connect(ui->lhsView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_view_doubleClicked(QModelIndex)));
    connect(ui->rhsView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_view_doubleClicked(QModelIndex)));



    connect(ui->lhsView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customMenuRequested(QPoint)));
    connect(ui->rhsView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customMenuRequested(QPoint)));


    ui->lhsView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->rhsView->setContextMenuPolicy(Qt::CustomContextMenu);

    createActions();
    addMenu();
}


void MainWindow::exit() {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Exit", "Are you sure? We will miss You.",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
        QApplication::instance()->quit();
}


void MainWindow::closeEvent(QCloseEvent *event) {
    emit exitAct->trigger();
    event->ignore();
}


void MainWindow::addMenu() {
    auto *view = qobject_cast<QTableView *>(sender());

    fileMenu = menuBar()->addMenu(tr("&File"));

    auto submenu = fileMenu->addMenu("Create");
    submenu->addAction(newDirAct);
    submenu->addAction(newFileAct);

    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    if (view == ui->rhsView) curr_context = context::rhs;
    else if (view == ui->lhsView) curr_context = context::lhs;
}


void MainWindow::newFile() {
    bool ok;

    QString text = QInputDialog::getText(this, tr("Create new file"),
                                         tr("File name:"), QLineEdit::Normal,
                                         "New File", &ok);

    QFile file(curr_context ? curr_rhs_path : curr_lhs_path + QDir::separator() + text);

    if (ok && !text.isEmpty() && !file.exists())
        file.open(QIODevice::WriteOnly);
    else return;
}

void MainWindow::newDir() {
    bool ok;

    QDir dir(curr_context ? curr_rhs_path : curr_lhs_path);

    QString text = QInputDialog::getText(this, tr("Create new directory"),
                                         tr("Dir name:"), QLineEdit::Normal,
                                         "New Folder", &ok);

    if (ok && !text.isEmpty() && !dir.exists(text))
        dir.mkdir(text);
    else return;
}


void MainWindow::about() {
    QMessageBox::about(this, tr("About KCc"),
                       tr("The <b>Best</b> project ever - <i>Kai Camber Commander</i>."));
}


MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::customMenuRequested(const QPoint &pos) {
    auto *menu = new QMenu(this);
    auto *view = qobject_cast<QTableView *>(sender());
    selectedIndex = view->indexAt(pos);
    selectedIndexes = view->selectionModel()->selectedRows();
    qDebug() << selectedIndexes.size();
            foreach (QModelIndex index, selectedIndexes) {
            qDebug() << model->fileInfo(index).absoluteFilePath();
        }

    if (selectedIndexes.size() >= 1) {
        menu->addAction(moveAct);
        menu->addSeparator();

        menu->addAction(copyAct);
        menu->addAction(cutAct);
        if (selectedIndexes.size() == 1) {
//            menu->addAction(openAct);
            menu->addAction(editAct);
            menu->addAction(deleteAct);
            menu->addSeparator();

            const QString filename = model->fileInfo(selectedIndexes[0]).fileName();

            if (isArchive(filename)) {
                menu->addSeparator();
                menu->addAction(extractAct);
                menu->addAction(extractToAct);
                menu->addSeparator();
            }
        }
    }
    if (view == ui->rhsView) curr_context = context::rhs;
    else if (view == ui->lhsView) curr_context = context::lhs;

    menu->addAction(pasteAct);
    copyInfo.exists() ? pasteAct->setDisabled(false) : pasteAct->setDisabled(true);

    menu->popup(view->viewport()->mapToGlobal(pos));

}

//void MainWindow::openFile() {
//     QTableView* view = qobject_cast<QTableView *>(sender());
//    emit view->doubleClick()
//}


void MainWindow::extractArchiveTo() {
    bool ok;

    QString arcName = model->fileName(selectedIndex);
    int index = arcName.lastIndexOf('.');

    QString text = QInputDialog::getText(this, tr("Extract To"),
                                         tr("Destination:"), QLineEdit::Normal,
                                         arcName.mid(0, index), &ok);

    QDir dir = model->fileInfo(selectedIndex).dir();

    if (!dir.exists(text)) dir.mkdir(text);

    if (ok && !text.isEmpty()) {
        Archive a(model->filePath(selectedIndex));
        a.extract(model->fileInfo(selectedIndex).path() + '/' + text);
    }

}

void MainWindow::extractArchive() {
    Archive a(model->filePath(selectedIndex));
    a.extract(model->fileInfo(selectedIndex).path() + '/');
}


void MainWindow::editRecord() {
    bool ok;

    QDir curr_dir = model->fileInfo(selectedIndex).dir();
    QString oldName = model->fileName(selectedIndex);

    QString text = QInputDialog::getText(this, tr("Rename file"),
                                         tr("New name:"), QLineEdit::Normal,
                                         oldName, &ok);

    if (ok && !text.isEmpty()) curr_dir.rename(oldName, text);
}

void MainWindow::deleteFile() {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete", "Are you sure? This action cannot be undone.",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        if (!model->remove(selectedIndex))
            QMessageBox::critical(this, tr("Error"),
                                  tr("Cannot delete this file."), QMessageBox::Ok);
    }
}


void MainWindow::on_view_doubleClicked(const QModelIndex &index) {
    QString sPath = model->fileInfo(index).absoluteFilePath();
    auto *view = qobject_cast<QTableView *>(sender());
    selectedIndexes.clear();

    if (model->fileInfo(index).isDir()) {
        if (view == ui->rhsView) curr_rhs_path = sPath;
        else if (view == ui->lhsView) curr_lhs_path = sPath;

        model->index(sPath);
        view->setRootIndex(model->index(sPath));
    } else if (model->fileInfo(index).isFile())
        QDesktopServices::openUrl(QUrl(sPath.prepend("file:///")));
}


void MainWindow::on_click(const QModelIndex &index) {
    selectedIndex = index;

}
