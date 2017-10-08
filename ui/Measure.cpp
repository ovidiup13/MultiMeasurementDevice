#include "Measure.h"

void Measure::display(void){
    this->display_items();
    laser->write(1);
}

void Measure::update(char c){
    //select down
    if(c == 's'){
        if(selected == 2) return; //do nothing
        selected = 2;
        current_line = 7;
    }
    //select up
    else if(c == 'w'){
        if(selected == 1) return; //do nothing
        selected = 1;
        current_line = 6;
    }
    //go to next screen
    else if(c == 'y'){
        if(selected == 1)
            this->setSelectedScreen(back);
        else if(selected == 2){
            this->setSelectedScreen(back);
        }
        else
            return;
        laser->write(0);
    }
    //display items
    display_items();
}

void Measure::display_description(char * r){
    st7565->drawstring(0, 2, description); //description
    //result
    if(hasResult)
        st7565->drawstring(30, 5, r);
}

void Measure::display_items(void){
    //clear screen
    st7565->clear();
    
    //display result if it is a result screen
    float r = 5;
    char result[15];
    if(hasResult){
        //calculate result
        sprintf(result, "%.2f Meters", r);
    }
    
    //display description
    display_description(result);
    
    //draw items
    st7565->drawstring(LEFT_MARGIN * 2, SELECTION_LINE, nextName);
    st7565->drawstring(LEFT_MARGIN * 2, SELECTION_LINE + 1, backName);
    
    //set first as selected
    st7565->drawcircle(2, (current_line * 8) + 3, 2, 20);
    st7565->display();
}
