#include "../sys.h"
#include "../ui/gui.h"
#include "keyboard.h"
#include "vga.h"



// Some constants we must keep track of
u16 cursor_position;
u16 previous_char;
u8 number_of_repeat_inputs;
u8 shift;


// Table of values that converts keyboard scancode to ascii values.
u8 scancode_table[128] = {
    30,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0x06,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0xdd,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

// Keyboard handler
void keyboard_handler(){
    u8 scancode = inp(0x60);                                                    // get input

    u8 character = scancode_table[scancode];                                    // convert to char


    // Checks if highest bit of scanned byte is on. If this is on, it means key has been de-pressed
    if ((scancode & 0x80) == 0x80){                                             
        switch (scancode_table[scancode - 0x80]){
            case 0x06:                                      // If shift key was released, toggle off shift
                shift = 0;

            default:                                        // If a long-pressed key has been released, reset the long-press toggle
                number_of_repeat_inputs = 0;
                previous_char = 129;
        }
    }


    // if last char is same as current char, acknowledge
    else if ((character == previous_char) && (number_of_repeat_inputs <= 6)){     
        number_of_repeat_inputs++;

    }


    // if new character or long-press, type out character on new pixel. Set previous char
    else if (character != previous_char || number_of_repeat_inputs > 6){      
        switch (character){
            case 0x08:                                                                          // If backspace, go back one space and delete char
                cursor_position = cursor_position-1;
                *(u16*) (0xa0000 + (cursor_position)) = (0xb000 + 0x20);
                break;

            case 0x06:                                                                          // If shift, toggle on shift
                shift = 32;
                break;

            default:                                                                            // Types new char then increments cursor position
                /**(u16*) (0xa0000 + (cursor_position)) = (0xb000 + (character - shift));

                cursor_position++;
                previous_char = character;*/

                sys_kb(character);   
        }                          
    }
  
  return;
}

u32 pos = 0xa0000;

void wait_out(){
        for (u32 timeout = 0; timeout <= 100000; timeout++){
        if ( (inp(0x64) && 2) == 1) {return;}
    }
}

void wait_in(){
        for (u32 timeout = 0; timeout <= 100000; timeout++){
        if ( (inp(0x64) && 1) == 1) {return;}
    }
}

//void keyboard_handler(){
  //*(u16*) pos = 0x0f;
  //pos++;
//}

void install_keyboard(){
  wait_out();
  outp(0x60, 0xf3);
  wait_in();
  inp(0x60);
  wait_out();
  outp(0x60, 0b00111100);  // Set typematic rate

  wait_out();
  outp(0x60, 0xf4);
  wait_in();
  inp(0x60);


}