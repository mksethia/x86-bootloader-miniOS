#include "../sys.h"
#include "vga.h"
#include "mouse.h"
#include "../ui/gui.h"

u8 mouse_ticks = 0; // how many mouse movements?
u8 mouse_cycle = 252; // which byte of mouse data?
u8 temp;            // temporary variable for storing data

mouse_data mouse = {0};

u8 test;

void mouse_handler(){
    mouse_ticks++;
    //draw_pixel(mouse_ticks, 100, 0x2c);    // shows that an IRQ12 has been recieved

    switch (mouse_cycle){                  // Each mouse movement sends 3 interrupts for each byte sent to the PS/2 controller. Hence we must handle each individually
        case 0:
            temp = inp(0x60);

            //mouse.y = (((temp & 0b00100000) >> 5) * -2) + 1;  // set negative or positive
            //mouse.x = (((temp & 0b00010000) >> 4) * -2) + 1;  // set negative or positive

            mouse.left = temp & 0b00000001;
            mouse.right = (temp & 0b00000010) >> 1;
            mouse.middle = (temp & 0b00000100) >> 2;

            
            mouse_cycle++;
            break;

        case 1:
            temp = inp(0x60);

            mouse.x = temp;

            mouse_cycle++;
            break;

        case 2:
            temp = inp(0x60);
            mouse.y = temp;
            mouse.y *= -1;

            sys_mouse(mouse);
            
            mouse_cycle = 0;
            break;

        default: 
            mouse_cycle++;

    };
}

// Wait for PS/2 controller to OK a recieve/send byte
void mouse_wait_out(){
    for (u32 timeout = 0; timeout <= 100000; timeout++){
        if ( (inp(0x64) && 2) == 1) {return;}
    }
    //draw_pixel(160, 100, 0x0f); // shows timeout
    return;
}
void mouse_wait_in(){
    for (u32 timeout = 0; timeout <= 100000; timeout++){
        if ( (inp(0x64) && 1) == 1) {return;}
    }
    //draw_pixel(160, 100, 0x0f);
    return;
}

// Function that waits to send byte saying we will send a byte, then waits to send byte with command, then waits for ack
void mouse_write(u8 data){
    mouse_wait_out();
    outp(0x64, 0xd4);

    mouse_wait_out();
    outp(0x60, data);

    mouse_wait_in();
    inp(0x60);
}

// Function that waits to recieve a byte then recieves the byte
u8 mouse_read(){
    mouse_wait_in();
    return inp(0x60);
}

void enable_mouse(){ 
    u8 mouse_status;
    u8 transform_byte_out;

    mouse_wait_out();
    outp(0x64, 0xa8);                                        
    mouse_wait_in();
    inp(0x60);                                             // Enable aux mouse device (not necessarily required but recommended), receive ack from keyboard

    mouse_wait_out();
    outp(0x64, 0x20);                                       // Asks for compaq status of mouse
    mouse_wait_in();
    mouse_status = inp(0x60);   
    transform_byte_out = (mouse_status | 2);                // Gets 'compaq status' of mouse, transforms it to enable mouse

    mouse_wait_out();
    outp(0x64, 0x60);
    mouse_wait_out();
    outp(0x60, transform_byte_out);                         // Tells PS/2 chip that compaq status is to be changed then send transformed byte

    mouse_write(0xf6);                                      // Mouse uses default settings

    mouse_write(0xe8);                                      // Mouse uses resolution 2 counts / mm
    mouse_wait_out();
    outp(0x60, 0x00);
    mouse_wait_in();
    inp(0x60);

    mouse_write(0xf3);                                      // Mouse uses sample rate 60/sec.
    mouse_wait_out();
    outp(0x60, 60);
    mouse_wait_in();
    inp(0x60);


    mouse_write(0xf4);                                      // Mouse enabled

    //mouse_wait_in();

    return;
}




