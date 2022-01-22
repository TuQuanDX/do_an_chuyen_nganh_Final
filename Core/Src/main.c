/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <i2c-lcd.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c3;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart6;

/* USER CODE BEGIN PV */
#ifdef __GNUC__
     #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
     #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit(&huart6,(uint8_t *)&ch,1,0xFFFF); //pc
	return ch;
}
uint8_t key;
int x = 0;
int y = 1;
char buffer [100] ;
char time_buffer[10];
char timer [20];
char pass [10] = "1234" ;
uint16_t value[2];
#define size 500
uint8_t rx_data6, rx_index6;   ////////du lieu nhan luu vao rx_data6
char rx_buffer6[size];
uint8_t rx_data2, rx_index2;   ////////du lieu nhan luu vao rx_data2
char rx_buffer2[size];
uint16_t temp =0, gas = 0;
char DATA[100];
char Str_temp[100];
char Str_gas[100];
int TTTB1 = 0,  TTTB2 = 0, TTTB3 = 0, TTTB4 = 0, TTTB5 = 0,  TTTB6 = 0;
char Str_TB1[5], Str_TB2[5], Str_TB3[5], Str_TB4[5], Str_TB5[5], Str_TB6[5] ;
long last = 0 ;
char Length[100];
uint8_t decToBcd(uint8_t t);
uint8_t bcdToDec(uint8_t t);
#define DS3231_ADDRESS (0x68<<1)
#define DS3231_REG_TIME (0X00)
#define DS3231_REG_ALARM_1 (0X07)
#define DS3231_REG_ALARM_2 (0X0B)
#define DS3231_REG_CONTROL (0X0E)
#define DS3231_REG_STATUS  (0X0F)
uint8_t decToBcd(uint8_t t) 						//đổi số thập phân bình thư�?ng thành số thập phân được mã hóa nhị phân
{
  return (t/10)<<4 | (t%10);
}
uint8_t bcdToDec(uint8_t t)						//đổi số thập phân được mã hóa nhị phân thành số thập phân bình thư�?ng
{
  return (t>>4)*10 + (t&0x0f);
}
typedef struct									//khai báo 1 struct để phục vụ cho việc đ�?c ghi dữ liệu th�?i gian thực
{
	uint8_t seconds ;
	uint8_t minutes ;
	uint8_t hour ;
	uint8_t day ;
	uint8_t date  ;
	uint8_t month ;
	uint8_t year ;

}TIME;
TIME time1 ;

