#ifndef memmgr_h
#define memmgr_h

#include "../sys.h"

extern u32 pmmb[32768];

void pmmb_set(u32 bit);
void pmmb_unset(u32 bit);
u32 pmmb_query(u32 bit);
u32 pmmb_findblock(u16 size);
u32 pmmb_findpage();

void pmmb_int15();

u32 return_pmmb32(u32 n);

#endif 