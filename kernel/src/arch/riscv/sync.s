.global mutex_lock

# Input:
# - a0 - mutex struct
# Output:
# - a0 - lock status
mutex_lock:
    # Set flag atomically
    li           t0, 1
    amoswap.w.aq t1, t0, 0(a0)

    # Check if old value was already set
    bnez         t1, mutex_lock_error
    li           a0, 1
    ret
mutex_lock_error:
    li           a0, 0
    ret

.global mutex_unlock

# Input:
# - a0 - mutex struct
# Output:
# - a0 - unlock status
mutex_unlock:
    # Clear flag atomically
    amoswap.w.rl t0, x0, 0(a0)

    # Check if old value was already clear
    beqz         t0, mutex_unlock_error
    li           a0, 1
    ret
mutex_unlock_error:
    li           a0, 0
    ret
