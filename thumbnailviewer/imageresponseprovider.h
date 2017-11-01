#ifndef IMAGERESPONSEPROVIDER_H
#define IMAGERESPONSEPROVIDER_H

#include <qquickimageprovider.h>
#include <QThreadPool>
class AsyncImageProvider : public QQuickAsyncImageProvider
{
public:
    QQuickImageResponse *requestImageResponse(const QString &id, const QSize &requestedSize);
private:
    QThreadPool pool;
};
#endif // IMAGERESPONSEPROVIDER_H
