#ifndef LISTVIEWTAB_H
#define LISTVIEWTAB_H

#include <QTabWidget>
#include <QSplitter>
#include <QGroupBox>
#include <QVBoxLayout>

#include "signallist.h"
#include "linkview.h"

class listviewtab: public QTabWidget
{
public:
    listviewtab(QTabWidget *parent);

    void update();
    void deviceEvent();
    void linkEvent();

private:

    SignalList *sources;
    SignalList *destinations;
    LinkView *links;

};

#endif // LISTVIEWTAB_H
