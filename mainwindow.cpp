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

    ui->plainTextEditTagMsg->setReadOnly(true);

    //ui->listWidgetChanges->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->listWidgetRevs->setSelectionMode(QAbstractItemView::SingleSelection);

    gitInterface = NULL;
    mapperJSON = NULL;

    currCommitSelection = currTagSelection = -1;
}

MainWindow::~MainWindow()
{
    //we put things back, as if nothing ever happened...

    QString dest = repoRoot + "/mapping.json";
    QString backup = repoRoot + "/mapping_bk.json";

    if (QFile::exists(dest))
        QFile::remove(dest);
    QFile::copy(backup, dest);

    delete ui;
    if (gitInterface != NULL)
        delete gitInterface;
    if (mapperJSON != NULL)
        delete mapperJSON;
}

void MainWindow::on_pushButtonLoadProj_clicked()
{
//    QFileDialog dialog;
//    dialog.setFileMode(QFileDialog::Directory);
//    dialog.setOption(QFileDialog::ShowDirsOnly);
//    int res = dialog.exec();

    int res = 1;
    if (res)
    {
        //QString mypath = dialog.selectedFiles()[0];

        //!!! for testing...
        QString mypath = QDir::currentPath()+ "/project_root";
        mypath = "/Users/johnty/Documents/libmapper-related/build-qt-mapper-tagger-Desktop_Qt_5_4_1_clang_64bit-Debug/project_root";
        ui->lineEditRepoDir->setText(mypath);
        repoRoot = ui->lineEditRepoDir->text();
        std::string repo_dir = repoRoot.toStdString();
        qDebug() << "opening existing repo at " << repoRoot;

        gitInterface = new gitinterface(repo_dir);

        if (gitInterface->isGitRepo())
        {
            qDebug() << "... Repo successfully opened!";

            //working files:
            //mapping == from repo
            //mapping_curr == display
            //mapping_bk == restore original on exit

            QString curr_file = repoRoot + "/mapping.json";
            QString disp_file = repoRoot + "/mapping_curr.json";
            QString back_file = repoRoot + "/mapping_bk.json";
            if (QFile::exists(disp_file))
                QFile::remove(disp_file);
            if (QFile::exists(back_file))
                QFile::remove(back_file);
            QFile::copy(curr_file, back_file);
            QFile::copy(curr_file, disp_file);

            loadMappingFile(disp_file);
            updateGitInfo();


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

void MainWindow::loadMappingFile(QString filepath)
{

    QFile myfile(filepath);
    if (myfile.open(QFile::ReadOnly))
    {
        QString data;
        QTextStream s1(&myfile);
        data.append(s1.readAll());

        //put into text view
        mapperTextViewTab->setText(data);

        //put into list view
        mapperJSON = new MapperJsonConfig(filepath, QIODevice::ReadOnly);
        mapperListViewTab->setMapperJSON(mapperJSON);
    }
    myfile.close();
}

void MainWindow::updateGitInfo()
{

    //put commit list into UI
    ui->listWidgetChanges->clear();
    vector<string>* ctags = gitInterface->getCommitTags();
    //note: order is most recent FIRST
    //
    for (int i=ctags->size()-1; i>=0; i--)
    {
        QString tag_str = QString(ctags->at(i).c_str());
        ui->listWidgetChanges->addItem(tag_str);
    }
    currCommitSelection = ui->listWidgetChanges->count()-1;
    ui->listWidgetChanges->setCurrentRow(currCommitSelection);

    //put tag list into UI

    //note: order is most recent LAST
    ui->listWidgetRevs->clear();
    ctags = gitInterface->getTagNames();
    for (int i=0; i<ctags->size();i++)
    {
        QString tag_str = QString(ctags->at(i).c_str());
        ui->listWidgetRevs->addItem(tag_str);
    }
    currTagSelection = ui->listWidgetRevs->count()-1;
    ui->listWidgetRevs->setCurrentRow(currTagSelection);
}

void MainWindow::on_listWidgetRevs_currentRowChanged(int currentRow)
{
    qDebug() << "tag selection row = " << currentRow;
    if (currentRow != currTagSelection)
    {
        qDebug() << "loading new rev...";
        currTagSelection = currentRow;
        gitInterface->checkoutTag(ui->listWidgetRevs->currentItem()->text().toStdString());
        QString curr_file = repoRoot + "/mapping.json";
        loadMappingFile(curr_file);
        QString tag_msg = gitInterface->getTagMessage(currentRow).c_str();
        ui->plainTextEditTagMsg->clear();
        ui->plainTextEditTagMsg->insertPlainText(tag_msg);
    }
}

void MainWindow::on_listWidgetChanges_currentRowChanged(int currentRow)
{
    qDebug() << "commit selection row = " <<currentRow;
    if (currentRow != currCommitSelection)
    {
        qDebug() << "loading new commit...";
        //because they're in reverse order...
        int row = gitInterface->getCommitTags()->size()-1-currentRow;
        gitInterface->checkout(gitInterface->getCommit(row));
        currCommitSelection = currentRow;
        QString curr_file = repoRoot + "/mapping.json";
        loadMappingFile(curr_file);
    }
}
