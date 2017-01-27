#include "window.h"

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include <QtWidgets>

#include "renderarea.cpp"

const int IdRole = Qt::UserRole;

Window::Window()
{
    close=false;
    graph=true; 


    //Graphs and Warning Lights
    QWidget *scrollView = new QWidget;
    QVBoxLayout *render = new QVBoxLayout(scrollView);

    QWidget *warningLights = new QWidget;
    QGridLayout *warningLayout = new QGridLayout;

    int renderWidth = 500;
    int renderHeight = 100;

    float dangerTemp=1000;

    //This code will need to change drastically depending on the number of thermistors
    QLabel *blankSquare = new QLabel("");
    blankSquare->setStyleSheet("QLabel { background-color : grey;}");
    warningLayout->addWidget(blankSquare,0,0);

    QFont f("Arial",20,QFont::Bold);
    for(int i=1;i<=24; i++){

        std::ostringstream convert;
        convert<<i;

        QLabel *warning = new QLabel(convert.str().c_str());
        warning->setFont(f);
        warning->setAlignment(Qt::AlignCenter);
        warning->setStyleSheet("QLabel { color : black; background-color : green;}");
         warningLayout->addWidget(warning,i/5,i%5);

        std::string text = "Therm "+convert.str()+" Temp";
        QLabel *renderLabel = new QLabel(text.c_str());
        RenderArea *renderArea = new RenderArea(renderWidth, renderHeight, warning, dangerTemp);
        renderList.push_back(renderArea);
        render->addWidget(renderLabel);
        render->addWidget(renderArea);
    }

    warningLights->setLayout(warningLayout);


    //Scrolling
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidget(scrollView);
    scrollArea->setMaximumWidth(renderWidth+50);
    scrollArea->setMinimumWidth(renderWidth+50);


    //Buttons
    QLabel *sPortLabel = new QLabel("Serial Port:");

    QPushButton *sPortButton = new QPushButton("/dev/ttyACM0"); //This doesn't do anything yet
    connect(sPortButton, SIGNAL (released()), this, SLOT (sPortButtonTrigger()));

    QLabel *tempUnitLabel = new QLabel("Temperature Units: Tenths of a Degree Celsius");
    QLabel *themistorLabel = new QLabel("Thermistor numbers are 1-24, inclusive");
    
    startButton = new QPushButton("Start");
    connect(startButton, SIGNAL (released()), this, SLOT (startButtonTrigger()));
    startButton->setEnabled(false);

    stopButton = new QPushButton("Stop");
    connect(stopButton, SIGNAL (released()), this, SLOT (stopButtonTrigger()));

    QPushButton *saveButton = new QPushButton("save");
    connect(saveButton, SIGNAL (released()), this, SLOT (saveButtonTrigger()));


    //Overall Layout, The order these are called does matter
    QHBoxLayout *mainLayout = new QHBoxLayout;

    QVBoxLayout *rightButtons = new QVBoxLayout;
    QVBoxLayout *rightSide = new QVBoxLayout;

    rightButtons->setAlignment(Qt::AlignCenter);

    rightButtons->addWidget(sPortLabel);
    rightButtons->addWidget(sPortButton);
    rightButtons->addWidget(tempUnitLabel);
    rightButtons->addWidget(themistorLabel);
    rightButtons->addWidget(startButton);
    rightButtons->addWidget(stopButton);
    rightButtons->addWidget(saveButton);

    rightSide->addLayout(rightButtons);
    rightSide->addWidget(warningLights);

    mainLayout->addWidget(scrollArea);
    mainLayout->addLayout(rightSide);

    setLayout(mainLayout);

    setWindowTitle(tr("Satellite Monitor"));
}

void Window::sPortButtonTrigger(){
    std::cout<<"Serial Port still needs to be set up"<<std::endl;
}

void Window::startButtonTrigger(){
    graph=true;
    stopButton->setEnabled(true);
    startButton->setEnabled(false);
}

void Window::stopButtonTrigger(){
    graph=false;
    startButton->setEnabled(true);
    stopButton->setEnabled(false);
}

void Window::saveButtonTrigger(){
    QString QFileName = QFileDialog::getSaveFileName(this, tr("Save a CSV or txt file"), tr(""),tr("csv files (*.csv);; Text files (*.txt);"));
    std::string fileName=QFileName.toStdString();

    if(fileName.length()>0){
        if(fileName.length()>4){
            if(fileName.substr(fileName.length() - 4).compare(".csv")){
                fileName+=".csv";
            }
        }
        
        std::ofstream myfile;
        myfile.open(fileName.c_str());
        for(int i=0;i<renderList.size();i++){

            std::ostringstream convert;
            convert<<i;

            myfile<<"Thermistor "<<convert.str()<<" Time: ";
            
            for(int p=0;p<renderList[i]->points.size();p++){
                myfile<<renderList[i]->points[p].x();
                if(p<renderList[i]->points.size()-1){
                    myfile<<",";
                }
                else{
                    myfile<<"\n";
                }
            }

            myfile<<"Thermistor "<<convert.str()<<" Temp: ";

            for(int p=0;p<renderList[i]->points.size();p++){
                myfile<<renderList[i]->points[p].y();
                if(p<renderList[i]->points.size()-1){
                    myfile<<",";
                }
                else{
                    myfile<<"\n";
                }
            }
        }
        myfile.close();
    }
}

void Window::closeEvent(QCloseEvent *event){
    close=true;
    event->accept();
}

void Window::addPoint(int number, float x, float y){
    //number is the thermistor number, not the index number (starts at 1)
    if(number>=1 and number<=renderList.size()){
        renderList[number-1]->addPoint(x,y);
    }
}

void Window::addPoint(int number, float y){
    if(number>=1 and number<=renderList.size()){
        renderList[number-1]->addPoint(y);
    }
}