
#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>
#include <QtWidgets>

class RenderArea : public QWidget
{
    Q_OBJECT

public:
    RenderArea(int width, int height, QLabel *warningLight, float temp = 1000, QWidget *parent = 0);

    QSize minimumSize() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;

    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    QPen pen;
    QWidget *graphInfo;
    QLabel *warning;

    std::vector <QPoint> points;

    int setHeight;
    int setWidth;

    float currentScale;
    int scaleDigits;
    float currentMax;
    float currentMin;
    int drawMax;
    int drawMin;

    float pMultiplier;

    //assuming 100 C in tenths of a degree based on the picture
    float dangerTemp;

    float shift;
    int index;

    void addPoint(float x, float y); //normal add point
    void addPoint(float y); //overload to allow for the use of the default x step
    int rescale(float y);
};

#endif // RENDERAREA_H
