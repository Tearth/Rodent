.global entry_point

entry_point:
    la  sp, __stack_pointer
    j   main
