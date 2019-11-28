/* A program to an Arduino board that receives UART (9600,8,N,1) commands from
 * a PC and controls the PIC32 through its JTAG port using the corresponding 
 * IEEE 1149.1 TAP controller. The commands (composed by a single character) 
 * are the following:
 *      d – Gets the ID CODE of the PIC32 and prints it in hexadecimal
 *      1 – Turn on the LED LD5 of the ChipKIT board
 *      0 – Turn off the LED LD5 of the ChipKIT board
 *      b – Print the state of the button connected to pin 29 of the ChipKIT
 *
 * Final project of course SELE, a.y. 19/20.
 * Students: Mateusz Dudek, Nowen Adamczak, Dino Ljubic
 */

int main() {
    while(1){};
}