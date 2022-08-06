#include "mmu.h"
#include "pagesim.h"
#include "va_splitting.h"
#include "swapops.h"
#include "stats.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

/* The frame table pointer. You will set this up in system_init. */
fte_t *frame_table;

/**
 * --------------------------------- PROBLEM 2 --------------------------------------
 * Checkout PDF sections 4 for this problem
 * 
 * In this problem, you will initialize the frame_table pointer. The frame table will
 * be located at physical address 0 in our simulated memory. You should zero out the 
 * entries in the frame table, in case for any reason physical memory is not clean.
 * 
 * HINTS:
 *      - mem: Simulated physical memory already allocated for you.
 *      - PAGE_SIZE: The size of one page
 * ----------------------------------------------------------------------------------
 */
void system_init(void) {
    // TODO: initialize the frame_table pointer.
    frame_table = (fte_t*)mem; //allocate the pysical memory
    memset(frame_table, 0, PAGE_SIZE); //place the frame table in physical frame 0
    frame_table->protected = 1; //Don’t forget to mark the first entry of the frame table as “protected”
}

/**
 * --------------------------------- PROBLEM 5 --------------------------------------
 * Checkout PDF section 6 for this problem
 * 
 * Takes an input virtual address and performs a memory operation.
 * 
 * @param addr virtual address to be translated
 * @param rw   'r' if the access is a read, 'w' if a write
 * @param data If the access is a write, one byte of data to write to our memory.
 *             Otherwise NULL for read accesses.
 * 
 * HINTS:
 *      - Remember that not all the entry in the process's page table are mapped in. 
 *      Check what in the pte_t struct signals that the entry is mapped in memory.
 * ----------------------------------------------------------------------------------
 */
uint8_t mem_access(vaddr_t addr, char rw, uint8_t data) {
    stats.accesses++;
    // TODO: translate virtual address to physical, then perfrom the specified operation
    vpn_t vpn = vaddr_vpn(addr);
    uint16_t offset = vaddr_offset(addr);

    pte_t* ptable_entry_pointer = (pte_t*)(mem + (PTBR * PAGE_SIZE)) + vpn; //base + ??
    
    if (!ptable_entry_pointer->valid) { //when that page table endtry is invalid
        stats.page_faults++;
        page_fault(addr);
    }

    (frame_table + (ptable_entry_pointer->pfn))->mapped = 1; //
    (frame_table + (ptable_entry_pointer->pfn))->process = current_process; //
    (frame_table + (ptable_entry_pointer->pfn))->vpn = vpn; //
    (frame_table + (ptable_entry_pointer->pfn))->referenced = 1; //check into the frmae table

    paddr_t translated_address = (paddr_t)(ptable_entry_pointer->pfn << OFFSET_LEN) | offset;
    /* Either read or write the data to the physical address
       depending on 'rw' */
    uint8_t carry = *(mem + translated_address); //to carry read from pysical memory or wirtting
    if (rw == 'r') { //read
        return carry;
    } else {
        *(mem + translated_address) = data;
        ptable_entry_pointer->dirty = 1;
        return *(mem + translated_address);
    }
}
