#include "../sys.h"
#include "idt.h"


// idt pointer with beginning address and size.
struct idt_ptr{ 
    u16 limit;
    u32 base;
}__attribute__((packed));

idt_entry idt[256] = {0}; // creates array of 256 IDT entries, initialises all to zero
idt_ptr idtptr; // creates pointer structure of IDT


extern "C" void idt_load(); // This func exists in kernel_entry.asm


// quick function to define idt entries
void def_isr(u8 num, u32 address, u16 selector) {
    idt[num].low_addr = (address & 0xffff); // get lower 16 bits
    idt[num].sel = selector; // set selector
    idt[num].zero = 0x00; // set zero bits
    idt[num].flags = 0x8e; // set flags
    idt[num].high_addr = (address >> 16) & 0xffff; // get upper 16 bits

    return;
};


// installs idt, incuding gate definitions
void install_idt(){
    idtptr.limit = 2048; // set idtptr limit (size of table in bytes = 8 bytes = 256 entries)
    idtptr.base = reinterpret_cast<u32>(&idt); // set idtptr base, force conversion of &idt to a 32 bit int.

    u32 idtptraddress = reinterpret_cast<u32>(&idtptr);

    // set all gates...

    idt_load();

    return;
};


