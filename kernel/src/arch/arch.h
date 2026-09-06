#ifndef ARCH_H
#define ARCH_H

bool arch_init();

void irq_attach_timer_handler(void (*handler)());

#endif