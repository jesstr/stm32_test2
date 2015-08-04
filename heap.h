#ifndef __HEAP_H
#define __HEAP_H


#define HEAP_BLOCK_SIZE		16
#define HEAP_BLOCKS_COUNT	16
#define HEAP_BUF_SIZE		HEAP_BLOCK_SIZE * HEAP_BLOCKS_COUNT

extern char heap_buf[];
/* Bit is set if block is allocated, cleared if block is free */
extern int block_alloc_flags[];


inline int Heap_BlockIsAlloc(unsigned int block);

inline void Heap_SetAllocFlag(unsigned int block);

inline void Heap_ClearAllocFlag(unsigned int block);

char *Heap_AllocBlock(unsigned int n);

void Heap_FreeBlock(char *ptr);



#endif /* __HEAP_H */
