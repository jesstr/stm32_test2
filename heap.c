#include <stdio.h>
#include "heap.h"


/* Staticaly allocated heap buffer */
char heap_buf[HEAP_BUF_SIZE];
/* Bit is set if memory block is allocated, cleared if memory block is free */
int block_alloc_flags[HEAP_BLOCKS_COUNT / sizeof(int)];


/* Check if memory block is allocated */
int Heap_BlockIsAlloc(unsigned int block) {
	if (block_alloc_flags[block / (sizeof(int) * 8)] &
			(1 << block % (sizeof(int) * 8))) {
		return 1;
	}
	else {
		return 0;
	}
}


/* Set "allocated" flag of memory block */
inline void Heap_SetAllocFlag(unsigned int block) {
	block_alloc_flags[block / (sizeof(int) * 8)] |= (1 << block % (sizeof(int) * 8));
}


/* Clear "allocated" flag of memory block */
inline void Heap_ClearAllocFlag(unsigned int block) {
	block_alloc_flags[block / (sizeof(int) * 8)] &= ~(1 << block % (sizeof(int) * 8));
}


/* Alocate first unused memory block in the heap */
char *Heap_AllocBlock(unsigned int n) {
	int i;

	/* Find free block  */
	for (i = 0; i < HEAP_BLOCKS_COUNT; i++) {
		int j = i;
		while (!(Heap_BlockIsAlloc(j)) && (j < HEAP_BLOCKS_COUNT) ) {
			j++;
			if ((j - i) == n) {
				/* Mark n blocks as allocated */
				int k;
				for (k = i ; k < j ; k++) {
					Heap_SetAllocFlag(k);
				}
				/* Return pointer to first alocated block */
				return &heap_buf[i * HEAP_BLOCK_SIZE];
			}
		}
	}
	return NULL;
}


/* Free momory block */
void Heap_FreeBlock(char *ptr) {
	Heap_ClearAllocFlag((ptr - heap_buf) / HEAP_BLOCK_SIZE);
}
