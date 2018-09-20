#include <QFile>
#include <QMutex>
#include <QTime>
#include <QThread>
#include<QTextStream>
#define VERSION_DEFINE
#include "version.h"
#include <QDebug>
#include <QDir>
QString log_file;

void init_log()
{
#ifdef Q_OS_WIN
    QString tmpdir = QDir::tempPath() + "/scanimage";
    QString realLogFile = tmpdir + "/VOP-" + QDateTime::currentDateTime().toString("yyMMdd-HHmmsszzz") + ".log";
    QString str;
    log_file = realLogFile;
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
    QString tmpdir = "/tmp";
    QString realLogFile = tmpdir + "/VOP-" + QDateTime::currentDateTime().toString("yyMMdd-HHmmsszzz") + ".log";
    QString str;
    log_file = tmpdir + "/VOP.log";
    int ret;
    str = str.sprintf("echo \"----------------VOP %s------------------\" > " ,vop_version);
    str += realLogFile;
    ret = system(str.toUtf8());

//    str = "";
    str = "chmod 666 " + realLogFile + " 2>>" + realLogFile;
    ret = system(str.toUtf8());
    str = "rm " + log_file + " 2>>" + realLogFile;
    ret = system(str.toUtf8());
    str = "ln -s " + realLogFile + " " + log_file + " 2>>" + realLogFile;
    ret = system(str.toUtf8());
    str = "uname -a >> " + realLogFile;
    ret = system(str.toUtf8());
#ifdef Q_OS_LINUX
    str = "cat /etc/issue >> " +realLogFile;
    ret = system(str.toUtf8());
#endif
#endif
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
