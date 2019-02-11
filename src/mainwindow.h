#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QFormLayout>
#include <QComboBox>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QSettings>
#include <QTimer>
#include "providers/ipvanish/ipvanish.h"
#include "networkmanager.h"
#include "settingsdialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);

private slots:
    void refreshStatus();
    void onSettingsTriggered(bool checked);
    void onNewCountry(IpvContry* country);
    void onLoadFinish();
    void onCountryClicked(const QString &text);
    void onServerClicked(QListWidgetItem* item);
    void onConnectClicked(bool checked = false);
    void onSettingsAccpeted();

private:
    void loadSettings();
    void connectVpn();
    void disconnectVpn();

private:
    QSettings* settings_;
    SettingsDialog *dlgSettings_;
    QMenuBar* mnuBar_;
    QMenu* mnuEdit_;
    QAction* actSttings_;
    QGridLayout* layout_;
    QFormLayout* lytServerForm_;
    QWidget* wgtServerForm_;
    QLabel* lblConnectionStatus_;
    QComboBox* wgtLstCountries_;
    QListWidget* wgtLstServers_;
    QPushButton* btnConnect_;
    IpVanish* ipvanish_;
    QMap<QString, IpvContry*>* countries_;
    NetworkManager* networkManager_;
    QTimer refreshTimer_;
    QString connectionName_;

};

#endif // MAINWINDOW_H
