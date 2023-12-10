; prints hex value of dx

print_hex_dx:
    push ax
    push bx
    push cx                     ; preserving old a,b,c registers

    mov cx, dx                  ; copy dx into cx

    and dh, 0xf0    
    and dl, 0xf0                ; zero the lower 4 bits, keeping the higher 4 bits of dh/dl
    shr dx, 4                   ; so that we can print the higher 4 bits    

    and ch, 0x0f                
    and cl, 0x0f                ; here we zero the higher 4 bits and keep the lower 4 bits of dh/dl

    mov ah, 0x0e                ; for printing with interrupt 0x10
    mov al, 0x30
    int 0x10
    mov al, 0x78
    int 0x10                    ; prints the '0x' before printing the number

    mov bl, dh
    call print_4_bit_hex
    mov bl, ch
    call print_4_bit_hex
    mov bl, dl 
    call print_4_bit_hex
    mov bl, cl 
    call print_4_bit_hex           ; print each of the 4 bit parts of dx

    print_4_bit_hex:
        cmp bl, 0x09            ; finds whether a letter or number has to be printed
        jle print_hex_number      ; if number go to printnumber routine
        jg print_hex_letter       ; if letter go to print letter routine

        print_hex_number:
            add bl, 0x30        ; offsets number value by 0x30 so the ASCII for said number is found
            mov al, bl          ; move into al to print
            int 0x10            ; prints
            ret                 ; return back to printhexdx function

        print_hex_letter:
            add bl, 0x57        ; offsets letter value by 0x37 so ASCII for said letter is found
            mov al, bl          ; move into al to print
            int 0x10            ; prints
            ret                 ; return back to printhexdx function
    

    pop cx
    pop bx
    pop ax                      ; restores old a,b,c registers
    ret