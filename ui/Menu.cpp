#include "Menu.h"

void Menu::display(void){
    //display items
    display_items(0); //display items starting from index 0
}

void Menu::update(char c){
    //case down
    if(c == DOWN){
        //if last item already selected, do nothing
        if(selected + 1 == size)
            return; //do nothing
            
        //if we are at the bottom of the screen
        if(current_line % LAST_ITEM_LINE == 0){
            // more items left => clear screen and add items
            selected++;
            //display items from the next selected item
            current_line = FIRST_ITEM_LINE; //current selected line will be top of screen
            //printf("got down, selected is: %d; current line is: %d\n", selected, current_line);
        }
        //it is not last on the screen
        else{
            //clear the screen
            current_line++;
            selected++;
            //printf("selected is: %d; current line is: %d\n", selected, current_line);
        }
        st7565->clear();
        display_items(selected/ITEMS_PER_SCREEN * ITEMS_PER_SCREEN);
        return;
    }
    //case up
    else if(c == UP){
        //we are at top of items
        if(selected == 0)
            return; //do nothing
        
        //if we are at the top of the screen => there are more items to show
        if(current_line == FIRST_ITEM_LINE){
            selected--;
            //display items from the next selected item
            current_line = LAST_ITEM_LINE; //current selected line will be top of screen
        }
        //it is not first on the screen
        else{
            current_line--;
            selected--;
        }
        st7565->clear();
        display_items(selected/ITEMS_PER_SCREEN * ITEMS_PER_SCREEN);
        return;
    }
    else if(c == NL && items[selected]->isSelectable){
        //set selected screen so that UI knows which screen we selected to display
        this->setSelectedScreen(items[selected]);
    }
}

void Menu::addItem(Item * i){
    //if there are many items, ignore
    if(size == MAX_ITEMS)
        return;
    //add pointer to item
    items[size] = i;
    size++;
    isSelectable = true;
}

/*
    Function that displays items on a new screen starting from an index.
*/
void Menu::display_items(int index){
    //draw title of menu
    //display 1 or more items on the screen
    int i = FIRST_ITEM_LINE, j;
    //draw the first items
    for(j = index; j < size && i <= LAST_ITEM_LINE; j++)
        st7565->drawstring(LEFT_MARGIN * 2, i++, items[j]->getTitle());
    
    //up arrow
    if(index != 0){
        st7565->drawline(119, 15, 125, 15,DEFAULT_COLOR);
        st7565->drawline(119, 15, 122, 12,DEFAULT_COLOR);
        st7565->drawline(125, 15, 122, 12,DEFAULT_COLOR);
    }
    
    //down arrow
    if(j < size){
        st7565->drawline(119, 60, 125, 60,DEFAULT_COLOR);
        st7565->drawline(119, 60, 122, 63,DEFAULT_COLOR);
        st7565->drawline(125, 60, 122, 63,DEFAULT_COLOR);
    }
    
    //set selected as first
    st7565->drawcircle(2, (current_line * 8) + 3, 2, 20);
    st7565->display();
}