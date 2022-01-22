
/** Put this in the src folder **/

#include "i2c-lcd.h"
extern I2C_HandleTypeDef hi2c1;  // change your handler here accordingly
extern I2C_HandleTypeDef hi2c3;
#define SLAVE_ADDRESS_LCD 0x4E // change this according to ur setup

void lcd_send_cmd (char cmd)
{
  char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  //en=1, rs=0
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

//====================================
void lcd_send_cmd1 (char cmd1)
{
  char data_u1, data_l1;
	uint8_t data_t1[4];
	data_u1 = (cmd1&0xf0);
	data_l1 = ((cmd1<<4)&0xf0);
	data_t1[0] = data_u1|0x0C;  //en=1, rs=0
	data_t1[1] = data_u1|0x08;  //en=0, rs=0
	data_t1[2] = data_l1|0x0C;  //en=1, rs=0
	data_t1[3] = data_l1|0x08;  //en=0, rs=0
	HAL_I2C_Master_Transmit (&hi2c3, SLAVE_ADDRESS_LCD,(uint8_t *) data_t1, 4, 100);
}
void lcd_send_data1 (char data1)
{
	char data_u1, data_l1;
	uint8_t data_t1[4];
	data_u1 = (data1&0xf0);
	data_l1 = ((data1<<4)&0xf0);
	data_t1[0] = data_u1|0x0D;  //en=1, rs=0
	data_t1[1] = data_u1|0x09;  //en=0, rs=0
	data_t1[2] = data_l1|0x0D;  //en=1, rs=0
	data_t1[3] = data_l1|0x09;  //en=0, rs=0
	HAL_I2C_Master_Transmit (&hi2c3, SLAVE_ADDRESS_LCD,(uint8_t *) data_t1, 4, 100);
}
void lcd_clear1 (void)
{
	lcd_send_cmd1 (0x80);
	for (int i=0; i<70; i++)
	{
		lcd_send_data1 (' ');
	}
}

void lcd_put_cur1(int row1, int col1)
{
    switch (row1)
    {
        case 0:
            col1 |= 0x80;
            break;
        case 1:
            col1 |= 0xC0;
            break;
    }

    lcd_send_cmd1 (col1);
}


void lcd_init1 (void)
{
	// 4 bit initialisation
	HAL_Delay(50);  // wait for >40ms
	lcd_send_cmd1 (0x30);
	HAL_Delay(5);  // wait for >4.1ms
	lcd_send_cmd1 (0x30);
	HAL_Delay(1);  // wait for >100us
	lcd_send_cmd1 (0x30);
	HAL_Delay(10);
	lcd_send_cmd1 (0x20);  // 4bit mode
	HAL_Delay(10);

  // dislay initialisation
	lcd_send_cmd1 (0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	HAL_Delay(1);
	lcd_send_cmd1 (0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
	HAL_Delay(1);
	lcd_send_cmd1 (0x01);  // clear display
	HAL_Delay(1);
	HAL_Delay(1);
	lcd_send_cmd1 (0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	HAL_Delay(1);
	lcd_send_cmd1 (0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
}

void lcd_send_string1 (char *str)
{
	while (*str) lcd_send_data1 (*str++);
}
//============================================
void lcd_send_data (char data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=0
	data_t[1] = data_u|0x09;  //en=0, rs=0
	data_t[2] = data_l|0x0D;  //en=1, rs=0
	data_t[3] = data_l|0x09;  //en=0, rs=0
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

void lcd_clear (void)
{
	lcd_send_cmd (0x80);
	for (int i=0; i<70; i++)
	{
		lcd_send_data (' ');
	}
}

void lcd_put_cur(int row, int col)
{
    switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xC0;
            break;
    }

    lcd_send_cmd (col);
}


void lcd_init (void)
{
	// 4 bit initialisation
	HAL_Delay(50);  // wait for >40ms
	lcd_send_cmd (0x30);
	HAL_Delay(5);  // wait for >4.1ms
	lcd_send_cmd (0x30);
	HAL_Delay(1);  // wait for >100us
	lcd_send_cmd (0x30);
	HAL_Delay(10);
	lcd_send_cmd (0x20);  // 4bit mode
	HAL_Delay(10);

  // dislay initialisation
	lcd_send_cmd (0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	HAL_Delay(1);
	lcd_send_cmd (0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
	HAL_Delay(1);
	lcd_send_cmd (0x01);  // clear display
	HAL_Delay(1);
	HAL_Delay(1);
	lcd_send_cmd (0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	HAL_Delay(1);
	lcd_send_cmd (0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
}

void lcd_send_string (char *str)
{
	while (*str) lcd_send_data (*str++);
}
