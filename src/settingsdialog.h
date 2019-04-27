#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QSettings>
#include <QProcess>
#include <QByteArray>

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    SettingsDialog(QSettings *settings, QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

protected:
    void showEvent(QShowEvent *event);

private slots:
    void onOpenVpnReadyRead();
    void onOpenVpnFinished(int result);
    void onAcceptClicked(bool checked = false);
    void onCancelClicked(bool checked = false);

private:
    void getOpenvpnCiphers();
    void loadSettings();
    void saveSettings();

private:
    QProcess openVpnProcess_;
    QByteArray openVpnCiphersRaw_;
    QStringList openVpnCiphers_;
    QSettings *settings_;
    QGridLayout* layaout_;
    QFormLayout* lytServerForm_;
    QWidget* wgtServerForm_;
    QLineEdit *lneConnectionName_;
    QComboBox *cmbAuth_;
    QLineEdit *lneCa_;
    QLineEdit *lneTa_;
    QComboBox *cmbCipher_;
    QComboBox *cmbCompLZO_;
    QComboBox *cmbConnectionType_;
    QComboBox *cmbFloat_;
    QComboBox *cmbMssFix_;
    QLineEdit *lnePasswordFlags_;
    QSpinBox  *spbPort_;
    QComboBox *cmbProtoTCP_;
    QComboBox *cmbRemoteRandom_;
    QComboBox *cmbTunIpv6_;
    QLineEdit *lneUserName_;
    QLineEdit *lnePassword_;
    QPushButton *btnAccept_;
    QPushButton *btnCancel_;

};

#endif // SETTINGSDIALOG_H
