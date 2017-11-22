#ifndef STRINGMODEL_H
#define STRINGMODEL_H
#include <QAbstractListModel>

class StringModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum StringRoles{
        strRole = Qt::UserRole + 1,
    };
    StringModel(QObject* parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    void addString(const QString& str);
    Q_INVOKABLE void clear();
    Q_INVOKABLE QVariant get(int row);

protected:
    QHash<int,QByteArray> roleNames() const;
private:
    QStringList m_list;
};

#endif // STRINGMODEL_H
