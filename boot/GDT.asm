GDT_start_address:
    ; The GDT table, required for moving into protected mode. Defines and describes the code and data segments of memory                                  
    null_descriptor:
        dd 0x00000000
        dd 0x00000000                       ; need 8 byte null descriptor at start of GDT. 

    code_segment_descriptor:                
    ; Describes code segment using 8 byte structure

        dw 0xffff                           ; This is first 2 bytes of the limit. 
                                            ; Limit defines the size of the code segment
                                            ; Because it is a 20-bit property, we set it to the max size allowed; 0xfffff

        dw 0x0000                           ; This is the first 3 bytes of the base. 
        db 0x00                             ; The base is the address of the start of the code segment, which is zero.       

        db 0b10011010                       ; First 4 bits are present, privilege, and type descriptors. Last 4 bits are type flags.
                                            ; Bit 0:   Present:      This segment is used, so it is 1
                                            ; Bit 1,2: Privilege:    Ring level. This is kernel code so ring 0
                                            ; Bit 3:   Type:         If code OR data segment, this is 1    
                                            ; ------------------------------------------------------------------------------------------
                                            ; Bit 4:   Code/Data:    Code segment = 1
                                            ; Bit 5:   Conform:      If 1, allows ring 3 code to execute this kernel code. Thus it is 0
                                            ; Bit 6:   Readable:     This is readable AND executable so it is 1
                                            ; Bit 7:   Access:       Set by CPU if sector is currently in use or not.

        db 0b11001111                       ; First 4 bits are misc flags. Last 4 bits are last 4 bits of limit (0xf)
                                            ; Bit 0:   Granularity:  Multiplies limit by 4 KiB, to allow access all 4 GiB of memory
                                            ; Bit 1:   32-bit     :  Yes = 1
                                            ; Bit 2:   64-bit     :  No = 0
                                            ; Bit 3:   AVL        :  Reserved = 0.        
                                            ; ------------------------------------------------------------------------------------------
                                            ; Bit 4,7: 0xf        : last 4 bits of limit 0xfffff.  

        db 0x00                             ; Last byte of base. Base = 0x00000000



    data_segment_descriptor:                
    ; This is identical to code segment descriptor with a few exceptions in type flags.

        dw 0xffff                           ; This is first 2 bytes of the limit. 
                                            ; Limit defines the size of the data segment
                                            ; Because it is a 20-bit property, we set it to the max size allowed; 0xfffff

        dw 0x0000                           ; This is the first 3 bytes of the base. 
        db 0x00                             ; The base is the address of the start of the data segment, which is zero.       

        db 0b10010010                       ; First 4 bits are present, privilege, and type descriptors. Last 4 bits are type flags.
                                            ; Bit 0:   Present:      This segment is used, so it is 1
                                            ; Bit 1,2: Privilege:    Ring level. This is kernel data so ring 0
                                            ; Bit 3:   Type:         If code OR data segment, this is 1    
                                            ; ------------------------------------------------------------------------------------------
                                            ; Bit 4:   Code/Data:    Data segment = 0
                                            ; Bit 5:   Direction:    If 1, data seg expands downwards. This could collide with code seg, so it is 0
                                            ; Bit 6:   writeable:    This is readable AND writable so it is 1
                                            ; Bit 7:   Access:       Set by CPU if sector is currently in use or not.

        db 0b11001111                       ; First 4 bits are misc flags. Last 4 bits are last 4 bits of limit (0xf)
                                            ; Bit 0:   Granularity:  Multiplies limit by 4 KiB, to allow access all 4 GiB of memory
                                            ; Bit 1:   32-bit     :  Yes = 1
                                            ; Bit 2:   64-bit     :  No = 0
                                            ; Bit 3:   AVL        :  Reserved = 0.        
                                            ; ------------------------------------------------------------------------------------------
                                            ; Bit 4,7: 0xf        : last 4 bits of limit 0xfffff.  

        db 0x00                             ; Last byte of base. Base - 0x00000000
    
    GDT_end:                                ; Label that gives address of end of GDT

    GDT_Descriptor:      
        dw GDT_end - GDT_start_address - 1  ; Gives size of GDT table. Because GDT-end label comes AFTER last byte of GDT, GDT_end - GDT_start_address
                                            ; would give the size 1 byte larger than the real size. 
         dd GDT_start_address               ; GDT start address. (32 bit)

    code_segment_offset equ code_segment_descriptor - GDT_start_address
    data_segment_offset equ data_segment_descriptor - GDT_start_address