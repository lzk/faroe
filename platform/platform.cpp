
#include <QString>
#include <QDir>
#include "log.h"
static QString tmppath;
void platform_init()
{
    init_log();
    tmppath = (QDir::tempPath() + "/scanimage");
    QDir dir = QDir(tmppath);
    if(!dir.exists()){
        if(dir.mkdir(tmppath))
            LOG_NOPARA("mkdir success");
    }
    LOG_NOPARA("tmp image path:" + tmppath);
    dir = QDir("/Users/Shared/Pictures");
    if(!dir.exists()){
        if(dir.mkdir("/Users/Shared/Pictures"))
            LOG_NOPARA("mkdir /Users/Shared/Pictures success");
    }
}

const QString& getTempPath()
{
    return tmppath;
}
