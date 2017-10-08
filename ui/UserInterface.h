#ifndef _UI_H_
#define _UI_H_

//include files
#include "st7565LCD.h"
#include "Header.h"
#include "Item.h"
#include "Menu.h"
#include "Compass.h"
#include "LevelMeter.h"
#include "Measure.h"
#include "Thermometer.h"

//include libs
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

//define brightness and contrast
#define _DEFAULT_BRIGHTNESS 25
#define _DEFAULT_CONTRAST 20
#define _MAX_BRIGHTNESS 200
#define _MIN_BRIGHTNESS 10

//define default color
#define _DEFAULT_COLOR 20

//define mbed pins
#define _MOSI p5
#define _SCLK p7
#define _RST p24
#define _A0 p8
#define _CS p6

using namespace std;

class Item;

class UI {
    public:
        
    //functions
    void init(void);
    //update all screen
    void update(char c);
    //update header only
    void display(void);
    //update current menu
    void setCurrent(Item * item){
        current = item;
    }
    //set header
    void setHeader(Header * h){
        header = h;
    }
    //set header title
    void setHeaderTitle(char * title){
        header->setTitle(title);
    }
    
    UI(ST7565 *lcd){
        current = NULL;
        header = NULL;
        st7565 = lcd;
    }
    
    private:
        //variables
        //current selected menu
        Item * current;
        //header object
        Header * header;
        //display pointer
        ST7565 * st7565;
        //set colors
        void set_colors(float r, float g, float b, float aa);
};

#endif