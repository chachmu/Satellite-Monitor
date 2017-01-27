
#include "renderarea.h"
#include <iostream>

#include <math.h>

#include <QPainter>


RenderArea::RenderArea(int width, int height, QLabel *warningLight, float temp, QWidget *parent) : QWidget(parent){
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);

    index=0;
    shift=0;

    setWidth=width;
    setHeight=height;

    dangerTemp=temp;
    warning = warningLight;

    addPoint(0,0); //DO NOT PUT THIS ABOVE warning = warnLight, it WILL segfault and you will probably be confused for 20 minutes

    currentScale=height/3;
    pMultiplier=height;
    scaleDigits=3;
    currentMax=0;
    currentMin=0;
    drawMax=0;
    drawMin=0;
}


QSize RenderArea::minimumSize() const
{
    return QSize(setWidth, setHeight);
}

QSize RenderArea::sizeHint() const
{
    return QSize(setWidth, setHeight);
}

void RenderArea::addPoint(float x,float y){
    if(y>=dangerTemp){
        warning->setStyleSheet("QLabel { background-color : red;}");
    }
    else{
        warning->setStyleSheet("QLabel { background-color : green;}");
    }

    //handle scaling and stuff
    points.push_back(QPoint(x,y));

    rescale(y);

    update();
}

void RenderArea::addPoint(float y){
    addPoint(points.back().x()+20,y);
}

void RenderArea::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(QPen(Qt::black));
    painter.setRenderHint(QPainter::Antialiasing);

    float i = drawMin;
    int fontHeight=12;
    int zero=fontHeight; //where the bottom of the graph "is"
    int top=setHeight-fontHeight; //where the top of the graph "is"
    float baseline = (drawMin*top / pMultiplier); //scaled zero location for nonzero min values

    float xVal = 12 + 7*scaleDigits;

    if(drawMin<0){
        //acount for width of negative sign
        xVal+=7;
    }

    //everything from here down is horrible but manages to account for character widths and other issues, if its not very broken its probably easier to just not mess with it

    painter.drawLine(QPoint(xVal,fontHeight-1),QPoint(xVal,setHeight - 2));
    while(round(i)<=round(drawMax)){
        std::ostringstream convert;
         convert<<(int) i;

        float yVal=top-(top*i/pMultiplier)+zero + baseline;
    
        painter.drawText(QPoint(0,yVal),QString(convert.str().c_str()));
        painter.drawLine(QPoint(xVal,yVal-1),QPoint(xVal+10,yVal-1));
        i+=currentScale;
    }

    QPainterPath path;
    if(points.size()>1){
        QPoint p1 = points[points.size()-1];
        QPoint p2 = points[points.size()-2];

        if(p1.x()+xVal-shift >= setWidth){
            //keep the rightmost value from going off the screen
            shift += p1.x()+xVal - shift -setWidth;
        }

        QPoint startPoint=QPoint(points[index].x()-shift,points[index].y());
        //stop indexing points that shift off the screen to the left
        while(startPoint.x()<0){
            index++;    
            startPoint=QPoint(points[index].x()-shift,points[index].y());
        }

        path.moveTo(QPoint(startPoint.x()+xVal,top-(top*startPoint.y()/pMultiplier)+zero + baseline));
        for(int i=index+1; i<points.size();i++){
            QPoint point = points[i];
            point = QPoint(point.x()-shift,point.y());
            path.lineTo(QPoint(point.x()+xVal,top-(top*point.y()/pMultiplier)+zero + baseline));
        }

        painter.drawPath(path);
    }
}

//Provides suprisingly effective smart scaling of the axis based on the input values
int RenderArea::rescale(float y){
    //this is really really ugly

    float max=0; //if max is not greater than 0 the graphs will be weird anyway, maybe solve later
    float min=0; //if min is not lower than 0 we should still graph starting at 0
    for(int i=0;i<points.size();i++){
        float cY=points[i].y();
        if(cY>currentMax and cY > max){
            max=cY;
        }
        if(cY<currentMin and cY<min){
            min=cY;
        }
    }

    int changeScale=0;

    //find if the max/min is greater/less than what we are currently using
    max=ceil(max);
    min=floor(min);

    if(max>currentMax){
        currentMax=max;

        int work= max;
        int digits = 0;
        while(work != 0){
            work/=10;
            digits++;
        }

        int mult=1;
        if(max<0){
            mult=-1;
        }

        if(scaleDigits>digits){
            digits=scaleDigits;
        }
        else{
            scaleDigits=digits;
        }

        int dmax, dmult = mult*pow(10,digits-1);
        for(int i=1;i<=10;i++){
            dmax=dmult*i;
            if(dmax>=max){
                if(i==10){
                    scaleDigits++;
                }
                break;
            }
        }
        if(dmax!=drawMax){
            drawMax=dmax;
            changeScale=1;
        }

    }
    else if(min<currentMin){
        currentMin=min;

        int work= min;
        int digits = 0;
        while(work != 0){
            work/=10;
            digits++;
        }


        int mult=1;
        if(min<0){
            mult=-1;
        }

        if(scaleDigits>digits){
            digits=scaleDigits;
        }
        else{
            scaleDigits=digits;
        }

        int dmin, dmult = mult*pow(10,digits-1);
        for(int i=1;i<=10;i++){
            dmin=dmult*i;
            if(dmin<=min){
                if(i==10){
                    scaleDigits++;
                }
                break;
            }
        }
        if(dmin!=drawMin){
            drawMin=dmin;
            changeScale=-1;
        }
    }

    if(changeScale!=0){
        pMultiplier=abs(drawMax-drawMin);
        currentScale=pMultiplier/3;
    }
}
