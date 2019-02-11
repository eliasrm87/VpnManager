#include <QDebug>
#include <QHeaderView>
#include <QApplication>
#include <QMenuBar>
#include <QSizePolicy>
#include "mainwindow.h"
#include "wgtserver.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    settings_  = new QSettings("eliasrm", "vpnManager");
    countries_ = new QMap<QString, IpvContry*>();

    setWindowTitle("VPN Manager");
    setMinimumSize(800, 600);

    layout_              = new QGridLayout(this);
    mnuBar_              = new QMenuBar(this);
    mnuEdit_             = new QMenu("Edit", mnuBar_);
    actSttings_          = new QAction("Settings", mnuEdit_);
    wgtServerForm_       = new QWidget(this);
    lytServerForm_       = new QFormLayout(this);
    lblConnectionStatus_ = new QLabel(this);
    wgtLstCountries_     = new QComboBox(this);
    wgtLstServers_       = new QListWidget(this);
    btnConnect_          = new QPushButton("Connect", this);
    networkManager_      = new NetworkManager(this);
    dlgSettings_         = new SettingsDialog(settings_, this);

    setMenuBar(mnuBar_);
    setCentralWidget(new QWidget(this));
    centralWidget()->setLayout(layout_);
    wgtServerForm_->setLayout(lytServerForm_);

    mnuBar_->addMenu(mnuEdit_);
    mnuEdit_->addAction(actSttings_);

    wgtLstCountries_->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    wgtLstServers_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    lytServerForm_->addRow("Status", lblConnectionStatus_);
    lytServerForm_->addRow("Country", wgtLstCountries_);

    layout_->addWidget(wgtServerForm_,  0, 0, 1, 3);
    layout_->addWidget(wgtLstServers_ , 1, 0, 1, 3);
    layout_->addWidget(btnConnect_,     2, 0, 1, 1);

    ipvanish_ = new IpVanish(this);

    connect(ipvanish_,        SIGNAL(newCountry(IpvContry*)),
            this,             SLOT(onNewCountry(IpvContry*)));
    connect(ipvanish_,        SIGNAL(loadFinish()),
            this,             SLOT(onLoadFinish()));
    connect(wgtLstCountries_, SIGNAL(currentIndexChanged(const QString&)),
            this,             SLOT(onCountryClicked(const QString&)));
    connect(wgtLstServers_,   SIGNAL(itemClicked(QListWidgetItem*)),
            this,             SLOT(onServerClicked(QListWidgetItem*)));
    connect(btnConnect_,      SIGNAL(clicked(bool)),
            this,             SLOT(onConnectClicked(bool)));
    connect(actSttings_,      SIGNAL(triggered(bool)),
            this,             SLOT(onSettingsTriggered(bool)));
    connect(dlgSettings_,     SIGNAL(accepted()),
            this,             SLOT(onSettingsAccpeted()));
    connect(&refreshTimer_,   SIGNAL(timeout()),
            this,             SLOT(refreshStatus()));

    loadSettings();
    ipvanish_->load();
    refreshStatus();

    refreshTimer_.start(5000);
}

void MainWindow::refreshStatus()
{
    if (networkManager_->isActiveConnection(connectionName_)) {
        lblConnectionStatus_->setText("Connected");
        btnConnect_->setText("Disconnect");
    } else {
        lblConnectionStatus_->setText("Disconnected");
        btnConnect_->setText("Connect");
    }
}

void MainWindow::onSettingsTriggered(bool checked)
{
    Q_UNUSED(checked);

    dlgSettings_->show();
}

void MainWindow::onNewCountry(IpvContry *country)
{
    countries_->insert(country->country(), country);
}

void MainWindow::onLoadFinish()
{
    QMapIterator<QString, IpvContry*> i(*countries_);
    while (i.hasNext()) {
        i.next();
        wgtLstCountries_->addItem(i.value()->country());
    }

    // Select currently configured server
    QString hostName = networkManager_->getVpnParam(connectionName_, "remote");
    QString serverCountry = ipvanish_->getServerCountry(hostName);
    if (serverCountry.length()) {
        wgtLstCountries_->setCurrentText(serverCountry);
        for(int j = 0; j < wgtLstServers_->count(); ++j) {
            QListWidgetItem *item = wgtLstServers_->item(j);
            WgtServer *itenWgt =(WgtServer*)wgtLstServers_->itemWidget(item);
            if (itenWgt->hostName() == hostName) {
                wgtLstServers_->setItemSelected(item, true);
            } else {
                wgtLstServers_->setItemSelected(item, false);
            }
        }
    }
}

