# Virtual-Memory-Simulation

- va_splitting.h - Break down a virtual address into its components.
- mmu.c - Initialize any system- and memory access-related bookkeeping.
- proc.c - Initialize any process-related bookkeeping.
- page_fault.c - Implement the page fault handler.
- page_replacement.c - Implement the frame eviction algorithms FIFO and Clock Sweep algorithm. • stats.c - Calculate the Average Memory Access Time (AMAT)


- mmu.h - Defines the structures used by mmu.c
- proc.h - Defines the structures used by proc.c
- pagesim.h - Defines simulation parameters as well as global structures.
- pagesim.c - Reads a trace file of memory operations and calls each operation’s corresponding function implemented in proc.c.
- stats.h - Defines parameters that can be used when calculating AMAT.
- swap.c - Initializes functions to support a queue that are used in swapops.c
- swapops.c - Initializes functions to keep track of the frames swapped out to physical memory. Discussed in section 8
- types.h - Defines different types that are used throughout the simulation
- util.c - Initializes a random function used for the random replacement algorithm
