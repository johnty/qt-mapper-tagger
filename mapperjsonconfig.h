#ifndef MAPPERJSONCONFIG_H
#define MAPPERJSONCONFIG_H

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QIODevice>
#include <QDebug>

class MapperJsonConfig
{
public:
    MapperJsonConfig();
    MapperJsonConfig(const QString filePath, QIODevice::OpenModeFlag mode = QIODevice::ReadWrite);

    const QList<QString>* getSrcs() { return &mySources; }
    const QList<QString>* getDests() { return &myDestinations; }
    const QList<QString>* getConns() { return &myConnectionExprs; }

private:
    bool ParseFile(const QJsonObject& json_doc);
    QList<QString> mySources;
    QList<QString> myDestinations;
    QList<QString> myConnectionExprs;
    //TODO: data to hold other stuff...
};

#endif // MAPPERJSONCONFIG_H
