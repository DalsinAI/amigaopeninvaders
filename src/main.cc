/* Open Invaders 0.3
 * (c) 2006-2007 Darryl LeCount
 *
 *
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
#include <iostream>

#include "config.h"
#include "headers/main.h"
#include "headers/declare.h"
#include "headers/graphics.h"
#include "headers/init.h"
#include "headers/logic.h"
#include "headers/collision.h"
#include "headers/intro.h"

int fullscreen_mode=0;
int frames_missed=0;

item bulletposition, ufoposition, shieldunits[4], shipposition;

extern bool program_active, game_active;	// Until I can find a tidier way to solve this, these two need to be global
extern void interrupt_time_control();
extern int music_volume, level;
extern DUH *endingsong, *gamesong;
extern AL_DUH_PLAYER *gamesongplayer;
extern BITMAP *messagebuffer;
extern FONT *gamefont;

using namespace std;

int main(int argc, char *argv[])
{
	cout << PACKAGE_DATA_DIR << endl;
	
	// Parse parameters, perhaps include -windowed and -fullscreen?
	
	if(argc>1)
	{
		if(argv[1][0]=='-')
		{
			fullscreen_mode=0;
			
			switch(argv[1][1])
			{
				case 'f': fullscreen_mode=1; break;
				case 'w': fullscreen_mode=2; break;
			};
		};
	};
	
		// Lock all timer-based functions
	
	LOCK_FUNCTION(interrupt_keys);
	LOCK_FUNCTION(interrupt_time_control);
	LOCK_FUNCTION(add_counter);
	LOCK_FUNCTION(counter_reset);
	
	LOCK_VARIABLE(frames_missed);
	LOCK_VARIABLE(counter);
	
	// Setup variables, load files etc.
	
	initialise_game();
	cout << "Allegro initialised...\n";
	
	// Quickly load config file and parse to see if the fullscreen variable
	// has been set - I know this could be done more efficiently but hey

	display_setup(fullscreen_mode);
		
	define_sprites();
	cout << "Allegro display established...\n";
	
	predefine_variables();
	program_active=true;
	
	create_bitmasks();
	
	cout << "Collision bitmasks initialised...\n";

	cout << "Have fun!\n";
	
	intro_sequence();  // This is a self-contained block, all within intro.cc (I hope!)
	
	// The program now continues to run as long as program_active is true. 
	
	while(program_active)
	{	
		gamesongplayer=al_start_duh(gamesong,2,0,(float) music_volume/10,
		get_config_int("sound", "buffer_size", 4096),
		get_config_int("sound", "sound_freq", 22500));
		
		title_screen();
		predefine_variables();
		reset_enemies_position();
		reset_enemies_state();
		create_scrolling_message();
		display_scrolling_message(messagebuffer);

		install_int_ex(interrupt_time_control,BPS_TO_TIMER(60));
		
		// The actual game loop, continues playing as long as game_active is true
		
		if(!program_active)
		{
			game_active=false;
		}
		
		while(game_active)
		{
			for(int repeats=0;repeats<frames_missed;repeats++) 
			{
				update_logic();
				if(!game_active||level==15) break;
			};
			
			//update_animation();
					
			frames_missed=0;	// because the screen is just about to be updated
								// we can assume no more frames will be missed
				
			game_display();
						
			rest(1);			// this rest(0) is needed so that the CPU is yielded
		};
		
		//al_stop_duh(gamesongplayer);
		
		// Cheap hack to reinitialise the sound system since it cuts out in-game
		// after six or seven games
		
		remove_sound();
		install_sound(DIGI_AUTODETECT,MIDI_NONE,NULL);
		
		save_hiscore();		

		remove_int(interrupt_time_control);
	};
	
	unload_duh(endingsong);
	unload_duh(gamesong);
	
	cout << "Thank you for playing!\n";
	
	allegro_exit();
}
END_OF_MAIN();