void Set_Time (uint8_t sec, uint8_t min, uint8_t hour, uint8_t day, uint8_t date, uint8_t month, uint8_t year)
{
	uint8_t set_time1[7];
	set_time1[0] = decToBcd(sec);
	set_time1[1] = decToBcd(min);
	set_time1[2] = decToBcd(hour);
	set_time1[3] = decToBcd(day);
	set_time1[4] = decToBcd(date);
	set_time1[5] = decToBcd(month);
	set_time1[6] = decToBcd(year);

	HAL_I2C_Mem_Write(&hi2c3, DS3231_ADDRESS, 0x00, 1, set_time1, 7, 1000);
}
void Get_Time (void)
{
	uint8_t get_time[7];
	HAL_I2C_Mem_Read(&hi2c3, DS3231_ADDRESS, 0x00, 1, get_time, 7, 1000);
	time1.seconds = bcdToDec(get_time[0]);
	time1.minutes = bcdToDec(get_time[1]);
	time1.hour = bcdToDec(get_time[2]);
	time1.day = bcdToDec(get_time[3]);
	time1.date = bcdToDec(get_time[4]);
	time1.month = bcdToDec(get_time[5]);
	time1.year = bcdToDec(get_time[6]);
}
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C3_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART6_UART_Init(void);
/* USER CODE BEGIN PFP */
void SendData(void);
void 	ReceivedEnd(void);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void clearbufferEnd(void);
void XuLy(void);
void Button(void);
void cambien(void);
char keypad(void);
void CheckPass (void);
void clearBuffer(void);
void RtC(void);
void Data( unsigned int temp, unsigned int gas, unsigned int TB1 , unsigned int TB2,  unsigned int TB3,  unsigned int TB4, unsigned int TB5, unsigned int TB6);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void clearBuffer()
{
	for(int i = 0 ; i < 100 ; i++)
							{
								buffer[i] = 0 ;
							}
					y=1;
					key =0;
							lcd_clear ();
							keypad();
}
void CheckPass  ()
{
	int compare = 1;
	compare = strcmp (buffer, pass);
	if(compare == 0)
	{
		lcd_clear();
		lcd_put_cur(1,3);
		lcd_send_string ("Open Door");
		HAL_Delay(1000);
		clearBuffer();
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 125);

	}
	else
	{
		lcd_clear();
		lcd_put_cur(1,5);
		lcd_send_string ("ERORR");
		HAL_Delay(1000);
		clearBuffer();

	}
}
char keypad()
{
	HAL_GPIO_WritePin  (GPIOB,GPIO_PIN_5,GPIO_PIN_RESET);   // Kéo R1 xuống thấp
	HAL_GPIO_WritePin  (GPIOB,GPIO_PIN_3,GPIO_PIN_SET);   // Kéo R2 cao
	HAL_GPIO_WritePin  (GPIOD,GPIO_PIN_6,GPIO_PIN_SET);   // Kéo R3 cao
	HAL_GPIO_WritePin  (GPIOD,GPIO_PIN_4,GPIO_PIN_SET);   // Kéo R4 cao

	if  ( (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2))==0)    // nếu Col 1 thấp
	{
		return  '1' ;
	}

	if  (  (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_0))==0)    // nếu Col 2 thấp
	{
		return  '2';
	}

	if  (  (HAL_GPIO_ReadPin  (GPIOC,GPIO_PIN_11))==0)    // nếu Col 3 thấp
	{
		return  '3';
	}

	if  (  (HAL_GPIO_ReadPin  (GPIOA,GPIO_PIN_15))==0)    // nếu Col 4 thấp
	{
		CheckPass();
		clearBuffer();
		return 0;
	}
	HAL_GPIO_WritePin  (GPIOB,GPIO_PIN_5,GPIO_PIN_SET);
		HAL_GPIO_WritePin  (GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);
		HAL_GPIO_WritePin  (GPIOD,GPIO_PIN_6,GPIO_PIN_SET);
		HAL_GPIO_WritePin  (GPIOD,GPIO_PIN_4,GPIO_PIN_SET);

		if  ( (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2))==0)
		{
			return  '4' ;
		}

		if  (  (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_0))==0)
		{
			return  '5';
		}

		if  (  (HAL_GPIO_ReadPin  (GPIOC,GPIO_PIN_11))==0)
		{
			return  '6';
		}

		if  (  (HAL_GPIO_ReadPin  (GPIOA,GPIO_PIN_15))==0)
		{
			if(x>0)
			{
				HAL_Delay (200);
				buffer [strlen (buffer) - 1] = 0;
				lcd_clear();
				lcd_put_cur(1,1);
				lcd_send_string (buffer);
				y--;
				key=0;
			}
			return 0;
		}
		HAL_GPIO_WritePin  (GPIOB,GPIO_PIN_5,GPIO_PIN_SET);
			HAL_GPIO_WritePin  (GPIOB,GPIO_PIN_3,GPIO_PIN_SET);
			HAL_GPIO_WritePin  (GPIOD,GPIO_PIN_6,GPIO_PIN_RESET);
			HAL_GPIO_WritePin  (GPIOD,GPIO_PIN_4,GPIO_PIN_SET);

			if  ( (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2))==0)
			{
				return  '7' ;
			}

			if  (  (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_0))==0)
			{
				return  '8';
			}

			if  (  (HAL_GPIO_ReadPin  (GPIOC,GPIO_PIN_11))==0)
			{
				return  '9';
			}

			if  (  (HAL_GPIO_ReadPin  (GPIOA,GPIO_PIN_15))==0)
			{
				clearBuffer();
				return 0;
			}
			HAL_GPIO_WritePin  (GPIOB,GPIO_PIN_5,GPIO_PIN_SET);
				HAL_GPIO_WritePin  (GPIOB,GPIO_PIN_3,GPIO_PIN_SET);
				HAL_GPIO_WritePin  (GPIOD,GPIO_PIN_6,GPIO_PIN_SET);
				HAL_GPIO_WritePin  (GPIOD,GPIO_PIN_4,GPIO_PIN_RESET);

				if  ( (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2))==0)
				{
					return  '*' ;
				}

				if  (  (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_0))==0)
				{
					return  '0';
				}

				if  (  (HAL_GPIO_ReadPin  (GPIOC,GPIO_PIN_11))==0)
				{
					return  '#';
				}

				if  (  (HAL_GPIO_ReadPin  (GPIOA,GPIO_PIN_15))==0)
				{
					__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 25);

						return 0;
				}
}
void matkhau(void )
{
		keypad();
		  key = keypad();
		  if (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2)==0 || HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_0)==0 ||HAL_GPIO_ReadPin (GPIOC,GPIO_PIN_11)==0||HAL_GPIO_ReadPin (GPIOA,GPIO_PIN_15)==0)
		  {
		x = strlen (buffer);
		for ( int i = x; i<=x +1; i ++)
			buffer[x]= key  ;
		  }
		  lcd_put_cur(0,1);
		  lcd_send_string ("Enter Password ");
		  lcd_put_cur(1,1);
		  lcd_send_string (buffer);
