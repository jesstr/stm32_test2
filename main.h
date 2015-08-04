#ifndef __MAIN_H
#define __MAIN_H


/* Receipt item structure */
typedef struct {
	char *title; 				/* pointer to memory block containing title of goods */
	unsigned long price;		/* prise of goods */
	unsigned short count; 		/* count of goods in item */
	unsigned short next_index; 	/* link to the next item */
	unsigned short code;		/* code of goods */
	unsigned int n:12;			/* ordinary number of items in receipt */
	unsigned int title_length:4;/* number of memory blocks allocated to store title */
} item2_t;


/* Add item */
item2_t *AddItem(unsigned long code, unsigned long price, unsigned short count, char *title);
/* Delete item */
int DelItem(item2_t *item);
/* Search item by number */
item2_t *SearchItem(unsigned short n);
/* Find previos item in the chain */
item2_t *FindPrevItem(item2_t *ref);

/* Print string over UART */
void UART_PrintStr(char *str);
/* Print data over UART */
void UART_PrintData(unsigned char *data, unsigned int n);


#endif /* __MAIN_H */
