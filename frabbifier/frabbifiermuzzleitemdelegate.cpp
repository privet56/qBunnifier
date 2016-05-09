#include "frabbifiermuzzleitemdelegate.h"
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

fRabbifierMuzzleItemDelegate::fRabbifierMuzzleItemDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

void fRabbifierMuzzleItemDelegate::paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    QStyleOptionViewItemV4 options = option;
    initStyleOption(&options, index);

    painter->save();


#ifndef USE_TEXTDOCUMENT
    QTextDocument doc;
    doc.setHtml(options.text);

    options.text = "";
    options.widget->style()->drawControl(QStyle::CE_ItemViewItem, &options, painter);

    // shift text right to make icon visible
    QSize iconSize = options.icon.actualSize(options.rect.size());
    painter->translate(options.rect.left()+iconSize.width(), options.rect.top());
    QRect clip(0, 0, options.rect.width()+iconSize.width(), options.rect.height());

    //doc.drawContents(painter, clip);

    painter->setClipRect(clip);
    QAbstractTextDocumentLayout::PaintContext ctx;
    // set text color to red for selected item
    if (option.state & QStyle::State_Selected)
       ctx.palette.setColor(QPalette::Text, QColor("red"));
    ctx.clip = clip;
    doc.documentLayout()->draw(painter, ctx);
#else
    //incomplete
    QString sAbsFN = str::makeAbsFN(qApp->applicationDirPath(), "muzzle"+options.text+".png");
    QLabel label;
    //QImage qimg(sAbsFN);
    QPixmap qpx(sAbsFN);
    label.setPixmap(qpx);
    label.drawFrame(painter);
#endif

    painter->restore();
}

// TableView need to create an Editor
// Create Editor when we construct MyDelegate
// and return the Editor
QWidget* fRabbifierMuzzleItemDelegate::createEditor(QWidget *parent,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const
{
    return QStyledItemDelegate::createEditor(parent, option, index);
}

// Then, we set the Editor
// Gets the data from Model and feeds the data to Editor
void fRabbifierMuzzleItemDelegate::setEditorData(QWidget *editor,
                               const QModelIndex &index) const
{
    QStyledItemDelegate::setEditorData(editor, index);
}

// When we modify data, this model reflect the change
void fRabbifierMuzzleItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                              const QModelIndex &index) const
{

}

// Give the SpinBox the info on size and location
void fRabbifierMuzzleItemDelegate::updateEditorGeometry(QWidget *editor,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
