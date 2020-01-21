#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QMainWindow>
#include<QtCore>
#include<QtGui>
#include<QFileSystemModel>
#include<QDebug>
#include <QApplication>
#include<QTableView>
#include"finder.h"

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

    void pasteFile();

    void moveFile();

    void pasteTo(int index, const QString &dest);

    void copyFile();

  void on_view_doubleClicked(const QModelIndex &index);


    void extractArchive();

    void extractArchiveTo();
//    void openFile();
//    void on_lhsView_doubleClicked(const QModelIndex &index);

//    void on_rhsView_doubleClicked(const QModelIndex &index);

    void cutFile();

    bool overwriteExisted(const QString &path);

    void find();

    void on_click(const QModelIndex &index);
//    void navigate(QTableView  *view, const QModelIndex &index, QString& curr_path );


protected:
    void closeEvent(QCloseEvent *event) override;


private:
    bool pastable = false;
    int move = 0;

    int cut = 0;
    Ui::MainWindow *ui;
    QFileSystemModel *model;
    QAction *newDirAct{};
    QAction *newFileAct{};
    QAction *aboutAct{};
    QAction *aboutQtAct{};
    QAction *exitAct{};
    QAction *deleteAct{};
    QAction *editAct{};
    QAction *copyAct{};
    QAction *cutAct{};
    QAction *moveAct{};
    QAction *pasteAct{};
    QAction *extractAct{};
    QAction *findAct{};
//    QAction *openAct;
    QAction *extractToAct{};
    QMenu *fileMenu{};
    QMenu *helpMenu{};
    QMenu *findMenu{};


    QList<QFileInfo> copyInformation = {};
    QModelIndexList selectedIndexes = {};
    QString curr_lhs_path;
    QString curr_rhs_path;
    bool curr_context = 0;
    std::function<QFileInfo(const QModelIndex &index)> transform = [&](const QModelIndex &index) {
        return model->fileInfo(index);
    };
    QScopedPointer<Finder> finder;
public:


};

enum context {
    lhs = 0, rhs = 1
};

#endif // MAINWINDOW_H
