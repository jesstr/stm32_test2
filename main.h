#ifndef __MAIN_H
#define __MAIN_H


//typedef struct {
//	char *title;
//	item2_t *next;
//	unsigned long n;
//	unsigned long price;
//	unsigned long code;
//	unsigned long count;
//	unsigned long title_length;
//} item2_t;

//
//typedef struct {
//	unsigned long price;
//	unsigned long code;
//	char *title;
//	unsigned short count;
//	unsigned int n:12;
//	unsigned int title_length:4;
//} item2_t;

typedef struct {
	char *title;
	unsigned long price;
	unsigned short count;
	unsigned short next_index;
	unsigned short code;
	unsigned int n:12;
	unsigned int title_length:4;
} item2_t;

//typedef struct {
//	unsigned long price;
//	unsigned long code;
//	unsigned short title_index;
//	unsigned short next_index;
//	unsigned short count;
//	unsigned int n:12;
//	unsigned int title_length:4;
//} item2_t;


item2_t *AddItem(unsigned long code, unsigned long price, unsigned short count, char *title);

int DelItem(item2_t *item);

item2_t *SearchItem(unsigned short n);

item2_t *FindPrevItem(item2_t *ref);

/* Print string over UART */
void UART_PrintStr(char *str);
/* Print data over UART */
void UART_PrintData(unsigned char *data, unsigned int n);

#endif /* __MAIN_H */
