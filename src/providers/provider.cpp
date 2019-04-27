#include "provider.h"

Provider::Provider(QObject *parent) : QObject(parent)
{

}

void Provider::load() {
    qDebug() << "Downloading server list";

    while (countries_.count()) {
        Country* vpnCountry = countries_.first();
        vpnCountry->clearServers();
        countries_.remove(vpnCountry->country());
        vpnCountry->deleteLater();
    }

    replyData_.clear();

    QNetworkRequest request((QUrl(apiUrl_)));
    // Provider API is kinda special with the request headers...
    request.setRawHeader("Accept", "*/*");
    request.setRawHeader("Accept-Encoding", "*");
    request.setRawHeader("Accept-Language", "*");

    networkReply_ = networkAccessManager_.get(request);

    connect(networkReply_, &QNetworkReply::finished, this, &Provider::httpFinished);
    connect(networkReply_, &QIODevice::readyRead, this, &Provider::httpReadyRead);
    connect(networkReply_, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(httpError(QNetworkReply::NetworkError)));
}

void Provider::httpReadyRead()
{
    replyData_.push_back(networkReply_->readAll());
}

void Provider::httpError(QNetworkReply::NetworkError networkError)
{
    Q_UNUSED(networkError);
    qDebug() << "Error downloading server list, retrying ...";
    load();
}

QString Provider::getName() const
{
    return name_;
}

bool Provider::loadFinished() const
{
    return loadFinished_;
}

QString Provider::getServerCountry(QString hostName)
{
    QMapIterator<QString, Country*> i(countries_);
    while (i.hasNext()) {
        i.next();
        if (i.value()->hostNames().contains(hostName)) {
            return i.key();
        }
    }

    return "";
}

Country *Provider::country(QString country)
{
    return countries_[country];
}
