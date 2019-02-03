#ifndef WGTSERVER_H
#define WGTSERVER_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>

class WgtServer : public QWidget
{
    Q_OBJECT
public:
    explicit WgtServer(QString hostName, int capacity, QWidget *parent = Q_NULLPTR);
    ~WgtServer();
    QString hostName();

signals:

public slots:

private:
    QHBoxLayout  *layout_;
    QLabel       *lblHostName_;
    QProgressBar *pbServerUsage_;
};

#endif // WGTSERVER_H
