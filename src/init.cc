/***************************************************************************
 *            init.cc
 *
 *  Fri Jun  1 00:11:12 2007
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
#include <string.h>
#include <iostream>
#include <sstream>
#include <fstream>

#include "config.h"
#include "headers/init.h"
#include "headers/declare.h"
#include "headers/input.h"

using namespace std;
 
extern int score, lives, posx, wavedirection, level, delay, ufo_delay, projlimit;
extern int next_extra_life, bkoffsetx, bkoffsety, game_condition, hiscore[10], difficulty;
extern char hiscore_names[10][4];
extern int music_volume, fullscreen_mode;
extern int sfx_volume;
extern bool in_process_of_fading, game_active, program_active, hiscore_beaten;
extern item bulletposition, ufoposition, shipposition;
extern BITMAP *shield_original, *shield_copies[3];
extern BITMAP *logo, *level2bk[2], *level3bk, *level4bk[2], *level5bk, *level6bk;
extern BITMAP *level7bk,*level9bk[3],*level11bk, *level12bk, *level13bk, *level14bk;
extern BITMAP *ship,*projectile,*alien[4][4],*death[11],*ufo;
extern PALETTE gamepalette;
extern FONT *gamefont;
extern SAMPLE *shoot, *explode, *newhighscore, *screenshotsaved, *welcome;
extern SAMPLE *letsgo, *gameisover;
extern startype stars[500], clouds[4], meteors[10], gummis[30];
extern int keysdefined[7];
extern string keynames[7];
extern int ufos_shot_in_a_row, missed_in_a_row;
bool life_lost;
int start_level=1, graphics_style=0;
DUH *gamesong;
AL_DUH_PLAYER *gamesongplayer;

// Graphics style, 0 = normal, 1 = 0.01, 2 = 0.05, 3 = silly


void load_data_files()
{
	bool failed=false;
	BITMAP *alienbuffer, *explosbuffer;
	
	alienbuffer=create_bitmap(192,64);
	explosbuffer=create_bitmap(704,64);
	
	cout << "Loading graphics...\n";
	
	ship=oi_load_graphic("ship.pcx",GFX_SHIP);
	
	for(int aliens=0; aliens<4; aliens++)
	{
		switch(aliens)
		{
			case 0: alienbuffer=oi_load_graphic("invader_top.pcx",GFX_ALIEN_TOP); break;
			case 1: alienbuffer=oi_load_graphic("invader_middle.pcx",GFX_ALIEN_MIDDLE); break;
			case 2: alienbuffer=oi_load_graphic("invader_bottom.pcx",GFX_ALIEN_BOTTOM); break;
			case 3: alienbuffer=oi_load_graphic("invader_bottom.pcx",GFX_ALIEN_BOTTOM); break;
		}
		
		for(int animframes=0; animframes<3; animframes++)
		{
			alien[aliens][animframes]=create_bitmap(64,64);
			blit(alienbuffer,alien[aliens][animframes],animframes*64,0,0,0,64,64);
		}
		
		alien[aliens][3]=create_bitmap(64,64);
		blit(alien[aliens][1],alien[aliens][3],0,0,0,0,64,64);
	}
	
	explosbuffer=oi_load_graphic("explosion.pcx",GFX_EXPLOSION);
	
	for(int deathanim=0; deathanim<11; deathanim++)
	{
		death[deathanim]=create_bitmap(64,64);
		blit(explosbuffer,death[deathanim],deathanim*64,0,0,0,64,64);
	}
	
	ufo=oi_load_graphic("ufo.pcx",GFX_UFO);
	projectile=oi_load_graphic("bullet.pcx",GFX_BULLET);
	shield_original=oi_load_graphic("shield.pcx",SHIELD_GRAPHIC);
	logo=oi_load_graphic("oi_logo.pcx",OPEN_INVADERS_LOGO);
	level2bk[0]=oi_load_graphic("level2bk.pcx",LEVEL_TWO_BACKGROUND_LEFT);
	level2bk[1]=oi_load_graphic("level2bk2.pcx",LEVEL_TWO_BACKGROUND_RIGHT);
	level3bk=oi_load_graphic("level3bk.pcx",LEVEL_THREE_BACKGROUND);
	level4bk[0]=oi_load_graphic("level4bk.pcx",LEVEL_FOUR_BACKGROUND_ONE);
	level4bk[1]=oi_load_graphic("level4bk2.pcx",LEVEL_FOUR_BACKGROUND_TWO);
	level5bk=oi_load_graphic("level5bk.pcx",LEVEL_FIVE_BACKGROUND);
	level6bk=oi_load_graphic("level6bk.pcx",LEVEL_SIX_BACKGROUND);
	level7bk=oi_load_graphic("level7bk.pcx",LEVEL_SEVEN_BACKGROUND);
	level9bk[0]=oi_load_graphic("level9bk1.pcx",LEVEL_NINE_BACKGROUND_RED);
	level9bk[1]=oi_load_graphic("level9bk2.pcx",LEVEL_NINE_BACKGROUND_GREEN);
	level9bk[2]=oi_load_graphic("level9bk3.pcx",LEVEL_NINE_BACKGROUND_YELLOW);
	level11bk=oi_load_graphic("level11bk.pcx",LEVEL_ELEVEN_BACKGROUND);
	level12bk=oi_load_graphic("level12bk.pcx",LEVEL_TWELVE_BACKGROUND);
	level13bk=oi_load_graphic("level13bk.pcx",LEVEL_THIRTEEN_BACKGROUND);
	level14bk=oi_load_graphic("level14bk.pcx",LEVEL_FOURTEEN_BACKGROUND);
	
	cout << "Loading sound effects...\n";
	
	shoot=oi_load_sfx("shoot.wav",SHOOT_SOUND);
	explode=oi_load_sfx("destroy.wav",DESTROY_SOUND);
	newhighscore=oi_load_sfx("new_high_score.wav",NEW_HIGH_SCORE_SOUND);
	screenshotsaved=oi_load_sfx("screenshotsaved.wav",SCREENSHOT_SOUND);
	welcome=oi_load_sfx("welcome.wav",WELCOME_SOUND);
	letsgo=oi_load_sfx("letsgo.wav",LETS_GO_SOUND);
	gameisover=oi_load_sfx("gameisover.wav",GAME_IS_OVER_SOUND);
	
	cout << "Loading in-game song...\n";
	gamesong=dumb_load_mod_quick("data/gamesong.mod");
		
	gamefont=load_bitmap_font("data/arcade_font.pcx",0,0);
	
	cout << "Loading highscore file...\n";
	load_hiscore();
	
	cout << "Loading config file...\n";
	load_config();
	
	if(!gamefont)
	{
		gamefont=load_bitmap_font(GAME_FONT,0,0);
	}

	if(!gamefont)
	{
		cout << "Fatal error: Could not load file arcade_font.pcx\n";
		failed=true;
	}
	
	if(!gamesong)
	{
		gamesong=dumb_load_mod_quick(GAME_SONG);
	}

	if(!gamesong)
	{
		cout << "Fatal error: Could not load file gamesong.mod\n";
		failed=true;
	}

	destroy_bitmap(alienbuffer);
	destroy_bitmap(explosbuffer);
};

void load_hiscore()
{
	ostringstream filepath;
	int checksum;
	
	#ifndef ALLEGRO_LINUX
	filepath << "data/hiscore.dat";
	#else
	filepath << getenv("HOME") << "/.openinvaders/hiscore";
	#endif
	
	ifstream hiscorefile;
	char teststring[5];
	bool valid_file=true;
	bool skip_checksum=false;
	extern int unlockables_unlocked[35];
	
	hiscorefile.open(filepath.str().c_str(),ios::binary);
	
	hiscorefile.read(teststring,4);
	
	if(!memcmp(teststring,"V003",4)==0&&hiscorefile.is_open())
	{
		clear(screen);
		textout_ex(screen, gamefont, "PLEASE NOTE", 30,30,-1,-1);
		textout_ex(screen, gamefont, "Open Invaders has detected that you", 30,80,-1,-1);
		textout_ex(screen, gamefont, "have a highscore file from version", 30,110,-1,-1);		
		textout_ex(screen, gamefont, "0.2. The game now uses a different",30,140,-1,-1);
		textout_ex(screen, gamefont, "extended format, so your highscore", 30,170,-1,-1);
		textout_ex(screen, gamefont, "will be lost. Sorry.", 30,200,-1,-1);
		textout_ex(screen, gamefont, "PRESS ANY KEY TO CONTINUE", 30,250,-1,-1);
		
		do {}
		while(!keypressed());
		valid_file=false;
		skip_checksum=true;
	}
	
	clear_keybuf();
	
	if(!hiscorefile.is_open())
	{
		valid_file=false;
	}
	
	if(valid_file)
	{
		for(int readhiscores=0; readhiscores<10; readhiscores++)
		{
			hiscorefile.read(reinterpret_cast<char*>(&hiscore_names[readhiscores]),4);
			hiscorefile.read(reinterpret_cast<char*>(&hiscore[readhiscores]),sizeof(hiscore[0]));
		}
	}
	
	int totalhiscore=0;
	
	for(int total=0; total<10; total++)
	{
		totalhiscore=totalhiscore+hiscore[total];
	}
	
	hiscorefile.read(reinterpret_cast<char*>(&checksum),sizeof(checksum));
	
	if(valid_file)
	{
		for(int readunlockables=0; readunlockables<35; readunlockables++)
		{
			hiscorefile.read(reinterpret_cast<char*>(&unlockables_unlocked[readunlockables]),sizeof(unlockables_unlocked[readunlockables]));
		}
	}
	
	if(checksum!=(totalhiscore*239)-686&&!skip_checksum&&hiscorefile.is_open())
	{
		clear(screen);
		textout_ex(screen, gamefont, "NAUGHTY, NAUGHTY!", 30,30,-1,-1);
		textout_ex(screen, gamefont, "It looks like you've been trying to", 30,80,-1,-1);
		textout_ex(screen, gamefont, "cheat by changing your highscore", 30,110,-1,-1);		
		textout_ex(screen, gamefont, "file. You obviously didn't know", 30,140,-1,-1);
		textout_ex(screen, gamefont, "about the security measures in place", 30,170,-1,-1);
		textout_ex(screen, gamefont, "to stop cheating gits like you doing", 30,200,-1,-1);
		textout_ex(screen, gamefont, "that. DON'T DO IT.",30,230,-1,-1);
		textout_ex(screen, gamefont, "PRESS ANY KEY TO CONTINUE", 30,280,-1,-1);
		
		do {}
		while(!keypressed());
		valid_file=false;
	}
		
	if(!valid_file)
	{
		for(int defhiscores=0; defhiscores<10; defhiscores++)
		{
			hiscore[defhiscores]=((10-defhiscores)*7300)+600;
		}

		strcpy(hiscore_names[0],"DAZ");
		strcpy(hiscore_names[1],"YIK");
		strcpy(hiscore_names[2],"IST");
		strcpy(hiscore_names[3],"BOB");
		strcpy(hiscore_names[4],"ROB");
		strcpy(hiscore_names[5],"ZED");
		strcpy(hiscore_names[6],"GUM");
		strcpy(hiscore_names[7],"LDP");
		strcpy(hiscore_names[8],"RAM");
		strcpy(hiscore_names[9],"JAY");
		
		save_hiscore();
	}
	
	hiscorefile.close();
}

void save_hiscore()
{
	extern int unlockables_unlocked[35];
	
	ostringstream filepath;
	int totalhiscore=0;
	
	for(int total=0; total<10; total++)
	{
		totalhiscore=totalhiscore+hiscore[total];
	}
	
	int checksum = (totalhiscore*239)-686; //simple, I know!
	
	#ifndef ALLEGRO_LINUX
	filepath << "data/hiscore.dat";
	#else
	filepath << getenv("HOME") << "/.openinvaders";
	mkdir(filepath.str().c_str(),S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH);
	filepath << "/hiscore";	
	#endif
	
	ofstream hiscorefile;
	hiscorefile.open(filepath.str().c_str(),ios::binary);
	
	hiscorefile.write("V003",4);
	for(int readhiscores=0; readhiscores<10; readhiscores++)
	{
		hiscorefile.write(reinterpret_cast<char*>(&hiscore_names[readhiscores]),4);
		hiscorefile.write(reinterpret_cast<char*>(&hiscore[readhiscores]),sizeof(hiscore[0]));
	}
	
	hiscorefile.write(reinterpret_cast<char*>(&checksum),sizeof(checksum));
	
	for(int readunlockables=0; readunlockables<35; readunlockables++)
	{
		hiscorefile.write(reinterpret_cast<char*>(&unlockables_unlocked[readunlockables]),sizeof(&unlockables_unlocked));
	}
	
	hiscorefile.close();
}


void load_config()
{
	ostringstream filepath;
	
	#ifndef ALLEGRO_LINUX
	filepath << "data/config.dat";
	#else
	filepath << getenv("HOME") << "/.openinvaders";
	mkdir(filepath.str().c_str(),S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH);
	filepath << "/config";	
	#endif
	
	ifstream configfile;
	string string_to_compare;
	string parameter,value;
	int position_of_equals;
	int length_of_value;
	bool success_check; 
	bool verified=false;
	configfile.open(filepath.str().c_str(),ios::binary);
	
	do
	{
		success_check=configfile >> string_to_compare;
		if(success_check)
		{
			position_of_equals=string_to_compare.find("=");
			if(position_of_equals>-1)
			{
				parameter=string_to_compare.substr(0,position_of_equals);
				length_of_value=string_to_compare.length()-position_of_equals;
				value=string_to_compare.substr(position_of_equals+1,length_of_value);
				if(parameter=="version"&&value=="0.3")
				{
					verified=true;
				}
				
				if(parameter=="difficulty")
				{
					difficulty=atoi(value.c_str());
					if(difficulty<0||difficulty>4)
					{
						difficulty=2;
					}
				}

				if(parameter=="fullscreen"&&value=="true"&&fullscreen_mode==0)
				{
					set_gfx_mode(GFX_TEXT,800,600,0,0);
					set_gfx_mode(GFX_AUTODETECT_FULLSCREEN,800,600,0,0);
					fullscreen_mode=4;
				}

				if(parameter=="fullscreen"&&value=="false"&&fullscreen_mode==0)
				{
					set_gfx_mode(GFX_TEXT,800,600,0,0);
					set_gfx_mode(GFX_AUTODETECT_WINDOWED,800,600,0,0);
					fullscreen_mode=4;
				}
				
				if(parameter=="vol_music")
				{
					music_volume=atoi(value.c_str());
					if(music_volume<0||music_volume>10)
					{
						music_volume=5;
					}
				}
				
				if(parameter=="vol_sfx")
				{
					sfx_volume=atoi(value.c_str());
					if(sfx_volume<0||sfx_volume>250)
					{
						sfx_volume=250;
					}
				}
				
				if(parameter=="keyboard_LEFT")
				{
					keysdefined[0]=atoi(value.c_str());
					if(keysdefined[0]<1||keysdefined[0]>=KEY_MAX)
					{
						keysdefined[0]=KEY_LEFT;
					}
				}					
				
				if(parameter=="keyboard_RIGHT")
				{
					keysdefined[1]=atoi(value.c_str());
					if(keysdefined[1]<1||keysdefined[1]>=KEY_MAX)
					{
						keysdefined[1]=KEY_RIGHT;
					}
				}
				
				if(parameter=="keyboard_UP")
				{
					keysdefined[2]=atoi(value.c_str());
					if(keysdefined[2]<1||keysdefined[2]>=KEY_MAX)
					{
						keysdefined[2]=KEY_UP;
					}
				}
				
				if(parameter=="keyboard_DOWN")
				{
					keysdefined[3]=atoi(value.c_str());
					if(keysdefined[3]<1||keysdefined[3]>=KEY_MAX)
					{
						keysdefined[3]=KEY_DOWN;
					}
				}
				
				if(parameter=="keyboard_FIRE")
				{
					keysdefined[4]=atoi(value.c_str());
					if(keysdefined[4]<1||keysdefined[4]>=KEY_MAX)
					{
						keysdefined[4]=KEY_LSHIFT;
					}
				}
				
				if(parameter=="keyboard_PAUSE")
				{
					keysdefined[5]=atoi(value.c_str());
					if(keysdefined[5]<1||keysdefined[5]>=KEY_MAX)
					{
						keysdefined[5]=KEY_P;
					}
				}
				
				if(parameter=="keyboard_QUIT")
				{
					keysdefined[6]=atoi(value.c_str());
					if(keysdefined[6]<1||keysdefined[6]>=KEY_MAX)
					{
						keysdefined[6]=KEY_Q;
					}
				}

			}
		}			
	}
	while(success_check);
		
	configfile.close();
}

void save_config()
{
	ostringstream filepath;
		
	#ifndef ALLEGRO_LINUX
	filepath << "data/config.dat";
	#else
	filepath << getenv("HOME") << "/.openinvaders";
	mkdir(filepath.str().c_str(),S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH);
	filepath << "/config";	
	#endif
	
	ofstream configfile;
	configfile.open(filepath.str().c_str(),ios::binary);
	
	configfile << "version=0.3\n";
	configfile << "fullscreen=";
	
	if(is_windowed_mode())
	{
		configfile << "false\n";
	}
	else
	{
		configfile << "true\n";
	}
		
	configfile << "difficulty=" << difficulty << "\n";
	configfile << "vol_music=" << music_volume << "\n";
	configfile << "vol_sfx=" << sfx_volume << "\n";
	
	for(int a=0; a<7; a++)
	{
		configfile << "keyboard_" << keynames[a] << "=" << keysdefined[a] << "\n";
	}
	
	configfile.close();
}
	  

void initialise_game()
{
	allegro_init();
	atexit(&dumb_exit);
	dumb_register_stdfiles();
	install_timer();
	install_keyboard();
	install_sound(DIGI_AUTODETECT,MIDI_NONE,NULL);
	install_joystick(JOY_TYPE_AUTODETECT);
	install_pmask();
	calibrate_joystick(0);
	install_int(interrupt_keys,60); // This routine checks for CTRL+C or CTRL+S
	difficulty=2;
	music_volume=5;
	sfx_volume=250;
	
	
};

void predefine_variables()
{
	extern particle explosionbits[500];
	extern int scrollposition;
	
	srand(unsigned(time(0)));
	
	score=0;
	lives=3;
	posx=20;
	life_lost=false;
	wavedirection=1;
	ufos_shot_in_a_row=0;
	missed_in_a_row=0;
	level=start_level;
	bulletposition.alive=dead;
	delay=0;
	ufo_delay=0;
	ufoposition.alive=dead;
	ufoposition.xpos=0;
	ufoposition.ypos=40;
	projlimit=4;
	next_extra_life=20000;
	bkoffsetx=0;
	bkoffsety=0;
	in_process_of_fading=false;
	game_condition=0;
	scrollposition=SCREEN_W;
	
	for(int initbits=0; initbits<500; initbits++)
	{
		explosionbits[initbits].active=false;
	}
	
	game_active=true;
	
	shipposition.alive=alive;
	
	reset_shields();
	
	for(int starcount=0; starcount<500; starcount++)
	{
		stars[starcount].x=rand()%SCREEN_W;
		stars[starcount].y=rand()%SCREEN_H;
		stars[starcount].speed=(rand()%4)+1;
		stars[starcount].size=(rand()%2);
		stars[starcount].brightness=rand()%100;
		stars[starcount].color=makecol16((125+rand()%120-stars[starcount].brightness),255-stars[starcount].brightness,(125+rand()%120)-stars[starcount].brightness);
		
	};
	
	for(int cloudcount=0; cloudcount<4; cloudcount++)
	{
		clouds[cloudcount].x=rand()%SCREEN_W;
		clouds[cloudcount].y=rand()%400;
		clouds[cloudcount].speed=(rand()%3)+1;
		clouds[cloudcount].color=rand()%2;
	}
	
	for(int meteorcount=0; meteorcount<10; meteorcount++)
	{
		meteors[meteorcount].x=rand()%SCREEN_W;
		meteors[meteorcount].y=rand()%SCREEN_H;
		meteors[meteorcount].speed=(rand()%7)+3;
	}
	
	for(int gummicount=0; gummicount<30; gummicount++)
	{
		gummis[gummicount].x=rand()%SCREEN_W;
		gummis[gummicount].y=rand()%SCREEN_H;
		gummis[gummicount].speed=(rand()%4)+2;
	}
};

void reset_shields()
{	
	for(int copies=0; copies<3; copies++)
	{
		blit(shield_original,shield_copies[copies],0,0,0,0,140,88);
	};
}

BITMAP *oi_load_graphic(std::string filename, std::string defsymbol)
{
	BITMAP *loaded_graphic;
	ostringstream filenamebuffer;
	
	filenamebuffer << "data/" << filename;
	
	loaded_graphic=load_bitmap(filenamebuffer.str().c_str(),NULL);

#ifdef ALLEGRO_LINUX	
	if(!loaded_graphic)
	{
		loaded_graphic=load_bitmap(defsymbol.c_str(), NULL);
	}
#endif	
	
	if(!loaded_graphic)
	{
		cout << "Fatal error: Could not load file " << filename << "\n";
		return false;
	}
	
	return loaded_graphic;
}


SAMPLE *oi_load_sfx(std::string filename, std::string defsymbol)
{
	SAMPLE *loaded_sound;
	ostringstream filenamebuffer;
	
	filenamebuffer << "data/" << filename;
	
	loaded_sound=load_wav(filenamebuffer.str().c_str());

#ifdef ALLEGRO_LINUX	
	if(!loaded_sound)
	{
		loaded_sound=load_wav(defsymbol.c_str());
	}
#endif	

	if(!loaded_sound)
	{
		cout << "Fatal error: Could not load file " << filename << "\n";
		return false;
	}
	
	return loaded_sound;
}
