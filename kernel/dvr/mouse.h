#ifndef mouse_h
#define mouse_h

void mouse_handler();

void enable_mouse();

struct mouse_data{
    s8 x;
    s8 y;
    u8 left;
    u8 right;
    u8 middle; 
};

#endif 