#ifndef FINDER_H
#define FINDER_H

#include <QDialog>
#include<QDir>
#include<QDirIterator>
#include<QMimeType>
#include<QDebug>
#include<QMimeDatabase>
#include<QDesktopServices>
#include<QUrl>
#include<QMenu>
#include<QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class Finder; }
QT_END_NAMESPACE

class Finder : public QDialog
{
    Q_OBJECT

public:
    Finder(QWidget *parent = nullptr);
    ~Finder();

private slots:


    void on_goButton_pressed();

    void on_filesTable_cellActivated(int row, int column);

    void on_filesTable_customContextMenuRequested(const QPoint &pos);

    void on_BrowseButton_pressed();

private:
    Ui::Finder *ui;
    QDir currentDir;
    QStringList findFiles(const QStringList &files, const QString &text);
    void showFiles(const QStringList &paths);
    bool cancel=false;
};
#endif // FINDER_H
