; This code contains all code that references and is referenced by our C++ code. 

;section .text
    [bits 32]
    extern main         ; This finds the extern "C" void main () in kernel.cpp

    ;jmp $

    call main           ; execute main func in kernel - transfer control toward C++

    jmp $               ; after main has been executed, hang


; --------------------------------------------------------------------- IDT LOAD ---------------------------------------------------------------------------------


global idt_load 
extern idtptr
idt_load:
    mov eax, idtptr
    lidt [idtptr]       ; loads IDT using pointer struct.
    ret


; --------------------------------------------------------- INTERRUPT SERVICE ROUTINES (ISRs) ---------------------------------------------------------------------


%include "kernel/idt/isrs.asm"


extern fault_handler    ; fault_handler function in C
isr_common_stub:        ; saves register states, sets kernel segments, calls fault handler, then restores stack frame.

    pusha
    push ds
    push es
    push fs
    push gs             ; saves register states

    mov ax, 0x10        
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax          ; loads kernel data segment descriptor 0x10 into each segment register.

    mov eax, esp  
    push eax
    mov eax, fault_handler
    call eax

    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa

    add esp, 8          ; offsets for error code and interrupt number

    iret
padder1:
padder2:
padder3:

; ------------------------------------------- INTERRUPT REQUESTS (IRQs) AND PROGRAMMABLE INTERRUPT CONTROLLERS ------------------------------------------------------

%include "kernel/idt/irqs.asm"

extern irq_handler
irq_common_stub:        ; follows exact same layout as isr stub.
    pusha
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov eax, esp
    push eax
    mov eax, irq_handler
    call eax

    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa

    add esp, 8
    iret

padder4:
padder5:
padder6:

; -------------------------------------------------------------------- MEMORY MAP -----------------------------------------------------------------------------------
