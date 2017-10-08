#include "LevelMeter.h"

LevelMeter::LevelMeter(ST7565 *lcd, Item * back, DigitalOut *gyro, DigitalOut *thermo)
{
    this->st7565 = lcd;
    this->title = " Level meter";
    this->back = back;
    this->gyro = gyro;
    this->thermo = thermo;
    lt = NULL;
}

void LevelMeter::display(void)
{
    //create a new thread to get and update compass - do later
    if(lt != NULL) return;//thread is already running
    thermo->write(0); gyro->write(1);
    lt = new Thread(&LevelMeter::lt_start, this);
    lt->signal_set(START_THREAD);
}

//trigger for starting the thread
void LevelMeter::lt_start(void const *args)
{
    LevelMeter *l = (LevelMeter*)args;
    l->update_cross();
}

void LevelMeter::update_cross()
{
    lt->signal_wait(START_THREAD); //wait for signal to start thread
    initSensors();
    Result_avrg result;
    //draw the static circle
    //int i = 0, j = 0;
    while(true) {
        calc_avrg_or(&result,2);
        draw_elements(result.x, result.y);
        printf("%0.0f, %0.0f\n", result.x, result.y);
        Thread::wait(15);
        st7565->clear();
    }
}

void LevelMeter::draw_elements(double rx, double ry){
    int cross_x, cross_y;
    
    //draw the circles
    st7565->drawcircle(X0, Y0, RADIUS_lvl, 1);

    //draw middle cross
    st7565->drawline(X0 - 2, Y0, X0 + 2, Y0, 20);
    st7565->drawline(X0, Y0 - 2, X0, Y0 + 2, 20);
    
    //check if rotation in negative interval
    if(rx >= 270) rx -= 360;
    if(ry >= 270) ry -= 360;
    
    //calculate coordinates
    cross_x = (int) ((2*rx * SCREEN_WIDTH)/360.0) + X0;
    cross_y = (int) ((2*ry * SCREEN_HEIGHT)/360.0) + Y0;
    
    //printf("Coordinates for cross are: %d, %d\n", cross_x, cross_y);
    
    //draw the cross
    st7565->drawline(cross_x, cross_y - RADIUS_lvl*3, cross_x, cross_y + RADIUS_lvl*3, DEFAULT_COLOR);
    st7565->drawline(cross_x - RADIUS_lvl*3, cross_y, cross_x + RADIUS_lvl*3, cross_y, DEFAULT_COLOR);
    
    //draw results
    char *rsx = (char*)malloc(sizeof(char)*7);
    char *rsy = (char*)malloc(sizeof(char)*7);
    sprintf(rsx, "X: %0.0f", rx);
    sprintf(rsy, "Y: %0.0f", ry);
    st7565->drawstring(3, 7, rsx);
    st7565->drawstring(SCREEN_WIDTH - 38, 7, rsy);
    
    //display
    st7565->display();
    //free(rsx);
    //free(rsy);
}

void LevelMeter::update(char c)
{
    if(c == 'y') {
        lt->terminate();
        free(lt); lt = NULL;
        gyro->write(0);
        st7565->clear();
        this->setSelectedScreen(back);
    }
}