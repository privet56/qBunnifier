#include "fRabbifierMuzzleModel.h"
#include <QBrush>
#include <QIcon>
#include <QDebug>
#include <QPainter>
#include <QAbstractTextDocumentLayout>
#include <QApplication>
#include <QMainWindow>
#include <QTimer>
#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QPixmap>
#include <QFileDialog>
#include "f.h"
#include "str.h"

/*!
 * Creates a model object with the given \a ds and \a parent.
 */
fRabbifierMuzzleModel::fRabbifierMuzzleModel(QObject *parent)
    :QAbstractTableModel(parent),
      m_pLog(nullptr)
{

}

void fRabbifierMuzzleModel::sort(int column, Qt::SortOrder order/* = Qt::AscendingOrder*/)
{
    emit layoutChanged();
}

/*!
 * Destroys the model.
 */
fRabbifierMuzzleModel::~fRabbifierMuzzleModel()
{

}

int fRabbifierMuzzleModel::rowCount(const QModelIndex &/*parent*/) const
{
    QString sMuzzlesDir = str::makeAbsFN(qApp->applicationDirPath(), "/res");
    QString sPattern = "muzzle?.png";

    QDirIterator it(sMuzzlesDir, QStringList() << sPattern, QDir::Files);
    int i=0;
    while (it.hasNext())
    {
        it.next();
        i++;
    }

    return i;	//TODO: look in dir for muzzle\d+
}

int fRabbifierMuzzleModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 1;
}

Qt::ItemFlags fRabbifierMuzzleModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;// | Qt::ItemIsEditable;
}

QVariant fRabbifierMuzzleModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int row2BeReturned = index.row();
    int col2BeReturned = index.column();

    if((role == Qt::DisplayRole) || (role == Qt::DecorationRole) || (role == Qt::SizeHintRole))
    {
        if(role == Qt::DisplayRole)
        {
            return QString::number(row2BeReturned);
        }

        QString sAbsFN = str::makeAbsFN(qApp->applicationDirPath(), "/res/muzzle"+QString::number(row2BeReturned)+".png");
        if(!QFile::exists(sAbsFN))
        {
            qDebug() << "fnf " << sAbsFN;
        }
        QImage qimg(sAbsFN);
        qimg = qimg.scaled(55, 55, Qt::KeepAspectRatio);
        QPixmap pixmap = QPixmap::fromImage(qimg);

        if(role == Qt::SizeHintRole)
        {
            return pixmap.size();
        }
        if(role == Qt::DecorationRole)
        {
            return pixmap;
        }

        return QString::number(row2BeReturned);
    }
    if((role == Qt::ToolTipRole) || (role == Qt::StatusTipRole))
    {
        return QString::number(row2BeReturned);
    }
    else if (role == Qt::EditRole)
    {
        return QVariant();	//should not happen
    }
    else if (role == Qt::TextAlignmentRole)
    {
        Qt::Alignment align;
        align |= Qt::AlignJustify;
        return QVariant(align);
    }
    else if (role == Qt::FontRole)
    {
		return QVariant();
    }
    else if (role == Qt::TextColorRole)
    {

    }
    else if (role == Qt::ForegroundRole)
    {

    }
    else if (role == Qt::BackgroundRole)
    {
       return QVariant::fromValue(QColor(Qt::white));
    }

    return QVariant();
}

QVariant fRabbifierMuzzleModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DecorationRole)
    {
        if (orientation == Qt::Vertical)
        {
		
        }
    }
    else if(role == Qt::SizeHintRole)
    {
        //if (orientation == Qt::Horizontal)
        if(section == 2)
        {
            //return QVariant(QSize(55, 55));
        }
    }
    else if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
			//column headers
            return "muzzle";
        }
        else
        {
			//row 'header'
            //return QString::number(section + 1);
        }
    }
    return QVariant();
}

bool fRabbifierMuzzleModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(value)

    if (!index.isValid())
        return false;

    if (role == Qt::EditRole)
    {	//should not happen
		return false;
    }

    return false;
}
void fRabbifierMuzzleModel::setEnv(logger* pLog)
{
    this->m_pLog = pLog;
}
QModelIndex fRabbifierMuzzleModel::buddy(const QModelIndex &index) const
{
    return index;
}
