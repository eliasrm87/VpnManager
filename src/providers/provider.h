#ifndef PROVIDER_H
#define PROVIDER_H

#include <QObject>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "abstractserver.h"
#include "country.h"

class Provider : public QObject
{
    Q_OBJECT
public:
    explicit Provider(QObject *parent = Q_NULLPTR);
    void load();
    Country* country(QString country);
    bool loadFinished() const;
    QString getServerCountry(QString hostName);
    QString getName() const;

signals:
    void newCountry(Country* country);
    void newServer(Country* country, AbstractServer* server);
    void loadFinish();

private slots:
    virtual void httpFinished() = 0;
    void httpReadyRead();
    void httpError(QNetworkReply::NetworkError networkError);

protected:
    bool loadFinished_;
    QString apiUrl_;
    QString name_;
    QNetworkAccessManager networkAccessManager_;
    QNetworkReply *networkReply_;
    QByteArray replyData_;
    QJsonDocument serversJson_;
    QMap<QString, Country*> countries_;

};

#endif // PROVIDER_H
