#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#undef signals
#include <libnm/NetworkManager.h>
#define signals Q_SIGNALS

#define NMQ_VPN_AUTH            "auth"
#define NMQ_VPN_CA              "ca"
#define NMQ_VPN_CIPHER          "cipher"
#define NMQ_VPN_COMP_LZO        "comp-lzo"
#define NMQ_VPN_CONNECTION_TYPE "connection-type"
#define NMQ_VPN_FLOAT           "float"
#define NMQ_VPN_MSSFIX          "mssfix"
#define NMQ_VPN_PASSWORD_FLAGS  "password-flags"
#define NMQ_VPN_PORT            "port"
#define NMQ_VPN_PROTO_TCP       "proto-tcp"
#define NMQ_VPN_REMOTE          "remote"
#define NMQ_VPN_REMOTE_RANDOM   "remote-random"
#define NMQ_VPN_TUN_IPV6        "tun-ipv6"
#define NMQ_VPN_USERNAME        "username"
#define NMQ_VPN_PASSWORD        "password"

class NetworkManager : public QObject
{
public:
    NetworkManager(QObject *parent = Q_NULLPTR);
    void connectVpn(QString connectionId, const QMap<QString, QString> &params);
    QString getVpnParam(QString connectionId, QString param);
    bool isActiveConnection(QString connectionId);

private:
    NMActiveConnection *getDefaultActiveConnection(NMDevice **device);
    NMActiveConnection *getActiveConnectionById(QString id);
    NMRemoteConnection *getConnectionById(QString id);

private:
    NMClient *nmClient_;

};

#endif // NETWORKMANAGER_H
