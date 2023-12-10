#include "../sys.h"
#include "idt.h"
#include "isr.h"
#include "irq.h"
#include "../dvr/pit.h"
#include "../dvr/keyboard.h"
#include "../dvr/vga.h"
#include "../dvr/mouse.h"

// These exist in irqs.asm
extern "C" void irq32();
extern "C" void irq33();
extern "C" void irq34();
extern "C" void irq35();
extern "C" void irq36();
extern "C" void irq37();
extern "C" void irq38();
extern "C" void irq39();
extern "C" void irq40();
extern "C" void irq41();
extern "C" void irq42();
extern "C" void irq43();
extern "C" void irq44();
extern "C" void irq45();
extern "C" void irq46();
extern "C" void irq47();


void wait(){
    for (u16 x = 0; x < 1000; x++) {}
}


// Remap IRQs 0-15 to 32-47 (this func also installs the irqs)
// See description of each irq in irqs.asm
void remap_irqs(){
    outp(0x20, 0x11);
    wait();
    outp(0xa0, 0x11);
    wait();
    outp(0x21, 0x20);
    wait();
    outp(0xa1, 0x28);
    wait();
    outp(0x21, 0x04);
    wait();
    outp(0xa1, 0x02);
    wait();
    outp(0x21, 0x01);
    wait();
    outp(0xa1, 0x01);
    wait();
    outp(0x21, 0x00);
    wait();
    outp(0xa1, 0x00);
    wait();

    def_isr(32, reinterpret_cast<u32>(irq32), 0x08);
    def_isr(33, reinterpret_cast<u32>(irq33), 0x08);
    def_isr(34, reinterpret_cast<u32>(irq34), 0x08);
    def_isr(35, reinterpret_cast<u32>(irq35), 0x08);
    def_isr(36, reinterpret_cast<u32>(irq36), 0x08);
    def_isr(37, reinterpret_cast<u32>(irq37), 0x08);
    def_isr(38, reinterpret_cast<u32>(irq38), 0x08);
    def_isr(39, reinterpret_cast<u32>(irq39), 0x08);
    def_isr(40, reinterpret_cast<u32>(irq40), 0x08);
    def_isr(41, reinterpret_cast<u32>(irq41), 0x08);
    def_isr(42, reinterpret_cast<u32>(irq42), 0x08);
    def_isr(43, reinterpret_cast<u32>(irq43), 0x08);
    def_isr(44, reinterpret_cast<u32>(irq44), 0x08);
    def_isr(45, reinterpret_cast<u32>(irq45), 0x08);
    def_isr(46, reinterpret_cast<u32>(irq46), 0x08);
    def_isr(47, reinterpret_cast<u32>(irq47), 0x08);
}; 






// Handler for irq
extern "C" void irq_handler(register_states *regs){
    

    u8 check_spurious_master;
    u8 check_spurious_slave;

    // select correct handler and run.
    switch(regs->int_no){
        case 32:                        // PIT interrupt
            pit_handler();
            break;

        case 33:                        // Keyboard interrupt
            keyboard_handler();
            break;

        case 39:                        // check if spurious interrupt, by looking at PIC chip's service register (if last bit is set then it is servicing real IRQ)
            outp(0x20, 0x0b);           // If it is NOT servicing an IRQ7, then ignore and do not send EOI
            check_spurious_master = inp(0x20);
            if (check_spurious_master && 0b1000000) {return;};
            break;

        case 44:                        // Mouse interrupt
            mouse_handler();
            break;

        case 47:                        // check for spurious interrupt but if it is spurious still need to send eoi to master PIC
            outp(0xa0, 0x0b);
            check_spurious_slave = inp(0xa0);
            if (check_spurious_slave && 0b1000000) {outp(0x20, 0x20); return;};
            break;

        default:
            draw_pixel(199, 199, 0x2c);
            break;
    }
    
    // send eoi to master PIC if not IRQ 2
    if (regs->int_no != 34) {outp(0x20, 0x20);}

    // if idt number was bigger than 40 (so irq8-15), send eoi (end of interrupt) to slave PIC
    if (regs->int_no >= 40) {outp(0xa0, 0x20);}

    

    return;
};
