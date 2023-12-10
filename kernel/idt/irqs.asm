; Similarly to isrs.asm, this code sends all irqs to a common stub to be handled. 
; IRQs are hardware interrupts like keyboard and mouse, disk etc. ISRs are exceptions (errors.)
; To make it easier to track, the irqs start from 32 rather than 0 as the interrupt code will also start from 32.

global irq32
global irq33
global irq34
global irq35
global irq36
global irq37
global irq38
global irq39
global irq40
global irq41
global irq42
global irq43
global irq44
global irq45
global irq46
global irq47


irq32:			; IRQ 32: PIT
	cli
	push byte 0
	push byte 32
	jmp irq_common_stub


irq33:			; IRQ 33: Keyboard
	cli
	push byte 0
	push byte 33
	jmp irq_common_stub


irq34:			; IRQ 34: Cascade (used internally)
	cli
	push byte 0
	push byte 34
	jmp irq_common_stub


irq35:			; IRQ 35: COM2
	cli
	push byte 0
	push byte 35
	jmp irq_common_stub


irq36:			; IRQ 36: COM1
	cli
	push byte 0
	push byte 36
	jmp irq_common_stub


irq37:			; IRQ 37: LPT2 (LPT = line print terminal)
	cli
	push byte 0
	push byte 37
	jmp irq_common_stub


irq38:			; IRQ 38: floppy disk
	cli
	push byte 0
	push byte 38
	jmp irq_common_stub


irq39:			; IRQ 39: 'spurious' interrupt (hack interrupt to error-correct) / LPT1 
	cli
	push byte 0
	push byte 39
	jmp irq_common_stub


irq40:			; IRQ 40: CMOS clock
	cli
	push byte 0
	push byte 40
	jmp irq_common_stub


irq41:			; IRQ 41: peripherals 
	cli
	push byte 0
	push byte 41
	jmp irq_common_stub


irq42:			; IRQ 42: peripherals 
	cli
	push byte 0
	push byte 42
	jmp irq_common_stub


irq43:			; IRQ 43: peripherals
	cli
	push byte 0
	push byte 43
	jmp irq_common_stub


irq44:			; IRQ 44: ps/2 mouse
	cli
	push byte 0
	push byte 44
	jmp irq_common_stub


irq45:			; IRQ 45: coprocessor / fpu
	cli
	push byte 0
	push byte 45
	jmp irq_common_stub


irq46:			; IRQ 46: primary disk
	cli
	push byte 0
	push byte 46
	jmp irq_common_stub


irq47:			; IRQ 47: secondary disk
	cli
	push byte 0
	push byte 47
	jmp irq_common_stub