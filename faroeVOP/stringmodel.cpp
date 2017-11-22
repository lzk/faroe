#include "stringmodel.h"
#include <QDebug>
StringModel::StringModel(QObject* parent)
    :QAbstractListModel(parent)
{

}

int StringModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_list.count();

}

QVariant StringModel::data(const QModelIndex &index, int role) const
{
    if(index.row()<0 || index.row() >= m_list.count())
        return QVariant();
    const QString& str = m_list[index.row()];
    if(role == strRole)
        return str;
    return QVariant();
}

QHash<int,QByteArray> StringModel::roleNames() const
{
    QHash<int ,QByteArray> roles;
    roles[strRole] = "str";
    return roles;
}

bool StringModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool isDataChanged = false;
    if (index.isValid()) {
        QString& str = m_list[index.row()];
        if(role == strRole){
            str = value.toString();
            isDataChanged = true;
        }
        if(isDataChanged){
            //emit dataChanged(index, index);//list out of range?
            emit dataChanged(index, index ,QVector<int>(1 ,role));
        }
    }
    return isDataChanged;
}

void StringModel::addString(const QString& str)
{
    beginInsertRows(QModelIndex() ,rowCount() ,rowCount());
    m_list << str;
    endInsertRows();
}

void StringModel::clear()
{
    int count = rowCount();
    if(count < 1)
        return;
    beginRemoveRows(QModelIndex(), 0, count - 1);
    for (int row = 0; row < count; ++row) {
        m_list.removeLast();
    }
    endRemoveRows();
}

QVariant StringModel::get(int row)
{
    qDebug()<<__func__;
    if(row >= 0)
    {
//        const ImageItem* item = &m_list.at(row);
//        QMap<QString, QVariant> itemData;
//        itemData.insert("url" ,QVariant(item->url()));
//        itemData.insert("sn" ,QVariant(item->sn()));
//        return QVariant(itemData);
        return QVariant(&m_list.at(row));
    }
    return QVariant();
}
