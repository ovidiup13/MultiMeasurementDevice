#ifndef _ITEM_H
#define _ITEM_H

#include "st7565LCD.h"
#include "rtos.h"
#include "gyro.h"
#include "mbed.h"

//screen configuration
#define LEFT_MARGIN 5
#define DEFAULT_COLOR 20

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

//control macros
#define NL    121  //select - y
#define UP    119  //up - w
#define DOWN  115  //down - s

//threading macros
#define START_THREAD 1
#define PAUSE_THREAD 2

class Item {
    public:
        //name
        char * title;
        ST7565 * st7565;
        bool isSelectable;
        Item *selectedScreen, *back;
        
        //declare pure virtual functions
        virtual void display(void) = 0;
        virtual void update(char c) = 0;
        
        //get title function
        char * getTitle(void){
            return title;
        }
        
        Item * getSelectedScreen(){
            return selectedScreen;
        }
        
        void setSelectedScreen(Item *s){
            selectedScreen = s;
        }
};

#endif