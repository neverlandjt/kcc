#include "inc/finder.h"
#include "ui_finder.h"

Finder::Finder(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Finder)
{
    ui->setupUi(this);
    ui->filesTable->setSelectionBehavior(QAbstractItemView::SelectRows);

       ui->filesTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
       ui->filesTable->verticalHeader()->hide();
       ui->filesTable->setShowGrid(false);

       ui->filesTable->setContextMenuPolicy(Qt::CustomContextMenu);
       ui->filenameBox->setCurrentText("*");
       ui->searchInBox->setCurrentText(currentDir.path());

}

Finder::~Finder()
{
    delete ui;
}



void Finder::on_goButton_pressed()
{

    ui->filesTable->setRowCount(0);

      QString fileName = "*"+ ui->filenameBox->currentText()+"*";
      QString text = ui->containsBox->currentText();
      QString path = QDir::cleanPath(ui->searchInBox->currentText());
      currentDir = QDir(path);
      QStringList filter;
      if (!fileName.isEmpty())
          filter << fileName;
      qDebug()<<filter;
      QDirIterator it(path, filter, QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
      QStringList files;
      while (it.hasNext())
          files << it.next();
      if (!text.isEmpty())
          files = findFiles(files, text);
      files.sort();
      showFiles(files);
qDebug()<<files;
}




QStringList Finder::findFiles(const QStringList &files, const QString &text)
{
    QMimeDatabase mimeDatabase;
      QStringList foundFiles;

      for (int i = 0; i < files.size(); ++i) {

          QCoreApplication::processEvents();
          const QString fileName = files.at(i);
                 const QMimeType mimeType = mimeDatabase.mimeTypeForFile(fileName);
                 if (mimeType.isValid() && !mimeType.inherits(QStringLiteral("text/plain"))) {
                     qWarning() << "Not searching binary file " << QDir::toNativeSeparators(fileName);
                     continue;
                 }
                 QFile file(fileName);
                 if (file.open(QIODevice::ReadOnly)) {
                     QString line;
                     QTextStream in(&file);
                     while (!in.atEnd()) {
                         if (cancel) break;
                         line = in.readLine();
                         if (line.contains(text, Qt::CaseInsensitive)) {
                             foundFiles << files[i];
                             break;
                         }
                     }
                 }
             }
             return foundFiles;
         }
enum { absoluteFileNameRole = Qt::UserRole + 1 };

void Finder::showFiles(const QStringList &paths)
{
    for (const QString &filePath : paths) {
        const QString toolTip = QDir::toNativeSeparators(filePath);
        const QString relativePath = QDir::toNativeSeparators(currentDir.relativeFilePath(filePath));
        const qint64 size = QFileInfo(filePath).size();
        QTableWidgetItem *fileNameItem = new QTableWidgetItem(relativePath);
        fileNameItem->setData(absoluteFileNameRole, QVariant(filePath));
        fileNameItem->setToolTip(toolTip);
        fileNameItem->setFlags(fileNameItem->flags() ^ Qt::ItemIsEditable);
        QTableWidgetItem *sizeItem = new QTableWidgetItem(QLocale().formattedDataSize(size));
        sizeItem->setData(absoluteFileNameRole, QVariant(filePath));
        sizeItem->setToolTip(toolTip);
        sizeItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        sizeItem->setFlags(sizeItem->flags() ^ Qt::ItemIsEditable);

        int row =    ui->filesTable->rowCount();
           ui->filesTable->insertRow(row);
           ui->filesTable->setItem(row, 0, fileNameItem);
          ui-> filesTable->setItem(row, 1, sizeItem);
    }

}

void Finder::on_filesTable_cellActivated(int row, int column)
{
     const QTableWidgetItem *item = ui->filesTable->item(row, 0);
     QDesktopServices::openUrl(QUrl::fromLocalFile(item->data(absoluteFileNameRole).toString()));
}

void Finder::on_filesTable_customContextMenuRequested(const QPoint &pos)
{
    const QTableWidgetItem *item = ui->filesTable->itemAt(pos);
       if (!item)
           return;
       QMenu menu;
       QAction *openAction = menu.addAction("Open");
       QAction *action = menu.exec(ui->filesTable->mapToGlobal(pos));
       if (!action)
           return;
       if (action == openAction)
             QDesktopServices::openUrl(QUrl::fromLocalFile(item->data(absoluteFileNameRole).toString()));

}

void Finder::on_BrowseButton_pressed()
{
    QString directory = QFileDialog::getExistingDirectory(this,
                                 tr("Find Files"), QDir::currentPath());
    if (!directory.isEmpty()) {
            if (ui->searchInBox->findText(directory) == -1)
                ui->searchInBox->addItem(directory);
            ui->searchInBox->setCurrentIndex(ui->searchInBox->findText(directory));}
}