//		  y = strlen (buffer);
//		 		  if(y==4)
//		 		  {
//		 			  CheckPass  ();
//		 		  }
		  HAL_Delay(150);
}

void RtC()
{
	Get_Time();
	sprintf(time_buffer, "%d:%d", time1.hour, time1.minutes);
	lcd_put_cur1(2,7);
	lcd_send_string1(time_buffer);
	printf("time :: %d:%d:%d   ", time1.hour, time1.minutes, time1.seconds);
	sprintf (time_buffer, "%02d-%02d-%02d", time1.date, time1.month, time1.year + 2000);
	lcd_put_cur1(3,0);
	lcd_send_string1(time_buffer);
	printf("date :: %d:%d:%d \r\n", time1.date, time1.month, time1.year + 2000);
}
void cambien()
{
	HAL_ADC_Start_DMA(&hadc1,(uint32_t*)value,2);
	temp = value[0]*0.0732 - 3;
	gas = value[1]*0.02442;
	printf("gas: %d\n", gas );
	printf("temp: %d\n", temp );
	lcd_put_cur1(0,0);
	lcd_send_string1("temp: ");
	lcd_put_cur1(0,7);
	lcd_send_string1 (temp);
	lcd_put_cur1(0,10);
	lcd_send_string1 (" C");
	lcd_put_cur1(0,8);
	lcd_send_string1 ("gas: ");
	lcd_put_cur1(0,14);
	lcd_send_string1 (gas);
	lcd_put_cur1(0,18);
	lcd_send_string1 ("%");
	if(temp>50||gas>25)
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, RESET);
	}
}
void Button()
{
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == 0)  // den1
		{
			HAL_Delay(10);
			while(1)
			{

				last = HAL_GetTick();
				if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == 1)
				{
					if(TTTB1 == 0)
					{
						TTTB1 = 1; //TB1 = 1;
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, RESET);
					}
					else if(TTTB1 == 1)
					{
						TTTB1 = 0;//TB1 = 0;
						 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, SET);
					}
					HAL_Delay(10);
					break;
				}
			}
		}

		if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_7) == 0)     //quat 1
		{
			HAL_Delay(10);
			while(1)
			{
				last = HAL_GetTick();
				if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_7) == 1)
				{
					if(TTTB2 == 0)
					{
						TTTB2 = 1; //TB2 = 1;
						HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, RESET);
					}
					else if(TTTB2 == 1)
					{
						TTTB2 = 0; //TB2 = 0;
						HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, SET);
					}
					HAL_Delay(10);

					break;

				}
			}
		}
		if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_13) == 0)		//den 2          ///////////////
			{
				HAL_Delay(10);
				while(1)
				{

					last = HAL_GetTick();
					if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_13) == 1)
					{
						if(TTTB3 == 0)
						{
							TTTB3 = 1; //TB3 = 1;
							HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, RESET);
						}
						else if(TTTB3 == 1)
						{
							TTTB3 = 0;  //TB3 = 0;
							 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, SET);
						}
						HAL_Delay(10);
						break;
					}
				}
			}
		if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_15) == 0)		//quat2
				{
					HAL_Delay(10);
					while(1)
					{

						last = HAL_GetTick();
						if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_15) == 1)
						{
							if(TTTB4 == 0)
							{
								TTTB4 = 1; //TB4 = 1;
								HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, RESET);///////////////
							}
							else if(TTTB4 == 1)
							{
								TTTB4 = 0;   //TB4 = 0;
								 HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, SET);
							}
							HAL_Delay(10);
							break;
						}
					}
				}

		if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9) == 0)		//den3
					{
						HAL_Delay(10);
						while(1)
						{

							last = HAL_GetTick();
							if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9) == 1)
							{
								if(TTTB5 == 0)
								{
									TTTB5 = 1;     //TB5=1;
									HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, RESET);
								}
								else if(TTTB5 == 1)
								{
									TTTB5 = 0;         // TB5=0;
									 HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, SET);
								}
								HAL_Delay(10);
								break;
							}
						}
					}
		if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_11) == 0)		//quat 3
					{
						HAL_Delay(10);
						while(1)
						{

							last = HAL_GetTick();
							if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_11) == 1)
							{
								if(TTTB6 == 0)
								{
									TTTB6= 1;
									//TB6= 1;
									HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, RESET);
								}
								else if(TTTB6 == 1)
								{
									TTTB6 = 0;
									//TB6=0;
									 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, SET);
								}
								HAL_Delay(10);
								break;
							}
						}
					}
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13)==1)
		  {
			  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, RESET);

			  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, RESET);
		  }
	else
		  {
			  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, SET);

			  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, SET);
		  }
	last = HAL_GetTick();
}
void XuLy()
{
	if(strstr(rx_buffer2,"TB1=1") != NULL)
		{
			if(TTTB1==0)
			{
				TTTB1=1;
				//TB1=1;
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
			}
		}
	last = HAL_GetTick();
	if(strstr(rx_buffer2,"TB1=0") != NULL)
		{
		if(TTTB1==1)
			{
				TTTB1=0;
				//TB1=0;
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
			}
		}
	last = HAL_GetTick();///////////////////////////////
	if(strstr(rx_buffer2,"TB2=1") != NULL)
		{
		if(TTTB2==0)
			{
				TTTB2=1; //TB2=1;
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_RESET);
			}
		}
	last = HAL_GetTick();
	if(strstr(rx_buffer2,"TB2=0") != NULL)
		{
		if(TTTB2==1)
			{
				TTTB2=0; //TB2=0;
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET);
			}
		}//////////////////////////////////////////////////
	last = HAL_GetTick();
	if(strstr(rx_buffer2,"TB3=1") != NULL)
			{
			if(TTTB3==0)
				{
					TTTB3=1; //TB3=1;
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
				}
			}
	last = HAL_GetTick();
		if(strstr(rx_buffer2,"TB3=0") != NULL)
			{
			if(TTTB3==1)
				{
					TTTB3=0; //TB3=0;
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
				}
			}////////////////////////////////////////////////////
		last = HAL_GetTick();
		if(strstr(rx_buffer2,"TB4=1") != NULL)
					{
					if(TTTB4==0)
						{
							TTTB4=1; //TB4=1;
							HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_RESET); ////////////////////////////////////
						}
					}
			last = HAL_GetTick();
				if(strstr(rx_buffer2,"TB4=0") != NULL)
					{
					if(TTTB4==1)
						{
							TTTB4=0;  //TB4=0;
							HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_SET);
						}
					}///////////////////////////////////////////
				last = HAL_GetTick();
				if(strstr(rx_buffer2,"TB5=1") != NULL)
							{
							if(TTTB5==0)
								{
									TTTB5=1;
									//TB5=1;
									HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);
								}
							}
					last = HAL_GetTick();
						if(strstr(rx_buffer2,"TB5=0") != NULL)
							{
							if(TTTB5==1)
								{
									TTTB5=0;
									//TB5=0;
									HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
								}
							}/////////////////////////////////////////////////
						last = HAL_GetTick();
						if(strstr(rx_buffer2,"TB6=1") != NULL)
									{
									if(TTTB6==0)
										{
											TTTB6=1;
											//TB6=1;
											HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
										}
									}
							last = HAL_GetTick();
								if(strstr(rx_buffer2,"TB6=0") != NULL)
									{
									if(TTTB6==1)
										{
											TTTB6=0;
											//TB6=0;
											HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
										}
									}/////////////////////////////////////////////////
								last = HAL_GetTick();
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
if(huart -> Instance == USART2)
		{
		ReceivedEnd();
		HAL_UART_Receive_IT(&huart2,&rx_data2,1);
		last = HAL_GetTick() ;

		}
}
void 	ReceivedEnd(void)
{
	//xóa bộ đệm buffer
	if (rx_data2 != '\n')
	{
		rx_buffer2[rx_index2++] = rx_data2;
	}
	else
	{
		rx_index2 = 0;
		//printf("stm nhan :  %s\n",rx_buffer2);
		XuLy();
		clearbufferEnd();
	}

}
void clearbufferEnd(void)
{
	for(int i = 0 ; i < size ; i++)
		{
			rx_buffer2[i] = 0;
		}
		rx_index2 = 0;
}

