#include "Item.h"

#define SELECTION_LINE 6

class Measure: public Item {
    public:
        //constructor for thermometer
        Measure(char * title, ST7565 *lcd, Item *back, DigitalOut *laser){
            this->title = title;
            this->st7565 = lcd;
            this->back = back;
            backName = " Back";
            hasResult = false;
            selected = 1;
            current_line = 6;
            isOn = false;
            this->laser = laser;
        };
        
        //Measure(char *title, ST7565, *lcd, Item *back, DigitalOut *laser);
        //Measure(char * title, ST7565 *lcd, Item *back);
        //constructor for thermometer
        //Measure(char *title, ST7565 *lcd, Item *back, DigitalOut *gyro, DigitalOut *thermo);
        Measure();
        
        //inherited functions
        virtual void display();
        virtual void update(char c);
        
        //set next screen
        void setNext(char * t, Item *item){
            this->nextName = t;
            this->next = item;
        }
        
        //set description of screen
        void setDescription(char * description){
            this->description = description;
        }
        
        //function to set screen as result screen
        void setResult(bool r){
            hasResult = r;
        }
        
        private:
            //digital outputs for controlling stuff
            DigitalOut *laser;
            
            //menu items - only 2 -> start and back
            Item **items[2];
            Item *next; //screen to go next
            
            
            bool hasResult, isOn;
        
            int selected, current_line;
            char * description, *nextName, *backName;
            void display_description(char *r);
            void set_selected(int s);
            void display_result(double result);
            void display_items(void);
};