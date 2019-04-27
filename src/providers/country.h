#ifndef COUNTRY_H
#define COUNTRY_H

#include <QObject>
#include <QMap>
#include "abstractserver.h"

class Country : public QObject
{
    Q_OBJECT
public:
    Country();
    Country(const QString &country, QObject* parent = Q_NULLPTR);
    Country(const Country& other);
    void addServer(const QString &hostName, AbstractServer *server);
    void clearServers();
    AbstractServer* server(QString hostName);
    QString country() const;
    QList<QString> hostNames();
    int serversCount();

protected:
    QString country_;
    QMap<QString, AbstractServer*> servers_;

};

#endif // COUNTRY_H
