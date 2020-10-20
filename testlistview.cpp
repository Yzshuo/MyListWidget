#include "testlistview.h"

TestListView::TestListView(QWidget *parent):
    QListView (parent)
{
    setAcceptDrops(true);
}
//记录拖拽初始位置
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
    setCurrentIndex(index);  //鼠标relesse时才选中
}
void TestListView::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        if((event->pos() - startPos).manhattanLength() < QApplication::startDragDistance()) return;

        QModelIndex theDragIndex = indexAt(startPos);
        theDragRow = theDragIndex.row();

        setCurrentIndex(theDragIndex);  //拖拽即选中

        QStandardItemModel *listModel = qobject_cast<QStandardItemModel *>(model());
        TestListViewItem *theDragItem = static_cast<TestListViewItem *>(listModel->item(theDragRow));

//[1]把拖拽数据放在QMimeData容器中
        QString text = theDragItem->text();
        QIcon icon = theDragItem->Img;
        QIcon icon_hover = theDragItem->Img_hover;
        QByteArray itemData;
        QDataStream dataStream(&itemData, QIODevice::WriteOnly);
        dataStream << text << icon << icon_hover;

        QMimeData *mimeData = new QMimeData;
        mimeData->setData(myMimeType(), itemData);
//[1]
        //[2]设置拖拽时的缩略图
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
        //删除的行需要根据theInsertRow和theDragRow的大小关系来判断(这个也是我根据实际情况测试发现的)
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
        //IsDraging(标志位)判断是否正在拖拽
        IsDraging = true;
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}
void TestListView::dragLeaveEvent(QDragLeaveEvent *event)
{
    oldHighlightedRow = theHighlightedRow;
    theHighlightedRow = -2;

    //之前QListWidget用的是update(QRect)，这里用的是update(QModelIndex)，当然这里也可以使用update(QRect)，只是想换一种方法而已
    update(model()->index(oldHighlightedRow, 0));  //UpRow
    update(model()->index(oldHighlightedRow + 1, 0));  //DownRow

    IsDraging = false;  //IsDraging(标志位)判断是否正在拖拽

    theInsertRow = -1;
    event->accept();
}
void TestListView::dragMoveEvent(QDragMoveEvent *event)
{
    TestListView *source = qobject_cast<TestListView *>(event->source());
    if (source && source == this) {

        oldHighlightedRow = theHighlightedRow;
        theHighlightedRow = indexAt(event->pos() - QPoint(0, offset())).row();

        //offset() = 19 = 40 / 2 - 1，其中40是行高
        if(event->pos().y() >= offset()){

            if(oldHighlightedRow != theHighlightedRow){
                //刷新旧区域使dropIndicator消失
                update(model()->index(oldHighlightedRow, 0));
                update(model()->index(oldHighlightedRow + 1, 0));

                //刷新新区域使dropIndicator显示
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

        IsDraging = false;  //IsDraging(标志位)判断是否正在拖拽

        oldHighlightedRow = theHighlightedRow;
        theHighlightedRow = -2;

        //刷新旧区域使dropIndicator消失
        update(model()->index(oldHighlightedRow, 0));
        update(model()->index(oldHighlightedRow + 1, 0));


        if(theInsertRow == theDragRow || theInsertRow == theDragRow + 1) return;

        //这里我像QListWidget那样调用父类dropEvent(event)发现不起作用(原因尚不明)，没办法，只能删除旧行，插入新行
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

        setCurrentIndex(model()->index(theInsertRow, 0));  //插入行保持选中状态

        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}
