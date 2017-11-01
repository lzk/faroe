#ifndef JKINTERFACE_H
#define JKINTERFACE_H

#include <QObject>
#include "../thumbnailviewer/imagemodel.h"

class JKInterface : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(ImageModel* model READ getImageModel CONSTANT)
    explicit JKInterface(QObject *parent = nullptr);

    ImageModel* getImageModel();
    Q_INVOKABLE void add();
signals:

public slots:


private:
    ImageModel model;
    const QString tmppath;
};

#endif // JKINTERFACE_H
