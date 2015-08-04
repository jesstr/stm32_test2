#ifndef __TEST_H
#define __TEST_H

#include <stdio.h>
#include "main.h"

char *TestAlloc(size_t n);

void TestFree(char *ptr);

item2_t *TestAdd(char *title);

void TestDel(item2_t *item);

void TestSearch(unsigned short n);

void PrintBlocksMap(void);

void PrintBlocksMapLite(void);

void PrintItems(void);


#endif /* __TEST_H */
