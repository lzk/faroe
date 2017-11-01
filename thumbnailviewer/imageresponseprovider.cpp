#include "imageresponseprovider.h"
#include <QImage>
#include <QDebug>
class AsyncImageResponse : public QQuickImageResponse, public QRunnable
{
    public:
        AsyncImageResponse(const QString &id, const QSize &requestedSize)
         : m_id(id), m_requestedSize(requestedSize)
        {
            setAutoDelete(false);
        }

        QQuickTextureFactory *textureFactory() const
        {
            return QQuickTextureFactory::textureFactoryForImage(m_image);
        }

        void run()
        {
            QString path = m_id;
//            qDebug()<<"url:"<<url.scheme()<<"\tpath:"<<url.path()<<"\tname:"<<url.fileName();
            m_image = QImage(path);
//            if (m_requestedSize.isValid() && (m_image.size() -m_requestedSize).isValid())
//                m_image = m_image.scaled(m_requestedSize);
//            qDebug()<<"request:"<<m_requestedSize;
            emit finished();
        }

        QString m_id;
        QSize m_requestedSize;
        QImage m_image;
};

QQuickImageResponse *AsyncImageProvider::requestImageResponse(const QString &id, const QSize &requestedSize)
{
    AsyncImageResponse *response = new AsyncImageResponse(id, requestedSize);
    pool.start(response);
    return response;
}


