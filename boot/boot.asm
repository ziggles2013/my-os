; Tell assembler: 16-bit code, BIOS boot sector
BITS 16
ORG 0x7C00

start:
    ; Set video mode to 80x25 text mode (just in case)
    mov ax, 0x0003
    int 0x10

    ; Print message using BIOS teletype function
    mov si, msg

.print_loop:
    lodsb               ; AL = [SI], SI++
    cmp al, 0
    je .done
    mov ah, 0x0E        ; BIOS teletype
    int 0x10
    jmp .print_loop

.done:
    ; Hang the CPU
.hang:
    hlt
    jmp .hang

msg: db "Hello from my OS!", 0

; BIOS boot signature at end of 512-byte sector
times 510-($-$$) db 0
dw 0xAA55

  
