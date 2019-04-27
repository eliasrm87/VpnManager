#ifndef NVPNSERVER_H
#define NVPNSERVER_H

#include <providers/abstractserver.h>

class NvpnServer : public AbstractServer
{
    Q_OBJECT
public:
    explicit NvpnServer(const QJsonObject &data, QObject *parent = Q_NULLPTR);
    QString hostName();
    QString city();
    int capacity();
};

#endif // NVPNSERVER_H
