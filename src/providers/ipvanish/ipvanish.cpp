#include <QApplication>
#include "ipvanish.h"

IpVanish::IpVanish(QObject *parent) : Provider(parent)
{
    apiUrl_ = "https://www.ipvanish.com/api/servers.geojson";
    name_   = "IpVanish";
}

void IpVanish::httpFinished()
{
    qDebug() << "Server list download finished ...";

    serversJson_ = QJsonDocument::fromJson(replyData_);

    foreach(QJsonValue val, serversJson_.array()) {
        QJsonObject data = val.toObject();
        QJsonObject properties = data.value("properties").toObject();
        QString country = properties.value("country").toString();
        Country* vpnCountry;
        if (!countries_.contains(country)) {
            vpnCountry = new Country(country, this);
            countries_.insert(country, vpnCountry);
            emit newCountry(vpnCountry);
        } else {
            vpnCountry = countries_[country];
        }
        if (properties.value("online").toBool()) {
            IpvServer* server = new IpvServer(properties, this);
            vpnCountry->addServer(properties.value("hostname").toString(), server);
            emit newServer(vpnCountry, server);
        }
        QApplication::processEvents();
    }

    emit loadFinish();
    loadFinished_ = true;
}
