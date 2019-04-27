#ifndef NORDVPN_H
#define NORDVPN_H

#include "nvpnserver.h"
#include "../provider.h"

class NordVPN : public Provider
{
    Q_OBJECT
public:
    explicit NordVPN(QObject *parent = Q_NULLPTR);

private slots:
    void httpFinished();

};

#endif // NORDVPN_H
