#include <QDebug>
#include "wgtserver.h"

WgtServer::WgtServer(QString hostName, int capacity, QWidget *parent) : QWidget(parent)
{
    layout_        = new QHBoxLayout(this);
    lblHostName_   = new QLabel(this);
    pbServerUsage_ = new QProgressBar(this);

    lblHostName_->setText(hostName);
    pbServerUsage_->setValue(capacity);

    setLayout(layout_);
    layout_->addWidget(lblHostName_);
    layout_->addWidget(pbServerUsage_);
}

WgtServer::~WgtServer()
{

}

QString WgtServer::hostName()
{
    return lblHostName_->text();
}