void MainWindow::onCountryClicked(const QString &text)
{
    IpvContry* country = countries_->value(text);

    while (wgtLstServers_->count()) {
        QListWidgetItem* item = wgtLstServers_->takeItem(0);
        delete(item);
    }

    for(int i = 0; i < country->hostNames().size(); ++i) {
        QListWidgetItem* item;
        item = new QListWidgetItem(wgtLstServers_);
        wgtLstServers_->addItem(item);
        QString hostName = country->hostNames()[i];
        WgtServer* itenWgt = new WgtServer(hostName, country->server(hostName)->capacity(), this);
        wgtLstServers_->setItemWidget(item, itenWgt);
    }
}

void MainWindow::onServerClicked(QListWidgetItem *item)
{
    Q_UNUSED(item);

}

void MainWindow::onConnectClicked(bool checked)
{
    Q_UNUSED(checked);
    if (networkManager_->isActiveConnection(connectionName_)) {
        disconnectVpn();
    } else {
        connectVpn();
    }
}

void MainWindow::onSettingsAccpeted()
{
    loadSettings();
}

void MainWindow::loadSettings()
{
    connectionName_ = settings_->value("ConnectionName", "IPVanish").toString();
}

void MainWindow::connectVpn()
{
    if (!wgtLstServers_->selectedItems().count()) return;

    QListWidgetItem* selectedItem = wgtLstServers_->selectedItems()[0];
    WgtServer* itenWgt =(WgtServer*)wgtLstServers_->itemWidget(selectedItem);
    qDebug() << "Connecting to" << itenWgt->hostName() << "...";

    QMap<QString, QString> connectionParams;
    connectionParams.insert(NMQ_VPN_AUTH,            settings_->value("Auth").toString().toStdString().c_str());
    connectionParams.insert(NMQ_VPN_CA,              settings_->value("CA").toString().toStdString().c_str());
    connectionParams.insert(NMQ_VPN_CIPHER,          settings_->value("Cipher").toString().toStdString().c_str());
    connectionParams.insert(NMQ_VPN_COMP_LZO,        settings_->value("CompLZO").toString().toStdString().c_str());
    connectionParams.insert(NMQ_VPN_CONNECTION_TYPE, settings_->value("ConnectionType").toString().toStdString().c_str());
    connectionParams.insert(NMQ_VPN_FLOAT,           settings_->value("Float").toString().toStdString().c_str());
    connectionParams.insert(NMQ_VPN_MSSFIX,          settings_->value("MssFix").toString().toStdString().c_str());
    connectionParams.insert(NMQ_VPN_PASSWORD_FLAGS,  settings_->value("PasswordFlags").toString().toStdString().c_str());
    connectionParams.insert(NMQ_VPN_PORT,            settings_->value("Port").toString().toStdString().c_str());
    connectionParams.insert(NMQ_VPN_PROTO_TCP,       settings_->value("ProtoTCP").toString().toStdString().c_str());
    connectionParams.insert(NMQ_VPN_REMOTE,          itenWgt->hostName());
    connectionParams.insert(NMQ_VPN_REMOTE_RANDOM,   settings_->value("RemoteRandom").toString().toStdString().c_str());
    connectionParams.insert(NMQ_VPN_TUN_IPV6,        settings_->value("TunIpv6").toString().toStdString().c_str());
    connectionParams.insert(NMQ_VPN_USERNAME,        settings_->value("UserName").toString().toStdString().c_str());
    connectionParams.insert(NMQ_VPN_PASSWORD,        settings_->value("Password").toString().toStdString().c_str());

    networkManager_->connectVpn(connectionName_, connectionParams);
    btnConnect_->setText("Disconnect");
}

void MainWindow::disconnectVpn()
{
    networkManager_->disconnectVpn(connectionName_);
    btnConnect_->setText("Connect");
}
