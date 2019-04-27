#ifndef IPVANISH_H
#define IPVANISH_H

#include "ipvserver.h"
#include "../provider.h"

class IpVanish : public Provider
{
    Q_OBJECT
public:
    explicit IpVanish(QObject *parent = Q_NULLPTR);

private slots:
    void httpFinished();

};

#endif // IPVANISH_H
