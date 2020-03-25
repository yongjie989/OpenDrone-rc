/**
  ******************************************************************************
  * @file    OpenDrone-RC-WiFi/beep.h 
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
 
#ifndef __BEEP_H
#define __BEEP_H
 
#include "stm32f0xx.h"

void init_beep(void);
void toggle_beep(int delay);
void rc_match_enable_beep(void);
void rc_web_match_enable_beep(void);
void rc_arming_beep(void);
void rc_arming_deny_beep(void);
void rc_disarming_beep(void);

#endif

