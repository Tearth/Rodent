#ifndef IRQ_ARCH_H
#define IRQ_ARCH_H

bool irq_enable();
bool irq_disable();
bool irq_is_enabled();
void irq_attach_timer_handler(void (*handler)());

#endif