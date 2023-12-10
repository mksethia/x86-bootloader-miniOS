#ifndef SYS_H
#define SYS_H


typedef unsigned char u8;
typedef signed char s8;

typedef unsigned short u16;
typedef signed short s16;

typedef unsigned long u32;
typedef signed long s32;

void memcpy(void *dest, const void *src, u32 len);

void outp(u16 port, u8 data);

void outpw(u16 port, u16 data);

u8 inp(u16 port);





#endif