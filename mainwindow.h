#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "textviewtab.h"
#include "listviewtab.h"
#include "gitinterface.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButtonLoadProj_clicked();

private:
    //main UI stuff
    Ui::MainWindow *ui;
    textviewtab *mapperTextViewTab;
    listviewtab * mapperListViewTab;

    //UI related members
    QString repoRoot;

    //git interface
    gitinterface *gitInterface;




};

#endif // MAINWINDOW_H
