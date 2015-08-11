#include "gitinterface.h"

gitinterface::gitinterface(std::string repo_root)
{
    git_libgit2_init();

    sig = NULL;
    repo = NULL;
    head = NULL;

    is_repo = false;

    qDebug() << " internal repo_root = " << repo_root.c_str();

    if (git_repository_open_ext(
                NULL, repo_root.c_str(), GIT_REPOSITORY_OPEN_NO_SEARCH, NULL) == 0)
    {
        if (git_repository_open(&repo,repo_root.c_str()) == 0)
        {
            is_repo = true;
            getCommits();
        }
        else is_repo = false;
    }
}

gitinterface::~gitinterface()
{
    if (sig != NULL)
    {
        git_signature_free(sig);
    }
    if (head != NULL)
    {
        git_commit_free(head);
    }
    if (repo != NULL)
    {
        git_repository_free(repo);
    }

    while (commitList.size() != 0)
    {
        //qDebug() << "freeing commit...";
        git_commit_free(commitList.at(commitList.size()-1));
        commitList.pop_back();
    }

    git_libgit2_shutdown();
}

void gitinterface::initRepo(const char* repo_dir)
{
    if (git_repository_init(&repo, repo_dir,0) == 0)
    {
        is_repo = true;
    }
}

void gitinterface::setComitterSig(char* sig_name, char* sig_addr)
{
    git_signature_now(&sig, sig_name, sig_addr);
}

void gitinterface::getTags()
{
    if (repo != NULL)
    {
        tagNames.clear();
        git_strarray tags = {0};
        int error = git_tag_list(&tags, repo);
        qDebug() <<"found numTags =" <<tags.count;
        if (error == 0) {
            for (int i=0; i<tags.count; i++) {
                tagNames.push_back(tags.strings[i]);
            }
        }
    }
}

void gitinterface::getCommits()
{
    if (repo != NULL)
    {
        git_oid parent_id;
        git_commit* commit;

        //get the head commit ID
        int error  = git_reference_name_to_id(&parent_id, repo, "HEAD");

        //get the actual commit object
        error = git_commit_lookup(&commit, repo, &parent_id);
        qDebug() << "Head commit = " << git_commit_message(commit);
        unsigned int count = git_commit_parentcount(commit);
        qDebug() << "numParents = " << count;

        walkHistory(commit);
        getTags();
    }
}

void gitinterface::walkHistory(git_commit* commit)
{
    git_commit* wcommit;
    const char* cmsg;
    const git_oid* oid = git_commit_id(commit);

    git_oid commit_oid;
    git_oid_cpy(&commit_oid, oid);

    git_revwalk* walker;

    git_revwalk_new(&walker, repo);
    git_revwalk_sorting(walker, GIT_SORT_TOPOLOGICAL | GIT_SORT_TIME);
    git_revwalk_push(walker, &commit_oid);

    commitList.clear();
    commitTags.clear();

    while (git_revwalk_next(&commit_oid, walker) == 0)
    {
        int error = git_commit_lookup(&wcommit, repo, &commit_oid);

        commitList.push_back(wcommit);
        cmsg  = git_commit_message(wcommit);
        qDebug() << "Commit msg = " << cmsg << "cList size = " << commitList.size();

        //think of something more meaningful to use for incremental labels...
        git_time_t time =  git_commit_time(wcommit);
        ostringstream oss;
        oss << time;
        string time_str = oss.str();
        commitTags.push_back(time_str);

        //git_commit_free(wcommit);
    }
    git_revwalk_free(walker);
}

int gitinterface::checkout(git_commit* commit)
{
    git_checkout_options opts = GIT_CHECKOUT_OPTIONS_INIT;
    opts.checkout_strategy = GIT_CHECKOUT_FORCE | GIT_CHECKOUT_DONT_UPDATE_INDEX;

    char *paths[] = { "mapping.json" }; //only check out mapping file
    opts.paths.strings = paths;
    opts.paths.count = 1;

    char shortsha[10] = {0};
    git_oid_tostr(shortsha, 9, git_commit_id(commit));

    qDebug() << "checking out commit id " << shortsha;

    int error = git_checkout_tree(repo, (git_object*)commit, &opts);

    return error;
}

git_commit* gitinterface::getCommit(int idx)
{
    return commitList.at(idx);
}

