#ifndef IPVANISH_H
#define IPVANISH_H

#include <QObject>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "ipvcountry.h"

class IpVanish : public QObject
{
    Q_OBJECT
public:
    explicit IpVanish(QObject *parent = Q_NULLPTR);
    void load();
    IpvContry* country(QString country);
    bool loadFinished() const;
    QString getServerCountry(QString hostName);

signals:
    void newCountry(IpvContry* country);
    void newServer(IpvContry* country, IpvServer* server);
    void loadFinish();

private slots:
    void httpFinished();
    void httpReadyRead();
    void httpError(QNetworkReply::NetworkError networkError);

private:
    bool loadFinished_;
    QNetworkAccessManager networkAccessManager_;
    QNetworkReply *networkReply_;
    QByteArray replyData_;
    QJsonDocument serversJson_;
    QMap<QString, IpvContry*> countries_;

};

#endif // IPVANISH_H
