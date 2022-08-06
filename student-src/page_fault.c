#include "mmu.h"
#include "pagesim.h"
#include "swapops.h"
#include "stats.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

/**
 * --------------------------------- PROBLEM 6 --------------------------------------
 * Checkout PDF section 7 for this problem
 * 
 * Page fault handler.
 * 
 * When the CPU encounters an invalid address mapping in a page table, it invokes the 
 * OS via this handler. Your job is to put a mapping in place so that the translation 
 * can succeed.
 * 
 * @param addr virtual address in the page that needs to be mapped into main memory.
 * 
 * HINTS:
 *      - You will need to use the global variable current_process when
 *      altering the frame table entry.
 *      - Use swap_exists() and swap_read() to update the data in the 
 *      frame as it is mapped in.
 * ----------------------------------------------------------------------------------
 */
void page_fault(vaddr_t addr) {
   // TODO: Get a new frame, then correctly update the page table and frame table

   pfn_t get_frame_freed = free_frame(); //Using free_frame() get the PFN of a new frame.

   pte_t *ptable_entry_pointer = (pte_t*)(mem + (PTBR * PAGE_SIZE)) + vaddr_vpn(addr);

   pte_t *new_pte = (pte_t*)(mem + (get_frame_freed * PAGE_SIZE));

   if(swap_exists(ptable_entry_pointer)){
      swap_read(ptable_entry_pointer, new_pte);
   } else {
      //clear the new frame
      memset(new_pte, 0, PAGE_SIZE);
   }

   ptable_entry_pointer->valid = 1;
   ptable_entry_pointer->dirty = 0;
   ptable_entry_pointer->pfn = get_frame_freed;

   fte_t *new_frame_table_entry = get_frame_freed + frame_table;
   new_frame_table_entry->process = current_process;
   new_frame_table_entry->mapped = 1;
   new_frame_table_entry->vpn = vaddr_vpn(addr);
}

#pragma GCC diagnostic pop
