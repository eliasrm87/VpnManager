#include "ipvcountry.h"

IpvContry::IpvContry() :
    QObject(), country_("")
{

}

IpvContry::IpvContry(const QString &country, QObject *parent) :
    QObject(parent), country_(country)
{

}

IpvContry::IpvContry(const IpvContry &other) :
    QObject(), country_(other.country_), servers_(other.servers_)
{
    setParent(other.parent());
}

QString IpvContry::country() const
{
    return country_;
}

QList<QString> IpvContry::hostNames()
{
    return servers_.keys();
}

int IpvContry::serversCount()
{
    return servers_.count();
}

void IpvContry::addServer(const QString &hostName, IpvServer *server)
{
    servers_.insert(hostName, server);
}

void IpvContry::clearServers()
{
    while (servers_.count()) {
        IpvServer* vpnServer = servers_.first();
        servers_.remove(vpnServer->hostName());
        vpnServer->deleteLater();
    }
}

IpvServer *IpvContry::server(QString hostName)
{
    if (!servers_.contains(hostName)) {
        return nullptr;
    }
    return servers_[hostName];
}
