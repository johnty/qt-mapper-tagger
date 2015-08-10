#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->mappingViewTab->clear();
    mapperListViewTab = new listviewtab(ui->mappingViewTab);
    mapperTextViewTab = new textviewtab(ui->mappingViewTab);

    ui->mappingViewTab->addTab(mapperTextViewTab, "text");
    ui->mappingViewTab->addTab(mapperListViewTab, "list");

    gitInterface = NULL;
}

MainWindow::~MainWindow()
{
    delete ui;
    if (gitInterface != NULL)
        delete gitInterface;
}

void MainWindow::on_pushButtonLoadProj_clicked()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly);
    int res = dialog.exec();
    if (res)
    {
        QString mypath = dialog.selectedFiles()[0];
        ui->lineEditRepoDir->setText(mypath);
        repoRoot = ui->lineEditRepoDir->text();
        std::string repo_dir = repoRoot.toStdString();
        qDebug() << "opening existing repo at " << repoRoot;


        gitInterface = new gitinterface(repo_dir);

        if (gitInterface->isGitRepo())
        {
            qDebug() << "... Repo successfully opened!";


//            ui->pushButtonInit->setDisabled(true);

            //load em tags

//            ui->listRevisions->clear();
//            git_strarray tags = {0};
//            int error = git_tag_list(&tags, repo);
//            qDebug() <<"found numTags =" <<tags.count;
//            if (error == 0) {
//                QString text_str;
//                for (int i=0; i<tags.count; i++) {
//                    text_str+=tags.strings[i];
//                    text_str+="\n";
//                    ui->listRevisions->addItem(tags.strings[i]);
//                }
//            }

            //look up commit info

            //lookupCommits();

        }
//        else {
//            ui->pushButtonInit->setEnabled(true);
//        }

    }

}
