#ifndef _MENU_H_
#define _MENU_H_

#include "Item.h"

int const MAX_ITEMS = 16;
int const TITLE_LINE = 1;
int const FIRST_ITEM_LINE = 2;
int const LAST_ITEM_LINE = 7;
int const ITEMS_PER_SCREEN = 6;

class Menu: public Item {
    public:
        Item ** items;
        int size, selected, current_line;
        bool isScreen;
        
        //menu constructor
        Menu(char * t, ST7565 *lcd){
            title = t;
            st7565 = lcd;
            items = new Item*[MAX_ITEMS];
            selected = size = 0;
            current_line = FIRST_ITEM_LINE;
            isScreen = false;
            isSelectable = false;
        }
        
        //menu item constructor - go to a screen
        Menu(char * t, Item *screen){
            title = t;
            st7565 = NULL;
            items = new Item*[1];
            items[0] = screen;
            selected = 0;
            size = 1;
            current_line = FIRST_ITEM_LINE;
            isScreen = true;
            isSelectable = true;
        }
        
        //display the menu
        virtual void display(void);
        
        //update current selection
        virtual void update(char c);
        
        //set the title
        void setTitle(char * t){
            title = t;
        }
        
         //highlights the current selection
        void addItem(Item * i);
        
        private:
             //display items on a new screen starting with a specific index
             void display_items(int index);
};

#endif

