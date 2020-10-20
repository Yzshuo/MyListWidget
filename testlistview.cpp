#include "testlistview.h"

TestListView::TestListView(QWidget *parent):
    QListView (parent)
{
    setAcceptDrops(true);
}
//��¼��ק��ʼλ��
void TestListView::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton){
        startPos = event->pos();
    }
}
void TestListView::mouseReleaseEvent(QMouseEvent *event)
{
    if((event->pos() - startPos).manhattanLength() > 5) return;

    QModelIndex index = indexAt(event->pos());
    setCurrentIndex(index);  //���relesseʱ��ѡ��
}
void TestListView::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        if((event->pos() - startPos).manhattanLength() < QApplication::startDragDistance()) return;

        QModelIndex theDragIndex = indexAt(startPos);
        theDragRow = theDragIndex.row();

        setCurrentIndex(theDragIndex);  //��ק��ѡ��

        QStandardItemModel *listModel = qobject_cast<QStandardItemModel *>(model());
        TestListViewItem *theDragItem = static_cast<TestListViewItem *>(listModel->item(theDragRow));

//[1]����ק���ݷ���QMimeData������
        QString text = theDragItem->text();
        QIcon icon = theDragItem->Img;
        QIcon icon_hover = theDragItem->Img_hover;
        QByteArray itemData;
        QDataStream dataStream(&itemData, QIODevice::WriteOnly);
        dataStream << text << icon << icon_hover;

        QMimeData *mimeData = new QMimeData;
        mimeData->setData(myMimeType(), itemData);
//[1]
        //[2]������קʱ������ͼ
        /*
                thumbnail *DragImage = new thumbnail(this);
                DragImage->setupthumbnail(icon_hover, text);
                //DragImage->setIconSize(18);  //default:20
                QPixmap pixmap = DragImage->grab();

                QDrag *drag = new QDrag(this);
                drag->setMimeData(mimeData);
                drag->setPixmap(pixmap);
                drag->setHotSpot(QPoint(pixmap.width() / 2, pixmap.height() / 2));
                */
        //[2]
        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        //ɾ��������Ҫ����theInsertRow��theDragRow�Ĵ�С��ϵ���ж�(���Ҳ���Ҹ���ʵ��������Է��ֵ�)
        if(drag->exec(Qt::MoveAction) == Qt::MoveAction)
        {
            int theRemoveRow = -1;
            if(theInsertRow < theDragRow) theRemoveRow = theDragRow + 1;
            else theRemoveRow = theDragRow;
            model()->removeRow(theRemoveRow);
        }
    }
}
void TestListView::dragEnterEvent(QDragEnterEvent *event)
{
    TestListView *source = qobject_cast<TestListView *>(event->source());
    if (source && source == this) {
        //IsDraging(��־λ)�ж��Ƿ�������ק
        IsDraging = true;
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}
void TestListView::dragLeaveEvent(QDragLeaveEvent *event)
{
    oldHighlightedRow = theHighlightedRow;
    theHighlightedRow = -2;

    //֮ǰQListWidget�õ���update(QRect)�������õ���update(QModelIndex)����Ȼ����Ҳ����ʹ��update(QRect)��ֻ���뻻һ�ַ�������
    update(model()->index(oldHighlightedRow, 0));  //UpRow
    update(model()->index(oldHighlightedRow + 1, 0));  //DownRow

    IsDraging = false;  //IsDraging(��־λ)�ж��Ƿ�������ק

    theInsertRow = -1;
    event->accept();
}
void TestListView::dragMoveEvent(QDragMoveEvent *event)
{
    TestListView *source = qobject_cast<TestListView *>(event->source());
    if (source && source == this) {

        oldHighlightedRow = theHighlightedRow;
        theHighlightedRow = indexAt(event->pos() - QPoint(0, offset())).row();

        //offset() = 19 = 40 / 2 - 1������40���и�
        if(event->pos().y() >= offset()){

            if(oldHighlightedRow != theHighlightedRow){
                //ˢ�¾�����ʹdropIndicator��ʧ
                update(model()->index(oldHighlightedRow, 0));
                update(model()->index(oldHighlightedRow + 1, 0));

                //ˢ��������ʹdropIndicator��ʾ
                update(model()->index(theHighlightedRow, 0));
                update(model()->index(theHighlightedRow + 1, 0));
            }else{
                update(model()->index(theHighlightedRow, 0));
                update(model()->index(theHighlightedRow + 1, 0));
            }

            theInsertRow = theHighlightedRow + 1;
        }else{
            theHighlightedRow = -1;
            update(model()->index(0, 0));
            update(model()->index(1, 0));
            theInsertRow = 0;
        }

        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}
void TestListView::dropEvent(QDropEvent *event)
{
    TestListView *source = qobject_cast<TestListView *>(event->source());
    if (source && source == this){

        IsDraging = false;  //IsDraging(��־λ)�ж��Ƿ�������ק

        oldHighlightedRow = theHighlightedRow;
        theHighlightedRow = -2;

        //ˢ�¾�����ʹdropIndicator��ʧ
        update(model()->index(oldHighlightedRow, 0));
        update(model()->index(oldHighlightedRow + 1, 0));


        if(theInsertRow == theDragRow || theInsertRow == theDragRow + 1) return;

        //��������QListWidget�������ø���dropEvent(event)���ֲ�������(ԭ���в���)��û�취��ֻ��ɾ�����У���������
        //if(theSelectedRow == theDragRow){
            //QListView::dropEvent(event);
            //return;
        //}

        QString text;
        QIcon icon, icon_hover;
        QByteArray itemData = event->mimeData()->data(myMimeType());
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);
        dataStream >> text >> icon >> icon_hover;

        model()->insertRow(theInsertRow);
        QStandardItemModel *listModel = qobject_cast<QStandardItemModel *>(model());
        listModel->setItem(theInsertRow, 0, new TestListViewItem(icon, icon_hover, text));

        setCurrentIndex(model()->index(theInsertRow, 0));  //�����б���ѡ��״̬

        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}
