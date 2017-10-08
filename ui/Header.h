#ifndef _HEADER_H_
#define _HEADER_H_

#include <stdio.h>
#include <stdlib.h>
#include "st7565LCD.h"

#define LEFT_MARGIN 5

using namespace std;

class Header{
    public:
        //functions
        //void setBatteryLevel(float d);
        void display(void);
        void setTitle(char *t);
        Header(char * t, ST7565 * lcd);
        
        private:
            //variables
            //uint8_t battery_level;
            char * title;
            ST7565 * st7565;
};

#endif