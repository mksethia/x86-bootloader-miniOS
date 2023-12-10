#include "sys.h"
#include "idt/idt.h"
#include "idt/isr.h"
#include "idt/irq.h"
#include "dvr/pit.h"
#include "dvr/keyboard.h"
#include "dvr/vga.h"
#include "dvr/mouse.h"
#include "ui/gui.h"
#include "mem/page.h"
#include "mem/memmgr.h"


void outp(u16 port, u8 data){
    __asm__ (
        "outb %%al,%%dx\n\t"           // send data out
        :                                   // no output
        :"d"(port), "a"(data)               // input
        //: no clobbered registers
    );
}

void outpw(u16 port, u16 data){
    __asm__ (
        "outw %%ax,%%dx\n\t"           // send data out
        :                                   // no output
        :"d"(port), "a"(data)               // input
        //: no clobbered registers
    );
}

u8 inp(u16 port){
    u8 output;
    __asm__ (
        "inb %%dx,%%al\n\t"            // recieve data
        :"=a" (output)                      // output
        :"d"(port)                          // input
        //: no clobbered registers
    );
    return output;
}

void memcpy(void *dest, const void *src, u32 len){
    u32 *d = (u32 *)dest;
    const u32 *s = (u32 *)src;
    
    while(len--){
        *d++ = *s++;
    }
}




extern "C" void main(){

    install_idt();

    install_isrs();

    remap_irqs();                // This func remaps irqs then also installs them,           

    __asm__ __volatile__("sti"); // sti = use external interrupts from now on

    pit_setup(240);

    install_keyboard();

    enable_mouse();

    unchain();

    pmmb_int15();

    vmm_init();

    //pageishere();

    for (;;) {};
}
