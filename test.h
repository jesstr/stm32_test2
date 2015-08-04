#ifndef __TEST_H
#define __TEST_H


#include <stdio.h>
#include "main.h"


/* Test framework for Heap_AllocBlock() routine */
char *TestAlloc(size_t n);
/* Test framework for Heap_FreeBlock() routine */
void TestFree(char *ptr);
/* Test framework for AddItem() routine */
item2_t *TestAdd(char *title);
/* Test framework for DelItem() routine */
void TestDel(item2_t *item);
/* Test framework for SearchItem() routine */
void TestSearch(unsigned short n);
/* Print extended map of memory blocks and its free/allocated flags */
void PrintBlocksMap(void);
/* Print lite map of memory blocks and its free/allocated flags */
void PrintBlocksMapLite(void);
/* Print items chain */
void PrintItems(void);


#endif /* __TEST_H */
