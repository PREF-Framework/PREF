#include "stringsmodel.h"

StringsModel::StringsModel(QHexEditData *hexeditdata, QObject *parent): QAbstractItemModel(parent)
{
    this->_reader = new QHexEditDataReader(hexeditdata, this);
}

void StringsModel::setData(const StringsModel::OffsetList &offsetlist, const StringsModel::StringMap &strings)
{
    this->beginInsertRows(QModelIndex(), offsetlist.length(), offsetlist.length());
    this->_offsetlist = offsetlist;
    this->_strings = strings;
    this->endInsertRows();
}

qint64 StringsModel::offset(int i) const
{
    return this->_offsetlist[i];
}

StringsModel::StringRange StringsModel::range(int i) const
{
    qint64 offset = this->_offsetlist[i];
    return this->_strings[offset];
}

QString StringsModel::string(int i) const
{
    const StringsModel::StringRange& range = this->range(i);
    return this->_reader->read(range.first, range.second).simplified();
}

int StringsModel::columnCount(const QModelIndex&) const
{
    return 2;
}

QVariant StringsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
            case 0:
                return "Offset";

            case 1:
                return "String";

            default:
                break;
        }
    }

    return QVariant();
}

QVariant StringsModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        if(index.column() == 0)
            return QString("0x").append(QString("%1").arg(this->_offsetlist[index.row()], 8, 16, QLatin1Char('0')).toUpper());
        else if(index.column() == 1)
            return this->string(index.row());
    }

    return QVariant();
}

QModelIndex StringsModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!this->hasIndex(row, column, parent))
        return QModelIndex();

    return this->createIndex(row, column);
}

QModelIndex StringsModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int StringsModel::rowCount(const QModelIndex &) const
{
    return this->_offsetlist.length();
}

Qt::ItemFlags StringsModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}