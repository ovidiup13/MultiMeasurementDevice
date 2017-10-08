#ifndef _COMPASS_H
#define _COMPASS_H

#include "Item.h"
#include <stdio.h>

//math stuff
#define _USE_MATH_DEFINES
#include <math.h>
#define M_PI 3.14159265358979323846  /* pi */

//define center coordinates
#define X_CENTER 95
#define Y_CENTER 35
#define POINTER_LENGTH 10
#define RADIUS 19

class Compass: public Item {
    public:
        //inherited functions
        virtual void display(void);
        virtual void update(char c);
        
        //constructor
        Compass(ST7565 *lcd, Item *back, DigitalOut *gyro, DigitalOut *thermo);
        
    private:
        Thread *ct; //compass update thread
        
        //helper functions
        DigitalOut *gyro, *thermo;
        void draw_compass(double degrees);
        static void ct_start(void const *args);
        void compass_update(void);
};

#endif