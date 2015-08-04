
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "main.h"
#include "misc.h"
#include "test.h"
#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ITEM_NUMBER_MASK	0xFFF

/* String buffer for UART text output */
char str[256];
/* Pointer to last added item */
item2_t *prev_item = NULL;


/* GPIO_Configuration */
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

    /* Configure USART2 Tx (PA.02) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure USART2 Rx (PA.03) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure LEDs connected to PC.8, PC.9 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}


/* USART_Configuration */
void USART_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;

	/* Fill USART_InitStructure with default values
	* (9600, 8 bit, 1 stop bit, no flow control) */
	USART_StructInit(&USART_InitStructure);
	/* Set baudrate to 115200 */
	USART_InitStructure.USART_BaudRate = 115200;

	/* Init USART2 */
	USART_Init(USART2, &USART_InitStructure);
}


/* NVIC_Configuration */
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the USART2 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


/* RCC_Configuration */
void RCC_Configuration(void)
{
	/* Enable USART2 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* Enable GPIOA and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE);
}


/* Print string over UART */
void UART_PrintStr(char *str) {
	while (*str) {
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET) {
		}
		USART_SendData(USART2, *str++);
	}
}


/* Add item */
item2_t *AddItem(unsigned long code, unsigned long price, unsigned short count, char *title) {
	char *block;
	item2_t *item;
	static unsigned short number = 0;
	unsigned char title_length;

	if ((block = Heap_AllocBlock(1)) == NULL) {
		/* No free mem, return */
		return NULL;
	}

	item = (item2_t *)block;

	/* Calculate number of memory blocks needed to store the title */
	title_length = (strlen(title) - 1) / HEAP_BLOCK_SIZE + 1;

	if ((block = Heap_AllocBlock(title_length)) == NULL) {
		/* No free mem, free first allocated for item block, return */
		Heap_FreeBlock((char *)item);
		return NULL;
	}

	/* Fill item structure */
	(*item).n = (number++) & ITEM_NUMBER_MASK;
	(*item).code = code;
	(*item).price = price;
	(*item).count = count;
	(*item).title = block;
	(*item).next_index = 0;
	(*item).title_length = title_length;

	/* Fill title */
	strcpy((*item).title, title);

	/* Set link from previous item */
	if (prev_item != NULL) {
		(*prev_item).next_index = (unsigned long)( (char *)item - (char *)heap_buf ) / HEAP_BLOCK_SIZE;
	}
	/* Store item as last added */
	prev_item = item;

	return item;
}


/* Delete item */
int DelItem(item2_t *item) {
	int i;
	item2_t *next_item;

	if (item == NULL) {
		return 0;
	}

	/* Free memory blocks, allocated for items title */
	for (i = 0; i < (*item).title_length; i++) {
		Heap_FreeBlock(item->title + HEAP_BLOCK_SIZE * i);
	}

	if ((*item).next_index != 0 ) {
		/* If item is not last item in the chain */

		/* Copy data from next item, reset links, then delete next item */
		next_item = (item2_t *)((*item).next_index * HEAP_BLOCK_SIZE + (char *)heap_buf);

		(*item).next_index = (*next_item).next_index;
		(*item).code = (*next_item).code;
		(*item).n = (*next_item).n;
		(*item).count = (*next_item).count;
		(*item).title = (*next_item).title;
		(*item).title_length = (*next_item).title_length;
		(*item).price = (*next_item).price;

		Heap_FreeBlock((char *)next_item);

		/* If next item was last, store this item as last */
		if (next_item == prev_item) {
			prev_item = item;
		}
	}
	else {
		/* If item was last in the chain, reset previous item links and delete item */

		item2_t *pref = FindPrevItem(item);

//		sprintf(str, "\r\n\t: %p, (*pref).next_index: %d\r\n", pref, (*pref).next_index);
//		UART_PrintStr(str);

		(*pref).next_index = 0;
		prev_item = pref;

//		sprintf(str, "\r\nprev_item: %p, prev_item.next_index: %d\r\n", prev_item, (*prev_item).next_index);
//		UART_PrintStr(str);

		Heap_FreeBlock((char *)item);
	}

	return 0;
}


/* Find previos item in the chain */
item2_t *FindPrevItem(item2_t *ref) {
	item2_t *item = (item2_t *)(heap_buf);
	item2_t *prev = NULL;

	while ((*item).next_index != 0) {

		if (item == ref) {
			return prev;
		}
		prev = item;
		item = (item2_t *)((*item).next_index * HEAP_BLOCK_SIZE + (char *)heap_buf);
	};
	if (item == ref) {
		return prev;
	}
	else {
		return NULL;
	}
}


/* Search item by number */
item2_t *SearchItem(unsigned short n) {
	item2_t *item = (item2_t *)(heap_buf);

	while ((*item).next_index != 0) {

		if ((*item).n == n) {
			return item;
		}
		item = (item2_t *)((*item).next_index * HEAP_BLOCK_SIZE + (char *)heap_buf);
	};
	if ((*item).n == n) {
		return item;
	}
	else {
		return NULL;
	}
}


/* Main routine */
int main(void)
{
	RCC_Configuration();
	NVIC_Configuration();
	GPIO_Configuration();
	USART_Configuration();

	/* Enable USART2 */
	USART_Cmd(USART2, ENABLE);

	#if 0
	/* Tests for memory blocks allocation, for debug only */
	char *ptr = NULL;

	TestAlloc(3);

	ptr = TestAlloc(2);

	TestAlloc(2);

	TestFree(ptr);
	TestFree(ptr + HEAP_BLOCK_SIZE);

	ptr = TestAlloc(2);

	TestFree(ptr);
	TestFree(ptr + HEAP_BLOCK_SIZE);

	TestAlloc(4);

	TestAlloc(3);

	TestAlloc(0);

	TestAlloc(2);

	TestAlloc(1);
	#endif

	/* Tests for items add/delete/search routines */
	item2_t *item = NULL;

	/* Add item with 48 char title */
	TestAdd("0123456789abcde0123456789abcde0123456789abcde");

	/* Add item with 32 char title */
	item = TestAdd("0123456789abcde0123456789abcde");

	/* Add item with 32 char title */
	TestAdd("0123456789abcde0123456789abcde");

	/* Delete item in middle of chain */
	TestDel(item);

	/* Add item with 32 char title in an empty gap */
	item = TestAdd("0123456789abcde0123456789abcde");

	/* Delete added in the gap item */
	TestDel(item);

	/* Add item with 64 char title.
	 * Should fill 1 block in the gap and others at the end of heap */
	TestAdd("0123456789abcde0123456789abcde0123456789abcde0123456789abcde");

	/* Add item with 32 char title.
	 * Should fill 1 block in the gap and others at the end of heap */
	item = TestAdd("0123456789abcde0123456789abcde");

	/* Add item with 1 char title.
	 * There are no free space in heap.
	 * Should be aborted */
	TestAdd("0123456789abcde");

	/* Print all items */
	PrintItems();

	/* Delete last item in the chain */
	TestDel(item);

	/* Print all items gain */
	PrintItems();

	/* Search first item 0 */
	TestSearch(0);
	/* Search middle item  2 */
	TestSearch(2);
	/* Search last item 4 */
	TestSearch(4);
	/* Search deleted item 3 */
	TestSearch(3);

    while(1)
    {
    }
}
