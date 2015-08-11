#ifndef GITINTERFACE_H
#define GITINTERFACE_H
#include "git2.h"
#include <sstream>
#include <QDebug> //todo: remove this dependency once done

using namespace std;

class gitinterface
{
public:
    gitinterface(std::string repo_root_dir);
    ~gitinterface();
    void setComitterSig(char* sig_name, char* sig_addr);
    bool isGitRepo() {return is_repo;}
    void initRepo(const char* repo_root_dir);

    void getTags();
    void getCommits();
    void walkHistory(git_commit *commit);

    //TODO: this should be refactored
    int checkout(git_commit* commit);

    git_commit* getCommit(int idx);

    void checkoutTag(string tag);

    //todo: make this safer
    vector<string> * getCommitTags() { return &commitTags; }
    vector<string> * getTagNames() {return &tagNames; }

    string getTagMessage(int index); //probably implement other methods of doing this too...


private:
    //libgit related members
    git_repository  *repo; //repository object
    git_signature   *sig;  //git commiter signature
    git_commit      *head; //head commit

    git_commit* getCommit(string tag_name);

    bool is_repo;
    int numRevs;

    vector<string> tagNames;
    vector<git_commit*> commitList;
    vector<string> commitTags;
    vector<git_tag*> tagList;
};

#endif // GITINTERFACE_H
