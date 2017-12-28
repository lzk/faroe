#include "jkinterface.h"
#include <QPrinterInfo>

JKInterface::JKInterface(QObject *parent) : QObject(parent)
{
}

JKInterface::~JKInterface()
{
}

QStringList JKInterface::getPrinterName()
{
    return QPrinterInfo::availablePrinterNames();
}
