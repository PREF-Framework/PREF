#include "functionmodel.h"

FunctionModel::FunctionModel(DisassemblerListing* listing, QObject *parent): QAbstractItemModel(parent), _listing(listing)
{
    this->_monospacefont.setFamily("Monospace");
    this->_monospacefont.setPointSize(qApp->font().pointSize());
    this->_monospacefont.setStyleHint(QFont::TypeWriter);

    for(int i = 0; i < this->_listing->segmentsCount(); i++)
    {
        Segment* segment = this->_listing->segment(i);

        for(int j = 0; j < segment->functionsCount(); j++)
            this->_functions.append(segment->function(j));
    }

    std::sort(this->_functions.begin(), this->_functions.end(), &FunctionModel::sortByAddress);
}

bool FunctionModel::sortByAddress(Function *f1, Function *f2)
{
    return f1->startAddress() < f2->startAddress();
}

QString FunctionModel::functionType(Function *f) const
{
    switch(f->type())
    {
        case FunctionTypes::EntryPoint:
            return "Entry Point";

        case FunctionTypes::Import:
            return "Import";

        case FunctionTypes::Export:
            return "Export";

        default:
            break;
    }

    return "F";
}

int FunctionModel::columnCount(const QModelIndex &) const
{
    return 4;
}

QVariant FunctionModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
            case 0:
                return "Name";

            case 1:
                return "Segment";

            case 2:
                return "Address";

            case 3:
                return "Type";

            default:
                break;
        }
    }

    return QVariant();
}

QVariant FunctionModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    Function* f = reinterpret_cast<Function*>(index.internalPointer());

    if(role == Qt::DisplayRole)
    {   
        switch(index.column())
        {                
            case 0:
                return this->_listing->symbolTable()[f->startAddress()]->name();

            case 1:
                return QString("%1").arg(f->startAddress(), 8, 16, QLatin1Char('0')).toUpper();

            case 2:
                return QString("'%1'").arg(f->segmentName());

            case 3:
                return this->functionType(f);

            default:
                break;
        }
    }
    else if(role == Qt::BackgroundRole)
    {
        switch(f->type())
        {
            case FunctionTypes::EntryPoint:
                return QColor(Qt::yellow);

            case FunctionTypes::Import:
                return QColor(Qt::darkGray);

            case FunctionTypes::Export:
                return QColor(Qt::darkYellow);

            default:
                break;
        }
    }
    else if(role == Qt::ForegroundRole)
    {
        if(index.column() == 1)
                return QColor(Qt::darkBlue);

        if(index.column() == 2)
            return QColor(Qt::darkGreen);
    }
    else if(role == Qt::FontRole)
        return this->_monospacefont;

    return QVariant();
}

QModelIndex FunctionModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!this->hasIndex(row, column, parent))
        return QModelIndex();

    return this->createIndex(row, column, this->_functions[row]);
}

QModelIndex FunctionModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int FunctionModel::rowCount(const QModelIndex &) const
{
    return this->_functions.count();
}

Qt::ItemFlags FunctionModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}