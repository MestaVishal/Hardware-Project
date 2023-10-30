#include<lpc21xx.h>             //include lpc header file
//#include "uart.h"

/*MACROs definition*/
#define RS (1<<10)
#define RW (1<<12)
#define EN (1<<13)
#define DATA (0xFF<<15)

void lcd_config(void);
void lcd_cmd(unsigned char c);
void lcd_data(unsigned char d);
void lcd_str(unsigned char *ptr);
void lcd_num(unsigned int num);
void delay(unsigned int a);