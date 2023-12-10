#ifndef gui_h
#define gui_h

#include "../dvr/mouse.h"

void sys_mouse(mouse_data mouse);

void sys_kb(u8 character);

void frame();

void pageishere();

void change_infocus(u8 no);

void quit(u8 no);

//extern u16 fps = 60;

#endif