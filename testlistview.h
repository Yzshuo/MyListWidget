#ifndef TESTLISTVIEW_H
#define TESTLISTVIEW_H

#include <QObject>
#include<QListView>
#include<QMouseEvent>
#include<QDragEnterEvent>
#include<QDragLeaveEvent>
#include<QDragMoveEvent>
#include<QDropEvent>
#include<QDebug>
#include<QDrag>
#include<QMimeData>
#include<QtGui>
#include<QApplication>
#include"testlistviewitem.h"

class TestListView : public QListView
{
    Q_OBJECT
public:
    explicit TestListView(QWidget *parent=nullptr);
    bool isDraging() const {return IsDraging;}
       int offset() const {return 19;}
       int highlightedRow() const {return theHighlightedRow;}
       int dragRow() const {return theDragRow;}
       static QString myMimeType() { return QStringLiteral("TestListView/text-icon-icon_hover"); }
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    QPoint startPos;
    bool IsDraging = false;
    int theHighlightedRow = -2;
    int oldHighlightedRow = -2;
    int theDragRow = -1;
    int theInsertRow = -1;

};

#endif // TESTLISTVIEW_H
