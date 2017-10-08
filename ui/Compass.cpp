#include "Compass.h"

//constructor
Compass::Compass(ST7565 * lcd, Item * back, DigitalOut *gyro, DigitalOut *thermo)
{
    this->title = " Compass";
    this->st7565= lcd;
    this->back = back;
    this->gyro = gyro;
    this->thermo = thermo;
    
    ct = NULL;
}

//display function, starts the thread and displays the compass
void Compass::display(void)
{
    //create a new thread to get and update compass - do later
    if(ct != NULL) return; //thread is already running
    thermo->write(0); gyro->write(1);
    ct = new Thread(&Compass::ct_start, this);
    ct->signal_set(START_THREAD);
}

//trigger for starting the thread
void Compass::ct_start(void const *args)
{
    Compass *c = (Compass*)args;
    c->compass_update();
}

//function that the thread runs, waits for the signal before starting
void Compass::compass_update()
{
    ct->signal_wait(START_THREAD);
    initSensors();
    
    SensorState_t state;
    //get degrees from other functions and display the compass
    while(true) {
        #ifdef LSM303_on
            LSM303(&state);
        #endif
        draw_compass(state.heading - 33); //add offset
        Thread::wait(30);
        st7565->clear();
    }
}

//update function handles updates from the user - cancels the thread
//and returns to main menu
void Compass::update(char c)
{
    //kill thread and go back
    if(c == 'y') {
        ct->terminate();
        free(ct); ct = NULL;
        gyro->write(0);
        st7565->clear();//clear everything
        this->setSelectedScreen(back);
    } else
        return;
}

//get direction
char * get_direction(double degrees)
{
    if(degrees >= 330 || degrees < 30)
        return "East";
    else if(degrees >= 30 && degrees <= 60)
        return "North-East";
    else if(degrees >= 60 && degrees < 120)
        return "North";
    else if(degrees >= 120 && degrees < 150)
        return "North-West";
    else if(degrees >= 150 && degrees < 210)
        return "West";
    else if(degrees >= 210 && degrees < 240)
        return "South-West";
    else if(degrees >= 240 && degrees < 300)
        return "South";
    else
        return "South-East";
}

//function that draws the compass on the screen
void Compass::draw_compass(double degrees)
{
    //variables
    int x_temp, y_temp;
    double rad = (degrees + 90) * M_PI / 180; //radians

    //calculate coordinates to point
    x_temp = X_CENTER + (int) (POINTER_LENGTH * cos(rad));
    y_temp = Y_CENTER + (int) (POINTER_LENGTH * (-sin(rad)));

    //draw the main circle and small one
    st7565->drawcircle(X_CENTER, Y_CENTER, RADIUS, 20);
    st7565->drawcircle(X_CENTER, Y_CENTER, 2, 20);

    //draw the lines
    st7565->drawline(X_CENTER, Y_CENTER, x_temp, y_temp, 20); //draw line from center to coordinates
    st7565->drawline(X_CENTER, Y_CENTER - RADIUS, X_CENTER, Y_CENTER - 15, 20); //north line
    st7565->drawline(X_CENTER, Y_CENTER + RADIUS, X_CENTER, Y_CENTER + 15, 20); //south line
    st7565->drawline(X_CENTER + RADIUS, Y_CENTER, X_CENTER + 15, Y_CENTER, 20); //east line
    st7565->drawline(X_CENTER - RADIUS, Y_CENTER, X_CENTER - 15, Y_CENTER, 20); //west line

    //draw the initials
    st7565->drawstring(X_CENTER - 2, 1, "N");
    st7565->drawstring(X_CENTER - 2, 7, "S");
    st7565->drawstring(X_CENTER + 21, 4, "E");
    st7565->drawstring(X_CENTER - 25, 4, "W");

    //display pointing direction
    st7565->drawstring(0, 2, "Pointing:");
    char * pointer = get_direction(degrees + 90);
    st7565->drawstring(0, 4, pointer);

    //display degrees and radians in bottom left corner
    char s_deg[10], s_rad[10];
    sprintf(s_deg, "DEG:%d", (int)degrees % 360);
    sprintf(s_rad, "RAD:%0.2f", (int)degrees % 360 * M_PI / 180.0);
    st7565->drawstring(1, 6, s_deg);
    st7565->drawstring(1, 7, s_rad);

    st7565->display();
}