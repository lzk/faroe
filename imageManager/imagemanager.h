#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H

#include <QObject>

class ImageManager : public QObject
{
    Q_OBJECT
public:
    explicit ImageManager(QObject *parent = nullptr);

signals:

public slots:
    void imagesToPrint(QStringList ,QString);
    void imagesToEmail(QStringList ,int);
    void imagesToFile(QStringList ,QString);
    void imagesToFTP(QStringList ,QString);
    void imagesToApplication(QStringList ,QString);
    void imagesToCloud(QStringList ,QString);
};

#endif // IMAGEMANAGER_H
