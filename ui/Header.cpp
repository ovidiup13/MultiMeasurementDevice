#include "Header.h"

Header::Header(char * t, ST7565 * lcd){
    //battery_level = lvl;
    title = t;
    st7565 = lcd;
}

void Header::setTitle(char *t){
    title = t;
}

void Header::display(void){    
    //create strings
    //char temp[7];
    //char level[5];
    //char sign = ' ';
    //set sign
    //if(temp > 0) sign = 43;
    //else if(temp < 0) sign = 45;
    
    //strcat(temp, sign);
    
    //convert to char array
    //sprintf(temp, " %c%.1f%c", sign, degrees, scale);
    //sprintf(level, "%d%%", battery_level);
    
    //add to header
    st7565->drawstring(LEFT_MARGIN*5, 0, title);
    //st7565->drawstring(LCDWIDTH - LEFT_MARGIN*4, 0, level);
    
    st7565->display();
}