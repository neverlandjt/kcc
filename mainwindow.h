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

     void PasteFile();

     void moveFile();

    void on_lhsView_doubleClicked(const QModelIndex &index);

    void on_rhsView_doubleClicked(const QModelIndex &index);

    void cutFile();

    void pasteTo( const QString& dest);

    void on_click(const QModelIndex &index);

    void navigate(QTableView  *view, const QModelIndex &index, QString& curr_path );

protected:
    void closeEvent(QCloseEvent *event) override;
    bool overwrite_existed(const QString &path);


private:
    // flags for cut/move
    bool move=false;
    bool cut=false;


    Ui::MainWindow *ui;
    QFileSystemModel *model;


    QAction *newDirAct;
    QAction *newFileAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *exitAct;

    //context menu actions
    QAction *deleteAct;
    QAction *editAct;
    QAction *copyAct;
    QAction *cutAct;
    QAction *moveAct;
    QAction *PasteAct;


    // create menu
    QMenu *fileMenu;
    QMenu *helpMenu;


    QFileInfo copyInfo;
    QModelIndex selectedIndex;

    QString curr_lhs_path;
    QString curr_rhs_path;
    bool curr_context=0;




};

enum context{lhs=0, rhs=1 };
#endif // MAINWINDOW_H
