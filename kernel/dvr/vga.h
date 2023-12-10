#ifndef vga_h
#define vga_h

void unchain();

void update_buffer_loc(u32 loc);


void draw_pixel(u16 x, u16 y, u8 rgb);
void draw_4(u16 x, u16 y, u8 rgb);

void draw_rectangle(u16 x, u16 y, u16 w, u16 h, u8 rgb);

void draw_hor_line(u16 x, u16 y, u16 w, u8 rgb);
void draw_hor_line_exact(u16 x, u16 y, u16 w, u8 rgb);

void draw_vert_line(u16 x, u16 y, u16 h, u8 rgb);



void draw_cursor(u16 x, u16 y);
void draw_cursor_clickable(u16 x, u16 y);


void print_text(u16 x, u16 y, u8 rgb, const char* txt);
void print_hex(u16 x, u16 y, u8 rgb, u32 data);

#endif