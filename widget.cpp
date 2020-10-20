#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    QStringList list;
    list<<"A"<<"B"<<"C"<<"D"<<"E";
    ui->listWidget->addItems(list);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_listWidget_itemClicked(QListWidgetItem *item)
{
    qDebug()<<item->text()<<ui->listWidget->count();
    ui->listWidget->count();
}
/*
void Widget::on_comboBox_currentIndexChanged(int index)
{
    switch (index)
    {
    case 0:
        qDebug()<< ui->comboBox->currentText()<<"0";
        break;
    case 1:

       qDebug()<< ui->comboBox->currentText()<<"1";
        break;
    case 2:
        qDebug()<< ui->comboBox->currentText()<<"2";
        break;
    case 3:
        qDebug()<< ui->comboBox->currentText()<<"3";
        break;
    default:
        break;

    }
    ui->comboBox->setCurrentIndex(0);
}
*/
void Widget::on_comboBox_currentTextChanged(const QString &arg1)
{
    qDebug()<< ui->comboBox->currentText();
    ui->comboBox->setCurrentIndex(0);
}
