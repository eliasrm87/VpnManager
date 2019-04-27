#include "abstractserver.h"

AbstractServer::AbstractServer(const QJsonObject &data, QObject *parent) :
    QObject(parent), data_(data)
{
    properties_ = data;
}

QJsonObject AbstractServer::data() const
{
    return data_;
}

void AbstractServer::setData(const QJsonObject &data)
{
    data_ = data;
}
