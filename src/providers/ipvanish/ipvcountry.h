#ifndef VPNCOUNTRY_H
#define VPNCOUNTRY_H

#include <QObject>
#include <QMap>
#include "ipvserver.h"

class IpvContry : public QObject
{
public:
    IpvContry();
    IpvContry(const QString &country, QObject* parent = Q_NULLPTR);
    IpvContry(const IpvContry& other);
    void addServer(const QString &hostName, IpvServer *server);
    void clearServers();
    IpvServer* server(QString hostName);
    QString country() const;
    QList<QString> hostNames();
    int serversCount();

private:
    QString country_;
    QMap<QString, IpvServer*> servers_;

};

#endif // VPNCOUNTRY_H
