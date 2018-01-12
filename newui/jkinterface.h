#ifndef JKINTERFACE_H
#define JKINTERFACE_H

#include <QObject>
class JKInterface : public QObject
{
    Q_OBJECT
public:
    explicit JKInterface(QObject *parent = nullptr);
    ~JKInterface();



public:
    Q_INVOKABLE QStringList getPrinterName();

signals:

public slots:

private:
};

#endif // JKINTERFACE_H
