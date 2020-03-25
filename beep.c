/**
  ******************************************************************************
  * @file    OpenDrone-RC-WiFi/beep.c
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
 
#include "beep.h"
#include "delay.h"

void init_beep(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void toggle_beep(int delay)
{
  GPIO_SetBits(GPIOA, GPIO_Pin_5);
  DelayMilli(delay);
  GPIO_ResetBits(GPIOA, GPIO_Pin_5);
  DelayMilli(delay);

}
void rc_match_enable_beep(void)
{
  GPIO_SetBits(GPIOA, GPIO_Pin_5);
  DelayMilli(100);
  GPIO_ResetBits(GPIOA, GPIO_Pin_5);
  DelayMilli(100);
  GPIO_SetBits(GPIOA, GPIO_Pin_5);
  DelayMilli(100);
  GPIO_ResetBits(GPIOA, GPIO_Pin_5);
  DelayMilli(2000);  
}

void rc_web_match_enable_beep(void)
{
  GPIO_SetBits(GPIOA, GPIO_Pin_5);
  DelayMilli(100);
  GPIO_ResetBits(GPIOA, GPIO_Pin_5);
  DelayMilli(100);
  GPIO_SetBits(GPIOA, GPIO_Pin_5);
  DelayMilli(100);
  GPIO_ResetBits(GPIOA, GPIO_Pin_5);
  DelayMilli(100);
  GPIO_SetBits(GPIOA, GPIO_Pin_5);
  DelayMilli(100);
  GPIO_ResetBits(GPIOA, GPIO_Pin_5);
  DelayMilli(2000);  
}

void rc_arming_beep(void)
{
  GPIO_SetBits(GPIOA, GPIO_Pin_5);
  DelayMilli(50);
  GPIO_ResetBits(GPIOA, GPIO_Pin_5);
  DelayMilli(50);
  GPIO_SetBits(GPIOA, GPIO_Pin_5);
  DelayMilli(50);
  GPIO_ResetBits(GPIOA, GPIO_Pin_5);
  DelayMilli(50);
  GPIO_SetBits(GPIOA, GPIO_Pin_5);
  DelayMilli(50);
  GPIO_ResetBits(GPIOA, GPIO_Pin_5);
  DelayMilli(50);
}

void rc_arming_deny_beep(void)
{
  GPIO_SetBits(GPIOA, GPIO_Pin_5);
  DelayMilli(50);
  GPIO_ResetBits(GPIOA, GPIO_Pin_5);
  DelayMilli(50);
  GPIO_SetBits(GPIOA, GPIO_Pin_5);
  DelayMilli(50);
  GPIO_ResetBits(GPIOA, GPIO_Pin_5);
  DelayMilli(50);
  GPIO_SetBits(GPIOA, GPIO_Pin_5);
  DelayMilli(1500);
  GPIO_ResetBits(GPIOA, GPIO_Pin_5);
}

void rc_disarming_beep(void)
{
  GPIO_SetBits(GPIOA, GPIO_Pin_5);
  DelayMilli(50);
  GPIO_ResetBits(GPIOA, GPIO_Pin_5);
  DelayMilli(50);
}