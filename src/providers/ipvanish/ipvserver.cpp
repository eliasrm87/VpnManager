#include "ipvserver.h"

IpvServer::IpvServer(const QJsonObject &data, QObject *parent) :
    AbstractServer(data, parent)
{

}

QString IpvServer::hostName()
{
    return properties_.value("hostname").toString();
}

QString IpvServer::city()
{
    return properties_.value("city").toString();
}

int IpvServer::capacity()
{
    return properties_.value("capacity").toInt();
}
