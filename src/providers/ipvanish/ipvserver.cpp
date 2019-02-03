#include "ipvserver.h"

IpvServer::IpvServer(const QJsonObject &data, QObject *parent) :
    QObject(parent), data_(data)
{
    properties_ = data_.value("properties").toObject();
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

QJsonObject IpvServer::data() const
{
    return data_;
}

void IpvServer::setData(const QJsonObject &data)
{
    data_ = data;
}
