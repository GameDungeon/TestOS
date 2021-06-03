global gdt_flush      ; Allows the C code to link to this
gdt_flush:
    lgdt [rdi]       
    mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    push qword 0x08        ; new code segment selector
    push qword flush2      ; function to execute next
    retfq                  ; far return (pops address and code segment)
flush2:
    ret              