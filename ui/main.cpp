#include "UserInterface.h"
#include "st7565LCD.h"

#define WAIT_TIME 0.3

ST7565 st7565(_MOSI, _SCLK, _CS, _RST, _A0); // mosi, sclk, cs, rst, a0
Serial pc(USBTX, USBRX); //rx, tx

//buttons
DigitalIn down(p19);
DigitalIn select(p22);
DigitalIn up(p20);

//laser
DigitalOut laser_mod(p21, 0);

//gyro and thermo digital out
DigitalOut gyro(P0_29, 0); //pin31
DigitalOut thermo(P0_30, 0); //pin32

int main(){
    //create god UI object
    UI * ui = new UI(&st7565);
    //initialize configuration
    //create main menu
    Menu * main_menu = new Menu(" Main Menu", &st7565);
    
    
    //create distance screens
    Measure *distance = new Measure(" Distance", &st7565, main_menu, &laser_mod);
    distance->setDescription("Select Start from the menu below to start laser."); 
    main_menu->addItem(distance);
    
    /*
    //create point-to-point screens
    Measure *p2p = new Measure(" Point-to-Point", &st7565, main_menu);
    p2p->setDescription("Select Start from the menu to start laser for #1 target.");
    Measure *p2p2 = new Measure(" Point-to-Point", &st7565, p2p);
    p2p2->setDescription("Press select button to fix target #1.");
    Measure *p2p3 = new Measure(" Point-to-Point", &st7565, p2p2);
    p2p3->setDescription("Press select button to fix target #2.");
    Measure *p2p4 = new Measure(" Point-to-Point", &st7565, p2p3);
    p2p4->setDescription("Distance between targets is:");
    p2p4->setResult(true); //result screen
    
    p2p->setNext(" Start", p2p2);
    p2p2->setNext(" Select", p2p3);
    p2p3->setNext(" Select", p2p4);
    p2p4->setNext(" Start again", p2p);
    main_menu->addItem(p2p);
    */
    
    //create level meter screen
    LevelMeter *lvl = new LevelMeter(&st7565, main_menu, &gyro, &thermo);
    main_menu->addItem(lvl);
    
    //create compass screen
    Compass *compass = new Compass(&st7565, main_menu, &gyro, &thermo);
    main_menu->addItem(compass);
    
    /*
    //create thermo screen
    Thermometer *thermo_screen = new Thermometer(&st7565, main_menu, &gyro, &thermo);
    main_menu->addItem(thermo_screen);
    */
    
    /*
    Measure *thermo = new Measure(" Thermometer", &st7565, main_menu);
    thermo->setDescription("Press Start from the menu to start laser.");
    Measure *thermo2 = new Measure(" Thermometer", &st7565, thermo);
    thermo2->setDescription("Press select button to fix target.");
    Measure *thermo3 = new Measure(" Thermometer", &st7565, main_menu);
    thermo3->setDescription("Target temperature is:");
    thermo3->setResult(true);
    
    thermo->setNext(" Start", thermo2);
    thermo2->setNext(" Start", thermo3);
    thermo3->setNext(" Start", thermo);
    */
    
    //create header object
    Header * header = new Header("", &st7565);
    
    //set header and current menu
    ui->setCurrent(main_menu);
    ui->setHeader(header);
    ui->init();
    
    while(1) {
        if(select){
            wait(0.1);
            //printf("select\n");
            ui->update('y'); //ascii 121
            wait(WAIT_TIME);
        }
        else if(up){
            wait(0.1);
            //printf("up\n");
            ui->update('w'); //ascii 119
            wait(WAIT_TIME);
        }
        else if(down){
            wait(0.1);
            //printf("down\n");
            ui->update('s'); //ascii 115
            wait(WAIT_TIME);
        }
    }
}