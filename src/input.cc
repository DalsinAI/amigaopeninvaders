/***************************************************************************
 *            input.cc
 *
 *  Fri Jun  1 00:11:29 2007
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
#include <aldumb.h>
#include <string>
#include <sstream>

#include "config.h"
#include "headers/declare.h"
#include "headers/input.h"
#include "headers/init.h"

using namespace std;

extern PALETTE gamepalette;
extern bool program_active, game_active, in_process_of_fading;
extern int posx, fullscreen_mode, sfx_volume, level;
extern item bulletposition;
extern SAMPLE *shoot, *screenshotsaved;
extern bool title_screen_active;
extern void interrupt_time_control();
bool pausekeyreleased = false;

bool paused = false;

int keysdefined[7]={KEY_LEFT,KEY_RIGHT,KEY_UP,KEY_DOWN,KEY_LSHIFT,KEY_P,KEY_Q};
string keynames[7]={"LEFT","RIGHT","UP","DOWN","FIRE","PAUSE","QUIT"};

void interrupt_keys()
{
	PALETTE bitmappalette;
	ostringstream filename;
	int result;
	static bool keyreleased=true;
	
	#ifdef ALLEGRO_LINUX
		string filepath=getenv("HOME");
	#endif
	
	#ifndef ALLEGRO_LINUX
		string filepath='.';
	#endif

	if((key[KEY_LCONTROL]||key[KEY_RCONTROL])&&key[KEY_S]&&keyreleased)
	{	
		filename << filepath << "/oi_screen_" << (rand()%8998)+1000 << ".bmp";
		get_palette(bitmappalette);
		result=save_bitmap(filename.str().c_str(),screen,bitmappalette);
		play_sample(screenshotsaved,sfx_volume,128,1000,0);
		keyreleased=false;
	};
	
	if((key[KEY_LCONTROL]||key[KEY_RCONTROL])&&key[KEY_C])
	{
		program_active=false;
		game_active=false;
		title_screen_active=false;
		//allegro_exit();
		//std::terminate();
	};
	
	if(!input_pressed()&&!keyreleased)
	{
		keyreleased=true;
	}
};

END_OF_FUNCTION(interrupt_keys);


void read_input()
{
	poll_joystick();
	
	if((key[keysdefined[0]]||joy[0].stick[0].axis[0].d1)&&posx>0&&!in_process_of_fading&&!paused)
	{
		posx=posx-2;
	};
	
	if((key[keysdefined[1]]||joy[0].stick[0].axis[0].d2)&&posx<735&&!in_process_of_fading&&!paused)
	{
		posx=posx+2;
	};
	
	if((key[keysdefined[4]]||joy[0].button[0].b)&&bulletposition.alive!=alive&&!in_process_of_fading&&!paused)
	{
		bulletposition.xpos=posx;
		bulletposition.ypos=526;
		bulletposition.alive=alive;
		play_sample(shoot,sfx_volume,128,1000,0);
	};

	if(key[keysdefined[5]]&&pausekeyreleased)
	{
		if(paused)
		{
			paused=false;
		}
		else
		{
			paused=true;
		}
		
		pausekeyreleased=false;
	}
	
	if(!key[keysdefined[5]]&&!pausekeyreleased)
	{
		pausekeyreleased=true;
	}
			
	
	if(key[keysdefined[6]])
	{
		game_active=false;
	};
	
	if(((key[KEY_LCONTROL]||key[KEY_RCONTROL])&&key[KEY_F])||(key[KEY_ALT]&&key[KEY_ENTER]))
	{
		if(!is_windowed_mode())
		{
			set_gfx_mode(GFX_TEXT,800,600,0,0);
			set_gfx_mode(GFX_AUTODETECT_WINDOWED,800,600,0,0);
		}
		else
		{
			set_gfx_mode(GFX_TEXT,800,600,0,0);
			set_gfx_mode(GFX_AUTODETECT_FULLSCREEN,800,600,0,0);
		}

		save_config();		
	};
	
	clear_keybuf();
};

bool input_pressed()
{
	for(int scanned_keys=1; scanned_keys<KEY_MAX+1; scanned_keys++)
	{
		if(key[scanned_keys])
		{
			return true;
		}
	}
	
	if(joy[0].stick[0].axis[0].d1||joy[0].stick[0].axis[0].d2||joy[0].stick[0].axis[1].d1||joy[0].stick[0].axis[1].d2)
	{
		return true;
	}
	
	if(joy[0].button[0].b)
	{
		return true;
	}
	
	return false;
}