void Data(unsigned int temp, unsigned int gas, unsigned int TTTB1 , unsigned int TTTB2,  unsigned int TTTB3,  unsigned int TTTB4, unsigned int TTTB5, unsigned int TTTB6)
{
	for(int i = 0 ; i <= 100; i++)
		{
			Str_temp[i] = 0;
			Str_gas[i] = 0;
			DATA[i] = 0;
		}
	for(int i = 0 ; i <= 5; i++)
			{
				Str_TB1[i] = 0;
				Str_TB2[i] = 0;
				Str_TB3[i] = 0;
				Str_TB4[i] = 0;
				Str_TB5[i] = 0;
				Str_TB6[i] = 0;
			}

		sprintf(Str_temp, "%d", temp);
		sprintf(Str_gas, "%d", gas);
		sprintf(Str_TB1, "%d", TTTB1);
		sprintf(Str_TB2, "%d", TTTB2);
		sprintf(Str_TB3, "%d", TTTB3);
		sprintf(Str_TB4, "%d", TTTB4);
		sprintf(Str_TB5, "%d", TTTB5);
		sprintf(Str_TB6, "%d", TTTB6);

				strcat(DATA,"A");
				strcat(DATA,Str_temp);

				strcat(DATA,"B");
				strcat(DATA,Str_gas);

				strcat(DATA,"C");
				strcat(DATA,Str_TB1);

				strcat(DATA,"D");
				strcat(DATA,Str_TB2);

				strcat(DATA,"E");
				strcat(DATA,Str_TB3);

				strcat(DATA,"F");
				strcat(DATA,Str_TB4);

				strcat(DATA,"G");
				strcat(DATA,Str_TB5);

				strcat(DATA,"H");
				strcat(DATA,Str_TB6);
				strcat(DATA,"J");
				strcat(DATA,"\r\n");
		//HAL_Delay(300);
		last = HAL_GetTick();

}
void SendData()
{
		  		Data(temp, gas, TTTB1, TTTB2, TTTB3, TTTB4, TTTB5, TTTB6 );
		  		HAL_UART_Transmit(&huart2,(uint8_t *)DATA ,strlen(DATA),500);
		  		//printf("pc    %s\n", DATA);
		  		last = HAL_GetTick();


}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_I2C3_Init();
  MX_TIM3_Init();
  MX_USART2_UART_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart6,&rx_data6,1);
  HAL_UART_Receive_IT(&huart2,&rx_data2,1);
  lcd_init();
  lcd_init1();
  lcd_clear();
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 25);
  last = HAL_GetTick();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if(HAL_GetTick() - last >= 500)
	 	  {
	 		matkhau();
	 		RtC();
	 	  	cambien();
	 	  	Button();
	 	  	SendData();
	 	  	last = HAL_GetTick();
	 	  }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 50;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 2;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_144CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_7;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C3_Init(void)
{

  /* USER CODE BEGIN I2C3_Init 0 */

  /* USER CODE END I2C3_Init 0 */

  /* USER CODE BEGIN I2C3_Init 1 */

  /* USER CODE END I2C3_Init 1 */
  hi2c3.Instance = I2C3;
  hi2c3.Init.ClockSpeed = 100000;
  hi2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C3_Init 2 */

  /* USER CODE END I2C3_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 999;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 500;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 9600;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_10|GPIO_PIN_12
                          |GPIO_PIN_14|GPIO_PIN_3|GPIO_PIN_5, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8|GPIO_PIN_10|GPIO_PIN_12|GPIO_PIN_14, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4|GPIO_PIN_6, GPIO_PIN_SET);

  /*Configure GPIO pins : PB0 PB2 PB10 PB12
                           PB14 PB3 PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_10|GPIO_PIN_12
                          |GPIO_PIN_14|GPIO_PIN_3|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB1 PB13 PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_13|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PE7 PE9 PE11 PE13
                           PE15 */
  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_9|GPIO_PIN_11|GPIO_PIN_13
                          |GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PE8 PE10 PE12 PE14 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_10|GPIO_PIN_12|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PD8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PD9 PD0 PD2 */
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_0|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PC11 */
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PD4 PD6 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
