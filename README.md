# Multi Measurement Device - Glasgow Robotics Team Alpha

### Abstract

The main purpose of this concept is to offer a compact, commercially oriented, low cost product which can acquire, compute and display various measurements.
The proposed prototype is based on the NXP MBED LPC1768 microcontroller and several breakout board modules which are interfaced via custom designed double-sided PCB (Printed Circuit Board). 
This device is not revolutionary however it provides several technologies into a single package. In terms of pricing the budget limit for the prototype was set at a maximum of £120, including manufacturing, components and breakout boards. The total cost for production in large quantities could be reduced further by combining all of the separate modules into a single more complicated PCB design. 
This approach will also reduce the physical size of the device and the manufacturing costs. 
However due to time limitations and the fact that the device has been developed by students as an extracurricular activity, it was decided that the prototype would be as simplified as possible in terms of hardware but fully functional. 

## Code

This repository contains the code for programming the user interface for the ST7565 LCD display, part of multi measurement device.

## LED Display

For the purposes of this project, it was decided that a ST7565 Adafruit LED display will be used. It fits the requirements as it is low-cost, easy to integrate with embedded controllers and has a lot of development resources available. It provides a resolution of 128x64 pixels on a full RGB backlight which allows different colour combinations to be  used. In addition, it has a SPI serial interface and therefore it requires fewer data pins since only one bit in being written at a time. The library makes use of several functions and includes a buffer which is modified and sent to the display via SPI, one bit at a time. When a command or some data has to be written on the screen, a single bit is written on the data bus and clock is pulsed high-low, making it possible to write at achieve fast responses.
However, one disadvantage is that there is no official code library compatible with the mbed LPC1768, instead a custom repository/library translated from Arduino code will be used (see reference [5]). The display requires a buffer of 1024 bytes - one for every pixel - and cannot be accessed unless kept in the memory of the controller. Therefore 1024 bytes of RAM must be used to store the contents of the buffer.

The screen displays a GUI (Graphical User Interface) which the user will interact with. Therefore, it must be easy to use and it has to represent effectively the data from the calculations. The next sections will indicate what were the requirements for the interface, its design and implementation in the C++ programming language supported by the Mbed.

### Initial requirements

The first stages of creating the user interface were dedicated to gathering and refining the requirements needed. The following are very general to yet fundamental to the project: 

* a simple user interface to interact with - must be simple and easy to use
* a scrollable menu, user must interact with it using the 5-way tactile switch
* every item in the menu must lead to a screen or to another menu
* multiple screens to display data coming from other modules (thermometer, level meter..)
* a header, indicating the current screen and battery level
* measurement screens, used when interacting with the camera

### Design

It was decided that GUI will make use of object-oriented principles, which in this case are supported by the C++ libraries. By using objects it is easier to separate the UI into self-contained modules, each providing its own set of functions and interacting with a different parts of the device. It also allows more flexibility in designing each module as well as ease of maintenance. Figure 3.6.1 represents an UML class diagram of the design.
Following our initial requirements the user interface was designed as a combination of a ‘virtual screen’ object together with a header object. The header will be present on the screen at all times, while the screen may change multiple times according to actions taken by the user. 
The ‘UserInterface’ object is the actual interface on the screen. As stated, it contains at all times a screen object and a header. In addition, it contains functions to start, initialize the home screen and set-up initial colours when the device boots up.
The Header object consists of title of current screen and a battery level indicator. Initially,  temperature of the room was included as well but the screen would appear too congested. The header is static and is placed at the top of the screen.
 
The ‘Item’ class is abstract - it has no implementation. The implementation is provided by the classes that inherit the abstract class. This class is following the composite design pattern since the ‘Menu’ object must be an ‘Item’ that can be represented as a part of the screen, but also to contain other ‘Items’. Therefore, an instantiation of the ‘Menu’ class represents the main screen ‘Main Menu’ and contains pointers to other screens. In this way the user will have control over a list of Items which will be displayed on the screen. In essence, the ‘Menu’ contains an array of pointers to ‘Items’ and at the same time will also be an ‘Item’ itself.

The ‘Screen’ object will represent a static screen i.e. the user interacts with it only by recording data visually or on paper. For example, it is used for implementation of the compass and level meter screens. As the user moves the device around, the interface will receive data from the gyroscope, accelerometer and magnetometer and display it in a user friendly format.

