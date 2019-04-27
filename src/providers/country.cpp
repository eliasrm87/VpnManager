#include "country.h"

Country::Country() :
    QObject(), country_("")
{

}

Country::Country(const QString &country, QObject *parent) :
    QObject(parent), country_(country)
{

}

Country::Country(const Country &other) :
    QObject(), country_(other.country_), servers_(other.servers_)
{
    setParent(other.parent());
}

QString Country::country() const
{
    return country_;
}

QList<QString> Country::hostNames()
{
    return servers_.keys();
}

int Country::serversCount()
{
    return servers_.count();
}

void Country::addServer(const QString &hostName, AbstractServer *server)
{
    servers_.insert(hostName, server);
}

void Country::clearServers()
{
    while (servers_.count()) {
        AbstractServer* vpnServer = servers_.first();
        servers_.remove(vpnServer->hostName());
        vpnServer->deleteLater();
    }
}

AbstractServer *Country::server(QString hostName)
{
    if (!servers_.contains(hostName)) {
        return nullptr;
    }
    return servers_[hostName];
}
