#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "textviewtab.h"
#include "listviewtab.h"
#include "gitinterface.h"
#include "mapperjsonconfig.h"

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

    void on_listWidgetRevs_currentRowChanged(int currentRow);

    void on_listWidgetChanges_currentRowChanged(int currentRow);

private:
    //main UI stuff
    Ui::MainWindow *ui;
    textviewtab *mapperTextViewTab;
    listviewtab * mapperListViewTab;

    //UI related members
    QString repoRoot;
    int currTagSelection;
    int currCommitSelection;

    //file i/o
    void loadMappingFile(QString filepath);
    void updateGitInfo();

    //git interface
    gitinterface *gitInterface;

    //json interface for loading mapper config
    MapperJsonConfig *mapperJSON;
};

#endif // MAINWINDOW_H
