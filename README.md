# SELE - final project
 Task 1 - simple JTAG controller for ChipKIT Uno board
## Project wiki
Task1 of the final project is to implement a simple JTAG controller for executing specific functions over standard 4-pin JTAG interface on the on-board PIC32 microcontroller.

Below is an explanation of the functionalities of the project files, along with brief descriptions of functions used. When powering up the system, serial monitor on the computer displays a menu with 4 options, as follows:
d - print out contents of the microcontroller's IDCODE register,  
0 - set LED on,  
1 - set LED off,  
b - get status of the button (ON, OFF)   

### main
Main function of the project. Runs initialization of other modules (IO and serial), and sets value of 1 to the control cell of the control cell for pin where the LED is connected. This is necessary to connect the output cell to the pin, since all the pins are initialized to input by default.

### io
Input/output module contains functions for initializing Arduino pins used in this project, along with a getter/setter functionality for controling those pins. Pin definitions in this module can be changed to work with any other pin/port by setting defined constants in the header file. This is enabled with use of macro functions to access specified IO registers.

### serial
Serial module is used to input and display data through a serial monitor connected to the board. It implements UART functions (init, transmit, receive), and higher-level functions for serial interface. *serial_init* displays the menu, while *serial_transmit* and *serial_transmitHex* can be used to print out data to the serial monitor - as ASCII or hexadecimal, accordingly.  *Serial_data_parser* is used to call the function selected from the menu.
All data received from the serial monitor are received in an interrupt handler, which also echoes back the character.

### jtag_ll
Low-level JTAG module. Implements basic JTAG low-level functions. Firstly the static functions, not used outside this module:  
*jtag_ll_clk* does one clock cycle on the JTAG_CLK pin  
*jtag_ll_fsmTransition* takes as input number of steps to traverse in the JTAG state machine and a bit representation of these transitions. More on the use is given later in example for setting the state to Shift-DR.  
*jtag_ll_shiftData* takes as inputs the size of a bit stream with two pointers to unsigned char arrays for the input and output stream. It is to be used when in Shift-DR or Shift_IR states to set or read data from the boundary scan registers or the instruction registers. If any pointer parameter is set to 0, a default, zero-initialized buffer is used.  
API functions of this module are setters and getters for IR and DR, which take as parameters bit stream length and a pointer for the stream. All of them use the state transition function, but to give an example of it's usage, we'll look at the *jtag_ll_setDR* function. The bit stream length is 3 and bits are *0b001*, which means the TMS pin will be set to values 1, 0, 0, shifting the state from Idle to Shift-DR.  
Because all of the API functions assume the starting state is Idle one should first call *jtag_ll_resetAndIdle* to reset the controller and set state to Idle. If the system does not implement the TRST pin, clocking TMS=1 five times is used to same effect.

### jtag
Jtag module contains higher-level functions for the PIC32 microcontroller. By using *jtag_setPin* and *jtag_getPin* user can set or read value of a pin. *Note for setting outputs in module main*.  Also there is a function for getting the contents of the IDCODE register. All functions can be called in any order, since they all reset the JTAG controller and only then execute the wanted function. When setting a value to a pin, value of the boundary scan register is firstly read and then the one value is modified before sending the bit stream back.
