#ifndef IMAGEMODEL_H
#define IMAGEMODEL_H
#include <QAbstractListModel>
#include <QSize>
#include <QJSValue>

class ImageItem{
public:
    ImageItem(const QString &url ,QSize sourceSize ,int sn = 0);
    QString url() const;
    void setUrl(const QString& url);
    int sn() const;
    void setSn(int sn);
    QSize sourceSize() const;
    void setSourceSize(QSize sourceSize);

private:
    QString m_url;
    QSize m_sourceSize;
    int m_sn;
};

class QJSValue;
class ImageModel : public QAbstractListModel
{
    Q_OBJECT
    Q_ENUMS(ImageRoles)
public:
    enum ImageRoles{
        UrlRole = Qt::UserRole + 1,
        SnRole,
        SourceSizeRole
    };
    ImageModel(QObject* parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    void addImage(const ImageItem& ip);
    static QString getTempFilename(const QString& origin_file_name ,const QString& str);

    int getCount() const;
    Q_PROPERTY(int count READ getCount)
    Q_INVOKABLE QVariant get(int row);
    Q_INVOKABLE void remove(int row ,int count = 1);
    Q_INVOKABLE void saveRotatoedImage(int row ,int angle);
    Q_INVOKABLE void removeAll();
    Q_INVOKABLE static QString getThumbnailFilename(const QString&);

    Q_INVOKABLE void toPrint(QList<int>);
    Q_INVOKABLE void toEmail(QList<int> ,int);
    Q_INVOKABLE void toFile(QList<int> ,const QString&);
    Q_INVOKABLE void toFTP(QList<int> ,QJSValue);
    Q_INVOKABLE void toApplication(QList<int> ,const QString&);
    Q_INVOKABLE void toCloud(QList<int> ,QJSValue);
protected:
    QHash<int,QByteArray> roleNames() const;

private:
    QList<ImageItem> m_list;
};

#endif // IMAGEMODEL_H
