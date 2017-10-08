#include "Thermometer.h"

Thermometer::Thermometer(ST7565 *lcd, Item *back, DigitalOut *gyro, DigitalOut *thermo){
    this->title = " Thermometer";
    this->st7565 = lcd;
    this->back = back;
    this->gyro = gyro;
    this->thermo = thermo;
    tt = NULL;
}

void Thermometer::display(void){
    if(tt != NULL) return;
    gyro->write(0); thermo->write(1);
    tt = new Thread(&Thermometer::tt_start, this);
    tt->signal_set(START_THREAD);    
}

void Thermometer::tt_start(const void * args){
    Thermometer *t = (Thermometer*)args;
    t->display_temperature();
}

void Thermometer::display_temperature(){
    tt->signal_wait(START_THREAD);
    
    float temp;
    char temp_char[50];
    while(1){
        temp = get_temperature();
        //get temperature and display it
        sprintf(temp_char, "Temperature is: %0.2f", temp);
        st7565->drawstring(5, 3, temp_char);
        st7565->display();
        Thread::wait(15);
        st7565->clear();
    }
}

void Thermometer::update(char c){
    if(c == 'y'){
        tt->terminate();
        free(tt); tt = NULL;
        thermo->write(0);
        st7565->clear();
        this->setSelectedScreen(back);
    }
}


        