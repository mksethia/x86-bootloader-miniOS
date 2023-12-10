#ifndef IRQ_H
#define IRQ_H

void install_irq_handler(int num, void (*handler)(register_states *regs));

void uninstall_irq_handler(int num);

void remap_irqs();


#endif