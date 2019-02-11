#ifndef IPVSERVER_H
#define IPVSERVER_H

#include <providers/abstractserver.h>

class IpvServer : public AbstractServer
{
    Q_OBJECT
public:
    explicit IpvServer(const QJsonObject &data, QObject *parent = Q_NULLPTR);
    QString hostName();
    QString city();
    int capacity();
};

#endif // IPVSERVER_H
