#include "../sys.h"
#include "memmgr.h"

// There exists up to 4096MB RAM
// so we have bitmap of 4096MB RAM into 4096B chunks: 1024^2 blocks = 1048576 bits = 32768 x 32bit arrays
// 0 = free, 1 = in use

u32 pmmb[32768]; 

// physical memory manager bitmap
// memory map exists at 0x00028004 as defined in boot.asm


// change nth bit to 1
void pmmb_set(u32 bit){
    u32 locinarray = bit / 32;
    u32 locinu32 = bit % 32;

    pmmb[locinarray] = pmmb[locinarray] | (0b1 << locinu32);
}

// change nth bit to 0
void pmmb_unset(u32 bit){
    u32 locinarray = bit / 32;
    u32 locinu32 = bit % 32;

    pmmb[locinarray] = pmmb[locinarray] & (~(0b1 << locinu32));
}

// query nth bit
u32 pmmb_query(u32 bit){
    u32 locinarray = bit / 32;
    u32 locinu32 = bit % 32;

    u32 test = pmmb[locinarray] & (0b1 << locinu32);
    
    if (test) {return 1;}
    else {return 0;}
}

// find free region of n blocks
/*u16 pmmb_findblock(u16 size){
    for (u8 i = 0; i < 32; i++){

        // if found block not full
        if (pmmb[i] != 0xffffffff){

            
            // find first free bit in block
            for (u8 j = 0; j < 32; j++){

                // when found 
                if (pmmb_query(i*32 + j) == 0) {
                    
                    // test whether subsequent bits are free
                    // u8 extra_blocks_needed_to_check = (j + size) / 32
                    u8 current_i = i;
                    u16 tested_bits;
                    for (u16 k = j+1; k <= size+j; k++){
                                                
                        // check if we are in next u32 of bitmap array
                        if ( (k % 32) == 0) {current_i++;}

                        // test next bit...
                        if (pmmb_query(current_i*32 + k) == 1) {break;}

                        tested_bits = k;
                    }

                    if (tested_bits = size+j) {return i*32 + j;}
                    return 0;
                }              
            }
        }  
    }
    return 0;
}*/



u32 pmmb_findpage(){

    //Iterate over pmmb u32s
    for (u32 i = 0; i < 32768; i++){

        // Find non-full u32 
        if (pmmb[i] != 0xffffffff){

            // Iterate over each bit in u32
            for (u8 j = 0; j < 32; j++){

                // When found free block, return position
                if (( (pmmb[i]) & (0b1 << j)) == 0) {
                    pmmb_set(i*32 + j);
                    return i*32 + j;
                }
            }
        }
    }

    // If fail
    return 0;
}



void pmmb_int15(){
    u32 one_block = 0x1000;

    for (u32 i = 0; i < 256; i++){
        pmmb_set(i);
    }

    u32 free_region_end = *reinterpret_cast<u32 *>(0x28054);

    u32 free_region_end_bit = free_region_end / one_block;

    for (u32 i = free_region_end_bit; i < 1048576; i++){
        pmmb_set(i);
    }
}


u32 return_pmmb32(u32 n){
    return pmmb[n];
}

