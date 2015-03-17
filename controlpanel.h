#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "diagram2_plus.h"
#include <iostream>
#include <string>
using namespace std;

class controlpanel
{
    public:
    unsigned char HOTKEY_NextDiagram;
    unsigned char HOTKEY_LastDiagram;
    unsigned char HOTKEY_NextColume;
    unsigned char HOTKEY_LastColume;
    unsigned char HOTKEY_IncreaseMaxvalue;
    unsigned char HOTKEY_DecreaseMaxvalue;
    diagram2_plus *SELECT_Diagram;
    void draw()
    {
        if(SELECT_Diagram!=NULL)
        {
            int boader = 40;
            int x = SELECT_Diagram->getx();
            int y = SELECT_Diagram->gety();
            int w = SELECT_Diagram->getwidth();
            int h = SELECT_Diagram->getheight();
            //cout<<"panel draw xywh="<<x<<","<<y<<","<<w<<","<<h<<endl;
            glColor3f(0,1,0); //R G B
            glBegin(GL_LINES);
            glVertex2d(x- boader ,y- boader);
            glVertex2d(x- boader,y+h+ boader);
            glVertex2d(x- boader,y- boader);
            glVertex2d(x+w+ boader,y- boader);
            glVertex2d(x+w+ boader,y+h+ boader);
            glVertex2d(x+w+ boader,y- boader);
            glVertex2d(x+w+ boader ,y+h+ boader);
            glVertex2d(x- boader,y+h+ boader);
            glEnd();
        }

    }
    void setindex(int _index)
    {
        index = _index;
    }
    int getindex()
    {
        return index;
    }
    controlpanel()
    {
        HOTKEY_NextDiagram = 'w' ;
        HOTKEY_LastDiagram = 's';
        HOTKEY_NextColume = 'd';
        HOTKEY_LastColume = 'a';
        HOTKEY_IncreaseMaxvalue = 'e';
        HOTKEY_DecreaseMaxvalue = 'q';
        index=0;
    }
    private:
    int index;



};

#endif
