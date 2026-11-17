/***************************************************************************
 *            interrupt.cc
 *
 *  Thu Jun  7 14:38:07 2007
 *  Copyright  2007  Darryl LeCount
 *  darryl@jamyskis.net
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <allegro.h>

#ifdef ALLEGRO_LINUX
#include "config.h"
#include "headers/declare.h"
#endif

#ifndef ALLEGRO_LINUX
#include "../include/declare.h"
#endif
 
bool suspend_time_control;
extern int frames_missed;

void interrupt_time_control()
{
	if(!suspend_time_control)
	{
		frames_missed++;
	}
	else
	{
		frames_missed=0;
	}
};

END_OF_FUNCTION(interrupt_time_control);

void interrupt_time_control_title()
{
	if(!suspend_time_control)
	{
		frames_missed++;
	}
	else
	{
		frames_missed=0;
	}
};

END_OF_FUNCTION(interrupt_time_control_title);

