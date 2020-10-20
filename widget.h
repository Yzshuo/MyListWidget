#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include"qmylistwidget.h"
#include<QDebug>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);

//    void on_comboBox_currentIndexChanged(int index);

    void on_comboBox_currentTextChanged(const QString &arg1);

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
