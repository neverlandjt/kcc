#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QtCore>
#include<QtGui>
#include<QFileSystemModel>
#include<QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private
    slots:
            void

    createActions();

    void addMenu();

    void newFile();

    void newDir();

    void about();

    void deleteFile();

    void editRecord();

    void contextMenuEvent(QContextMenuEvent *event);

    void on_lhsView_doubleClicked(const QModelIndex &index);

    void on_rhsView_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QFileSystemModel *rhsmodel;
    QFileSystemModel *lhsmodel;
    QAction *newDirAct;
    QAction *newFileAct;
    QAction *aboutAct;
    QAction *exitAct;
    QMenu *fileMenu;


};
#endif // MAINWINDOW_H
