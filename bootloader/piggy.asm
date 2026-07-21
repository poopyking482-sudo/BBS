section .rodata

global input_data
global input_data_end
global input_len

input_data:
    incbin "bbs_kernel.ditos"    ; BBS kernel image compressed by Ditos
input_data_end:

input_len:
    dd input_data_end - input_data
