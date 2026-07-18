bits 32
global entry_32bit
extern read_ata_sectors
extern BBS_kernel        ; This is our staging function

entry_32bit:
    ; Load the remaining sectors from disk
    mov eax, 2              
    mov ecx, 64             
    mov edi, 0x10000        
    call read_ata_sectors

    ; Jump to the staging kernel
    call BBS_kernel        
    
    cli
    hlt
    jmp $
