#ifndef LOG_H
#define LOG_H

#include <QDebug>
#include <QString>
#define LOG_NOPARA(x) logout(x)
#define LOG_PARA(format, ...) logout(QString().sprintf(format ,##__VA_ARGS__))

void init_log();
void logout(const QString& msg);

#endif // LOG_H
