/**
  ******************************************************************************
  * @file    OpenDrone-RC-WiFi/main.c 
  * @author  Ethan Huang <ethan@robotlab.tw>
  * @version V1.0.1
  * @date    2018/05/08
  * @website http://www.robotlab.tw/
  ******************************************************************************
  * @attention
  *

  * This file is part of OpenDrone.
  *
  * OpenDrone are free software. You can redistribute
  * this software and/or modify this software under the terms of the
  * GNU General Public License as published by the Free Software
  * Foundation, either version 3 of the License, or (at your option)
  * any later version.
  *
  * OpenDrone are distributed in the hope that they
  * will be useful, but WITHOUT ANY WARRANTY; without even the implied
  * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  * See the GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this software.
  *
  * If not, see <http://www.gnu.org/licenses/>.
 **/

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "delay.h"  
#include "beep.h"
#include "gpio.h"

extern volatile uint8_t channel_end;
extern volatile uint16_t adc_data[5];

int rc_match_mode_button_count = 0;
int rc_match_mode_button_times = 0;
int rc_match_mode_enable_times = 3;

int rc_web_match_mode_button_count = 0;
int rc_web_match_mode_button_times = 0;
int rc_web_match_mode_enable_times = 3;

char rc_match_str[] = {"00"};
char rc_web_match_str[] = {"00"};
char rc_arming_str[] = {"00"};

int rc_match_flag = 0;
int rc_web_match_flag = 0;
int rc_arming_flag = 0;

int system_timer = 0;
int arming_throttle_value = 3800;

