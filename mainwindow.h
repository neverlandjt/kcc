#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QMainWindow>
#include<QtCore>
#include<QtGui>
#include<QFileSystemModel>
#include<QDebug>
#include <QApplication>
#include<QTableView>

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

    void exit();

    void customMenuRequested(const QPoint &pos);

    void deleteFile();

    void editRecord();

    void copyFile();

    void rhsPasteFile();
     void lhsPasteFile();

    void on_lhsView_doubleClicked(const QModelIndex &index);

    void on_rhsView_doubleClicked(const QModelIndex &index);

    void cutFile();

    void on_click(const QModelIndex &index);
    void navigate(QTableView  *view, const QModelIndex &index, QString& curr_path );

protected:
    void closeEvent(QCloseEvent *event) override;


private:
    bool cut=false;
    Ui::MainWindow *ui;
    QFileSystemModel *model;
    QAction *newDirAct;
    QAction *newFileAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *exitAct;
    QAction *deleteAct;
    QAction *editAct;
    QAction *copyAct;
    QAction *cutAct;
    QAction *rhsPasteAct;
    QAction *lhsPasteAct;
    QMenu *fileMenu;
    QMenu *helpMenu;


    QFileInfo copyInfo;
    QModelIndex selectedIndex;
    QString curr_lhs_path;
    QString curr_rhs_path;

public:


};
#endif // MAINWINDOW_H
