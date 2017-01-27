#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QtWidgets>

#include <string>
#include <iostream>

#include "renderarea.h"

class QCheckBox;
class QComboBox;
class QLabel;
class QSpinBox;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

    QPushButton *startButton;
    QPushButton *stopButton;

    void closeEvent(QCloseEvent *event);

    void addPoint(int number, float x, float y);
    void addPoint(int number, float y); //use default x step

    std::vector <RenderArea*> renderList;

    bool isOpen(){return true;}

    bool close;

    bool graph;

private slots:
	void sPortButtonTrigger();
	void startButtonTrigger();
	void stopButtonTrigger();
    void saveButtonTrigger();
};

#endif // WINDOW_H
