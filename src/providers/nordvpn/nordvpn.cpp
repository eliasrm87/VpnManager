#include <QApplication>
#include "nordvpn.h"

NordVPN::NordVPN(QObject *parent) : Provider(parent)
{
    apiUrl_ = "https://api.nordvpn.com/server";
    name_   = "NordVPN";
}

void NordVPN::httpFinished()
{
    qDebug() << "Server list download finished ...";

    serversJson_ = QJsonDocument::fromJson(replyData_);

    foreach(QJsonValue val, serversJson_.array()) {
        QJsonObject data = val.toObject();
        QString country = data.value("country").toString();
        Country* vpnCountry;
        if (!countries_.contains(country)) {
            vpnCountry = new Country(country, this);
            countries_.insert(country, vpnCountry);
            emit newCountry(vpnCountry);
        } else {
            vpnCountry = countries_[country];
        }
        NvpnServer* server = new NvpnServer(data, this);
        vpnCountry->addServer(data.value("domain").toString(), server);
        emit newServer(vpnCountry, server);
        QApplication::processEvents();
    }

    emit loadFinish();
    loadFinished_ = true;
}
