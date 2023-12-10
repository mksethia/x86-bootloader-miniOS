#ifndef ISR_H
#define ISR_H

#include "../sys.h"

// Shows stack state before exception
struct register_states {
    u32 gs, fs, es, ds;                           // segment registers
    u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;  // pusha
    u32 int_no, err_code;    // 'push byte #', error codes
    u32 eip, cs, eflags, useresp, ss;   // pushed by the processor automatically 
};

void install_isrs();

void fault_handler();


#endif