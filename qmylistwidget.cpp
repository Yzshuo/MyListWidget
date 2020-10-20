#include "qmylistwidget.h"


#pragma execution_character_set("utf-8")
QMyListWidget::QMyListWidget(QWidget *parent)
    :QListWidget(parent)
{
    //����QListWidget������ק
    setAcceptDrops(true);
    setDragEnabled(true);

//    setDropIndicatorShown(true);
//    m_dragItem=nullptr;
}

void QMyListWidget::addPiece(QPixmap pixmap, QPoint location)
{
    QListWidgetItem *pieceItem = new QListWidgetItem(this);
    pieceItem->setIcon(QIcon(pixmap));
    pieceItem->setData(Qt::UserRole, QVariant(pixmap));
    pieceItem->setData(Qt::UserRole+1, location);
    pieceItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
}

void QMyListWidget::addPiece(QPoint location,QString name)
{
      QListWidgetItem *pieceItem = new QListWidgetItem(this);
//      pieceItem->setText("test");
      pieceItem->setData(Qt::UserRole, location);
      pieceItem->setData(Qt::UserRole+1, name);
//      pieceItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
      pieceItem->setFlags(Qt::ItemIsDragEnabled);
}
void QMyListWidget::dragEnterEvent(QDragEnterEvent *event)
{

    QMyListWidget *source=qobject_cast<QMyListWidget *>(event->source());
    if(source && source == this)
    {
        IsDraging =true;
        //���ö���Ϊ�ƶ�����
        event->setDropAction(Qt::MoveAction);
        //Ȼ������¼������һ��Ҫд
        event->accept();
    }

}

void QMyListWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    theHighlightedRow =-2;
    update(theHighlightedRect);
    IsDraging=false;
    event->accept();
}

void QMyListWidget::dragMoveEvent(QDragMoveEvent *event)
{

    QMyListWidget *source=qobject_cast<QMyListWidget *>(event->source());
    if(source && source == this)
    {
        oldHighlightedRect=theHighlightedRect;
        theHighlightedRect=targetRect(event->pos());
        if(event->pos().y() >= QApplication::startDragDistance())
        {
            theHighlightedRow=row(itemAt(event->pos() - QPoint(0,QApplication::startDragDistance())));
            if(oldHighlightedRect != theHighlightedRect)
            {

                update(oldHighlightedRect);  //ˢ�¾�����ʹDropIndicator��ʧ
                update(theHighlightedRect);  //ˢ��������ʹDropIndicator��ʾ
            }
            else
            {
                update(theHighlightedRect);
            }
        }
        else
        {
            theHighlightedRow = -1;
            update(QRect(0, 0, width(), 80));  //��ˢ�µ�һ��
        }

    }

    event->setDropAction(Qt::MoveAction);
    event->accept();

}
//����ק�����ʱ�Ĳ���
void QMyListWidget::dropEvent(QDropEvent *event)
{

    QMyListWidget *source = qobject_cast<QMyListWidget *>(event->source());
    if (source && source == this)
    {

        IsDraging = false;

        theHighlightedRow = -2;
        update(theHighlightedRect);  //��ק��ɣ�ˢ����ʹDropIndicator��ʧ

        //��Ϊ����ק��ѡ�У����Կ��Ե��ø���dropEvent(event)
        QListWidget::dropEvent(event);

        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void QMyListWidget::startDrag(Qt::DropActions supportedActions)
{

    QListWidgetItem *theDragItem = currentItem();
    theDragRow = row(theDragItem);

    QString text=theDragItem->text();

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    QPixmap pixmap = qvariant_cast<QPixmap>(theDragItem->data(Qt::UserRole));
    QPoint location = theDragItem->data(Qt::UserRole+1).toPoint();

    dataStream << text;

    QMimeData *mimeData = new QMimeData;
    mimeData->setData(myMimeType(), itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    //drag->setHotSpot(QPoint(pixmap.width()/2, pixmap.height()/2));
    //drag->setPixmap(pixmap);

    if (drag->exec(Qt::MoveAction) == Qt::MoveAction)
    {
        //delete takeItem(row(item));
    }

    qDebug()<<"startDrag";

}
//QApplication::startDragDistance()
const QRect QMyListWidget::targetRect(const QPoint &position) const
{
    //40��item���и�
       if(position.y() >= QApplication::startDragDistance())
           return QRect(0, (position.y() - QApplication::startDragDistance()) / 40 * 40, width(), 2 * 40);
       else
           return QRect(0, 0, width(), 40);
}
