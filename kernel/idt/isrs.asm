; This code simply makes the routines 0-31 global, then defines each. Look at ISR0 for explanation of an interrupt routine, all of them are defined the same way.

global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31

isr0:			; ISR 0:  division by zero 
	cli         ; disable interrupts - it is being handled here 'manually'.
	push byte 0 ; push dummy error code if interrupt does not already.
	push byte 0 ; push isr number onto stack
	jmp isr_common_stub 


isr1:			; ISR 1:  debug
	cli
	push byte 0
	push byte 1
	jmp isr_common_stub


isr2:			; ISR 2:  non maskable interrupt (non recoverable hardware error)
	cli
	push byte 0
	push byte 2
	jmp isr_common_stub


isr3:			; ISR 3:  breakpoint exception
	cli
	push byte 0
	push byte 3
	jmp isr_common_stub


isr4:			; ISR 4:  into detected overflow exception
	cli
	push byte 0
	push byte 4
	jmp isr_common_stub


isr5:			; ISR 5:  out of bounds exception
	cli
	push byte 0
	push byte 5
	jmp isr_common_stub


isr6:			; ISR 6:  invalid opcode exception
	cli
	push byte 0
	push byte 6
	jmp isr_common_stub


isr7:			; ISR 7:  no coprocesor exception
	cli
	push byte 0
	push byte 7
	jmp isr_common_stub


isr8:			; ISR 8:  double fault exception
	cli
	push byte 8
	jmp isr_common_stub


isr9:			; ISR 9:  coprocessor segment overrun exception
	cli
	push byte 0
	push byte 9
	jmp isr_common_stub


isr10:			; ISR 10: bad TSS exception (task state segment)
	cli
	push byte 10
	jmp isr_common_stub


isr11:			; ISR 11: segment not present exception
	cli
	push byte 11
	jmp isr_common_stub


isr12:			; ISR 12: stack fault exception 
	cli
	push byte 12
	jmp isr_common_stub


isr13:			; ISR 13: general protection fault exception (program accesses data not for its use)
	cli
	push byte 13
	jmp isr_common_stub


isr14:			; ISR 14: page fault exception (program accesses page improperly)
	cli
	push byte 14
	jmp isr_common_stub


isr15:			; ISR 15: unknown interrupt exception
	cli
	push byte 0
	push byte 15
	jmp isr_common_stub


isr16:			; ISR 16: coprocessor fault exception
	cli
	push byte 0
	push byte 16
	jmp isr_common_stub


isr17:			; ISR 17: alignment check exception (unaligned memory operand)
	cli
	push byte 0
	push byte 17
	jmp isr_common_stub


isr18:			; ISR 18: machine check exception (hardware error)
	cli
	push byte 0
	push byte 18
	jmp isr_common_stub


isr19:			; ISR 19: reserved 
	cli
	push byte 0
	push byte 19
	jmp isr_common_stub


isr20:			; ISR 20: reserved 
	cli
	push byte 0
	push byte 20
	jmp isr_common_stub


isr21:			; ISR 21: reserved 
	cli
	push byte 0
	push byte 21
	jmp isr_common_stub


isr22:			; ISR 22: reserved 
	cli
	push byte 0
	push byte 22
	jmp isr_common_stub


isr23:			; ISR 23: reserved 
	cli
	push byte 0
	push byte 23
	jmp isr_common_stub


isr24:			; ISR 24: reserved 
	cli
	push byte 0
	push byte 24
	jmp isr_common_stub


isr25:			; ISR 25: reserved 
	cli
	push byte 0
	push byte 25
	jmp isr_common_stub


isr26:			; ISR 26: reserved 
	cli
	push byte 0
	push byte 26
	jmp isr_common_stub


isr27:			; ISR 27: reserved 
	cli
	push byte 0
	push byte 27
	jmp isr_common_stub


isr28:			; ISR 28: reserved 
	cli
	push byte 0
	push byte 28
	jmp isr_common_stub


isr29:			; ISR 29: reserved 
	cli
	push byte 0
	push byte 29
	jmp isr_common_stub


isr30:			; ISR 30: reserved 
	cli
	push byte 0
	push byte 30
	jmp isr_common_stub


isr31:			; ISR 31: reserved 
	cli
	push byte 0
	push byte 31
	jmp isr_common_stub
