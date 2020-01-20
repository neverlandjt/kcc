#include "inc/mainwindow.h"
#include "inc/ui_mainwindow.h"
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>


void copyRecursive(const QString &src, const QString &dest) {

    QDir source(src);
    QDir destination(dest);
    QString srcName, destName;
    if (!source.exists())
        return;

    if (!destination.exists())
        destination.mkdir(dest);

            foreach (const QString &fileName, source.entryList(QDir::Files)) {
            srcName = src + QDir::separator() + fileName;
            destName = dest + QDir::separator() + fileName;
            QFile::copy(srcName, destName);
        }
            foreach (const QString &fileName, source.entryList(QDir::AllDirs | QDir::NoDotAndDotDot)) {
            srcName = src + QDir::separator() + fileName;
            destName = dest + QDir::separator() + fileName;
            copyRecursive(srcName, destName);
        }
}

void removeItem(const QFileInfo &file) {
    if (file.isDir()) {
        QDir dir(file.absoluteFilePath());
        dir.removeRecursively();
    } else {
        QDir dir(file.absolutePath());
        dir.remove(file.fileName());
    }

}

bool MainWindow::overwriteExisted(const QString &path) {
    if (QFile::exists(path) || QDir(path).exists()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Warning",
                                      "File with this name already exists. Do you want to overwrite it?",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            removeItem(QFileInfo(path));
            return true;
        } else
            return false;

    }
    return true;
}


void MainWindow::pasteTo(int index, const QString &dest) {
    if (!overwriteExisted(dest)) return;

    if (copyInformation[index].isDir())
        copyRecursive(copyInformation[index].filePath(), dest);
    else
        QFile::copy(copyInformation[index].filePath(), dest);

    if (cut) {
        removeItem(copyInformation[index]);
         pastable = move? false : true;
        copyInformation[index] = move ? QFileInfo("") : QFileInfo(dest);
        --cut;
        --move;
    }
}



void MainWindow::cutFile() {

    copyInformation.clear();
    foreach (QModelIndex index , selectedIndexes) {
        copyInformation.append(transform(index));
    }

    cut = selectedIndexes.size();
}


void MainWindow::pasteFile() {
    QString dest = curr_context ? curr_rhs_path : curr_lhs_path;
    for( int i=0; i< copyInformation.size();++i){
    pasteTo(i,dest + QDir::separator() + copyInformation[i].fileName());
   }
}


void MainWindow::moveFile() {
    bool ok;

    copyInformation.clear();
    foreach (QModelIndex index , selectedIndexes) {
        copyInformation.append(transform(index));

    }
    move = selectedIndexes.size();
    cut = selectedIndexes.size();
    QString dest = !curr_context ? curr_rhs_path : curr_lhs_path;
    QString text = QInputDialog::getText(this, tr("Move file(s)/directory"),
                                         tr("Destination:"), QLineEdit::Normal,
                                         dest, &ok);
    if (ok && !text.isEmpty() && QDir(text).exists()){
         for( int i=0; i< copyInformation.size();++i){
        pasteTo(i, text + QDir::separator() + copyInformation[i].fileName());}
    }
    else
        return;
}


void MainWindow::copyFile() {
    pastable = true;
    copyInformation.clear();
    foreach (QModelIndex index , selectedIndexes) {
        copyInformation.append(transform(index));
    }
}
