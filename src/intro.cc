/***************************************************************************
 *            intro.cc
 *
 *  Fri Jun  1 00:17:55 2007
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
#include <iostream>
#include <string>
#include <sstream>

#include "config.h"
#include "headers/declare.h"
#include "headers/input.h"
#include "headers/graphics.h"
#include "headers/init.h"
#include "headers/intro.h"
#include "headers/interrupt.h"

using namespace std;

bool title_screen_active=true;
extern PALETTE gamepalette;
extern BITMAP *display, *logo;
extern FONT *gamefont;
extern startype stars[500];
extern int todisplayx, todisplayy, difficulty, score;
extern bool program_active, game_active;
extern int hiscore[10], sfx_volume;
extern char hiscore_names[10][4];
extern int music_volume;
extern SAMPLE *welcome, *letsgo;
extern string keynames[7];
extern int keysdefined[7];
BITMAP *credits, *hiscore_screen;
AL_DUH_PLAYER *titlesongplayer;
int title_frames_missed = 0;
int flashcolor; // for "Press Start" message
int title_mode=1; //1=press start, 2=menu
int verticalposition, verticalsize;// these two are
bool logoinvert, growing; // for the logo 3D effect
int alpha_blend=0; //State of bounding box in menu options
int option_selected=0; //currently selected option_selected
int menu_timer=400; //time needed to go automatically back from menu to press start prompt


extern void delay_loop(int counter);
string keydescriptors[]={
   "(none)",     "A",          "B",          "C",
   "D",          "E",          "F",          "G",
   "H",          "I",          "J",          "K",
   "L",          "M",          "N",          "O",
   "P",          "Q",          "R",          "S",
   "T",          "U",          "V",          "W",
   "X",          "Y",          "Z",          "0",
   "1",          "2",          "3",          "4",
   "5",          "6",          "7",          "8",
   "9",          "NUMPAD 0",   "NUMPAD 1",   "NUMPAD 2",
   "NUMPAD 3",   "NUMPAD 4",   "NUMPAD 5",   "NUMPAD 6",
   "NUMPAD 7",   "NUMPAD 8",   "NUMPAD 9",   "F1",
   "F2",         "F3",         "F4",         "F5",
   "F6",         "F7",         "F8",         "F9",
   "F10",        "F11",        "F12",        "ESC",
   "TILDE",      "MINUS",      "EQUALS",     "BACKSPACE",
   "TAB",        "OPENBRACE",  "CLOSEBRACE", "ENTER",
   "COLON",      "QUOTE",      "BACKSLASH",  "BACKSLASH2",
   "COMMA",      "STOP",       "SLASH",      "SPACE",
   "INSERT",     "DEL",        "HOME",       "END",
   "PGUP",       "PGDN",       "LEFT",       "RIGHT",
   "UP",         "DOWN",       "NUMPAD /",   "ASTERISK",
   "NUMPAD -",  "NUMPAD +",   "NUMPAD DEL",    "NUMPAD ENTER",
   "PRTSCR",     "PAUSE",      "ABNT_C1",    "YEN",
   "KANA",       "CONVERT",    "NOCONVERT",  "AT",
   "CIRCUMFLEX", "COLON2",     "KANJI",      "EQUALS_PAD",
   "BACKQUOTE",  "SEMICOLON",  "COMMAND",    "UNKNOWN1",
   "UNKNOWN2",   "UNKNOWN3",   "UNKNOWN4",   "UNKNOWN5",
   "UNKNOWN6",   "UNKNOWN7",   "UNKNOWN8",   "LEFT SHIFT",
   "RIGHT SHIFT",     "LEFT CTRL",   "RIGHT CTRL",   "ALT",
   "ALTGR",      "LEFT WIN",       "RIGHT WIN",       "MENU",
   "SCRLOCK",    "NUMLOCK",    "CAPSLOCK",   "MAX"
};


void intro_sequence()
{	
	BITMAP *jamyskis;
	BITMAP *allegro;
	BITMAP *linuxlogo;

	jamyskis=load_pcx("./data/jamyskis_logo.pcx", NULL);
	allegro=load_pcx("./data/allegro_logo.pcx", NULL);
	linuxlogo=load_pcx("./data/linux.pcx", NULL);

#ifdef ALLEGRO_LINUX
	if(!jamyskis)
	{
		jamyskis=load_pcx(JAMYSKIS_LOGO, NULL);	
	}
	
	if(!allegro)
	{
		allegro=load_pcx(ALLEGRO_LOGO, NULL);
	}
	
	if(!linuxlogo)
	{
		linuxlogo=load_pcx(LINUX_LOGO, NULL);
	}
#endif

	
	if(!jamyskis||!allegro||!linuxlogo)
	{
		cout << "Could not load intro images";
		allegro_exit();
		abort();
	}
		
	clear_to_color(screen,0);
	set_palette(black_palette);
	rest(2000);
	
	alt_fade_in(jamyskis,5);
		
	blit(jamyskis,screen,0,0,0,0,800,600);
		
	rest(2000);
	alt_fade_between(jamyskis,allegro,5);
		
	blit(allegro,screen,0,0,0,0,800,600);
		
	rest(2000);
	alt_fade_between(allegro,linuxlogo,5);
	
	blit(linuxlogo,screen,0,0,0,0,800,600);
	
	rest(2000);
	alt_fade_out(linuxlogo,5);
	
	destroy_bitmap(jamyskis);
	destroy_bitmap(allegro);
	destroy_bitmap(linuxlogo);
};


void title_screen()
{
	bool keyreleased=false;
	int number_of_redefs_displayed=1;
	int position_achieved=0;
	int hiscore_cursorpos=0;
	char charbuf[2];
	extern int start_level, graphics_style, frames_missed;
	extern int unlockables_unlocked[35];
	
	string difficulties[5] = {
		"CHICKEN",
		"EASY",
		"MEDIUM",
		"HARD",
		"INSANE!"
	};
	
	int r=0,g=0,b=0;
	
	title_screen_active=true;
	
	string options_available[5] = {"START THE MASSACRE","HALL OF HEROES","OPTIONS","CREDITS","EXIT GAME"};
	
	BITMAP *logobuffer=create_bitmap(logo->w,logo->h);
	BITMAP *spike=create_bitmap(400,340);
	
	SAMPLE *move_sound;
	SAMPLE *select_sound;
	SAMPLE *goodbye_sound;
	SAMPLE *credits_sound;
	SAMPLE *options_sound;
	SAMPLE *listhiscores_sound;
	SAMPLE *insane_sound;
	SAMPLE *chicken_sound;
	
	FONT *big_hiscore_font;
	FONT *gamefont_trans;
		
	credits=create_bitmap(SCREEN_W,SCREEN_H);
	hiscore_screen=create_bitmap(SCREEN_W,SCREEN_H);
		
	DUH *titlesong,*hiscoresong;
	AL_DUH_PLAYER *hiscoresongplayer;
	
	ostringstream hiscore_as_string;
	ostringstream numberasstring;	
			
	float percentage=0;
	
	titlesong=dumb_load_mod_quick("./data/titlesong.mod");
	hiscoresong=dumb_load_mod_quick("./data/hiscore.mod");
	big_hiscore_font=load_font("./data/arcade_font_big.pcx",NULL,NULL);
	gamefont_trans=load_font("./data/arcade_font.pcx",NULL,NULL);	
	
#ifdef ALLEGRO_LINUX
	if(!titlesong)
	{
		titlesong=dumb_load_mod_quick(TITLE_SONG);
	}

	if(!hiscoresong)
	{
		hiscoresong=dumb_load_mod_quick(HISCORE_SONG);
	}
	
	if(!big_hiscore_font)
	{
		big_hiscore_font=load_font(BIG_FONT,NULL,NULL);
	}
	
	if(!gamefont_trans)
	{
		gamefont_trans=load_font(GAME_FONT,NULL,NULL);
	}
#endif
	
	make_trans_font(gamefont_trans);
		
	if(!titlesong)
	{
		cout << "Could not load title song";
		allegro_exit();
		abort();
	}
	
	if(!hiscoresong)
	{
		cout << "Could not load hiscore song";
		allegro_exit();
		abort();
	}
	
	if(!big_hiscore_font)
	{
		cout << "Could not load hiscore font";
		allegro_exit();
		abort();
	}
	
	options_sound=oi_load_sfx("options.wav",TITLE_OPTIONS_SOUND);
	credits_sound=oi_load_sfx("credits.wav",TITLE_CREDITS_SOUND);
	goodbye_sound=oi_load_sfx("goodbye.wav",TITLE_GOODBYE_SOUND);
	listhiscores_sound=oi_load_sfx("listhiscores.wav",LIST_HISCORES_SOUND);
	chicken_sound=oi_load_sfx("chicken.wav",OPTIONS_CHICKEN_SOUND);
	insane_sound=oi_load_sfx("insane.wav",OPTIONS_INSANE_SOUND);
	
	move_sound=oi_load_sfx("move.wav",TITLE_MOVE_SOUND);
	select_sound=oi_load_sfx("select.wav",TITLE_SELECT_SOUND);
	
	spike=oi_load_graphic("spike.pcx",SPIKE_IMAGE);
	
	titlesongplayer=al_start_duh(titlesong,2,0,(float) music_volume/10,
		get_config_int("sound", "buffer_size", 4096),
		get_config_int("sound", "sound_freq", 22500));
	
	hiscoresongplayer=al_start_duh(hiscoresong,2,0,(float) music_volume/10,
	get_config_int("sound", "buffer_size", 4096),
	get_config_int("sound", "sound_freq", 22500));
	
	flashcolor=makecol16(255,255,255);
	
	verticalsize=logo->h;
	verticalposition=150;
	logoinvert=false;
	growing=false;
	
	clear(display);	
	
	for(int starcount=0; starcount<500; starcount++)
	{
		stars[starcount].x=rand()%SCREEN_W;
		stars[starcount].y=rand()%SCREEN_H;
		stars[starcount].speed=(rand()%4)+1;
	};
	
	//Check if score has beaten highscore
	
	for(int hiscorecheck=0; hiscorecheck<10; hiscorecheck++)
	{
		if(score>hiscore[hiscorecheck])
		{
			for(int hiscoremove=8; hiscoremove>hiscorecheck-1; hiscoremove--)
			{
				hiscore[hiscoremove+1]=hiscore[hiscoremove];
				strcpy(hiscore_names[hiscoremove+1],hiscore_names[hiscoremove]);
			}
			
			strcpy(hiscore_names[hiscorecheck],"   ");
			
			position_achieved=hiscorecheck;
			title_mode=9;
			
			break;
		}
	}
	
	play_sample(welcome,sfx_volume,128,1000,0);
	install_int_ex(interrupt_time_control_title,BPS_TO_TIMER(60));
	
	while(title_screen_active)
	{
		for(int repeats=0;repeats<frames_missed;repeats++) 
		{
			title_screen_time_critical();
			if(!title_screen_active) break;
		};
		
		frames_missed=0;
		
		clear_to_color(display,0);
		
		al_poll_duh(titlesongplayer);
		
		poll_joystick();
		
    	for(int displaystars=0; displaystars<500; displaystars++)
	 	{
			todisplayx=stars[displaystars].x;
   			todisplayy=stars[displaystars].y;
   			putpixel(display,todisplayx,todisplayy,makecol16(255,255,255));
		}
		
		if(title_mode==1||title_mode==2||title_mode==9)
		{		
			if(logoinvert)
			{		
				draw_sprite_v_flip(logobuffer,logo,0,0);
				masked_stretch_blit(logobuffer,display,0,0,logobuffer->w,logobuffer->h,100,verticalposition/2,logo->w,verticalsize);
			}
			else
			{
				draw_sprite(logobuffer,logo,0,0);
				stretch_sprite(display,logobuffer,100,verticalposition/2,logo->w,verticalsize);
			}
		
			clear_to_color(logobuffer,makecol16(255,0,255));
		}
		
		if(title_mode==1||title_mode==2)
		{		
			textprintf_centre_ex(display,gamefont,400,550,-1,-1,"(C) 2006-2007 DARRYL LECOUNT / JAMYSKIS");
			textprintf_right_ex(display,gamefont,799,0,-1,-1,"VERSION 0.3");
		}
		
		switch(title_mode)
		{
			case 1: //mode 1 is the "press start" prompt
				
				if(flashcolor==1)
				{	
					textprintf_centre_ex(display,gamefont,400,400,flashcolor,0,"PRESS FIRE OR ANY KEY TO BEGIN");
				}
				
				hiscore_as_string.str("");
			
				if((input_pressed()||joy[0].button[0].b)&&keyreleased)
				{
					title_mode=2;
					menu_timer=1000;
					play_sample(select_sound,sfx_volume,128,1000,0);
					keyreleased=false;
				};
				
				if(!keyreleased&&!input_pressed())
				{
					keyreleased=true;
				}

				rest(0);
				
				if(menu_timer<=0)
				{
					alpha_blend=0;
					setup_hiscores_screen();
					title_mode=7;
					menu_timer=1000;
					option_selected=0;
					keyreleased=false;
				}
			
				break;
				
			case 2: //mode 2 is the main menu
				
				set_trans_blender(0,0,0,alpha_blend);
				drawing_mode(DRAW_MODE_TRANS,0,0,0);
				rectfill(display,220,330+(option_selected*42),620,365+(option_selected*42),makecol16(0,0,200));
				drawing_mode(DRAW_MODE_SOLID,0,0,0);

				for(int optionsdisplay=0; optionsdisplay<5; optionsdisplay++)
				{
					textout_ex(display,gamefont,options_available[optionsdisplay].c_str(),240,340+(optionsdisplay*42),-1,-1);
				}
				
				if(alpha_blend>255)
				{
					alpha_blend=0;
				}
				
				poll_joystick();
				
				if(keyreleased)
				{
					if((key[KEY_UP]||key[keysdefined[2]]||joy[0].stick[0].axis[1].d1)&&option_selected>0)
					{
						option_selected--;
						keyreleased=false;
						menu_timer=1000;
						play_sample(move_sound,sfx_volume,128,1000,0);
					}
					else
					if((key[KEY_UP]||key[keysdefined[2]]||joy[0].stick[0].axis[1].d1)&&option_selected==0)
					{
						option_selected=4;
						keyreleased=false;
						menu_timer=1000;
						play_sample(move_sound,sfx_volume,128,1000,0);
					}
				
					if((key[KEY_DOWN]||key[keysdefined[3]]||joy[0].stick[0].axis[1].d2)&&option_selected<4)
					{
						option_selected++;
						keyreleased=false;
						menu_timer=1000;
						play_sample(move_sound,sfx_volume,128,1000,0);
					}
					else
					if((key[KEY_DOWN]||key[keysdefined[3]]||joy[0].stick[0].axis[1].d2)&&option_selected==4)
					{
						option_selected=0;
						keyreleased=false;
						menu_timer=1000;
						play_sample(move_sound,sfx_volume,128,1000,0);
					}

					if(key[keysdefined[4]]||key[KEY_ENTER]||joy[0].button[0].b)
					{
						play_sample(select_sound,sfx_volume,128,1000,0);
						
						switch(option_selected)
						{
							case 0:
								title_screen_active=false;
								break;
							case 1:
								alpha_blend=0;
								title_mode=7;
								keyreleased=false;
								setup_hiscores_screen();
								play_sample(listhiscores_sound,sfx_volume,128,1000,0);
								break;								
							case 2:
								alpha_blend=0;
								title_mode=5;
								keyreleased=false;
								option_selected=0;
								play_sample(options_sound,sfx_volume,128,1000,0);
								break;							
							case 3:
								option_selected=0;
								alpha_blend=0;
								title_mode=3;
								keyreleased=false;
								setup_credits_screen();
								play_sample(credits_sound,sfx_volume,128,1000,0);
								break;
							case 4:
								game_active=false;
								program_active=false;
								al_pause_duh(titlesongplayer);
								play_sample(goodbye_sound,sfx_volume,128,1000,0);
								alt_fade_out(screen,1);			
								title_screen_active=false;
								break;
						}	
					}					
				}
				
				if(input_pressed())
				{
					keyreleased=false;
				}
				else
				{
					keyreleased=true;
				}
				
				if(menu_timer==0)
				{
					menu_timer=400;
					title_mode=1;
					option_selected=0;
					keyreleased=false;
				}
								
				break;
				
			case 3: //mode 3 is the credits screen
				set_trans_blender(0,0,0,alpha_blend);
				draw_trans_sprite(display,credits,0,0);
							
				if(alpha_blend>255)
				{
					alpha_blend=255;
				}
				
				if(!input_pressed())
				{
					keyreleased=true;
				}
				
				if(input_pressed()&&keyreleased)
				{	
					keyreleased=false;
					title_mode=4;
				};
				break;
				
			case 4: //mode 4 is the credits screen fading out 
				set_trans_blender(0,0,0,alpha_blend);
				draw_trans_sprite(display,credits,0,0);
				
				alpha_blend=alpha_blend-4;
			
				if(alpha_blend<0)
				{
					option_selected=0;
					title_mode=2;
					alpha_blend=0;
				}
				break;
			case 5: //mode 5 is the options screen
				set_trans_blender(0,0,0,alpha_blend);
				
				drawing_mode(DRAW_MODE_TRANS,0,0,0);
				rectfill(display,30,122+(option_selected*60),380,152+(option_selected*60),makecol16(0,0,200));
				drawing_mode(DRAW_MODE_SOLID,0,0,0);	
			
				textout_centre_ex(display,gamefont,"OPTIONS",399,50,-1,-1);
				textout_ex(display,gamefont,"DIFFICULTY LEVEL",40,130,-1,-1);
				textout_ex(display,gamefont,difficulties[difficulty].c_str(),540,130,-1,-1);
				textout_ex(display,gamefont,"MUSIC VOLUME",40,190,-1,-1);
				
				for(int volumesteps=0; volumesteps<music_volume+1; volumesteps++)
				{
					if(volumesteps>=0&&volumesteps<=4)
					{
						r=255; g=0; b=0;
					}
					if(volumesteps>=5&&volumesteps<=8)
					{
						r=255; g=255; b=0;
					}
					if(volumesteps>8)
					{
						r=0; g=255; b=0;
					}
										
					rectfill(display,540+(volumesteps*15),190,550+(volumesteps*15),210,makecol16(r,g,b));
				}
				textout_ex(display,gamefont,"SFX VOLUME",40,250,-1,-1);
		for(int volumesteps=0; volumesteps<(sfx_volume/25)+1; volumesteps++)
	{					
		if(volumesteps>=0&&volumesteps<=4)
		{
			r=255; g=0; b=0;
		}
		if(volumesteps>=5&&volumesteps<=8)
		{
			r=255; g=255; b=0;
		}
		if(volumesteps>8)
		{
			r=0; g=255; b=0;
		}
		
		rectfill(display,540+(volumesteps*15),250,550+(volumesteps*15),270,makecol16(r,g,b));
	}
	textout_ex(display,gamefont,"REDEFINE KEYS",40,310,-1,-1);
	textout_ex(display,gamefont,"FULLSCREEN MODE",40,370,-1,-1);
	
	if(!is_windowed_mode())
	{
		textout_ex(display,gamefont,"ON",550,370,-1,-1);
	}
	else
	{
		textout_ex(display,gamefont,"OFF",550,370,-1,-1);
	}
				
	textout_ex(display,gamefont,"UNLOCKABLES",40,430,-1,-1);
			
	textout_ex(display,gamefont,"RETURN TO MENU",40,490,-1,-1);
			
	if(alpha_blend>255)
	{
		alpha_blend=0;
	}				
				poll_joystick();
				
				if(keyreleased)
				{
					if((key[KEY_UP]||key[keysdefined[2]]||joy[0].stick[0].axis[1].d1)&&option_selected>0)
					{
						play_sample(move_sound,sfx_volume,128,1000,0);
						option_selected--;
						keyreleased=false;
					}
					else
					if((key[KEY_UP]||key[keysdefined[2]]||joy[0].stick[0].axis[1].d1)&&option_selected==0)
					{
						play_sample(move_sound,sfx_volume,128,1000,0);
						option_selected=6;
						keyreleased=false;
					}	
				
					if((key[KEY_DOWN]||key[keysdefined[3]]||joy[0].stick[0].axis[1].d2)&&option_selected<6)
					{
						play_sample(move_sound,sfx_volume,128,1000,0);
						option_selected++;
						keyreleased=false;
					}
					else
					if((key[KEY_DOWN]||key[keysdefined[3]]||joy[0].stick[0].axis[1].d2)&&option_selected==6)
					{
						play_sample(move_sound,sfx_volume,128,1000,0);
						option_selected=0;
						keyreleased=false;
					}
					
					if((key[KEY_LEFT]||key[keysdefined[0]]||joy[0].stick[0].axis[0].d1)&&(option_selected<3||option_selected==4))
					{
						play_sample(move_sound,sfx_volume,128,1000,0);
						switch(option_selected)
						{
							case 0:
								if(difficulty>0)
								{
									difficulty--;
									if(difficulty==0)
									{
										play_sample(chicken_sound,sfx_volume,128,1000,0);
									}
								}
								break;
							case 1:
								if(music_volume>0)
								{
									music_volume=music_volume-1;
									al_duh_set_volume(titlesongplayer,(float) music_volume/10);
								}
								break;
							case 2:
								if(sfx_volume>0)
								{
									sfx_volume=sfx_volume-25;
								}
								break;
							case 4:
								if(!is_windowed_mode())
								{
									set_gfx_mode(GFX_TEXT,800,600,0,0);
									set_gfx_mode(GFX_AUTODETECT_WINDOWED,800,600,0,0);
								}
						}
					}
					
					if((key[KEY_RIGHT]||key[keysdefined[1]]||joy[0].stick[0].axis[0].d2)&&(option_selected<3||option_selected==4))
					{
						play_sample(move_sound,sfx_volume,128,1000,0);
						switch(option_selected)
						{
							case 0:
								if(difficulty<4)
								{
									difficulty++;
									if(difficulty==4)
									{
										play_sample(insane_sound,sfx_volume,128,1000,0);
									}
								}
								break;
							case 1:
								if(music_volume<10)
								{
									music_volume=music_volume+1;
									al_duh_set_volume(titlesongplayer,(float) music_volume/10);
								}
								break;
							case 2:
								if(sfx_volume<250)
								{
									sfx_volume=sfx_volume+25;
								}
								break;
							case 4:
								if(is_windowed_mode())
								{
									set_gfx_mode(GFX_TEXT,800,600,0,0);
									set_gfx_mode(GFX_AUTODETECT_FULLSCREEN,800,600,0,0);
								}
						}
					}					

					if(key[KEY_LSHIFT]||key[keysdefined[4]]||key[KEY_SPACE]||key[KEY_ENTER]||joy[0].button[0].b)
					{
						if(option_selected==3||option_selected==5||option_selected==6)
						{
							play_sample(select_sound,sfx_volume,128,1000,0);
						}
						switch(option_selected)
						{
							case 3:
								title_mode=6;
								keyreleased=false;
								option_selected=0;
								break;
							case 5:
								save_config();
								title_mode=10;
								keyreleased=false;
								option_selected=0;
								break;
							case 6:
								save_config();
								title_mode=2;
								keyreleased=false;
								option_selected=0;
								break;
						}	
					}	
				}				
				if(input_pressed())
				{
					keyreleased=false;
				}
				else
				{
					keyreleased=true;
				}
				
				break;					
			case 6: // This is the redefine keys screen
				textout_ex(display,gamefont,"REDEFINE KEYS",270,100,-1,-1);

				for(int keysdisplays=0; keysdisplays<number_of_redefs_displayed; keysdisplays++)
				{
					textout_ex(display,gamefont,keynames[keysdisplays].c_str(),230,200+(keysdisplays*50),-1,-1);
					if(keysdisplays<number_of_redefs_displayed-1)
					{
						textout_ex(display,gamefont,keydescriptors[keysdefined[keysdisplays]].c_str(),460,200+(keysdisplays*50),-1,-1);
					}
				}
					
				for(int scanned_keys=1; scanned_keys<KEY_MAX; scanned_keys++)
				{
					if(key[scanned_keys]&&keyreleased)
					{
						bool alreadyused;
						alreadyused=false;
						
						for(int keysalreadydefd=0; keysalreadydefd<number_of_redefs_displayed-1; keysalreadydefd++)
						{
							if(scanned_keys==keysdefined[keysalreadydefd])
							{
								alreadyused=true;
							}
						}
						
						if(!alreadyused)
						{
							keysdefined[number_of_redefs_displayed-1]=scanned_keys;
							number_of_redefs_displayed++;
							play_sample(select_sound,sfx_volume,128,1000,0);
						}
					}
				}
					
				if(number_of_redefs_displayed>7)
				{
					title_mode=5;
					number_of_redefs_displayed=1;
				}
			
				if(input_pressed())
				{
					keyreleased=false;
				}
				else
				{
					keyreleased=true;
				}
				break;
			case 7: // This is the highscore table
				
				set_trans_blender(0,0,0,alpha_blend);
				draw_trans_sprite(display,hiscore_screen,0,0);
			
				if(alpha_blend>255)
				{
					alpha_blend=255;
				}
				
				if(input_pressed()&&keyreleased)
				{
					title_mode=8;
					menu_timer=400;
					keyreleased=false;
					play_sample(select_sound,sfx_volume,128,1000,0);
				};
				
				if(!keyreleased&&!input_pressed())
				{
					keyreleased=true;
				}
				
				if(menu_timer==0)
				{
					menu_timer=400;
					title_mode=8;
					option_selected=0;
					keyreleased=false;
				}
				
				break;
			case 8: // This is the highscore screen fading out
				set_trans_blender(0,0,0,alpha_blend);
				draw_trans_sprite(display,hiscore_screen,0,0);
				
				alpha_blend=alpha_blend-4;
			
				if(alpha_blend<0)
				{
					option_selected=0;
					title_mode=2;
					alpha_blend=0;
				}
				break;
			case 9: // This is the highscore entry screen
				al_pause_duh(titlesongplayer);
				al_poll_duh(hiscoresongplayer);
			
				textprintf_centre_ex(display,gamefont,SCREEN_W/2,350,-1,-1,"YOU HAVE A NEW HIGH SCORE!");
				textprintf_centre_ex(display,gamefont,SCREEN_W/2,370,-1,-1,"YOU HAVE REACHED POSITION %d",position_achieved+1);
				textprintf_centre_ex(display,gamefont,SCREEN_W/2,420,-1,-1,"ENTER YOUR NAME");
				for(int charprint=0; charprint<3; charprint++)
				{
					// Cheap hack necessary as terminate characters were not being
					// appended properly to the letters to print
					charbuf[0]=hiscore_names[position_achieved][charprint];
					charbuf[1]='\0';
					textout_ex(display,big_hiscore_font,charbuf,(SCREEN_W/2)-75+(charprint*50),460,-1,-1);
				}

				set_trans_blender(0,0,0,alpha_blend);
				drawing_mode(DRAW_MODE_TRANS,0,0,0);
				rectfill(display,(SCREEN_W/2)-75+(hiscore_cursorpos*50),455,(SCREEN_W/2)-30+(hiscore_cursorpos*50),505,makecol16(0,0,200));
				drawing_mode(DRAW_MODE_SOLID,0,0,0);
			
				if(alpha_blend>255)
				{
					alpha_blend=0;
				}
				
				for(int keyscan=KEY_A; keyscan<KEY_9_PAD+1; keyscan++)
				{
					if(key[keyscan]&&keyreleased&&hiscore_cursorpos<3)
					{
						if(keyscan>=KEY_A&&keyscan<=KEY_Z)
						{
							hiscore_names[position_achieved][hiscore_cursorpos]=keyscan+64;
						}
						if(keyscan>=KEY_0&&keyscan<=KEY_9)
						{
							hiscore_names[position_achieved][hiscore_cursorpos]=keyscan+21;
						}
						if(keyscan>=KEY_0_PAD&&keyscan<=KEY_9_PAD)
						{
							hiscore_names[position_achieved][hiscore_cursorpos]=keyscan+11;
						}
						keyreleased=false;
						hiscore_cursorpos++;
					}
				}
				
				if(key[KEY_BACKSPACE]&&keyreleased&&hiscore_cursorpos>0)
				{
					hiscore_cursorpos--;
					hiscore_names[position_achieved][hiscore_cursorpos]=' ';
					keyreleased=false;
				}
				
				if(key[KEY_LEFT]&&keyreleased&&hiscore_cursorpos>0)
				{
					hiscore_cursorpos--;
					keyreleased=false;
				}

				if(key[KEY_RIGHT]&&keyreleased&&hiscore_cursorpos<2)
				{
					hiscore_cursorpos++;
					keyreleased=false;
				}
				
				if(key[KEY_ENTER]||key[KEY_ENTER_PAD])
				{
					hiscore[position_achieved]=score;
					al_pause_duh(hiscoresongplayer);
					al_resume_duh(titlesongplayer);
					save_hiscore();
					title_mode=1;
					keyreleased=false;
				}
				
				if(!input_pressed()&&!keyreleased)
				{
					keyreleased=true;
				}
			
				break;
			case 10: // This is the unlockables menu
				
				percentage=0;
			
				for(int checkunlocks=0; checkunlocks<35; checkunlocks++)
				{
					if(unlockables_unlocked[checkunlocks])
					{
						percentage++;
					}
				}
				
				percentage=(percentage/35)*100;
			
				set_trans_blender(0,0,0,alpha_blend);
				
				drawing_mode(DRAW_MODE_TRANS,0,0,0);
				rectfill(display,30,42+(option_selected*25),770,72+(option_selected*25),makecol16(0,0,200));
				drawing_mode(DRAW_MODE_SOLID,0,0,0);	
			
				textprintf_centre_ex(display,gamefont,399,10,-1,-1,"UNLOCKABLES (%d%% UNLOCKED)",(int) percentage);

				if(unlockables_unlocked[0])
				{
					textout_ex(display,gamefont,"THE UNLOCKABLE MESSAGE",40,50,-1,-1);
				}
				else
				{
					textout_ex(display,gamefont,"????????????",40,50,-1,-1);
				}
			
				if(unlockables_unlocked[1])
				{
					textout_ex(display,gamefont,"START LEVEL",40,75,-1,-1);
					numberasstring.str("");
					numberasstring << start_level;

					textout_right_ex(display,gamefont,numberasstring.str().c_str(),769,75,-1,-1);					
				}
				else
				{
					textout_ex(display,gamefont,"????????????",40,75,-1,-1);
				}

				if(unlockables_unlocked[14])
				{
					textout_ex(display,gamefont,"SPIKE THE CHICKEN",40,100,-1,-1);
				}
				else
				{
					textout_ex(display,gamefont,"????????????",40,100,-1,-1);
				}
				
				if(unlockables_unlocked[15])
				{
					textout_ex(display,gamefont,"INVADERS PONG",40,125,-1,-1);
				}
				else
				{
					textout_ex(display,gamefont,"????????????",40,125,-1,-1);
				}
				
				if(unlockables_unlocked[16])
				{
					textout_ex(display,gamefont,"INVADERS BREAKOUT",40,150,-1,-1);
				}
				else
				{
					textout_ex(display,gamefont,"????????????",40,150,-1,-1);
				}
				
				if(unlockables_unlocked[17])
				{
					textout_ex(display,gamefont,"ENDLESS MODE",40,175,-1,-1);
				}
				else
				{
					textout_ex(display,gamefont,"????????????",40,175,-1,-1);
				}
				
				if(unlockables_unlocked[18])
				{
					textout_ex(display,gamefont,"PLAY ENDING CREDITS",40,200,-1,-1);
				}
				else
				{
					textout_ex(display,gamefont,"????????????",40,200,-1,-1);
				}
				
				if(unlockables_unlocked[19])
				{
					textout_ex(display,gamefont,"GRAPHICS STYLE",40,225,-1,-1);
				
					switch(graphics_style)
					{
						case 0: textout_right_ex(display,gamefont,"NEW",769,225,-1,-1); break;
						case 1: textout_right_ex(display,gamefont,"RETRO",769,225,-1,-1); break;
						case 2: textout_right_ex(display,gamefont,"0.01",769,225,-1,-1); break;						
						case 3: textout_right_ex(display,gamefont,"0.05",769,225,-1,-1); break;
						case 4: textout_right_ex(display,gamefont,"SILLY",769,225,-1,-1); break;
					}
				}
				else
				{
					textout_ex(display,gamefont,"????????????",40,225,-1,-1);
				}
				
				if(unlockables_unlocked[23])
				{
					textout_ex(display,gamefont,"JAMYSKIS DEMO",40,250,-1,-1);
				}
				else
				{
					textout_ex(display,gamefont,"????????????",40,250,-1,-1);
				}
				
				if(unlockables_unlocked[24])
				{
					textout_ex(display,gamefont,"NAUGHTS & CROSSES SCREENS 1",40,275,-1,-1);
				}
				else
				{
					textout_ex(display,gamefont,"????????????",40,275,-1,-1);
				}
				
				if(unlockables_unlocked[25])
				{
					textout_ex(display,gamefont,"NAUGHTS & CROSSES SCREENS 2",40,300,-1,-1);
				}
				else
				{
					textout_ex(display,gamefont,"????????????",40,300,-1,-1);
				}
				
				if(unlockables_unlocked[26])
				{
					textout_ex(display,gamefont,"THE MAKING OF OPEN INVADERS I",40,325,-1,-1);				
				}
				else
				{
					textout_ex(display,gamefont,"????????????",40,325,-1,-1);
				}
				
				if(unlockables_unlocked[27])
				{
					textout_ex(display,gamefont,"THE MAKING OF OPEN INVADERS II",40,350,-1,-1);				
				}
				else
				{
					textout_ex(display,gamefont,"????????????",40,350,-1,-1);
				}
				
				if(unlockables_unlocked[28])
				{
					textout_ex(display,gamefont,"COW ABDUCTION",40,375,-1,-1);				
				}
				else
				{
					textout_ex(display,gamefont,"????????????",40,375,-1,-1);
				}
				
				if(unlockables_unlocked[29])
				{
					textout_ex(display,gamefont,"WILD WEST INVADERS",40,400,-1,-1);			
				}
				else
				{
					textout_ex(display,gamefont,"????????????",40,400,-1,-1);
				}
				
				if(unlockables_unlocked[30])
				{
					textout_ex(display,gamefont,"ALIEN ROMANCE",40,425,-1,-1);				
				}
				else
				{
					textout_ex(display,gamefont,"????????????",40,425,-1,-1);
				}
				
				if(unlockables_unlocked[31])
				{
					textout_ex(display,gamefont,"MATHS WITH THE INVADERS",40,450,-1,-1);				
				}
				else
				{
					textout_ex(display,gamefont,"????????????",40,450,-1,-1);
				}
				
				if(unlockables_unlocked[32])
				{
					textout_ex(display,gamefont,"CAN THE CANS DO THE CAN CAN?",40,475,-1,-1);			
				}
				else
				{
					textout_ex(display,gamefont,"????????????",40,475,-1,-1);
				}
				
				if(unlockables_unlocked[33])
				{
					textout_ex(display,gamefont,"THE INVADER DISCO DANCE",40,500,-1,-1);		
				}
				else
				{
					textout_ex(display,gamefont,"????????????",40,500,-1,-1);
				}
				
				if(unlockables_unlocked[34])
				{
					textout_ex(display,gamefont,"OLD MEETS NEW",40,525,-1,-1);
				}
				else
				{
					textout_ex(display,gamefont,"????????????",40,525,-1,-1);
				}

				textout_ex(display,gamefont,"RETURN TO OPTIONS",40,550,-1,-1);
			
				if(alpha_blend>255)
				{
					alpha_blend=0;
				}
				
				poll_joystick();
				
				if(keyreleased)
				{
					if((key[KEY_UP]||key[keysdefined[2]]||joy[0].stick[0].axis[1].d1)&&option_selected>0)
					{
						play_sample(move_sound,sfx_volume,128,1000,0);
						option_selected--;
						keyreleased=false;
					}
					else
					if((key[KEY_UP]||key[keysdefined[2]]||joy[0].stick[0].axis[1].d1)&&option_selected==0)
					{
						play_sample(move_sound,sfx_volume,128,1000,0);
						option_selected=20;
						keyreleased=false;
					}
				
					if((key[KEY_DOWN]||key[keysdefined[3]]||joy[0].stick[0].axis[1].d2)&&option_selected<20)
					{
						play_sample(move_sound,sfx_volume,128,1000,0);
						option_selected++;
						keyreleased=false;
					}
					else
					if((key[KEY_DOWN]||key[keysdefined[3]]||joy[0].stick[0].axis[1].d2)&&option_selected==20)
					{
						play_sample(move_sound,sfx_volume,128,1000,0);
						option_selected=0;
						keyreleased=false;
					}
					
					if((key[KEY_LEFT]||key[keysdefined[0]]||joy[0].stick[0].axis[0].d1)&&(option_selected==1||option_selected==7))
					{
						if((option_selected==1&&unlockables_unlocked[1])||(option_selected==7&&unlockables_unlocked[19]))
						{
							play_sample(move_sound,sfx_volume,128,1000,0);
							switch(option_selected)
							{
								case 1:
									if(start_level>1)
									{
										start_level--;
									}
									break;
								case 7:
									if(graphics_style>0)
									{
										graphics_style--;
										
										while(!unlockables_unlocked[graphics_style+18]&&graphics_style>0)
										{
											graphics_style--;
										};
									}
									break;
							}
						}
					}
					
					if((key[KEY_RIGHT]||key[keysdefined[1]]||joy[0].stick[0].axis[0].d2)&&(option_selected==1||option_selected==7))
					{
						if((option_selected==1&&unlockables_unlocked[1])||(option_selected==7&&unlockables_unlocked[19]))
						{
							play_sample(move_sound,sfx_volume,128,1000,0);
							switch(option_selected)
							{
								case 1:
									if(start_level<14&&unlockables_unlocked[start_level])
									{
										start_level++;
									}
									break;
								case 7:
									if(graphics_style<4)
									{
										int orig_graphics_style;
										
										orig_graphics_style=graphics_style;
										
										graphics_style++;
										
										while(!unlockables_unlocked[graphics_style+18]&&graphics_style<5)
										{
											graphics_style++;
										};
										
										if(graphics_style==5)
										{
											graphics_style=orig_graphics_style;
										}
									}
									break;
							}
						}
					}					

					if(key[KEY_LSHIFT]||key[keysdefined[4]]||key[KEY_SPACE]||key[KEY_ENTER]||joy[0].button[0].b)
					{
						if(option_selected!=1&&option_selected!=7)
						{
							play_sample(select_sound,sfx_volume,128,1000,0);
						}
						switch(option_selected)
						{
							case 0:
								if(unlockables_unlocked[0])
								{
									title_mode=11;
									keyreleased=false;
								}
								break;
							case 2:
								if(unlockables_unlocked[14])
								{
									title_mode=12;
									alpha_blend=0;
									menu_timer=200;
									option_selected=0;
									keyreleased=false;
								}
								break;
							case 3:
								if(unlockables_unlocked[15])
								{
									title_mode=13;
									keyreleased=false;
								}
								break;
							case 4:
								if(unlockables_unlocked[16])
								{
									title_mode=14;
									keyreleased=false;
								}
								break;
							case 5:
								if(unlockables_unlocked[17])
								{
									title_mode=15;
									keyreleased=false;
								}
								break;
							case 6:
								if(unlockables_unlocked[18])
								{
									title_mode=16;
									keyreleased=false;
								}
								break;
							case 8:
								if(unlockables_unlocked[23])
								{
									title_mode=17;
									keyreleased=false;
								}
								break;
							case 9:
								if(unlockables_unlocked[24])
								{
									title_mode=18;
									keyreleased=false;
								}
								break;
							case 10:
								if(unlockables_unlocked[25])
								{
									title_mode=19;
									keyreleased=false;
								}
								break;
							case 11:
								if(unlockables_unlocked[26])
								{
									title_mode=20;
									keyreleased=false;
								}
								break;
							case 12:
								if(unlockables_unlocked[27])
								{
									title_mode=21;
									keyreleased=false;
								}
								break;
							case 13:
								if(unlockables_unlocked[28])
								{
									title_mode=22;
									keyreleased=false;
								}
								break;
							case 14:
								if(unlockables_unlocked[29])
								{
									title_mode=23;
									keyreleased=false;
								}
								break;
							case 15:
								if(unlockables_unlocked[30])
								{
									title_mode=24;
									keyreleased=false;
								}
								break;
							case 16:
								if(unlockables_unlocked[31])
								{
									title_mode=25;
									keyreleased=false;
								}
								break;
							case 17:
								if(unlockables_unlocked[32])
								{
									title_mode=26;
									keyreleased=false;
								}
								break;
							case 18:
								if(unlockables_unlocked[33])
								{
									title_mode=27;
									keyreleased=false;
								}
								break;
							case 19:
								if(unlockables_unlocked[34])
								{
									title_mode=28;
									keyreleased=false;
								}
								break;
							case 20:
								title_mode=5;
								keyreleased=false;
								option_selected=0;
								break;
						}	
					}	
				}				
				if(input_pressed())
				{
					keyreleased=false;
				}
				else
				{
					keyreleased=true;
				}				
				break;				
			case 11: // This is the unlockable message
				textout_centre_ex(display,gamefont,"WELCOME TO THE INCREDIBLE",SCREEN_W/2,30,-1,-1);
				textout_centre_ex(display,gamefont,"UNLOCKABLE MESSAGE! THIS",SCREEN_W/2,60,-1,-1);
				textout_centre_ex(display,gamefont,"IS THE FIRST FEATURE IN",SCREEN_W/2,90,-1,-1);
				textout_centre_ex(display,gamefont,"OPEN INVADERS TO BE UNLOCKED.",SCREEN_W/2,120,-1,-1);
				textout_centre_ex(display,gamefont,"YOU MANAGED THIS BY COMPLETING",SCREEN_W/2,150,-1,-1);
				textout_centre_ex(display,gamefont,"LEVEL ONE ON ANY DIFFICULTY",SCREEN_W/2,180,-1,-1);
				textout_centre_ex(display,gamefont,"LEVEL. OTHER UNLOCKABLES ARE",SCREEN_W/2,210,-1,-1);
				textout_centre_ex(display,gamefont,"NOT SO EASY TO FIND. TRY",SCREEN_W/2,240,-1,-1);
				textout_centre_ex(display,gamefont,"SCORING POINTS, PLAYING ON",SCREEN_W/2,270,-1,-1);
				textout_centre_ex(display,gamefont,"HARDER DIFFICULTY LEVELS,",SCREEN_W/2,300,-1,-1);
				textout_centre_ex(display,gamefont,"AND MAKING FUNNY SHAPES OUT",SCREEN_W/2,330,-1,-1);
				textout_centre_ex(display,gamefont,"OF THE ALIENS, AND THE",SCREEN_W/2,360,-1,-1);
				textout_centre_ex(display,gamefont,"SECRETS WILL GRADUALLY REVEAL",SCREEN_W/2,390,-1,-1);
				textout_centre_ex(display,gamefont,"THEMSELVES. GOOD LUCK!",SCREEN_W/2,420,-1,-1);
				textout_centre_ex(display,gamefont,"PRESS ANY KEY TO CONTINUE",SCREEN_W/2,480,-1,-1);
	
				if(!input_pressed()&&!keyreleased)
				{
					keyreleased=true;
				}
		
				al_poll_duh(titlesongplayer);
		
				if(keyreleased&&input_pressed())
				{
					keyreleased=false;
					title_mode=10;
				}
				break;
			case 12: // This is the Spike the Chicken messagebuffer
				set_trans_blender(0,0,0,alpha_blend);
			
				switch(option_selected)
				{
					case 0: 
						textout_centre_ex(display,gamefont_trans,"YOUR GIFT FOR COMPLETING THE GAME",SCREEN_W/2,250,-1,-1);
						textout_centre_ex(display,gamefont_trans,"ON CHICKEN MODE!",SCREEN_W/2,270,-1,-1);
						break;
					case 1: 
						textout_centre_ex(display,gamefont_trans,"THE INTREPID...",SCREEN_W/2,250,-1,-1);
						break;
					case 2: 
						textout_centre_ex(display,gamefont_trans,"...THE INCREDIBLE...",SCREEN_W/2,250,-1,-1);
						break;
					case 3: 
						textout_centre_ex(display,gamefont_trans,"...THE INSANE...",SCREEN_W/2,250,-1,-1);
						break;
					case 4:
						draw_trans_sprite(display,spike,200,100);
						textout_centre_ex(display,gamefont_trans,"SPIKE THE CHICKEN!",SCREEN_W/2,520,-1,-1);
						break;
					case 5:
						draw_trans_sprite(display,spike,200,100);
						textout_centre_ex(display,gamefont_trans,"SPIKE THE CHICKEN!",SCREEN_W/2,520,-1,-1);
						if(alpha_blend<0)
						{
							title_mode=10;
							option_selected=0;
						}
					
						break;

				}
				
				if(alpha_blend>255)
				{
					alpha_blend=255;
				}
				
				if(menu_timer<0&&option_selected<4&&title_mode==12)
				{
					menu_timer=200;
					alpha_blend=0;
					option_selected++;
				}
				
				if(keyreleased&&input_pressed()&&option_selected==4)
				{
					keyreleased=false;
					option_selected=5;
				}
				
				if(!input_pressed()&&!keyreleased)
				{
					keyreleased=true;
				}
				break;
			case 13: // This is Invaders Pong
				set_trans_blender(0,0,0,255);
				textout_centre_ex(display,gamefont_trans,"NOT FINISHED",SCREEN_W/2,250,-1,-1);
				textout_centre_ex(display,gamefont_trans,"CHECK BACK IN A LATER VERSION",SCREEN_W/2,270,-1,-1);
				if(!input_pressed()&&!keyreleased)
				{
					keyreleased=true;
				}
			
				if(keyreleased&&input_pressed())
				{
					keyreleased=false;
					title_mode=10;
				}
				break;
			case 14: // This is Invaders Breakout
				set_trans_blender(0,0,0,255);
				textout_centre_ex(display,gamefont_trans,"NOT FINISHED",SCREEN_W/2,250,-1,-1);
				textout_centre_ex(display,gamefont_trans,"CHECK BACK IN A LATER VERSION",SCREEN_W/2,270,-1,-1);
				if(!input_pressed()&&!keyreleased)
				{
					keyreleased=true;
				}
			
				if(keyreleased&&input_pressed())
				{
					keyreleased=false;
					title_mode=10;
				}
				break;
			case 15: // This is endless mode
				set_trans_blender(0,0,0,255);
				textout_centre_ex(display,gamefont_trans,"NOT FINISHED",SCREEN_W/2,250,-1,-1);
				textout_centre_ex(display,gamefont_trans,"CHECK BACK IN A LATER VERSION",SCREEN_W/2,270,-1,-1);
				if(!input_pressed()&&!keyreleased)
				{
					keyreleased=true;
				}
			
				if(keyreleased&&input_pressed())
				{
					keyreleased=false;
					title_mode=10;
				}
				break;
			case 17: // This is the ending credits
				set_trans_blender(0,0,0,255);
				textout_centre_ex(display,gamefont_trans,"NOT FINISHED",SCREEN_W/2,250,-1,-1);
				textout_centre_ex(display,gamefont_trans,"CHECK BACK IN A LATER VERSION",SCREEN_W/2,270,-1,-1);
				if(!input_pressed()&&!keyreleased)
				{
					keyreleased=true;
				}
			
				if(keyreleased&&input_pressed())
				{
					keyreleased=false;
					title_mode=10;
				}
				break;
			case 18: // This is Naughts and Crosses Screen 1
				set_trans_blender(0,0,0,255);
				textout_centre_ex(display,gamefont_trans,"NOT FINISHED",SCREEN_W/2,250,-1,-1);
				textout_centre_ex(display,gamefont_trans,"CHECK BACK IN A LATER VERSION",SCREEN_W/2,270,-1,-1);
				if(!input_pressed()&&!keyreleased)
				{
					keyreleased=true;
				}
			
				if(keyreleased&&input_pressed())
				{
					keyreleased=false;
					title_mode=10;
				}
				break;
			case 19: // This is Naughts and Crosses Screen 2
				set_trans_blender(0,0,0,255);
				textout_centre_ex(display,gamefont_trans,"NOT FINISHED",SCREEN_W/2,250,-1,-1);
				textout_centre_ex(display,gamefont_trans,"CHECK BACK IN A LATER VERSION",SCREEN_W/2,270,-1,-1);
				if(!input_pressed()&&!keyreleased)
				{
					keyreleased=true;
				}
			
				if(keyreleased&&input_pressed())
				{
					keyreleased=false;
					title_mode=10;
				}
				break;
			case 20: // This is the making of Open Invaders I
				set_trans_blender(0,0,0,255);
				textout_centre_ex(display,gamefont_trans,"NOT FINISHED",SCREEN_W/2,250,-1,-1);
				textout_centre_ex(display,gamefont_trans,"CHECK BACK IN A LATER VERSION",SCREEN_W/2,270,-1,-1);
				if(!input_pressed()&&!keyreleased)
				{
					keyreleased=true;
				}
			
				if(keyreleased&&input_pressed())
				{
					keyreleased=false;
					title_mode=10;
				}
				break;
			case 21: // This is the making of Open Invaders II
				set_trans_blender(0,0,0,255);
				textout_centre_ex(display,gamefont_trans,"NOT FINISHED",SCREEN_W/2,250,-1,-1);
				textout_centre_ex(display,gamefont_trans,"CHECK BACK IN A LATER VERSION",SCREEN_W/2,270,-1,-1);
				if(!input_pressed()&&!keyreleased)
				{
					keyreleased=true;
				}
			
				if(keyreleased&&input_pressed())
				{
					keyreleased=false;
					title_mode=10;
				}
				break;			
			case 22: // This is Cow Abduction
				set_trans_blender(0,0,0,255);
				textout_centre_ex(display,gamefont_trans,"NOT FINISHED",SCREEN_W/2,250,-1,-1);
				textout_centre_ex(display,gamefont_trans,"CHECK BACK IN A LATER VERSION",SCREEN_W/2,270,-1,-1);
				if(!input_pressed()&&!keyreleased)
				{
					keyreleased=true;
				}
			
				if(keyreleased&&input_pressed())
				{
					keyreleased=false;
					title_mode=10;
				}
				break;
			case 23: // This is Wild West Invaders
				set_trans_blender(0,0,0,255);
				textout_centre_ex(display,gamefont_trans,"NOT FINISHED",SCREEN_W/2,250,-1,-1);
				textout_centre_ex(display,gamefont_trans,"CHECK BACK IN A LATER VERSION",SCREEN_W/2,270,-1,-1);
				if(!input_pressed()&&!keyreleased)
				{
					keyreleased=true;
				}
			
				if(keyreleased&&input_pressed())
				{
					keyreleased=false;
					title_mode=10;
				}
				break;
			case 24: // This is Alien Romance
				set_trans_blender(0,0,0,255);
				textout_centre_ex(display,gamefont_trans,"NOT FINISHED",SCREEN_W/2,250,-1,-1);
				textout_centre_ex(display,gamefont_trans,"CHECK BACK IN A LATER VERSION",SCREEN_W/2,270,-1,-1);
				if(!input_pressed()&&!keyreleased)
				{
					keyreleased=true;
				}
			
				if(keyreleased&&input_pressed())
				{
					keyreleased=false;
					title_mode=10;
				}
				break;
			case 25: // This is Maths with the Invaders
				set_trans_blender(0,0,0,255);
				textout_centre_ex(display,gamefont_trans,"NOT FINISHED",SCREEN_W/2,250,-1,-1);
				textout_centre_ex(display,gamefont_trans,"CHECK BACK IN A LATER VERSION",SCREEN_W/2,270,-1,-1);
				if(!input_pressed()&&!keyreleased)
				{
					keyreleased=true;
				}
			
				if(keyreleased&&input_pressed())
				{
					keyreleased=false;
					title_mode=10;
				}
				break;
			case 26: // This is Can the Cans Do The Can Can
				set_trans_blender(0,0,0,255);
				textout_centre_ex(display,gamefont_trans,"NOT FINISHED",SCREEN_W/2,250,-1,-1);
				textout_centre_ex(display,gamefont_trans,"CHECK BACK IN A LATER VERSION",SCREEN_W/2,270,-1,-1);
				if(!input_pressed()&&!keyreleased)
				{
					keyreleased=true;
				}
			
				if(keyreleased&&input_pressed())
				{
					keyreleased=false;
					title_mode=10;
				}
				break;
			case 27: // This is Invader Disco Dance
				set_trans_blender(0,0,0,255);
				textout_centre_ex(display,gamefont_trans,"NOT FINISHED",SCREEN_W/2,250,-1,-1);
				textout_centre_ex(display,gamefont_trans,"CHECK BACK IN A LATER VERSION",SCREEN_W/2,270,-1,-1);
				if(!input_pressed()&&!keyreleased)
				{
					keyreleased=true;
				}
			
				if(keyreleased&&input_pressed())
				{
					keyreleased=false;
					title_mode=10;
				}
				break;
			case 28: // Old Meets New
				set_trans_blender(0,0,0,255);
				textout_centre_ex(display,gamefont_trans,"NOT FINISHED",SCREEN_W/2,250,-1,-1);
				textout_centre_ex(display,gamefont_trans,"CHECK BACK IN A LATER VERSION",SCREEN_W/2,270,-1,-1);
				if(!input_pressed()&&!keyreleased)
				{
					keyreleased=true;
				}
			
				if(keyreleased&&input_pressed())
				{
					keyreleased=false;
					title_mode=10;
				}
				break;

		}
		
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
		
		blit(display,screen,0,0,0,0,800,600);
	};
	
	remove_int(interrupt_time_control_title);
	
	destroy_bitmap(logobuffer);
	destroy_bitmap(credits);
	destroy_bitmap(hiscore_screen);
	al_stop_duh(titlesongplayer);

	if(option_selected==0&&program_active)
	{
		play_sample(letsgo,sfx_volume,128,1000,0);
		alt_fade_out(screen,10);
	}
	
	unload_duh(titlesong);
	unload_duh(hiscoresong);
	
	destroy_sample(select_sound);
	destroy_sample(move_sound);
	destroy_sample(goodbye_sound);
	destroy_sample(credits_sound);
	destroy_sample(options_sound);
	destroy_sample(listhiscores_sound);
	destroy_sample(insane_sound);
	destroy_sample(chicken_sound);
};

void setup_credits_screen()
{	
	clear_to_color(credits,makecol16(255,0,255));
	
	textout_ex(credits,gamefont,"OPEN INVADERS",270,15,-1,-1);
	textout_ex(credits,gamefont,"VERSION 0.3",290,40,-1,-1);
	textout_ex(credits,gamefont,"(C) 2006 - 2007 JAMYSKIS",160,70,-1,-1);
	textout_ex(credits,gamefont,"PROGRAMMING",30,115,-1,-1);
	textout_ex(credits,gamefont,"DESIGN",30,135,-1,-1);
	textout_ex(credits,gamefont,"VOICE ACTING",30,155,-1,-1);
	textout_ex(credits,gamefont,"GRAPHICS",30,185,-1,-1);
	textout_ex(credits,gamefont,"LOGO FONT",30,235,-1,-1);
	textout_ex(credits,gamefont,"GAME FONT",30,265,-1,-1);
	textout_ex(credits,gamefont,"COLLISION DETECTION",30,295,-1,-1);
	textout_ex(credits,gamefont,"ALLEGRO LIBRARY",30,325,-1,-1);
	textout_ex(credits,gamefont,"MUSIC",30,355,-1,-1);
	textout_ex(credits,gamefont,"TESTING",30,445,-1,-1);
	textout_ex(credits,gamefont,"DARRYL LECOUNT",435,115,-1,-1);
	textout_ex(credits,gamefont,"AMBER ADAMS",435,185,-1,-1);
	textout_ex(credits,gamefont,"DARRYL LECOUNT",435,205,-1,-1);
	textout_ex(credits,gamefont,"JAKOB FISCHER",435,235,-1,-1);
	textout_ex(credits,gamefont,"CODY BOISCLAIR",435,265,-1,-1);
	textout_ex(credits,gamefont,"ORZ",435,295,-1,-1);
	textout_ex(credits,gamefont,"SHAWN HARGREAVES",435,325,-1,-1);
	textout_ex(credits,gamefont,"ROBERT SCHNEIDER",435,355,-1,-1);
	textout_ex(credits,gamefont,"AKA META",435,375,-1,-1);
	//textout_ex(credits,gamefont,"NOISE",435,395,-1,-1);
	//textout_ex(credits,gamefont,"FLYGUY",435,415,-1,-1);
	textout_ex(credits,gamefont,"AMBER ADAMS",435,445,-1,-1);
	textout_ex(credits,gamefont,"THE LINUX GUY",435,465,-1,-1);
	textout_ex(credits,gamefont,"LAUREN MAGGS",435,485,-1,-1);
	textout_ex(credits,gamefont,"VISIT JAMYSKIS ONLINE",200,520,-1,-1);
	textout_ex(credits,gamefont,"AT WWW.JAMYSKIS.NET",220,545,-1,-1);
}

void setup_hiscores_screen()
{
	ostringstream hiscorebuffer;
	
	clear_to_color(hiscore_screen,makecol16(255,0,255));

	textout_centre_ex(hiscore_screen,gamefont,"GREATEST HEROES",SCREEN_W/2,80,-1,-1);

	for(int disphigh=0; disphigh<10; disphigh++)
	{
		hiscorebuffer.str("");
		hiscorebuffer << hiscore[disphigh];

		textout_ex(hiscore_screen,gamefont,hiscore_names[disphigh],(SCREEN_W/2)-100,200+(disphigh*30),-1,-1);
		textout_right_ex(hiscore_screen,gamefont,hiscorebuffer.str().c_str(),(SCREEN_W/2)+100,200+(disphigh*30),-1,-1);
	}
}

void title_screen_time_critical()
{
	static int flashdelay=0;
	
	flashdelay++;
		
	if(flashdelay==10)
	{
		flashdelay=0;
		if(flashcolor==0)
		{
			flashcolor=1;
		}
		else
		{
			flashcolor=0;
		};
	};
		
	for(int displaystars=0; displaystars<500; displaystars++)
	{
   		stars[displaystars].x=stars[displaystars].x-stars[displaystars].speed;
   		if(stars[displaystars].x<0)
   		{
   			stars[displaystars].x=SCREEN_W-1;
   			stars[displaystars].y=rand()%SCREEN_H;
   			stars[displaystars].speed=(rand()%20)+1;
   			stars[displaystars].color=(rand()%14)+1;
   		};
	};
	
	if(title_mode==1||title_mode==2||title_mode==9)
	{
		if(verticalsize<=0&&logoinvert)
		{
			growing=true;
			verticalsize=1;
			logoinvert=false;
		}
		
		if(verticalsize>=logo->h&&logoinvert)
		{
			growing=false;
			verticalsize=logo->h;
		}
	
		if(verticalsize<=0&&!logoinvert)
		{
			growing=true;
			verticalsize=1;
			logoinvert=true;
		}
	
		if(verticalsize>=logo->h&&!logoinvert)
		{
			growing=false;
			verticalsize=logo->h;
		}
		
		if(growing)
		{
			verticalsize=verticalsize+3;
			verticalposition=verticalposition-3;
		}
		else
		{	
			verticalsize=verticalsize-3;
			verticalposition=verticalposition+3;
		}
	}
	
	switch(title_mode)
	{
		case 1:
			menu_timer--;
			break;
		case 2:
			alpha_blend=alpha_blend+5;
			menu_timer--;
			break;
		case 3:
			alpha_blend=alpha_blend+2;
			break;
		case 5:
			alpha_blend=alpha_blend+5;
			break;
		case 7:
			alpha_blend=alpha_blend+2;
			menu_timer--;
			break;
		case 9:
			alpha_blend=alpha_blend+5;
			break;
		case 10:
			alpha_blend=alpha_blend+5;
			break;
		case 12:
			if(option_selected<5)
			{
				alpha_blend=alpha_blend+2;
			}
			else
			{
				alpha_blend=alpha_blend-2;
			}
		
			menu_timer--;
			break;
	}
}



