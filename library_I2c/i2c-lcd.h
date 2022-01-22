#include "stm32f4xx_hal.h"

void lcd_init (void);   // initialize lcd

void lcd_send_cmd (char cmd);  // send command to the lcd

void lcd_send_data (char data);  // send data to the lcd

void lcd_send_string (char *str);  // send string to the lcd

void lcd_put_cur(int row, int col);  // put cursor at the entered position row (0 or 1), col (0-15);

void lcd_clear (void);





void lcd_init1 (void);   // initialize lcd

void lcd_send_cmd1 (char cmd1);  // send command to the lcd

void lcd_send_data1 (char data1);  // send data to the lcd

void lcd_send_string1 (char *str);  // send string to the lcd

void lcd_put_cur1(int row1, int col1);  // put cursor at the entered position row (0 or 1), col (0-15);

void lcd_clear1 (void);
