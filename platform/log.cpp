#include <QFile>
#include <QMutex>
#include <QTime>
#include <QThread>
#include<QTextStream>
#define VERSION_DEFINE
#include "version.h"
#include <QDebug>
QString log_file;

void init_log()
{
    int ret;
    log_file = "/tmp/VOP.log";
    QString realLogFile = "/tmp/VOP-" + QDateTime::currentDateTime().toString("yyMMdd-HHmmsszzz") + ".log";
    QString str;
#if 0
    str = "touch " + realLogFile;
    QFile file(realLogFile);
    if(file.exists())
        file.remove();
    if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)){
        QTextStream out(&file);
        QString str;
        out << str.sprintf("----------------VOP %s------------------" ,vop_version) << endl;
        file.close();
    }
#else
    str = str.sprintf("echo \"----------------VOP %s------------------\" > " ,vop_version);
    str += realLogFile;
    ret = system(str.toLatin1());
#endif
//    str = "";
    str = "chmod 666 " + realLogFile + " 2>>" + realLogFile;
    ret = system(str.toLatin1());
    str = "rm " + log_file + " 2>>" + realLogFile;
    ret = system(str.toLatin1());
    str = "ln -s " + realLogFile + " " + log_file + " 2>>" + realLogFile;
    ret = system(str.toLatin1());
    str = "uname -a >> " + realLogFile;
    ret = system(str.toLatin1());
#ifdef Q_OS_LINUX
    str = "cat /etc/issue >> " +realLogFile;
    ret = system(str.toLatin1());
#endif
    if(ret){;}
}

static QMutex mutex_write_log_file;
void logout(const QString& msg)
{
    qDebug()<<msg;
    QMutexLocker locker(&mutex_write_log_file);
    QFile file(log_file);
    if(file.open(QIODevice::WriteOnly | QIODevice::Append)){
        QTextStream out(&file);
        out << QDateTime::currentDateTime().toString("dd.MM.yyyy-hh:mm:ss.zzz:")
               <<"                        " <<msg << endl;
//            <<QThread::currentThreadId()<<"                        " <<msg << endl;
        file.close();
    }
}
