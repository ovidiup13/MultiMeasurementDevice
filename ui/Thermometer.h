#include "Item.h"


class Thermometer: public Item {
    public:
        Thermometer(ST7565 *lcd, Item *back, DigitalOut *gyro, DigitalOut *thermo);
        
        //inherited functions
        virtual void display(void);
        virtual void update(char c);
        
    private:
        Thread *tt;
    
        DigitalOut *gyro, *thermo;
        char * title;
        static void tt_start(const void *args);
        void display_temperature();
};