#include "../sys.h"
#include "idt.h"
#include "isr.h"


// These exist in isrs.asm
extern "C" void isr0();
extern "C" void isr1();
extern "C" void isr2();
extern "C" void isr3();
extern "C" void isr4();
extern "C" void isr5();
extern "C" void isr6();
extern "C" void isr7();
extern "C" void isr8();
extern "C" void isr9();
extern "C" void isr10();
extern "C" void isr11();
extern "C" void isr12();
extern "C" void isr13();
extern "C" void isr14();
extern "C" void isr15();
extern "C" void isr16();
extern "C" void isr17();
extern "C" void isr18();
extern "C" void isr19();
extern "C" void isr20();
extern "C" void isr21();
extern "C" void isr22();
extern "C" void isr23();
extern "C" void isr24();
extern "C" void isr25();
extern "C" void isr26();
extern "C" void isr27();
extern "C" void isr28();
extern "C" void isr29();
extern "C" void isr30();
extern "C" void isr31();

//void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags)
//void def_isr(u8 num, u32 address, u16 sel)

// See description of each isr in isrs.asm
void install_isrs(){
    def_isr(0, reinterpret_cast<u32>(isr0), 0x08);
    def_isr(1, reinterpret_cast<u32>(isr1), 0x08);
    def_isr(2, reinterpret_cast<u32>(isr2), 0x08);
    def_isr(3, reinterpret_cast<u32>(isr3), 0x08);
    def_isr(4, reinterpret_cast<u32>(isr4), 0x08);
    def_isr(5, reinterpret_cast<u32>(isr5), 0x08);
    def_isr(6, reinterpret_cast<u32>(isr6), 0x08);
    def_isr(7, reinterpret_cast<u32>(isr7), 0x08);
    def_isr(8, reinterpret_cast<u32>(isr8), 0x08);
    def_isr(9, reinterpret_cast<u32>(isr9), 0x08);
    def_isr(10, reinterpret_cast<u32>(isr10), 0x08);
    def_isr(11, reinterpret_cast<u32>(isr11), 0x08);
    def_isr(12, reinterpret_cast<u32>(isr12), 0x08);
    def_isr(13, reinterpret_cast<u32>(isr13), 0x08);
    def_isr(14, reinterpret_cast<u32>(isr14), 0x08);
    def_isr(15, reinterpret_cast<u32>(isr15), 0x08);
    def_isr(16, reinterpret_cast<u32>(isr16), 0x08);
    def_isr(17, reinterpret_cast<u32>(isr17), 0x08);
    def_isr(18, reinterpret_cast<u32>(isr18), 0x08);
    def_isr(19, reinterpret_cast<u32>(isr19), 0x08);
    def_isr(20, reinterpret_cast<u32>(isr20), 0x08);
    def_isr(21, reinterpret_cast<u32>(isr21), 0x08);
    def_isr(22, reinterpret_cast<u32>(isr22), 0x08);
    def_isr(23, reinterpret_cast<u32>(isr23), 0x08);
    def_isr(24, reinterpret_cast<u32>(isr24), 0x08);
    def_isr(25, reinterpret_cast<u32>(isr25), 0x08);
    def_isr(26, reinterpret_cast<u32>(isr26), 0x08);
    def_isr(27, reinterpret_cast<u32>(isr27), 0x08);
    def_isr(28, reinterpret_cast<u32>(isr28), 0x08);
    def_isr(29, reinterpret_cast<u32>(isr29), 0x08);
    def_isr(30, reinterpret_cast<u32>(isr30), 0x08);
    def_isr(31, reinterpret_cast<u32>(isr31), 0x08);
};

// When exception occurs, hang
extern "C" void fault_handler(struct register_states *regs){
    for (;;) {};
};

