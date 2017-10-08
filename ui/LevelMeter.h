#include "Item.h"
#include "mbed.h"

//define coordinates
#define X0 63 //center
#define Y0 31 //center
#define POINTER_LENGTH 10
#define RADIUS_lvl 3

class LevelMeter: public Item {
    public:
        //constructors
        LevelMeter(ST7565 *lcd, Item * back, DigitalOut *gyro, DigitalOut *thermo);
        
        //inherited functions
        virtual void display(void);
        virtual void update(char c);
        
        private:
            Thread *lt;
            
            DigitalOut *gyro, *thermo;
            void draw_elements(double rx, double ry);
            static void lt_start(void const *args);
            void update_cross(void);
        
};