[org 0x7c00]                             ; offsets all dereferences by 0x7c00 as bootloader is loaded into offset 0x7c00 in memory by BIOS.
jmp start

KERNEL_LOCATION equ 0x1000

;%include "boot/routines/print_string.asm"
;%include "boot/routines/print_hex_dx.asm"

start:
    init_segment_registers:                 ; all registers could be some random value with the exception of dl loaded with drive number. 
                                            ; therefore, the data and extra segment registers are initialised to zero 
        xor ax, ax
        mov ds, ax
        mov es, ax                          ; zero es, ds

        mov bp, 0x8000
        mov ss, ax                              
        mov sp, bp                          ; seting stack above bootloader in memory


    msg_starting_in_real_mode:              ; Prints "Starting in real mode..."

        ;pusha 
        ;mov bx, real_mode_msg
        ;call print_string
        ;popa
;


; --------------------------------------------------------------------- LOAD DISK --------------------------------------------------------------------------------



load_disk_sectors:                      ; At first we are limited to the first 512-byte disk sector than is read by BIOS.
                                        ; The first action of the bootloader must be to load more sectors of the disk and start executing code from those sectors.
                                        ; Hence, the boot sector of the disk is dedicated entirely to loading the kernel.


    ; Loads rest of disk using BIOS interrupts
    mov ah, 0x02                        ; read sectors interrupt routine
    mov al, [number_of_sectors]         ; number of sectors to be read

    mov ch, 0x00                        ; cylinder number of CHS address 
    mov cl, 0x02                        ; sector number (sectors start from 1, and we want to read the next sector after the first, the bootloader sector)
    mov dh, 0x00                        ; head number of CHS address
    mov dl, dl                          ; dl is the disk number, it is already preloaded into dl when BIOS boots from this sector

    mov bx, KERNEL_LOCATION             ; this describes where to place the loaded sectors of the disk. They are placed at the same point where the kernel is loaded
                                        ; as the sectors that are loaded are all kernel code / data.  


    int 0x13                            ; interrupts - reads and loads disk sectors


    jc disk_error                       ; if an error occured, call the routine to notify user of disk error. error is flagged in carry flag.
    cmp al, [number_of_sectors]         ; double check that all sectors have actually been read
    jne disk_error                      ; if the correct number of sectors were not read, then disk_error routine called.

    ;mov bx, disk_success_msg
    ;call print_string                   ; prints success message

    jmp check_a20_enable                ; if success, then jump to next section


    disk_error:
        ;mov bx, disk_error_msg 
        ;call print_string              ; print disk read error message
        ;mov dl, al
        ;call print_hex_dx              ; print number of sectors read

        jmp $                          ; if there is a disk error, then hang
        ;ret
    
    
    ; Variables for disk loading routine
    number_of_sectors: db 0d35
    ;disk_error_msg: db 'Disk fail. sectors read - ', 0x00
    ;disk_success_msg: db 'Disk read success', 0x0a
    ;disk_success_msg_new_line: times 17 db 0x08
    ;disk_success_msg_null_terminator: db 0x00
;                                    



; --------------------------------------------------------------------- A20 ENABLE -------------------------------------------------------------------------------



check_a20_enable:                           ; The A20 line is a gate that opens up all of the address lines rather than just 20. When this is enabled,
                                            ; we can now access full 32 or 48 bit memory adressing rather than just 20 bit. (2^20 = 1 MiB) 


    ; Checks if A20 is enabled, and if it is not, then enables it through 2 separate methods
    pusha 
    push es 
    
    xor bx, bx
    mov cx, 0xaa55                          ; loads aa55 to cx to compare with value 1 MiB higher. If they are same, then memory wraps around and
                                            ; A20 is not enabled (i.e we are still restricted to 1 MiB of memory)

    xor ax, ax                              ; zeroes ax
    not ax                                  ; ax = 0xFFFF
    mov es, ax                              ; es = 0xFFFF
    mov bx, 0x7e0e                          ; FFFF:7e0e is 1 MiB above 0000:7dfe which is where the dw aa55 is located in memory.

    cmp cx, [es:bx]                         ; cmp 0xaa55, FFFF:7e0e
    je enable_A20                           ; if they are equal, then memory wraps around and A20 must be enabled

    ;mov bx, A20_enable_success_msg
    ;call print_string                      ; prints A20 success message

    pop es
    popa

    jmp map_memory

    enable_A20:
        cmp bx, 1                           ; Checks which method to try
        je enable_A20_FAST
        ;cmp bx, 2                          ; if bx is 2, then A20 has not been enabled. terminate boot and print error message
        ;je enable_A20_fail

        enable_A20_BIOS:
            ; BIOS method 
            mov ax, 0x2401
            int 0x15                        ; a BIOS interrupt to enable A20 that could work.

            mov bx, 0x1                     ; lets the bootloader know if this method has been tried yet or not.
            jmp check_a20_enable            ; checks if A20 has been enabled yet or not

        enable_A20_FAST:
            ; FAST method (more risky, could crash computer)
            in al, 0x92                     ; reads from IO port 0x92
            or al, 2
            out 0x92, al                    ; writes to port, enabling A20

            mov bx, 2                       ; lets the bootloader know if this has also been tried yet or not
            jmp check_a20_enable

        enable_A20_fail:
            ;mov bx, A20_enable_fail_msg   
            ;call print_string               ; prints A20 failed message

            jmp $                           ; Hangs

    
    ; Messages
    ;A20_enable_fail_msg: db 'Boot failure - A20 not enabled', 0x00
    ;A20_enable_success_msg: db 'A20 enable success', 0x0a
    ;A20_enable_success_msg_new_line: times 18 db 0x08, 
    ;A20_enable_success_msg_null_terminator: db 0x00
