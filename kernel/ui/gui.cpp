#include "../sys.h"
#include "../dvr/mouse.h"
#include "../dvr/vga.h"
#include "../mem/memmgr.h"
#include "../mem/page.h"

#include "../programs/program1.h"
#include "../programs/program2.h"
#include "../programs/program3.h"



s16 cursor_x = 160;
s16 cursor_y = 100;

s16 cursor_x_old = 0;
s16 cursor_y_old = 0;

u8 mouse_left = 0;
u8 mouse_middle = 0;
u8 mouse_right = 0;

s8 sens = 2;

u8 ccharacter = 0;
u8 newchar = 0;


u32 tempp;
u32 frontbuffer = 0xa0000;
u32 backbuffer = 0xa4b00;

//u8 hexstr[8];

u8 infocus = 0;


void frame(){
    // tell graphics driver where to write to video memory (in buffer 1 or buffer 2)
    update_buffer_loc(backbuffer);


    // change program based on mouse click
    if (infocus == 0){
        if (mouse_left == 1){
            if (cursor_y >= 72 && cursor_y <= (72+76)){
                if (cursor_x >= 016 && cursor_x <= (016+72)) {infocus = 1; vmm_pd1();}
                if (cursor_x >= 120 && cursor_x <= (120+72)) {infocus = 2;}
                if (cursor_x >= 224 && cursor_x <= (224+72)) {infocus = 3;}
            }
        }
    }


    // exec main gui frame draw, or frame draw for porgram, depending on what is in focus
    switch (infocus) {
        case 0:
            draw_rectangle(0, 0, 320, 240, 0x12);

            // if program running draw in diff colour
            draw_rectangle(8, 84, 72, 72, 0x00);
            draw_rectangle(16, 72, 72, 76, 0x34);

            draw_rectangle(112, 84, 72, 72, 0x00);
            draw_rectangle(120, 72, 72, 76, 0x34);

            draw_rectangle(216, 84, 72, 72, 0x00);
            draw_rectangle(224, 72, 72, 76, 0x34);

            print_text(148, 20, 0x34, "HOME");
            print_text(26, 106, 0x00, "Program 1");
            print_text(130, 106, 0x00, "Program 2");
            print_text(234, 106, 0x00, "Program 3");
            //print_text(40, 200, 0x34, "sethiabasicOS - by Mannan Sethia 2022");*/

            print_hex(140, 200, 0xf, infocus);
            print_hex(140, 210, 0xf, mouse_left);

            print_hex(200, 180, 0xf, givepd0addr());
            print_hex(200, 190, 0xf, givepd1addr());
            print_hex(200, 200, 0xf, givepd2addr());
            print_hex(200, 210, 0xf, givepd3addr());

            print_hex(200, 230, 0xf, pmm_get_pd());


            if ( ((cursor_x >= 16 && cursor_x <= (16+72)) || (cursor_x >= 120 && cursor_x <= (120+72)) || (cursor_x >= 224 && cursor_x <= (224+72))) && cursor_y >= 72 && cursor_y <= (72+76)) 
                {draw_cursor_clickable(cursor_x, cursor_y);}
            else    
                {draw_cursor(cursor_x, cursor_y);}

            break;

        case 1:
            p1_frame(cursor_x, cursor_y, mouse_left, mouse_middle, mouse_right, ccharacter, newchar);
    }
    
    


    // change video offset
    outpw(0x03d4, 0x0c | (backbuffer & 0xff00));
    outpw(0x03d4, 0x0d | (backbuffer << 8));


    // swap buffers
    tempp = frontbuffer;
    frontbuffer = backbuffer;
    backbuffer = tempp;

    // no new char
    newchar = 0;


    return;
}


// updates cursor position
void sys_mouse(mouse_data mouse){

    mouse_left = mouse.left;
    mouse_middle = mouse.middle;
    mouse_right = mouse.right;

    cursor_x_old = cursor_x;
    cursor_y_old = cursor_y;


    cursor_x = cursor_x + (mouse.x / sens);
    if (cursor_x < 0) {cursor_x = 0;};
    if (cursor_x > 319) {cursor_x = 319;};

    cursor_y = cursor_y + (mouse.y / sens);
    if (cursor_y < 0) {cursor_y = 0;};
    if (cursor_y > 239) {cursor_y = 239;};


    //frame();
}


void sys_kb(u8 character){

    newchar = 1;
    ccharacter = character;

}

void change_infocus(u8 no){
    if (no == 0){vmm_pd0();}
    if (no == 1){vmm_pd1();}
    if (no == 2){vmm_pd2();}
    if (no == 3){vmm_pd3();}

    infocus = no;
}

void quit(u8 no){
    if (no == 1){vmm_restorepd(1);}
    if (no == 2){vmm_restorepd(2);}
    if (no == 3){vmm_restorepd(3);}

    change_infocus(0);
}














//u32 mapaddr = 0x28004;
    /*for (u8 k = 0; k < 10; k++){
        print_hex(1,   1 + (k*10), 0xf, *reinterpret_cast<u32 *>(mapaddr + 4  + (k*24)));
        print_hex(49,  1 + (k*10), 0xf, *reinterpret_cast<u32 *>(mapaddr      + (k*24)));
        print_hex(104, 1 + (k*10), 0xf, *reinterpret_cast<u32 *>(mapaddr + 12  + (k*24)));
        print_hex(152, 1 + (k*10), 0xf, *reinterpret_cast<u32 *>(mapaddr + 8 + (k*24)));
        print_hex(210, 1 + (k*10), 0xf, *reinterpret_cast<u32 *>(mapaddr + 16 + (k*24)));
        print_hex(258, 1 + (k*10), 0xf, *reinterpret_cast<u32 *>(mapaddr + 20 + (k*24)));
    }*/

    //print_hex(5, 200, 0xf, *reinterpret_cast<u32 *>(0x28054));

    /*if (pageenable == 0) {print_text(90, 150, 0xf, "its not looking good brav");}
    if (pageenable == 1) {print_text(90, 150, 0x24, "big man ting");}
    

    for (u8 l = 0; l < 32; l++){
        u32 map = return_pmmb32(l);
        print_hex(0, l*9, 0xf, map);
    }*/
