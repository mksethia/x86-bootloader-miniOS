#ifndef page_h
#define page_h


struct pagetable{
    u32 entry[1024];
};

struct pagedirectory{
    u32 entry[1024];
};

void vmm_init();

void vmm_change_pd(pagedirectory* pd);


void vmm_pd0();
void vmm_pd1();
void vmm_pd2();
void vmm_pd3();

void vmm_restorepd(u8 no);


u32 givepd0addr();
u32 givepd1addr();
u32 givepd2addr();
u32 givepd3addr();

u32 pmm_get_pd();

#endif