#ifndef __HEAP_H
#define __HEAP_H


#define HEAP_BLOCK_SIZE		16
#define HEAP_BLOCKS_COUNT	16
#define HEAP_BUF_SIZE		HEAP_BLOCK_SIZE * HEAP_BLOCKS_COUNT


/* Staticaly allocated heap buffer */
extern char heap_buf[];
/* Bit is set if block is allocated, cleared if block is free */
extern int block_alloc_flags[];


/* Check if memory block is allocated */
int Heap_BlockIsAlloc(unsigned int block);
/* Set "allocated" flag of memory block */
inline void Heap_SetAllocFlag(unsigned int block);
/* Clear "allocated" flag of memory block */
inline void Heap_ClearAllocFlag(unsigned int block);
/* Alocate first unused memory block in the heap */
char *Heap_AllocBlock(unsigned int n);
/* Free momory block */
void Heap_FreeBlock(char *ptr);


#endif /* __HEAP_H */
