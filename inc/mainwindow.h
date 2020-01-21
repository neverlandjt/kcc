#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QMainWindow>
#include<QtCore>
#include<QtGui>
#include<QFileSystemModel>
#include<QDebug>
#include <QApplication>
#include<QTableView>
#ifdef _WIN32
#include <fileapi.h>
#endif

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

    void compressFile();

    void on_view_doubleClicked(const QModelIndex &index);

    void extractArchive();

    void extractArchiveTo();
#ifdef _WIN32
    void setUpBoxes();
#endif
    void linePressed();
    void changeContext();

    void changeDrive(QString drive);


    void dragEnterEvent(QDragEnterEvent *e) override;
    void dropEvent(QDropEvent *e) override;
    void cutFile();

    QStringList listDirectoryRecursive(const QString& dirPath);

    bool overwriteExisted(const QString &path);

    QList<QPair<QString, QString>> getSelectedFiles();


    void find();

    void on_click(const QModelIndex &index);


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
    QAction *compressAct{};
//    QAction *openAct;
    QAction *extractToAct{};
    QMenu *fileMenu{};
    QMenu *helpMenu{};
    QMenu *toolsMenu{};


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
