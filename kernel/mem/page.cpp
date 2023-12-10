#include "../sys.h"
#include "../ui/gui.h"
#include "page.h"
#include "memmgr.h"


// vmm = virtual memory manager

// current page directory
u32 currentpdaddr;

// pagedirs
pagedirectory* pd0 = reinterpret_cast<pagedirectory*>(0x101000);
pagedirectory* pd1 = reinterpret_cast<pagedirectory*>(0x102000);
pagedirectory* pd2 = reinterpret_cast<pagedirectory*>(0x103000);
pagedirectory* pd3 = reinterpret_cast<pagedirectory*>(0x104000);


// some macros 
const u32 ptepresent        = 0b001;
const u32 ptewriteable      = 0b010;
const u32 pteuser           = 0b100;


const u32 pdepresent        = 0b00000001;
const u32 pdewriteable      = 0b00000010;
const u32 pdeuser           = 0b00000100;
const u32 pdewritethrough   = 0b00001000;
const u32 pdecached         = 0b00010000;
const u32 pde4mbpages       = 0b10000000;


// more macros
u32 return_pdir_entry_no(u32 vaddr){return (vaddr & 0xffc00000);}
u32 return_ptable_entry_no(u32 vaddr){return (vaddr & 0x3ff000);}
u32 return_addr_in_page(u32 vaddr){return (vaddr & 0xfff);}



void pmm_load_pd(pagedirectory* pd){
    //u32 addr = reinterpret_cast<u32>(&pd);

    __asm__(
        "mov %%eax, %%cr3\n\t"
        :
        :"a"(pd)
    );
}


// get current pd from cr3?
u32 pmm_get_pd(){
    u32 addr;

    __asm__(
        "mov %%cr3, %%eax\n\t"
        :"=a"(addr)
    );

    return addr;
}


// alloc page
void vmm_alloc_page(u32* ptentry){
    // find free block
    u32 free_physical_memory_block_address = pmmb_findpage() * 0x1000;

    // map to page
    *ptentry += ptepresent;
    *ptentry += ptewriteable;
    *ptentry += free_physical_memory_block_address;
}


// free page
void vmm_free_page(u32* ptentry){
    // get addr of free page
    u32 addr_of_freed_page = *ptentry & ~0x111;

    // clear present and writeable bits
    *ptentry -= ptepresent;
    *ptentry -= ptewriteable;

    // free physical memory
    pmmb_unset(addr_of_freed_page / 0x1000);
}


// returns address of (entry no. x in pt)
u32* vmm_return_ptentry(pagetable* pt, u32 vaddr){
    return &(pt->entry[return_ptable_entry_no(vaddr)]);
}


// returns address of (entry no. x in pd)
u32* vmm_return_pdentry(pagedirectory* pd, u32 vaddr){
    return &(pd->entry[return_pdir_entry_no(vaddr)]);
}


// change pd..
void vmm_change_pd(pagedirectory* pd){
    currentpdaddr = reinterpret_cast<u32>(pd);
    pmm_load_pd(pd);
}


// get current addr of pd from cr3
pagedirectory* vmm_get_pd(){
    return reinterpret_cast<pagedirectory*>(pmm_get_pd());
}


// flush entry x in pt from invlpg 
void flush_page_entry(u32 vaddr){
    /*__asm__(
        "cli\n\t"
        "invlpg %%eax\n\t"
        "sti\n\t"
        :
        :"a"(vaddr)
    );*/
}


// maps x physical addr to y virtual addr
void vmm_map_page(u32 paddr, u32 vaddr, pagedirectory* pd){

    // get pdentry
    u32* pdentry = vmm_return_pdentry(pd, vaddr);

    // if not present, alloc
    if ((*pdentry & pdepresent) != pdepresent){

        // find free 4kb block for page table to sit in
        pagetable* newtable = reinterpret_cast<pagetable*>(pmmb_findpage() * 0x1000);

        // zero pagetable
        for (u32 i = 0; i < 1024; i++){newtable->entry[i] = 0;}

        // add entry into pd
        *pdentry += reinterpret_cast<u32>(newtable);
        *pdentry += pdepresent;
        *pdentry += pdewriteable; 
    }

    // get pagetable  
    pagetable* table = reinterpret_cast<pagetable*>(*pdentry & ~0xfff);

    // get ptentry
    u32* ptentry = &(table->entry[return_ptable_entry_no(vaddr)]);

    // map paddr to ptentry
    *ptentry += paddr & ~0xfff;
    *ptentry += ptepresent;
    *ptentry += ptewriteable;

}


