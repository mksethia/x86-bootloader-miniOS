#ifndef IDT_H
#define IDT_H

struct idt_entry{
    u16 low_addr; // lower 16 bits of interrupt handler address 
    u16 sel; // kernel segment selector
    u8 zero; // always zero
    u8 flags; // flags: 1110 for 32 bit interrupt, then 0, then priviledge level (00), then 1 for present = 1000 1110 = 0x8E
    u16 high_addr; // upper 16 bits of interrupt handler address
} __attribute__((packed)); // no padding, each member of struct comes directly after the other

void def_isr(u8 num, u32 address, u16 sel);

void install_idt();

#endif