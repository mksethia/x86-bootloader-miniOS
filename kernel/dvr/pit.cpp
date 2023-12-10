#include "../sys.h"
#include "pit.h"
#include "vga.h"
#include "../ui/gui.h"



u32 timer_ticks = 0;     // tick count

//setup pit. divisor for hz value must be 16 bit.
void pit_setup(u16 hz){ 
    u16 divisor = 1193180 / hz;   // divide PIT input clock of 1193180Hz to get desired frequency
    outp(0x43, 0x36);             // PIT control register is 43. 0x36 sets square wave mode
    outp(0x40, divisor & 0xFF); // set low byte
    outp(0x40, divisor >> 8);   // set high byte;
}

void pit_handler(){
    timer_ticks++;       // inc timer

    // updates frame at set framerate (1/8 of pit)
    if ((timer_ticks & 0b0011) == 0) {frame();}

    return;
}