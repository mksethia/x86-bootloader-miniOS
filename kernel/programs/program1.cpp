#include "../sys.h"

#include "../dvr/keyboard.h"
#include "../dvr/vga.h"
#include "../dvr/mouse.h"
#include "../ui/gui.h"
#include "../mem/page.h"
#include "../mem/memmgr.h"

#include "program1.h"

// Press ESC to go back to homepage. Press DELETE to quit.

u32 indend = 0;

u8* cha = reinterpret_cast<u8*>(0x109000);


void p1_frame(u32 mousex, u32 mousey, u32 mouse1, u32 mouse2, u32 mouse3, u8 character, u32 newchar){
    draw_rectangle(0, 0, 320, 240, 0x40);
    draw_rectangle(0, 0, 120, 120, 0x56);


    
    if (newchar){
        if (character == 0x1b) {change_infocus(0);}
        if (character == 0xdd) {quit(0);}


        *cha = character;
    }

    print_hex(0, 0, 0xf, *cha);

    print_hex(200, 200, 0xf, pmm_get_pd());

    //ESC character = 0x1b

    draw_cursor(mousex, mousey);
}
