#include "inc/mainwindow.h"
#include "inc/ui_mainwindow.h"
#include "inc/archivehandler.h"
#include <QDesktopServices>
#include <QUrl>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>



MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow), finder(new Finder) {

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

    ui->lhsline->setCurrentText(curr_lhs_path);
    ui->rhsline->setCurrentText(curr_rhs_path);

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


    ui->lhsView->setDragDropMode(QAbstractItemView::InternalMove);
    ui->lhsView->setDragEnabled(true);
    ui->lhsView->setAcceptDrops(true);
    ui->lhsView->setDropIndicatorShown(true);

    ui->rhsView->setDragDropMode(QAbstractItemView::InternalMove);
    ui->rhsView->setDragEnabled(true);
    ui->rhsView->setAcceptDrops(true);
    ui->rhsView->setDropIndicatorShown(true);

    ui->lhsView->viewport()->installEventFilter(this);
    ui->rhsView->viewport()->installEventFilter(this);

    ui->rhsView->setAlternatingRowColors(true);
    ui->lhsView->setAlternatingRowColors(true);

    ui->rhsView->horizontalHeader()->setSortIndicator(-1, Qt::AscendingOrder);
    ui->lhsView->horizontalHeader()->setSortIndicator(-1, Qt::AscendingOrder);

    ui->rhsView->setSortingEnabled(true);
    ui->lhsView->setSortingEnabled(true);

    connect(ui->lhsline, SIGNAL(activated(int)), this, SLOT(linePressed()));
    connect(ui->rhsline, SIGNAL(activated(int)), this, SLOT(linePressed()));


#ifdef _WIN32
    setUpBoxes();
#else
    ui->rhsBox->setVisible(false);
    ui->lhsBox->setVisible(false);
#endif
}

#ifdef _WIN32
void MainWindow::setUpBoxes(){
    QStringList drives;

    DWORD dr = GetLogicalDrives();
    int n;
    QString drive;

    for( int i = 0; i < 26; i++ ){
        n = ((dr>>i)&0x00000001);
        if( n == 1 ){
            drive = char(65+i);
            drive += ":\\";
            drives.append(drive);
        }
    }

    ui->rhsBox->addItems(drives);
    ui->lhsBox->addItems(drives);

    QString currRhsDrive{curr_rhs_path[0]};
    ui->rhsBox->setCurrentText(currRhsDrive);

    QString currLhsDrive{curr_lhs_path[0]};
    ui->lhsBox->setCurrentText(currLhsDrive);

    connect(ui->rhsBox, SIGNAL(currentTextChanged(QString)), this, SLOT(changeDrive(QString)));
    connect(ui->lhsBox, SIGNAL(currentTextChanged(QString)), this, SLOT(changeDrive(QString)));
}
#endif

void MainWindow::changeDrive(QString drive){
    if (curr_context == context::rhs){
        ui->rhsView->setRootIndex(model->index(drive));
        curr_rhs_path = drive;
        ui->rhsline->setCurrentText(curr_rhs_path);
    }
    else{
        ui->lhsView->setRootIndex(model->index(drive));
        curr_lhs_path = drive;
        ui->lhsline->setCurrentText(curr_lhs_path);
    }

}


