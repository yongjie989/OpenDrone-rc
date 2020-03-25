/**
  ******************************************************************************
  * @file    OpenDrone-RC-WiFi/gpio.c
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
 
#include "gpio.h"
#include "delay.h"

volatile uint8_t channel_end = 0, channel = 0;
volatile uint16_t adc_data[5] = {0x00};

void init_gpio(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void init_joystick(void)
{
  ADC_InitTypeDef ADC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  ADC_DeInit(ADC1);
  ADC_StructInit(&ADC_InitStructure);
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
  ADC_Init(ADC1, &ADC_InitStructure); 
  
  ADC_ChannelConfig(ADC1, ADC_Channel_0|ADC_Channel_1|ADC_Channel_2|ADC_Channel_3|ADC_Channel_4, ADC_SampleTime_239_5Cycles);
  
  ADC_GetCalibrationFactor(ADC1);
  ADC_Cmd(ADC1, ENABLE);
  
  ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
  
  NVIC_InitStructure.NVIC_IRQChannel = ADC1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY));
  
  ADC_StartOfConversion(ADC1);

}

void ADC1_IRQHandler(void) 
{
	if(ADC_GetITStatus(ADC1, ADC_IT_EOC))
  {
		ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
		switch(channel)
    {
      case 0:
        adc_data[0] = ADC_GetConversionValue(ADC1);
        channel = 1;
        break;
      case 1:
        adc_data[1] = ADC_GetConversionValue(ADC1);
        channel = 2;
        break;
      case 2:
        adc_data[2] = ADC_GetConversionValue(ADC1);
        channel = 3;
        break;
      case 3:
        adc_data[3] = ADC_GetConversionValue(ADC1);
        channel = 4;
        break;
      case 4:
        adc_data[4] = ADC_GetConversionValue(ADC1);
        channel = 0;
        channel_end = 1;
        break;
		}
	}
}