;


; -------------------------------------------------------------------- MEMORY MAP -------------------------------------------------------------------------------

map_memory:
    ; use the INT 0x15, eax= 0xE820 BIOS function to get a memory map
    ; note: initially di is 0, be sure to set it to a value so that the BIOS code will not be overwritten. 
    ; The consequence of overwriting the BIOS code will lead to problems like getting stuck in `int 0x15`
    ; inputs: es:di -> destination buffer for 24 byte entries
    ; outputs: bp = entry count, trashes all registers except esi

    mmap_ent equ 0xFA00             ; the number of entries will be stored at 0x8000
    mov bx, 0x2000
    mov es, bx

    do_e820:
        mov di, 0x8004          ; Set di to 0x8004. Otherwise this code will get stuck in `int 0x15` after some entries are fetched 
        xor ebx, ebx		; ebx must be 0 to start
        xor bp, bp		; keep an entry count in bp
        mov edx, 0x0534D4150	; Place "SMAP" into edx
        mov eax, 0xe820
        mov [es:di + 20], dword 1	; force a valid ACPI 3.X entry
        mov ecx, 24		; ask for 24 bytes
        int 0x15
        jc short .failed	; carry set on first call means "unsupported function"
        mov edx, 0x0534D4150	; Some BIOSes apparently trash this register?
        cmp eax, edx		; on success, eax must have been reset to "SMAP"
        jne short .failed
        test ebx, ebx		; ebx = 0 implies list is only 1 entry long (worthless)
        je short .failed
        jmp short .jmpin

    .e820lp:
        mov eax, 0xe820		; eax, ecx get trashed on every int 0x15 call
        mov [es:di + 20], dword 1	; force a valid ACPI 3.X entry
        mov ecx, 24		; ask for 24 bytes again
        int 0x15
        jc short .e820f		; carry set means "end of list already reached"
        mov edx, 0x0534D4150	; repair potentially trashed register

    .jmpin:
        jcxz .skipent		; skip any 0 length entries
        cmp cl, 20		; got a 24 byte ACPI 3.X response?
        jbe short .notext
        test byte [es:di + 20], 1	; if so: is the "ignore this data" bit clear?
        je short .skipent

    .notext:
        mov ecx, [es:di + 8]	; get lower uint32_t of memory region length
        or ecx, [es:di + 12]	; "or" it with upper uint32_t to test for zero
        jz .skipent		; if length uint64_t is 0, skip entry
        inc bp			; got a good entry: ++count, move to next storage spot
        add di, 24

    .skipent:
        test ebx, ebx		; if ebx resets to 0, list is complete
        jne short .e820lp

    .e820f:
        mov word [mmap_ent], bp	; store the entry count
        clc			; there is "jc" on end of list to this point, so the carry must be cleared
        jmp GDT_load
        
    .failed:
        stc			; "function unsupported" error exit
        jmp $
        ret



; --------------------------------------------------------------------- LOAD GDT ---------------------------------------------------------------------------------




GDT_load:                                           ; Loads the GDT, the table that describes how memory is segmented and used in protected mode (look at GDT.asm)
    xor ah, ah                                      ; puts graphics into VGA 0x13
    mov al, 0x13
    int 0x10


    cli                                             ; Disable interrupts
    lgdt [GDT_Descriptor]                           ; Loads gdt descriptor to BIOS

    mov eax, cr0
    or eax, 1
    mov cr0, eax                                    ; last bit of cr0 must be changed to 1 to enter 32-bit protected mode. We cannot change cr0 directly.

    ; However, we still might not be in 32 bit mode fully yet, because of pipelining: The next few instructions could still be done in real mode beacause
    ; they were fetched in real mode. To force the CPU to flush the pipeline, we have to perform a far jump (a jump to another segment.) 
    ; (this is because the CPU cannot fetch any more instructions until after the jmp / call instruction has been completed, because it wont know
    ; which instructions are coming next.)
                                       
    jmp code_segment_offset:start_protected_mode    ; Jumped to another segment containing our new code 
;



; --------------------------------------------------------- JUMP TO CROSS-COMPILED KERNEL CODE -------------------------------------------------------------------



[bits 32]
start_protected_mode:                       ; Updating segment registers to new segments defined in GDT before jumping to kernel code.
    mov ax, data_segment_offset
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000                        ; update stack pointer to be on top of free space
    mov ss, ax
    mov esp, ebp

    jmp KERNEL_LOCATION
;


; ------------------------------------------------------------------------ GDT -----------------------------------------------------------------------------------



; The full GDT including descriptors and offset constants. 
%include "boot/GDT.asm"                 



; -------------------------------------------------------------- PADDING AND MAGIC NUMBER ------------------------------------------------------------------------

real_mode_strings:
    real_mode_msg: db 'Starting in real mode...', 0x0a
    real_mode_msg_newline: times 30 db 0x08
    real_mod_null_terminator: db 0x00        
;                     
 
padding_boot_sector:
    times 510-($-$$) db 0x0             ; padding to create 512 byte bootloader sector
    dw 0xaa55                           ; 'magic number' which signposts this binary as bootable.