// funcs to change pd
void vmm_pd0(){vmm_change_pd(pd0);}
void vmm_pd1(){vmm_change_pd(pd1);}
void vmm_pd2(){vmm_change_pd(pd2);}
void vmm_pd3(){vmm_change_pd(pd3);}


// func to restore pd back to default
void vmm_restorepd(u8 no){
    if (no == 1){
        for (u32 i = 0; i < 1024; i++)
            pd1->entry[i] = 0;
    }
}



// init...
void vmm_init(){

    // default table
    pagetable* table1 = reinterpret_cast<pagetable*>(pmmb_findpage() * 0x1000);

    // alloc page table @ 3G
    // pagetable* table2 = reinterpret_cast<pagetable*>(pmmb_findpage() * 0x1000);

    // zero table1
    for (u32 i = 0; i < 1024; i++){table1->entry[i] = 0;}


    // identity map up first table to 1MB
    for (u32 i = 0; i < 261; i++){
        u32 ptentry = ptepresent;
        ptentry += ptewriteable;
        ptentry += i*4096;

        table1->entry[i] = ptentry;
    }


    // default pdir + pdir for programs 1-3, store addresses of pd0-3
    // pagedirectory* pd0 = reinterpret_cast<pagedirectory*>(pmmb_findpage() * 0x1000);
    // pagedirectory* pd1 = reinterpret_cast<pagedirectory*>(pmmb_findpage() * 0x1000);
    // pagedirectory* pd2 = reinterpret_cast<pagedirectory*>(pmmb_findpage() * 0x1000);
    // pagedirectory* pd3 = reinterpret_cast<pagedirectory*>(pmmb_findpage() * 0x1000);
    // pd0addr = reinterpret_cast<u32>(pd0);
    // pd1addr = reinterpret_cast<u32>(pd1);
    // pd2addr = reinterpret_cast<u32>(pd2);
    // pd3addr = reinterpret_cast<u32>(pd3);


    // zero pdirs
    for (u32 i = 0; i < 1024; i++){pd0->entry[i] = 0;}
    for (u32 i = 0; i < 1024; i++){pd1->entry[i] = 0;}
    for (u32 i = 0; i < 1024; i++){pd2->entry[i] = 0;}
    for (u32 i = 0; i < 1024; i++){pd3->entry[i] = 0;}


    // set entry 0 of pd0,1,2,3 to table we created 
    pd0->entry[0] += pdepresent;
    pd0->entry[0] += pdewriteable;
    pd0->entry[0] += reinterpret_cast<u32>(table1);

    table1->entry[265] += ptepresent;
    table1->entry[265] += ptewriteable;
    table1->entry[265] += 0x200000;

    pd1->entry[0] += pdepresent;
    pd1->entry[0] += pdewriteable;
    pd1->entry[0] += reinterpret_cast<u32>(table1);

    pd2->entry[0] += pdepresent;
    pd2->entry[0] += pdewriteable;
    pd2->entry[0] += reinterpret_cast<u32>(table1);

    pd3->entry[0] += pdepresent;
    pd3->entry[0] += pdewriteable;
    pd3->entry[0] += reinterpret_cast<u32>(table1);

    

    // switch to current pd
    vmm_change_pd(pd0);

    // enable paging
    __asm__(
        "mov %%cr0, %%eax\n\t"
        "orl $0x80000000, %%eax\n\t"
        "mov %%eax, %%cr0\n\t"
        :
    );

    //pageishere();

}


u32 givepd0addr(){return reinterpret_cast<u32>(pd0);}
u32 givepd1addr(){return reinterpret_cast<u32>(pd1);}
u32 givepd2addr(){return reinterpret_cast<u32>(pd2);}
u32 givepd3addr(){return reinterpret_cast<u32>(pd3);}


// *currentpd 
// *pdentry
// *ptentry
// *newtable





















