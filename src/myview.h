#ifndef MYVIEW_H
#define MYVIEW_H

#include <QWidget>
#include <QGraphicsView>


class myView : public QGraphicsView
{

public:
    explicit myView(QWidget *parent = 0);

signals:

public slots:

    void wheelEvent(QWheelEvent *event);
    void setScale();
    void setNewScale(double scale){myScale = scale;}
    double getScale(){return myScale;}

private slots:

private:
    double myScale;
};

#endif // MYVIEW_H
