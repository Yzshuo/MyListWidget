#ifndef QMYLISTWIDGET_H
#define QMYLISTWIDGET_H

#include <QObject>
#include<QListWidget>
#include<QtGui>
#include<QApplication>
#include<QMouseEvent>
#include<QDragEnterEvent>
#include<QDragLeaveEvent>
#include<QDragMoveEvent>
#include<QDropEvent>
#include<QDebug>

class QMyListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit QMyListWidget(QWidget *parent= nullptr);
//    ~QMyListWidget();

    bool isDraging() const {return IsDraging;}
    int offset() const {return 19;}
    int highlightedRow() const {return theHighlightedRow;}
    int dragRow() const {return theDragRow;}
    static QString myMimeType() { return QStringLiteral("TestListWidget/text-icon"); }

    void addPiece(QPixmap pixmap, QPoint location);
    void addPiece(QPoint location,QString name);
    static QString puzzleMimeType() { return QStringLiteral("QMyListWidget/text"); }

protected:
    //重写下面两个函数来完成从该部件中拖拽出去的操作
//    void mousePressEvent(QMouseEvent *event) override;
//    void mouseMoveEvent(QMouseEvent *event) override;
    //重写下面函数来完成接受外来拖拽操作
    void dragEnterEvent(QDragEnterEvent *event) override;
   void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void startDrag(Qt::DropActions supportedActions) override;

private:
    //记录拖拽的起点
    QPoint m_dragPoint;
    //记录被拖拽的项
    QListWidgetItem *m_dragItem;
private:
    bool IsDraging = false;
    QRect oldHighlightedRect;
    QRect theHighlightedRect;
    int theHighlightedRow = -1;
    int theDragRow = -1;

    const QRect targetRect(const QPoint &position) const;
};

#endif // QMYLISTWIDGET_H
