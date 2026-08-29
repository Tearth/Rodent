.global _entry_point

_entry_point:
    la  sp, __stack_pointer
    j   kmain