void MainWindow::dragEnterEvent(QDragEnterEvent *e){
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *e)
{
    foreach (const QUrl &url, e->mimeData()->urls()) {
        QString fileName = url.toLocalFile();
    }
    if (curr_context == context::rhs){
            ui->rhsView->setAcceptDrops(true);

    } else{
        ui->lhsView->setAcceptDrops(true);

    }

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

    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(findAct);


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

void MainWindow::find() {
    finder->show();
    finder->activateWindow();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::customMenuRequested(const QPoint &pos) {
    auto *menu = new QMenu(this);
    auto *view = qobject_cast<QTableView *>(sender());
    selectedIndexes = view->selectionModel()->selectedRows();


    if (selectedIndexes.size() >= 1) {
        menu->addAction(moveAct);

        menu->addAction(copyAct);
        menu->addAction(cutAct);
        menu->addSeparator();
        menu->addAction(deleteAct);
        if (selectedIndexes.size() == 1) {
//            menu->addAction(openAct);
            menu->addAction(editAct);

            menu->addSeparator();

            const QString filename = model->fileInfo(selectedIndexes[0]).fileName();

            if (isArchive(filename)) {
                menu->addSeparator();
                menu->addAction(extractAct);
                menu->addAction(extractToAct);
                menu->addSeparator();
            }
        }
        menu->addAction(compressAct);

    }
    if (view == ui->rhsView) curr_context = context::rhs;
    else if (view == ui->lhsView) curr_context = context::lhs;

    menu->addAction(pasteAct);
    pastable ? pasteAct->setDisabled(false) : pasteAct->setDisabled(true);

    menu->popup(view->viewport()->mapToGlobal(pos));

}


QList<QPair<QString, QString>> MainWindow::getSelectedFiles(){
    QList<QPair<QString, QString>> selectedFiles;

    QDir dir(curr_context ? curr_rhs_path:curr_lhs_path);
    QString curr;
    foreach (QModelIndex index, selectedIndexes){
        curr = model->fileInfo(index).absoluteFilePath();

        const QFileInfo outputDir(curr);

        if (outputDir.isDir()){
            QStringList filesInDir = listDirectoryRecursive((curr));
            foreach(QString file, filesInDir){
                selectedFiles.append(QPair<QString, QString>{file, dir.relativeFilePath(file)});
            }
        } else{
            selectedFiles.append(QPair<QString, QString>{dir.absoluteFilePath(curr), dir.relativeFilePath(curr)});
        }
        }
   return selectedFiles;

}



QStringList MainWindow::listDirectoryRecursive(const QString& dirPath){
    QStringList filesInDir;
    QDir parent(dirPath);
    parent.setFilter(QDir::Files | QDir::NoDot | QDir::NoDotDot);

    QDirIterator it(parent,  QDirIterator::Subdirectories);
    while (it.hasNext())
        filesInDir.append(it.next());

    return filesInDir;

}

void MainWindow::compressFile(){
    auto filesToCompress = getSelectedFiles();
    bool ok;
    QString curr = curr_context ? curr_rhs_path : curr_lhs_path;
    QString text = QInputDialog::getText(this, tr("Create archive"),
                                         tr("New archive name:"), QLineEdit::Normal,
                                          curr + curr.mid(curr.lastIndexOf('/'), curr.length())  +  ".zip",
                                         &ok);

    if (ok && !text.isEmpty()) {
        compressFiles(filesToCompress, text, 'o');

    }
}


void MainWindow::extractArchiveTo() {
    bool ok;

    QString arcName = model->fileName(selectedIndexes[0]);
    int index = arcName.lastIndexOf('.');

    QString text = QInputDialog::getText(this, tr("Extract To"),
                                         tr("Destination:"), QLineEdit::Normal,
                                         arcName.mid(0, index), &ok);

    QDir dir = model->fileInfo(selectedIndexes[0]).dir();

    if (!dir.exists(text)) dir.mkdir(text);

    if (ok && !text.isEmpty()) {
        Archive a(model->filePath(selectedIndexes[0]));
        a.extract(model->fileInfo(selectedIndexes[0]).path() + '/' + text);
    }

}

void MainWindow::extractArchive() {
    Archive a(model->filePath(selectedIndexes[0]));
    a.extract(model->fileInfo(selectedIndexes[0]).path() + '/');
}


void MainWindow::editRecord() {
    bool ok;

    QDir curr_dir = model->fileInfo(selectedIndexes[0]).dir();
    QString oldName = model->fileName(selectedIndexes[0]);

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
                foreach (QModelIndex index, selectedIndexes) {
                if (!model->remove(index))
                    QMessageBox::critical(this, tr("Error"),
                                          tr("Cannot delete this file."), QMessageBox::Ok);
            }
    }

}


void MainWindow::on_view_doubleClicked(const QModelIndex &index) {
    QString sPath = model->fileInfo(index).absoluteFilePath();
    auto *view = qobject_cast<QTableView *>(sender());


    if (model->fileInfo(index).isDir()) {
        if (view == ui->rhsView) curr_rhs_path = sPath;
        else if (view == ui->lhsView) curr_lhs_path = sPath;

        model->index(sPath);
        view->setRootIndex(model->index(sPath));
        ui->rhsline->setCurrentText(curr_rhs_path);
        ui->lhsline->setCurrentText(curr_lhs_path);
    } else if (model->fileInfo(index).isFile())
        QDesktopServices::openUrl(QUrl(sPath.prepend("file:///")));
}


void MainWindow::on_click(const QModelIndex &index) {
    auto *view = qobject_cast<QTableView *>(sender());
    if (view == ui->rhsView) curr_context = context::rhs;
   else if (view == ui->lhsView)  curr_context = context::lhs;
    changeContext();
}

void MainWindow::linePressed(){
    auto *line = qobject_cast<QComboBox *>(sender());
    if (line == ui->rhsline) curr_context = context::rhs;
   else if (line == ui->lhsline)  curr_context = context::lhs;
    changeContext();
    QString sPath = line->currentText();
    QDir pathDir(sPath);
    if (pathDir.exists())
    {
    if (line == ui->lhsline){
       curr_lhs_path = sPath;
       model->index(sPath);
       ui->lhsView->setRootIndex(model->index(sPath));
        }
    else if (line == ui->rhsline){
       curr_rhs_path = sPath;
       model->index(sPath);
       ui->rhsView->setRootIndex(model->index(sPath));
        }

    }
     ui->lhsline->setCurrentText(curr_lhs_path);
     ui->rhsline->setCurrentText(curr_rhs_path);

}



void MainWindow::changeContext(){
    if (curr_context == context::rhs) {
        ui->lhsView->setStyleSheet("QHeaderView::section { border-color:white }");
        ui->lhsView->clearSelection();
        ui->rhsView->setStyleSheet("QHeaderView::section { border-color:blue }");

    } else if (curr_context == context::lhs) {
        ui->rhsView->setStyleSheet("QHeaderView::section { border-color:white }");
        ui->rhsView->clearSelection();
        ui->lhsView->setStyleSheet("QHeaderView::section { border-color:blue }");
    }
}
