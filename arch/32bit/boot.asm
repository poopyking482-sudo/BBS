bits 32

global switch_context
switch_context:

    push ebp
    push ebx
    push esi
    push edi
    mov eax, [esp + 20]     ; eax = old_esp pointer
    mov edx, [esp + 24]     ; edx = new_esp value
    mov [eax], esp
    mov esp, edx
    pop edi
    pop esi
    pop ebx
    pop ebp

    ret
