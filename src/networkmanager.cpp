#include <QDebug>
#include <gmodule.h>
#include "networkmanager.h"

static void added_cb(GObject *client, GAsyncResult *result, gpointer user_data)
{
    Q_UNUSED(user_data);
    GError *error = NULL;

    NMActiveConnection *remote = nm_client_add_and_activate_connection_finish(NM_CLIENT(client), result, &error);

    if (error) {
        g_print("Error adding connection: %s\n", error->message);
        g_error_free(error);
    } else {

        g_print("Added connection: %s\n", nm_active_connection_get_id(remote));
        g_object_unref(remote);
    }
}

NetworkManager::NetworkManager(QObject *parent) : QObject(parent)
{
    nmClient_ = nm_client_new(NULL, NULL);
    if (nmClient_)
        g_print("NetworkManager version: %s\n", nm_client_get_version(nmClient_));
}

void NetworkManager::connectVpn(QString connectionId, const QMap<QString, QString> &params)
{
    GError *error = NULL;

    // Deactivate connection if active
    NMActiveConnection *activeConnection = getActiveConnectionById(connectionId);
    if (activeConnection) {
        g_print("Deactivating connection...\n");
        nm_client_deactivate_connection(nmClient_, activeConnection, NULL, &error);
    }

    // Delete connection if exists
    NMRemoteConnection *remoteConnection = getConnectionById(connectionId);
    if (remoteConnection) {
        g_print("Deleting connection...\n");
        nm_remote_connection_delete(remoteConnection, NULL, &error);
    }

    // Create new connection
    g_print("Creating new connection...\n");
    NMConnection *connection = nm_simple_connection_new();

    NMSettingConnection * s_con =(NMSettingConnection *) nm_setting_connection_new();
    char* uuid = nm_utils_uuid_generate();
    g_object_set(G_OBJECT(s_con),
                  NM_SETTING_CONNECTION_UUID, uuid,
                  NM_SETTING_CONNECTION_ID,   connectionId.toStdString().c_str(),
                  NM_SETTING_CONNECTION_TYPE, "vpn",
                  NULL);
    g_free(uuid);
    nm_connection_add_setting(connection, NM_SETTING(s_con));

    NMSettingVpn* s_vpn =(NMSettingVpn *) nm_setting_vpn_new();
    nm_connection_add_setting(connection, NM_SETTING(s_vpn));

    g_object_set(s_vpn,
                  NM_SETTING_VPN_SERVICE_TYPE, "org.freedesktop.NetworkManager.openvpn",
                  NULL);

    QMapIterator<QString, QString> i(params);
    while (i.hasNext()) {
        i.next();
        if (i.key() == NMQ_VPN_PASSWORD) {
            nm_setting_vpn_add_secret(s_vpn,
                                      NMQ_VPN_PASSWORD,
                                      i.value().toStdString().c_str());
        } else {
            nm_setting_vpn_add_data_item(s_vpn,
                                         i.key().toStdString().c_str(),
                                         i.value().toStdString().c_str());
        }
    }

    NMSettingIPConfig *s_ip4 =(NMSettingIPConfig *) nm_setting_ip4_config_new();
    nm_connection_add_setting(connection, NM_SETTING(s_ip4));

    g_object_set(s_ip4,
                  NM_SETTING_IP_CONFIG_METHOD, NM_SETTING_IP4_CONFIG_METHOD_AUTO,
                  NULL);

    NMDevice* device;
    activeConnection = getDefaultActiveConnection(&device);

    nm_client_add_and_activate_connection_async(nmClient_, connection, device, NULL, NULL, added_cb, NULL);
}

QString NetworkManager::getVpnParam(QString connectionId, QString param)
{
    QString paramValue("");
    NMRemoteConnection *remoteConnection = getConnectionById(connectionId);
    if (remoteConnection) {
        NMSettingVpn *settingVpn = nm_connection_get_setting_vpn(NM_CONNECTION(remoteConnection));
        paramValue = QString(nm_setting_vpn_get_data_item(settingVpn, param.toStdString().c_str()));
        qDebug() << connectionId << "connection" << param << "=" << paramValue;
    }

    return paramValue;
}

bool NetworkManager::isActiveConnection(QString connectionId)
{
    return(getActiveConnectionById(connectionId) != NULL);
}

NMRemoteConnection *NetworkManager::getConnectionById(QString id) {
    const GPtrArray *connections = nm_client_get_connections(nmClient_);
    NMRemoteConnection *candidate = NULL;
    g_print("Found %d connections.\n", connections->len);
    for(unsigned int i = 0; i < connections->len; ++i) {
        NMRemoteConnection *remoteConnection =(NMRemoteConnection*)connections->pdata[i];

        QString candidateId(nm_connection_get_id(NM_CONNECTION(remoteConnection)));
        if (candidateId == id) {
            candidate = remoteConnection;
            break;
        }
    }

    return candidate;
}

NMActiveConnection* NetworkManager::getActiveConnectionById(QString id) {
    const GPtrArray *connections = nm_client_get_active_connections(nmClient_);

    if (!connections || connections->len == 0)
        return NULL;

    NMActiveConnection *candidate = NULL;
    g_print("Found %d active connections.\n", connections->len);
    for(unsigned int i = 0; i < connections->len; ++i) {
        NMActiveConnection *activeConnection =(NMActiveConnection*)connections->pdata[i];
        /**
         * For some weird reason, we may get non-active connections from
         * nm_client_get_active_connections() and that makes
         * nm_active_connection_get_id() to assert :-S
        **/
        QString candidateId(nm_active_connection_get_id(activeConnection));
        if (candidateId == id) {
            candidate = activeConnection;
            break;
        }
    }

    return candidate;
}

NMActiveConnection* NetworkManager::getDefaultActiveConnection(NMDevice **device) {
    NMActiveConnection *defaultActiveCon = NULL;
    NMDevice *nonDefaultDevice = NULL;
    NMActiveConnection *nonDefaultActiveCon = NULL;

    const GPtrArray *connections = nm_client_get_active_connections(nmClient_);
    for(unsigned int i = 0; i < connections->len; ++i) {
        NMActiveConnection *candidate =(NMActiveConnection*)connections->pdata[i];
        const GPtrArray *devices;

        devices = nm_active_connection_get_devices(candidate);
        if (!devices->len)
            continue;

        if (nm_active_connection_get_default(candidate)) {
            if (!defaultActiveCon) {
                *device =(NMDevice*)devices->pdata[0];
                defaultActiveCon = candidate;
            }
        } else {
            if (!nonDefaultActiveCon) {
                nonDefaultDevice =(NMDevice*)devices->pdata[0];
                nonDefaultActiveCon = candidate;
            }
        }
    }
    /**
     * Prefer the default connection if one exists, otherwise return the first
     * non-default connection.
     **/
    if (!defaultActiveCon && nonDefaultActiveCon) {
        defaultActiveCon = nonDefaultActiveCon;
        *device = nonDefaultDevice;
    }

    return defaultActiveCon;
}
