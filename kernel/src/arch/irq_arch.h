#ifndef IRQ_ARCH_H
#define IRQ_ARCH_H

bool irq_enable();
void irq_attach_timer_handler(void (*handler)());

#endif