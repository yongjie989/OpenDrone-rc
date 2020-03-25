/**
  ******************************************************************************
  * @file    OpenDrone-RC-WiFi/delay.c 
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
 
#include "delay.h"

#define DELAY_TIM_FREQUENCY_MS 1000
static volatile uint32_t usTicks = 0;
volatile uint32_t ticktack = 0;
uint32_t multiplier;

void Delay_Init(void) {
    RCC_ClocksTypeDef RCC_Clocks;
    
    /* Get system clocks */
    RCC_GetClocksFreq(&RCC_Clocks);
    
    /* While loop takes 4 cycles */
    /* For 1 us delay, we need to divide with 4M, in Linux compiler need to modify to 8000000 */
    multiplier = RCC_Clocks.HCLK_Frequency / 4000000;
    //multiplier = RCC_Clocks.HCLK_Frequency / 8000000;  // for linux environment compiler
}
void DelayMicros(uint32_t micros) {
    /* Multiply micros with multipler */
    /* Substract 10 */
    micros = micros * multiplier - 10;
    /* 4 cycles for one loop */
    while (micros--);
}

void DelayMilli(uint32_t milli)
{
  DelayMicros(milli * 1000);
}



