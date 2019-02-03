#ifndef VPNSERVER_H
#define VPNSERVER_H

#include <QObject>
#include <QJsonObject>

class IpvServer : public QObject
{
    Q_OBJECT
public:
    explicit IpvServer(const QJsonObject &data, QObject *parent = Q_NULLPTR);
    QString hostName();
    QString city();
    int capacity();
    QJsonObject data() const;
    void setData(const QJsonObject &data);

private:
    QJsonObject data_;
    QJsonObject properties_;
};

#endif // VPNSERVER_H
