#include <stdio.h>
#include "heap.h"

char heap_buf[HEAP_BUF_SIZE];
/* Bit is set if block is allocated, cleared if block is free */
int block_alloc_flags[HEAP_BLOCKS_COUNT / sizeof(int)];



inline int Heap_BlockIsAlloc(unsigned int block) {
	if (block_alloc_flags[block / (sizeof(int) * 8)] &
			(1 << block % (sizeof(int) * 8))) {
		return 1;
	}
	else {
		return 0;
	}
}


inline void Heap_SetAllocFlag(unsigned int block) {
	block_alloc_flags[block / (sizeof(int) * 8)] |= (1 << block % (sizeof(int) * 8));
}


inline void Heap_ClearAllocFlag(unsigned int block) {
	block_alloc_flags[block / (sizeof(int) * 8)] &= ~(1 << block % (sizeof(int) * 8));
}


char *Heap_AllocBlock(unsigned int n) {
	int i;

//	/* Find free block  */
//	for (i = 0; i < HEAP_BUF_SIZE; i+=HEAP_BLOCK_SIZE) {
//		int j = i;
//		int n;
//		while (!Heap_BlockIsAlloc(j+=HEAP_BLOCK_SIZE)) {
//			if ((j - i) == n * HEAP_BLOCK_SIZE) {
//				/* Mark n blocks as allocated */
//				int k;
//				for (k = i ; k < j ; k+=HEAP_BLOCK_SIZE) {
//					Heap_SetAllocFlag(k % HEAP_BLOCK_SIZE);
//				}
//				/* Return pointer to first alocated block */
//				return &heap_buf[i];
//			}
//		}
//	}

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

//	if (heap + n > heap_buf + HEAP_BUF_SIZE) {
//		/* Out of memory */
//		return NULL;
//	}
//
//	prev_heap = heap;
//	heap += n;
//
//	return prev_heap;

	return NULL;
}


void Heap_FreeBlock(char *ptr) {
	Heap_ClearAllocFlag((ptr - heap_buf) / HEAP_BLOCK_SIZE);
}
