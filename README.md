This repository contains a simple snake program implemented in c and gpio/analog in wrapper that is used to 
generate keypresses. Installation instructions below:

GPIO_Analog wrapper: This is a simple wrapper designed to read multiple gpio and analog pins, and generate keypresses
based off of them. It requires the xdotools library, which you should be able to get with "opkg install xdotool".
Before running please ensure analog inputs are enabled, by following these instructions:

beagle$ SLOTS=/sys/devices/bone_capemgr.*/slots beagle$
PINS=/sys/kernel/debug/pinctrl/44e10800.pinmux/pins
beagle$ echo cape-bone-iio > $SLOTS

The wrapper can then be run, and will interpret inputs from the gpio pins listed inside, along with ain0 and ain1.
Should you choose to edit the wrapper it can be recompiled with the included make file. The program is run with 
./Wrapper. Keypresses will be sent to the currently selected window.

Warning!: Since this uses gpio polling, it will interfere with other programs using gpio polling when buttons are pressed

Hardware: 10 GPIO buttons and 2 variable potentiometers. This may be run with less hardware, and the pins are specified in the .c file. (along with pullup or pulldown, although we make no guarantees as to accuracy)
Please use a pin diagram for this, and as pullup/pulldown may not be constant across generations please verify on your board.
If a signal is not sent, it may be due to wiring issues. 


Snake: In order to demonstrate our wrapper we set up a simple game of snake. It can be run by calling ./snake in the directory it is stored in, or remade by calling make. It is controlled by keyboard inputs, and requires enter to be pressed after each button press. This is dealt with automatically in the wrapper. WASD are used for directional controls, and p is pause. CTRL C is used to quit the game.

Hardware: Some means of generating keypresses, be it via our wrapper or a keyboard. Screen for output (note that it can be run properly through ssh) 
