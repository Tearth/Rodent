#ifndef ARCH_H
#define ARCH_H

bool arch_init();

bool irq_enable();
bool irq_disable();
bool irq_is_enabled();

void irq_attach_timer_handler(void (*handler)());

#endif