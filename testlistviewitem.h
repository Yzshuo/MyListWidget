#ifndef TESTLISTVIEWITEM_H
#define TESTLISTVIEWITEM_H

#include <QObject>
#include<QStandardItem>

class TestListViewItem : public QStandardItem
{
public:
    explicit TestListViewItem();
    explicit TestListViewItem(const QIcon &icon, const QIcon &icon_hover, const QString &text);

     QIcon Img;
     QIcon Img_hover;
};

#endif // TESTLISTVIEWITEM_H
