#include "window.h"
#include <stdlib.h> //this is just for random numbers
#include <iostream>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Window window;
    window.show();

    time_t timer;
    time(&timer);
    int start = timer;
    int old_time = 0;

    while(!window.close){
    	time(&timer);//timer is unneccessary, I just needed test cases that didn't go by at the speed of light
    	if(timer-start!= old_time and window.graph){
    		old_time=timer-start;
    		int num = rand() % 1000;
            window.addPoint(1,-num); //this gives graph 1 have a negative min value which is nice for testing
            for(int i=1;i<=24;i++){
    		  window.addPoint(i,num);
            }
    	}

    	app.processEvents();
    }

    return 0;
}
