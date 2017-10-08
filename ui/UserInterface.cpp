#include "UserInterface.h"

/*
//colors
PwmOut red(p21);
PwmOut green(p22);
PwmOut blue(p23);
PwmOut a(p24);

void UI::set_colors(float r, float g, float b, float aa){ //red, green, blue, a+
    //check that colors are between 0.0 and 1.0
    assert(r >=0 && r <= 1);
    assert(g >=0 && g <= 1);
    assert(b >=0 && b <= 1);
    assert(aa >=0 && aa <= 1);
    
    //define colors
    red = r;
    green = g;
    blue = b;
    a = aa;
}
*/

void UI::init(void){
    //initialize variables
    int brightness = _DEFAULT_BRIGHTNESS;
    
    //start LCD and display logo
    st7565->begin(_DEFAULT_CONTRAST);
    st7565->st7565_set_brightness(brightness);
    //set colors
    //set_colors(0, 1, 1, 1);
    //display logo
    st7565->display();
    wait(2.0);
    
    header->setTitle(current->getTitle());
    display();
}

void UI::display(void){
    st7565->clear();
    current->display();
    header->display();
}

void UI::update(char c){
    current->update(c);
    //set header after update
    if(c == 'y'){
        current = current->getSelectedScreen();
        header->setTitle(current->getTitle());
    }
    this->display();
}