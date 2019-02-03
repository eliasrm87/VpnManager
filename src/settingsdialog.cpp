#include <QStandardPaths>
#include <QDebug>
#include "settingsdialog.h"


SettingsDialog::SettingsDialog(QSettings *settings, QWidget *parent, Qt::WindowFlags f) :
    QDialog(parent, f), settings_(settings)
{
    layaout_           = new QGridLayout(this);
    lytServerForm_     = new QFormLayout(this);
    wgtServerForm_     = new QWidget(this);
    lneConnectionName_ = new QLineEdit(this);
    cmbAuth_           = new QComboBox(this);
    lneCa_             = new QLineEdit(this);
    cmbCipher_         = new QComboBox(this);
    cmbCompLZO_        = new QComboBox(this);
    cmbConnectionType_ = new QComboBox(this);
    cmbFloat_          = new QComboBox(this);
    cmbMssFix_         = new QComboBox(this);
    lnePasswordFlags_  = new QLineEdit(this);
    spbPort_           = new QSpinBox(this);
    cmbProtoTCP_       = new QComboBox(this);
    cmbRemoteRandom_   = new QComboBox(this);
    cmbTunIpv6_        = new QComboBox(this);
    lneUserName_       = new QLineEdit(this);
    lnePassword_       = new QLineEdit(this);
    btnAccept_         = new QPushButton("Accept", this);
    btnCancel_         = new QPushButton("Cancel", this);

    setWindowTitle("Settings");
    setMinimumWidth(600);
    setLayout(layaout_);

    connect(&openVpnProcess_, SIGNAL(readyReadStandardOutput()),   this, SLOT(onOpenVpnReadyRead()));
    connect(&openVpnProcess_, SIGNAL(finished(int)), this, SLOT(onOpenVpnFinished(int)));
    connect(btnAccept_,       SIGNAL(clicked(bool)), this, SLOT(onAcceptClicked(bool)));
    connect(btnCancel_,       SIGNAL(clicked(bool)), this, SLOT(onCancelClicked(bool)));

    wgtServerForm_->setLayout(lytServerForm_);

    layaout_->addWidget(wgtServerForm_, 0, 0, 1, 2);
    layaout_->addWidget(btnAccept_,     1, 0, 1, 1);
    layaout_->addWidget(btnCancel_,     1, 1, 1, 1);

    lytServerForm_->addRow("Connection name", lneConnectionName_);
    lytServerForm_->addRow("Auth", cmbAuth_);
    lytServerForm_->addRow("CA path", lneCa_);
    lytServerForm_->addRow("Cipher", cmbCipher_);
    lytServerForm_->addRow("Use LZO compression", cmbCompLZO_);
    lytServerForm_->addRow("Connection type", cmbConnectionType_);
    lytServerForm_->addRow("Float", cmbFloat_);
    lytServerForm_->addRow("Restrict Tunnel TCP Maximum Segment Size", cmbMssFix_);
    lytServerForm_->addRow("Password Flags", lnePasswordFlags_);
    lytServerForm_->addRow("Port", spbPort_);
    lytServerForm_->addRow("Use TCP", cmbProtoTCP_);
    lytServerForm_->addRow("Randomize remote hosts", cmbRemoteRandom_);
    lytServerForm_->addRow("TUN IPv6", cmbTunIpv6_);
    lytServerForm_->addRow("User name", lneUserName_);
    lytServerForm_->addRow("Password", lnePassword_);

    cmbAuth_->addItems(QString("none,RSA-MD4,MD5,SHA1,SHA224,SHA256,SHA384,SHA512,RIPEMD160").split(","));
    cmbCipher_->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    cmbCompLZO_->addItems(QString("yes,no").split(","));
    cmbConnectionType_->addItems(QString("password").split(","));
    cmbFloat_->addItems(QString("yes,no").split(","));
    cmbMssFix_->addItems(QString("yes,no").split(","));
    spbPort_->setRange(1, 65535);
    cmbProtoTCP_->addItems(QString("yes,no").split(","));
    cmbRemoteRandom_->addItems(QString("yes,no").split(","));
    cmbTunIpv6_->addItems(QString("yes,no").split(","));
    lnePassword_->setEchoMode(QLineEdit::Password);
}

void SettingsDialog::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    getOpenvpnCiphers();
    loadSettings();
}

void SettingsDialog::onOpenVpnReadyRead()
{
    openVpnCiphersRaw_.push_back(openVpnProcess_.readAllStandardOutput());
}