The ‘Measure’ class is used for communication with the other modules. Its role is to display guiding information, starting the laser, fixing a point and so on. In short, it is used for linking multiple screens together. For example, a distance measurement using the camera module will consist of multiple screens i.e. a screen for description, a screen for starting the laser, a screen to set a fixed point and one for the final result. In essence, it allows us to create linked list of screens as you can see in figure 3.6.2.

The ‘Measure’ objects interact with the camera, thermometer module and laser module. 

### Implementation

For the implementation phase most of the functionality specified in the design phase has been achieved. By switching easily between the numerous ‘virtual screens’, the LED buffer must be cleared, modified and redisplayed. This is managed with the API provided by the external library. 
The screen, with a resolution of 128x64, is structured into 8 ‘pages’ as you can see in figure 3.6.3. Each page is 8 pixels high and 128 pixels long. It was decided that a custom interface would be designed according to this structure, as it is much easier because the library provides the necessary functions to write in any of the provided pages. Of course, it also provides the possibility of writing to specific pixels as well. 

The first task was implementing the ‘UserInterface’ class which contains the function to initialize the display. init() is the function that is called when the display turns on, sets up the brightness and contrast level, colours, displays a welcome screen for two seconds, clears the screen, draws the header, the initial menu and waits for user interaction.
The colour of the display is set up by adjusting the potential on the of cathodes of the red, green and blue LEDs comprising the backlight. Usually this could be achieved with Digital I/O or PWM pins from the microcontroller. However, because of the lack digital I/O for this secondary in priority task, it was decided that the above would be adjusted manually via a set of jumpers. (see full schematic - Appendix.2.)
The buttons on the 5-way switch are implemented in our code as digital inputs. When the user presses a button, the ‘UserInterface’ is interrupted that an event has occurred and sends the update to the current screen. In this way, every ‘virtual screen’ will have a different behaviour when an update is signaled. 

The second task was to implement a simple Menu class with a few objects with which the user can interact easily. The finalized design can be seen in figure 3.6.4. It consists of a list of items with which the user can scroll up or down. The black dot indicates the current item selected by the user. If the user wishes to select that item, he will be redirected to the screen indicated by that item. The two arrows on the right side indirectly indicate the number of items on the screen. Since the screen has limited capacity, if the down arrow is present, that means there are items further down the list - when the user decides to see other items they will be displayed on a new screen thus clearing the old one. Therefore, the rate of scrolling is not by 1 item, but by the number of items that can be on the screen. When the up arrow is present, the menu indicates that there are items further up the screen. 

The next step in the implementation phase was to create the ‘Measure’ class, initialize and customize its objects. As stated in the design phase, the measure screens are used to interact with the camera, laser and thermometer module.
The measure screens that appear in the menu are:
* Distance - the option allows the user to start the laser, choose a fixed point and the controller will calculate the distance to that point (see section 3.3 for more details).
* Point-to-Point - similarly to the above option, it allows the user to calculate the distance between two fixed points.
* Thermometer - calculates the temperature in degrees (currently in Celsius) of a fixed point. 

Every linked screen contains a description of the currently selected option and the steps necessary to complete the calculation. The first screen contains information about the currently selected option and at each point provides the user with the option of returning to the previous screen or to the main menu. These provide the user with specific options according to their needs. 

The last step was to create the ‘Screen’ implementation. Similarly to ‘Measure’, ‘Screen’ class is abstract, since there are two implementations with many similarities but slightly different in terms of content. The two implementations are: compass and level meter. 
The compass design was simple and straightforward. A simple compass design with a single hand was implemented, which points to the cardinal direction in which the user holds the device. The outer circle which denotes the cardinal directions remains static, whilst the hand moves around the circle, pointing in the specified direction. This simple design makes our implementation easier, however, for a greater user experience it would be necessary to create a dynamic cardinality circle and set the hand static, as in most compass applications.
  
The level meter screen is composed of one dynamic cross and a static circle in the middle of the screen. The data from the gyroscope and accelerometer must be converted to fit the size of the screen. As the user tilts the device around, the interface receives data from the two modules and redraws the cross at a rate of 30Hz. Figure 3.6.5 (b) illustrates the level meter and the movement of the cross around the circle.
Interaction with any of the modules must not interfere with the user interface. The user must be able to interact with the device whilst the controller is gathering data from other modules. In order to implement this, the C++ library, which provides a specific threading functionality for the microcontroller, was used. Therefore, at all times there are at most two threads running: the main thread of the GUI and another thread which is either gathering data, making calculations, taking a picture, etc. 
When the user interacts with the GUI whilst a thread is running (for example, when reading gyroscope data), the secondary thread is stopped and returns control to the main thread. 
