#ifndef MYTABWIDGET_H
#define MYTABWIDGET_H
#include <QTabWidget>
#include <QObject>
#include <QColor>
#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QPaintEvent>

class myTabWidget : public QTabWidget
{
public:
    myTabWidget(QWidget *parent = nullptr);
private:
    void paintEvent(QPaintEvent *e) override;
};

#endif // MYTABWIDGET_H