void SettingsDialog::onOpenVpnFinished(int result)
{
    Q_UNUSED(result);

    const QList<QByteArray> stdOutLines = openVpnCiphersRaw_.split('\n');
    bool foundFirstSpace = false;
    openVpnCiphers_.clear();
    foreach(const QByteArray &cipher, stdOutLines) {
        if (cipher.length() == 0) {
            foundFirstSpace = true;
        } else if (foundFirstSpace) {
            openVpnCiphers_.push_back(QString(cipher.left(cipher.indexOf(' '))));
        }
    }

    cmbCipher_->addItems(openVpnCiphers_);
}

void SettingsDialog::onAcceptClicked(bool checked)
{
    Q_UNUSED(checked);

    saveSettings();
    accept();
}

void SettingsDialog::onCancelClicked(bool checked)
{
    Q_UNUSED(checked);

    loadSettings();
    reject();
}

void SettingsDialog::getOpenvpnCiphers()
{
    if (openVpnCiphersRaw_.length())
        return;

    // start openVPN process and get its cipher list
    const QString openVpnBinary = QStandardPaths::findExecutable("openvpn", QStringList() << "/sbin" << "/usr/sbin");
    const QStringList ciphersArgs("--show-ciphers");

    openVpnProcess_.setReadChannel(QProcess::StandardOutput);
    openVpnProcess_.setProcessChannelMode(QProcess::SeparateChannels);
    openVpnProcess_.setProgram(openVpnBinary);
    openVpnProcess_.setArguments(ciphersArgs);
    openVpnProcess_.start();
}

void SettingsDialog::loadSettings()
{
    lneConnectionName_->setText(settings_->value("ConnectionName", "IPVanish").toString());
    cmbAuth_->setCurrentText(settings_->value("Auth", "SHA256").toString());
    lneCa_->setText(settings_->value("CA", "").toString());
    cmbCipher_->setCurrentText(settings_->value("Cipher", "AES-256-CBC").toString());
    cmbCompLZO_->setCurrentText(settings_->value("CompLZO", "yes").toString());
    cmbConnectionType_->setCurrentText(settings_->value("ConnectionType", "password").toString());
    cmbFloat_->setCurrentText(settings_->value("Float", "no").toString());
    cmbMssFix_->setCurrentText(settings_->value("MssFix", "no").toString());
    lnePasswordFlags_->setText(settings_->value("PasswordFlags", "0").toString());
    spbPort_->setValue(settings_->value("Port", "443").toString().toInt());
    cmbProtoTCP_->setCurrentText(settings_->value("ProtoTCP", "yes").toString());
    cmbRemoteRandom_->setCurrentText(settings_->value("RemoteRandom", "no").toString());
    cmbTunIpv6_->setCurrentText(settings_->value("TunIpv6", "no").toString());
    lneUserName_->setText(settings_->value("UserName").toString());
    lnePassword_->setText(settings_->value("Password").toString());
}

void SettingsDialog::saveSettings()
{
    settings_->setValue("ConnectionName", lneConnectionName_->text());
    settings_->setValue("Auth", cmbAuth_->itemText(cmbAuth_->currentIndex()));
    settings_->setValue("CA", lneCa_->text());
    settings_->setValue("Cipher", cmbCipher_->itemText(cmbCipher_->currentIndex()));
    settings_->setValue("CompLZO", cmbCompLZO_->itemText(cmbCompLZO_->currentIndex()));
    settings_->setValue("ConnectionType", cmbConnectionType_->itemText(cmbConnectionType_->currentIndex()));
    settings_->setValue("Float", cmbFloat_->itemText(cmbFloat_->currentIndex()));
    settings_->setValue("MssFix", cmbMssFix_->itemText(cmbMssFix_->currentIndex()));
    settings_->setValue("PasswordFlags", lnePasswordFlags_->text());
    settings_->setValue("Port", QString::number(spbPort_->value()));
    settings_->setValue("ProtoTCP", cmbProtoTCP_->itemText(cmbProtoTCP_->currentIndex()));
    settings_->setValue("RemoteRandom", cmbRemoteRandom_->itemText(cmbRemoteRandom_->currentIndex()));
    settings_->setValue("TunIpv6", cmbTunIpv6_->itemText(cmbTunIpv6_->currentIndex()));
    settings_->setValue("UserName", lneUserName_->text());
    // TODO: Look for a safer method to save the password
    settings_->setValue("Password", lnePassword_->text());
}
