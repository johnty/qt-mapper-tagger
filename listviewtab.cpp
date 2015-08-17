#include "listviewtab.h"

listviewtab::listviewtab(QTabWidget *parent)
{
    //    data = _data;
    //    QGridLayout *layout = new QGridLayout(this);
    //    layout->setContentsMargins(0, 0, 0, 0);
    //    QSplitter *splitter = new QSplitter(Qt::Vertical, this);
        QVBoxLayout *splitter = new QVBoxLayout(this);
        splitter->setContentsMargins(0, 0, 0, 0);

        parent->insertTab(0, this, QString("List"));

    //    props = new ConnectionProps;
    //    splitter->insertWidget(0, props);

        QWidget *lists = new QWidget();
        splitter->insertWidget(1, lists);
        splitter->setStretch(0, 1);
        splitter->setStretch(1, 3);
        QHBoxLayout *listLayout = new QHBoxLayout;
        listLayout->setContentsMargins(0, 0, 0, 0);
        listLayout->setSpacing(0);
        lists->setLayout(listLayout);

        sources = new SignalList(this, " Sources", 1);
        listLayout->insertWidget(0, sources);
        links = new LinkView;
        listLayout->insertWidget(1, links);
        destinations = new SignalList(this, " Destinations", 0);
        listLayout->insertWidget(2, destinations);
        listLayout->setStretch(0, 2);
        listLayout->setStretch(1, 1);
        listLayout->setStretch(2, 2);
}


void listviewtab::setMapperJSON(MapperJsonConfig *data)
{
    sources->clear();
    for (int i= 0; i< data->getSrcs()->size(); i++)
    {
        sources->addDevice(i,data->getSrcs()->at(i).sig);
    }
    destinations->clear();
    for (int i=0; i< data->getDests()->size(); i++)
    {
        destinations->addDevice(i,data->getDests()->at(i).sig);
    }
}
