; prints string at address in bx

print_string:
    push ax
    push cx
    push dx                     ; preserves old register data by pushing all to stack


    mov ah, 0x0e                ; BIOS teletype routine for interrupt 0x10
    print_loop:
        mov al, [bx]            ; moves whatever dx points to to al for printing
        int 0x10                ; prints al using 0x10, the BIOS video output interrupt
        inc bx                  ; bx points to next char in string

        cmp byte [bx], 0        ; sees if null terminator has been reached
        jne print_loop          ; if it hasnt, go again and print next character


    pop dx
    pop cx
    pop ax                      ; pops old register data from stack
    ret