int main(void)
{
  Delay_Init();
  USART_Config();
  esp8266_USART_Config();
  init_gpio();
  init_beep();
  init_joystick();
  
  
  while (1)
  {
    //toggle_beep(500);
    char yaw_str[5], pitch_str[5], roll_str[5], throttle_str[5], ssid[6];
    
    char joystick_date[45];
    
    while(!channel_end);
    channel_end = 0;
    
    strcpy(ssid, "OD1111");     
    
    snprintf(yaw_str, 5, "%d", adc_data[0]);
    snprintf(pitch_str, 5, "%d", adc_data[1]);
    snprintf(roll_str, 5, "%d", adc_data[2]);
    snprintf(throttle_str, 5, "%d", adc_data[3]);
    
    strcpy(joystick_date, "@");                 /* package start_bit */
    strcat(joystick_date, "$");                 /* package start_bit */
    strcat(joystick_date, "|");
    strcat(joystick_date, roll_str);
    strcat(joystick_date, "|");
    strcat(joystick_date, pitch_str);
    strcat(joystick_date, "|");
    strcat(joystick_date, throttle_str);
    strcat(joystick_date, "|");
    strcat(joystick_date, yaw_str);
    strcat(joystick_date, "|");
    strcat(joystick_date, rc_arming_str);       /* AA = unlocked; 00 = locked */
    strcat(joystick_date, "|");
    strcat(joystick_date, rc_match_str);        /* 00 = normal mode; AA = RC matching mode */
    strcat(joystick_date, "|");
    strcat(joystick_date, rc_web_match_str);    /* 00 = nothing; AA = tell the RC esp8266 switch to AP mode and open web server 8080 */
    strcat(joystick_date, "|");
    strcat(joystick_date, ssid);                /* WiFi SSID */
    strcat(joystick_date, "|");
    strcat(joystick_date, "1F");                /* checksum = (roll_str + pitch_str + throttle_str + SSID(end of two bytes, etc. 1A2B3C = 2B3C) >> 8 ) */
        
    sendto_esp8266( joystick_date );
    
    //@$|1500|1500|1050|1500|00|00|00|1A2B3C|1F
    
    printf("@$|%d|%d|%d|%d|%s|%s|%s|OD1111|1F\n\r", adc_data[2], adc_data[1], adc_data[3], adc_data[0], rc_arming_str, rc_match_str, rc_web_match_str);
    
    if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 1 && GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 1)
    {
      rc_match_mode_button_count++;
      
      printf("rc_match_mode_button_count=%d\n\r", rc_match_mode_button_count);
      if( (rc_match_mode_button_count % 50) == 0 )
        rc_match_mode_button_times++;
      
      // into RC matching enable mode
      if(rc_match_mode_button_times >= rc_match_mode_enable_times)
      {
        strcpy(rc_match_str,"AA");
        rc_match_flag = 1;
        
        rc_match_mode_button_count = 0;
        rc_match_mode_button_times = 0;
        //DelayMilli(1000);
      }

      if(rc_arming_flag == 1)
      {
        rc_arming_flag = 0;
        strcpy(rc_arming_str,"00");
        rc_disarming_beep();
        //printf("rc_disarming_beep\n\r");
        //DelayMilli(1000);
        continue;
      }
      
      // > arming_throttle_value
      if( (adc_data[3] >= arming_throttle_value) && rc_arming_flag == 0 )
      {
        rc_arming_flag = 1;
        strcpy(rc_arming_str,"AA");
        rc_arming_beep();
        //printf("rc_arming_beep\n\r");
        DelayMilli(100);
        continue;
      }
      
      if( (adc_data[3] < arming_throttle_value) && rc_arming_flag == 0 )
      {
        strcpy(rc_arming_str,"00");
        rc_arming_deny_beep();
        DelayMilli(100);
        continue;
      }
            
      //printf("throttle=%d\n\r",adc_data[3]);
    }
    if( GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 1 && GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0  )
    {
      rc_web_match_mode_button_count++;
      
      //printf("rc_web_match_mode_button_count=%d\n\r", rc_web_match_mode_button_count);
      if( (rc_web_match_mode_button_count % 50) == 0 )
        rc_web_match_mode_button_times++;
      
      // tell esp8266 switch to AP mode and open web server.
      if(rc_web_match_mode_button_times >= rc_web_match_mode_enable_times)
      {
        strcpy(rc_web_match_str,"AA");
        rc_web_match_flag = 1;
        
        rc_web_match_mode_button_count = 0;
        rc_web_match_mode_button_times = 0;
        //DelayMilli(1000);
      }
    }
    /*
    if( GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 1)
    {
      printf("Front Function 1 Button\n\r");
    }
    if( GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 1)
    {
      printf("Front Function 2 Button\n\r");
    }
    if( GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2) == 1)
    {
      printf("FUNC_1 Button\n\r");
    }
    if( GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3) == 1)
    {
      printf("FUNC_2 Button\n\r");
    }
    if( GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) == 1)
    {
      printf("FUNC_3 Button\n\r");
    }
    if( GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == 1)
    {
      printf("FUNC_4 Button\n\r");
    }
    if( GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6) == 1)
    {
      printf("FUNC_5 Button\n\r");
    }
    if( GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7) == 1)
    {
      printf("FUNC_6 Button\n\r");
    }
    if( GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == 1)
    {
      printf("FUNC_7 Button\n\r");
    }
    if( GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) == 1)
    {
      printf("FUNC_8 Button\n\r");
    }
    */
    
    if(rc_match_flag == 1)
    {
      rc_match_enable_beep();
    }
    if(rc_web_match_flag == 1)
    {
      rc_web_match_enable_beep();
    }
    
    if( (system_timer % 100) ==  0 )
    {
      if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0 && GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
      {
        rc_match_mode_button_times = 0;
        rc_match_mode_button_count = 0;
      }
      
      if( GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0  )
      {
        rc_web_match_mode_button_times = 0;
        rc_web_match_mode_button_count = 0;
      }
    }
    
    system_timer++;
    DelayMilli(20);
  }
}

void USART1_puts(volatile char *data){
	
	while(*data){
		while( !(USART1->ISR & 0x00000040) ); 
		USART_SendData(USART1, *data);
		*data++;
	}    
}

void sendto_esp8266(volatile char *data){

	while(*data){
		while( !(USART2->ISR & 0x00000040) ); 
		USART_SendData(USART2, *data);
		*data++;
	}

}

static void USART_Config(void)
{ 
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  USART_Init(USART1, &USART_InitStructure);
    
  USART_Cmd(USART1, ENABLE);
}

static void esp8266_USART_Config(void)
{ 
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  //NVIC_InitTypeDef NVIC_InitStructure;
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); 

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource14, GPIO_AF_1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_1);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  
  USART_Init(USART2, &USART_InitStructure);
  
  /*
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
  */
  USART_Cmd(USART2, ENABLE);
}

void USART2_IRQHandler(void)
{
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  {
    char c = USART_ReceiveData(USART2);
    //printf("%x", c);
    
  }
}


int fputc(int ch, FILE *f)
{
  USART_SendData(USART1, (uint8_t) ch);
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){}

  return ch;
}


