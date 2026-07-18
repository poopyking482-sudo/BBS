bits 32
global trigger_sys_exit

trigger_sys_exit:
    mov eax, 1              ; Syscall ID 1 (Exit/Halt)
    int 0x80                ; Invoke custom kernel handler
    ret
