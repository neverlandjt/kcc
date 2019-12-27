#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>


void copyRecursive(QString src, QString dest)
{
    QDir source(src);
    QDir destination(dest);
    QString srcName, destName;
    if(!source.exists())
        return;

    if(!destination.exists())
            destination.mkdir(dest);

    foreach (const QString &fileName, source.entryList(QDir::Files))
    {
         srcName = src + QDir::separator() + fileName;
         destName = dest + QDir::separator() + fileName;
         QFile::copy(srcName, destName);
    }
    foreach (const QString &fileName, source.entryList(QDir::AllDirs | QDir::NoDotAndDotDot))
    {
         srcName = src +QDir::separator() + fileName;
         destName = dest + QDir::separator() + fileName;
        copyRecursive(srcName, destName);
    }
}

void removeItem(const QFileInfo& file){
    if (file.isDir()){
        QDir dir(file.absoluteFilePath());
        dir.removeRecursively();
    }
    else{
        QDir dir(file.absolutePath());
        dir.remove(file.fileName());
    }

}

bool MainWindow::overwriteExisted(const QString &path){
    if (QFile::exists(path) || QDir(path).exists()){
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Warning", "File with this name already exists. Do you want to overwrite it?",
                                  QMessageBox::Yes|QMessageBox::No);
     if (reply == QMessageBox::Yes){
         removeItem(QFileInfo(path));
         return true;
     }
     else
         return false;

}
    return true;
}




void MainWindow::pasteTo( const QString& dest){
    if(!overwriteExisted(dest))
        return;
    if (copyInfo.isDir())
        copyRecursive(copyInfo.filePath(),dest);
    else
        QFile::copy(copyInfo.filePath(),dest);
    if (cut){
        QString tmp = dest;
        removeItem(copyInfo);
        copyInfo = move? QFileInfo(""):QFileInfo(tmp);
        cut = false;
        move=false;
    }

}


void MainWindow::cutFile(){
     copyInfo = model->fileInfo(selectedIndex);
     cut = true;
}


void MainWindow::pasteFile() {
    QString dest = curr_context ? curr_rhs_path : curr_lhs_path;
 pasteTo(dest+"/"+copyInfo.fileName());
}


void MainWindow::moveFile(){
    bool ok;
    copyInfo =  model->fileInfo(selectedIndex);
    move=true;
    cut=true;
    QString dest = !curr_context ? curr_rhs_path : curr_lhs_path;
    QString text = QInputDialog::getText(this, tr("Move file/directory"),
                                         tr("Destination:"), QLineEdit::Normal,
                                         dest, &ok);
    if (ok && !text.isEmpty() && QDir(text).exists())
        pasteTo(text+"/"+copyInfo.fileName());
    else
        return;
}


void MainWindow::copyFile() {
   copyInfo = model->fileInfo(selectedIndex);
}
