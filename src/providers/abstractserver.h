#ifndef ABSTRACTSERVER_H
#define ABSTRACTSERVER_H

#include <QObject>
#include <QJsonObject>

class AbstractServer : public QObject
{
    Q_OBJECT
public:
    explicit AbstractServer(const QJsonObject &data, QObject *parent = Q_NULLPTR);
    virtual QString hostName() = 0;
    virtual QString city() = 0;
    virtual int capacity() = 0;
    QJsonObject data() const;
    void setData(const QJsonObject &data);

protected:
    QJsonObject data_;
    QJsonObject properties_;

};

#endif // ABSTRACTSERVER_H
