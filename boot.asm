BITS 16
ORG 0x7C00

start:
  mov ax 0x0003
  int 0x10

  mov si, msg

.print_loop:
  lodsb
  
