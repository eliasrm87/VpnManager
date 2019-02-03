#include <QApplication>
#include "ipvanish.h"

IpVanish::IpVanish(QObject *parent) : QObject(parent)
{

}

void IpVanish::load() {
    qDebug() << "Downloading server list";

    while (countries_.count()) {
        IpvContry* vpnCountry = countries_.first();
        vpnCountry->clearServers();
        countries_.remove(vpnCountry->country());
        vpnCountry->deleteLater();
    }

    replyData_.clear();


    QNetworkRequest request(QUrl("http://www.ipvanish.com/api/servers.geojson"));
    // IPVanish API is kinda special with the request headers...
    request.setRawHeader("Accept", "*/*");
    request.setRawHeader("Accept-Encoding", "*");
    request.setRawHeader("Accept-Language", "*");

    networkReply_ = networkAccessManager_.get(request);

    connect(networkReply_, &QNetworkReply::finished, this, &IpVanish::httpFinished);
    connect(networkReply_, &QIODevice::readyRead, this, &IpVanish::httpReadyRead);
    connect(networkReply_, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(httpError(QNetworkReply::NetworkError)));
}

void IpVanish::httpReadyRead()
{
    replyData_.push_back(networkReply_->readAll());
}

void IpVanish::httpError(QNetworkReply::NetworkError networkError)
{
    Q_UNUSED(networkError);
    qDebug() << "Error downloading server list, retrying ...";
    load();
}

void IpVanish::httpFinished()
{
    qDebug() << "Server list download finished ...";

    serversJson_ = QJsonDocument::fromJson(replyData_);

    foreach(QJsonValue val, serversJson_.array()) {
        QJsonObject data = val.toObject();
        QJsonObject propeties = data.value("properties").toObject();
        QString country = propeties.value("country").toString();
        IpvContry* vpnCountry;
        if (!countries_.contains(country)) {
            vpnCountry = new IpvContry(country, this);
            countries_.insert(country, vpnCountry);
            emit newCountry(vpnCountry);
        } else {
            vpnCountry = countries_[country];
        }
        if (propeties.value("online").toBool()) {
            IpvServer* server = new IpvServer(data, this);
            vpnCountry->addServer(propeties.value("hostname").toString(), server);
            emit newServer(vpnCountry, server);
        }
        QApplication::processEvents();
    }

    emit loadFinish();
    loadFinished_ = true;
}

bool IpVanish::loadFinished() const
{
    return loadFinished_;
}

QString IpVanish::getServerCountry(QString hostName)
{
    QMapIterator<QString, IpvContry*> i(countries_);
    while (i.hasNext()) {
        i.next();
        if (i.value()->hostNames().contains(hostName)) {
            return i.key();
        }
    }

    return "";
}

IpvContry *IpVanish::country(QString country)
{
    return countries_[country];
}
