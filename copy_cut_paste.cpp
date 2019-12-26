#include "mainwindow.h"
#include "ui_mainwindow.h"


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


void MainWindow::rhsPasteFile() {
     qDebug()<< cut;
    if (copyInfo.isDir())
        copyRecursive(copyInfo.filePath(),curr_rhs_path+"/"+copyInfo.fileName());
    else
        QFile::copy(copyInfo.filePath(),curr_rhs_path+"/"+copyInfo.fileName());
    if (cut){
        QString tmp = curr_rhs_path+"/"+copyInfo.fileName();
        if (copyInfo.isDir()){
            QDir dir(copyInfo.absoluteFilePath());
            dir.removeRecursively();
        }
        else{
            QDir dir(copyInfo.absolutePath());
            dir.remove(copyInfo.fileName());
        }
        qDebug()<< tmp;
        cut = false;
    }
}

void MainWindow::cutFile(){
     copyInfo = model->fileInfo(selectedIndex);
     cut = true;
}


void MainWindow::lhsPasteFile() {
    if (copyInfo.isDir())
        copyRecursive(copyInfo.filePath(),curr_lhs_path+"/"+copyInfo.fileName());
    else
        QFile::copy(copyInfo.filePath(),curr_lhs_path+"/"+copyInfo.fileName());
}

void MainWindow::copyFile() {
   copyInfo = model->fileInfo(selectedIndex);
//   qDebug() << copyInfo.filePath();

}
