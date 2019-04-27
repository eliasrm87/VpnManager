#include "nvpnserver.h"

NvpnServer::NvpnServer(const QJsonObject &data, QObject *parent) :
    AbstractServer(data, parent)
{

}

QString NvpnServer::hostName()
{
    return properties_.value("domain").toString();
}

QString NvpnServer::city()
{
    return properties_.value("name").toString();
}

int NvpnServer::capacity()
{
    return properties_.value("load").toInt();
}
