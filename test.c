#include <stdio.h>
#include "test.h"
#include "heap.h"
#include "main.h"
#include "stm32f10x_usart.h"


char str[256];


/* Test framework for Heap_AllocBlock() routine */
char *TestAlloc(size_t n) {
	char *ptr;
	UART_PrintStr("\r\n");
	if ((ptr = Heap_AllocBlock(n)) != NULL ) {
		sprintf(str, "TestAlloc(%d): %p\r\n \
				Memory blocks map:\r\n", n, ptr);
		//sprintf(str, "TestAlloc: ptr: %p\r\n", ptr);
		UART_PrintStr(str);
		PrintBlocksMapLite();
	}
	else {
		sprintf(str, "TestAlloc(%d): %p\r\n Allocation aborted. No free mem!\r\n", n, ptr);
		UART_PrintStr(str);
	}
	return ptr;
}


/* Test framework for AddItem() routine */
item2_t *TestAdd(char *title) {
	item2_t *item;

	if ((item = AddItem(1,1,1,title)) != NULL ) {
		sprintf(str, "\r\nTestAdd: %p\r\n", item);
		UART_PrintStr(str);
		sprintf(str, "n: %d, code: %d, price: %l, count: %d, title: %s\r\n",
				(*item).n, (*item).code, (*item).price, (*item).count, item->title);
		UART_PrintStr(str);

		UART_PrintStr("Memory blocks map:\r\n");
		PrintBlocksMapLite();
	}
	else {
		sprintf(str, "\r\nTestAdd: %p\r\n Allocation aborted. No free mem!\r\n", item);
		UART_PrintStr(str);
	}
	return item;
}


/* Test framework for Heap_FreeBlock() routine */
void TestFree(char *ptr) {
	Heap_FreeBlock(ptr);
	sprintf(str, "\r\nTestFree(%p)\r\nMemory blocks map:\r\n", ptr);
			UART_PrintStr(str);
	PrintBlocksMapLite();
}


/* Test framework for DelItem() routine */
void TestDel(item2_t *item) {
	DelItem(item);
	sprintf(str, "\r\nTestDel(%p)\r\nMemory blocks map:\r\n", item);
			UART_PrintStr(str);
	PrintBlocksMapLite();
}


/* Test framework for SearchItem() routine */
void TestSearch(unsigned short n) {
	sprintf(str, "\r\nSearchItem(%d): %p\r\n", n, (void *)SearchItem(n));
	UART_PrintStr(str);
}


/* Print extended map of memory blocks and its free/allocated flags */
void PrintBlocksMap(void) {
	int i;
	UART_PrintStr("\r\n[ ");
	for (i = 0; i < HEAP_BLOCKS_COUNT; i++) {
		sprintf(str, "%2d ", i);
		UART_PrintStr(str);
	}
	UART_PrintStr("]\r\n  ");

	for (i = 0; i < HEAP_BLOCKS_COUNT; i++) {
	if (Heap_BlockIsAlloc(i)) {
					UART_PrintStr(" * ");
			}
			else {
				UART_PrintStr("   ");
			}
	}
}


/* Print lite map of memory blocks and its free/allocated flags */
void PrintBlocksMapLite(void) {
	int i;
	for (i = 0; i < HEAP_BLOCKS_COUNT; i++) {
		sprintf(str, "Block %d flag: %d\r\n", i, Heap_BlockIsAlloc(i));
		UART_PrintStr(str);
	}
}


/* Print items chain */
void PrintItems(void) {
	item2_t *item = (item2_t *)(heap_buf);

	UART_PrintStr("\r\n");
	while ((*item).next_index != 0) {

		sprintf(str, "item: %p, next_item.n: %d, next_item.next_index: %d\r\n", item, (*item).n, (*item).next_index);
		UART_PrintStr(str);

		item = (item2_t *)( (*item).next_index * HEAP_BLOCK_SIZE + (char *)heap_buf );
	}

	sprintf(str, "item: %p,  next_item.n: %d, next_item.next_index: %d\r\n", item, (*item).n, (*item).next_index);
	UART_PrintStr(str);
}
