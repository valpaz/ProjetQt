#include "mytabwidget.h"
#include <QPainter>
#include <QPaintEvent>
myTabWidget::myTabWidget(QWidget *parent):QTabWidget(parent)
{

}

void myTabWidget::paintEvent(QPaintEvent *e)
{
    QTabWidget::paintEvent(e);
    QPainter painter(this);


    QColor black(Qt::black);
    QPen pen(black);
    pen.setWidth(3);
    painter.setPen(pen);

    painter.drawLine(2, 2, 30, 30);
    painter.drawLine(2, 2, 10, 30);
    painter.end();
}
