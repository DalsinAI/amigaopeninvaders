/***************************************************************************
 *            ending.cc
 *
 *  Sat Jun 16 20:20:48 2007
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
#include <iostream>
#include <sstream>

#include "config.h"
#include "headers/declare.h"
#include "headers/input.h"
#include "headers/ending.h"
#include "headers/graphics.h"
#include "headers/init.h"
#include "headers/unlock.h"

DUH *endingsong;
AL_DUH_PLAYER *endsongplayer;
extern int music_volume;

using namespace std;

int counter;

bool keyreleased=false;

extern int sfx_volume;
extern int start_level;

void game_ending()
{
	// Process final score
	
	extern bool game_active;
	extern FONT *gamefont;
	extern int score, lives, hiscore[10], difficulty;
	extern SAMPLE *newhighscore;
	int textlength;
	int finalscore;
	
	stringstream conversionbuffer[3];
	
	if(start_level==1)
	{
		finalscore=score+10000+(lives*1000);
		
		switch(difficulty)
		{
			case 1: finalscore=finalscore+2000; break;
			case 2: finalscore=finalscore+5000; break;
			case 3: finalscore=finalscore+10000; break;
			case 4: finalscore=finalscore+25000; break;
		};
	}
	else
	{
		finalscore=score+(lives*1000);
	}		
	
	conversionbuffer[0] << "SCORE: " << score;
	conversionbuffer[1] << "BONUS FOR LIVES REMAINING x"<< lives << ": " << lives*1000;
	conversionbuffer[2] << "FINAL SCORE: " << finalscore;
	
	string endinglines[] ={
	"THE WAR IS OVER. THE ALIENS WERE",//0
	"FORCED BACK TO WHENCE THEY CAME.",
	"THE EVIL INVADERS WERE SENT BACK",
	"TO THEIR PUNY HOMEWORLD WITH",
	"THEIR TAILS BETWEEN THEIR LEGS.",
	"",
	"AND THUSLY SPAKE THE PEOPLE OF",
	"EARTH:",
	"\"WE KICKED YO ASSES, MOTHER#""@%$*S!\"",
	"",
	"AT LEAST THEY DID UNTIL",
	"-- OPEN INVADERS II --",
	"",
	"BUT WHAT OF OUR HERO?",
	"",
	"HE'S BACK HOME, BRUSHING UP ON",//15
	"HIS JOYSTICK SKILLS, READY FOR",//16
	"THE INVADERS, LEST THEY DEFY",//17
	"THE HUMAN RACE AGAIN!",//18
	
	conversionbuffer[0].str(),//19
	"",//20
	"COMPLETION BONUS: 10000",//21
	conversionbuffer[1].str(),//22
	conversionbuffer[2].str(),//23
	"THIS IS A NEW HIGH SCORE!", //24
	
	"CREDITS",//25

	"    DESIGN",//26
	"DARRYL LECOUNT",
	
	" PROGRAMMING",//28
	"DARRYL LECOUNT",
	
	"   GRAPHICS",//30
	" AMBER ADAMS",
	"DARRYL LECOUNT", //32
	
	"COLLISION DETECTION CODE",//33
	"         ORZ",
	
	"   TESTING",//35
	" AMBER ADAMS",
	"THE LINUX GUY",
	"LAUREN MAGGS",
	
	"     MUSIC",//39
	"ROBERT SCHNEIDER",
	"    AKA META",
	"",
	"",
	
	"FONT <\"PRESS START\">",//44
	"  CODY BOISCLAIR",
	
	"FONT <\"ABDUCTION 2002\">",//46
	"    JAKOB FISCHER",
	
	" ALLEGRO LIBRARY", //48
	"SHAWN HARGREAVES",
	
	"            THANKS",//51
	"     #C++ AT IRC.QUAKENET.ORG",
	"ESPECIALLY BD-CALVIN AND SPRUDLING",
	
	"THE CREW AT HAPPYPENGUIN.ORG",//53
	
	"THE GUYS AT ALLEGRO.CC",//54
	
	"    A N D  Y O U",//55
	"                     ",
	"THANK YOU FOR PLAYING",//57
	
	"NO BONUS FOR CHICKEN MODE", //58
	"PLAY IT ON A HARDER LEVEL,",
	"YA CHICKEN!",

	"BONUS FOR EASY MODE: 2000", //61
	"NOW TRY IT ON MEDIUM!",
	"",
	
	"BONUS FOR MEDIUM MODE: 5000", //64
	"NOW TRY IT ON HARD!",
	"",
	
	"BONUS FOR HARD MODE: 10000", //67
	"TRY IT ON INSANE FOR THE",
	"ULTIMATE CHALLENGE!",
	
	"ULTIMATE INSANITY BONUS: 25000", //70
	"CONGRATULATIONS!",
	"YOU ARE AN OPEN INVADERS MASTER!",
	
	"NO BONUS FOR USING LEVELSKIP!", //73
	"",
	""
	};
	
	// Load and start ending song
	
	endingsong=dumb_load_mod_quick("data/endsong.mod");
	
	#ifdef ALLEGRO_LINUX
	if(!endingsong)
	{
		endingsong=dumb_load_mod_quick(ENDING_SONG);
	}
	#endif
		
	if(!endingsong)
	{
		cout << "Could not load end song";
		allegro_exit();
		abort();
	}
	
	endsongplayer=al_start_duh(endingsong,2,0,(float) music_volume/10,
		get_config_int("sound", "buffer_size", 4096),
		get_config_int("sound", "sound_freq", 22500));
	
	// Display ending lines
	
	clear(screen);
	
	for(int lines=0; lines<19; lines++)
	{
		textlength=endinglines[lines].length();
		
		for(int individualchars=0; individualchars<textlength+1; individualchars++)
		{
			textprintf_ex(screen,gamefont,40,(lines*22)+40,makecol16(255,255,255),0,endinglines[lines].substr(0,individualchars).c_str());
			delay_with_duh_poll(20,endsongplayer);
		}
	}
	
	delay_with_duh_poll(1700,endsongplayer);
	
	clear(screen);	
		
	for(int lines=19; lines<23; lines++)
	{
		textlength=endinglines[lines].length();
		
		if(!(lines==21&&start_level>1))
		{
			for(int individualchars=0; individualchars<textlength+1; individualchars++)
			{
				textprintf_ex(screen,gamefont,80,((lines-19)*22)+120,makecol16(255,255,255),0,endinglines[lines].substr(0,individualchars).c_str());
				delay_with_duh_poll(20,endsongplayer);
			}
		}
	}
	
	int texty=278;
	
	if(start_level==1)
	{
		for(int lines=58+(difficulty*3); lines<61+(difficulty*3); lines++)
		{
			textlength=endinglines[lines].length();
			
			for(int individualchars=0; individualchars<textlength+1; individualchars++)
			{
				textprintf_ex(screen,gamefont,80,texty,makecol16(255,255,255),0,endinglines[lines].substr(0,individualchars).c_str());
				delay_with_duh_poll(20,endsongplayer);
			}
			
			texty=texty+22;
		}
	}
	else
	{
		for(int lines=73; lines<76; lines++)
		{
			textlength=endinglines[lines].length();
			
			for(int individualchars=0; individualchars<textlength+1; individualchars++)
			{
				textprintf_ex(screen,gamefont,80,texty,makecol16(255,255,255),0,endinglines[lines].substr(0,individualchars).c_str());
				delay_with_duh_poll(20,endsongplayer);
			}
			
			texty=texty+22;
		}
	}
	
	textlength=endinglines[23].length();
	
	for(int individualchars=0; individualchars<textlength+1; individualchars++)
	{
		textprintf_ex(screen,gamefont,80,378,makecol16(255,255,255),0,endinglines[23].substr(0,individualchars).c_str());
		delay_with_duh_poll(20,endsongplayer);
	}
	
	if(finalscore>hiscore[9])
	{
		textlength=endinglines[24].length();
		
		play_sample(newhighscore,sfx_volume,128,1000,0);
		
		for(int individualchars=0; individualchars<textlength+1; individualchars++)
		{
			textprintf_ex(screen,gamefont,80,420,makecol16(255,255,255),0,endinglines[24].substr(0,individualchars).c_str());
			delay_with_duh_poll(20,endsongplayer);
		}
	}
	
	score=finalscore; // So that the unlock code can check if the needed score has been achieved
	
	delay_with_duh_poll(1700,endsongplayer);
	
	clear(screen);
	
	textlength=endinglines[25].length();
	
	for(int individualchars=0; individualchars<textlength+1; individualchars++)
	{
		textprintf_ex(screen,gamefont,340,250,makecol16(255,255,255),0,endinglines[25].substr(0,individualchars).c_str());
		delay_with_duh_poll(20,endsongplayer);
	}
	
	delay_with_duh_poll(1200,endsongplayer);
	
	clear(screen);
	
	for(int lines=26; lines<28; lines++)
	{
		textlength=endinglines[lines].length();
		
		for(int individualchars=0; individualchars<textlength+1; individualchars++)
		{
			textprintf_ex(screen,gamefont,80,((lines-19)*22)+120,makecol16(255,255,255),0,endinglines[lines].substr(0,individualchars).c_str());
			delay_with_duh_poll(20,endsongplayer);
		}
	}
	
	delay_with_duh_poll(1200,endsongplayer);

	clear(screen);
	
	for(int lines=28; lines<30; lines++)
	{
		textlength=endinglines[lines].length();
		
		for(int individualchars=0; individualchars<textlength+1; individualchars++)
		{
			textprintf_ex(screen,gamefont,160,((lines-19)*22)+50,makecol16(255,255,255),0,endinglines[lines].substr(0,individualchars).c_str());
			
			delay_with_duh_poll(20,endsongplayer);
		}
	}
	
	delay_with_duh_poll(1200,endsongplayer);

	clear(screen);
	
	for(int lines=30; lines<33; lines++)
	{
		textlength=endinglines[lines].length();
		
		for(int individualchars=0; individualchars<textlength+1; individualchars++)
		{
			textprintf_ex(screen,gamefont,80,((lines-19)*22)+120,makecol16(255,255,255),0,endinglines[lines].substr(0,individualchars).c_str());
			delay_with_duh_poll(20,endsongplayer);
		}
	}
	
	delay_with_duh_poll(1200,endsongplayer);
	
	clear(screen);
	
	for(int lines=33; lines<35; lines++)
	{
		textlength=endinglines[lines].length();
		
		for(int individualchars=0; individualchars<textlength+1; individualchars++)
		{
			textprintf_ex(screen,gamefont,80,((lines-20)*22)+120,makecol16(255,255,255),0,endinglines[lines].substr(0,individualchars).c_str());
			delay_with_duh_poll(20,endsongplayer);
		}
	}
	
	delay_with_duh_poll(1200,endsongplayer);
	
	clear(screen);
	
	for(int lines=35; lines<39; lines++)
	{
		textlength=endinglines[lines].length();
		
		for(int individualchars=0; individualchars<textlength+1; individualchars++)
		{
			textprintf_ex(screen,gamefont,230,((lines-24)*22)+10,makecol16(255,255,255),0,endinglines[lines].substr(0,individualchars).c_str());
			delay_with_duh_poll(20,endsongplayer);
		}
	}
	
	delay_with_duh_poll(1200,endsongplayer);
	
	clear(screen);	
	
	for(int lines=39; lines<44; lines++)
	{
		textlength=endinglines[lines].length();
		
		for(int individualchars=0; individualchars<textlength+1; individualchars++)
		{
			textprintf_ex(screen,gamefont,400,((lines-26)*22)+120,makecol16(255,255,255),0,endinglines[lines].substr(0,individualchars).c_str());
			delay_with_duh_poll(20,endsongplayer);
		}
	}
	
	delay_with_duh_poll(1200,endsongplayer);
	
	clear(screen);
	
	for(int lines=44; lines<46; lines++)
	{
		textlength=endinglines[lines].length();
		
		for(int individualchars=0; individualchars<textlength+1; individualchars++)
		{
			textprintf_ex(screen,gamefont,400,((lines-27)*22)+120,makecol16(255,255,255),0,endinglines[lines].substr(0,individualchars).c_str());
			delay_with_duh_poll(20,endsongplayer);
		}
	}
	
	delay_with_duh_poll(1200,endsongplayer);
	
	clear(screen);	
	
	for(int lines=46; lines<48; lines++)
	{
		textlength=endinglines[lines].length();
		
		for(int individualchars=0; individualchars<textlength+1; individualchars++)
		{
			textprintf_ex(screen,gamefont,350,((lines-37)*22)+120,makecol16(255,255,255),0,endinglines[lines].substr(0,individualchars).c_str());
			delay_with_duh_poll(20,endsongplayer);
		}
	}
	
	delay_with_duh_poll(1200,endsongplayer);
	
	clear(screen);
	
	for(int lines=48; lines<50; lines++)
	{
		textlength=endinglines[lines].length();
		
		for(int individualchars=0; individualchars<textlength+1; individualchars++)
		{
			textprintf_ex(screen,gamefont,40,((lines-38)*22),makecol16(255,255,255),0,endinglines[lines].substr(0,individualchars).c_str());
			delay_with_duh_poll(20,endsongplayer);
		}
	}
	
	delay_with_duh_poll(1200,endsongplayer);
	
	clear(screen);
	
	for(int lines=50; lines<53; lines++)
	{
		textlength=endinglines[lines].length();
		
		for(int individualchars=0; individualchars<textlength+1; individualchars++)
		{
			textprintf_ex(screen,gamefont,70,((lines-41)*22),makecol16(255,255,255),0,endinglines[lines].substr(0,individualchars).c_str());
			delay_with_duh_poll(20,endsongplayer);
		}
	}
	
	delay_with_duh_poll(2000,endsongplayer);
	
	clear(screen);
	
	textlength=endinglines[53].length();
		
	for(int individualchars=0; individualchars<textlength+1; individualchars++)
	{
		textprintf_ex(screen,gamefont,155,200,makecol16(255,255,255),0,endinglines[53].substr(0,individualchars).c_str());
		delay_with_duh_poll(20,endsongplayer);
	}
	
	delay_with_duh_poll(2000,endsongplayer);

	clear(screen);
	
	textlength=endinglines[54].length();
		
	for(int individualchars=0; individualchars<textlength+1; individualchars++)
	{
		textprintf_ex(screen,gamefont,210,330,makecol16(255,255,255),0,endinglines[54].substr(0,individualchars).c_str());
		delay_with_duh_poll(20,endsongplayer);
	}
	
	delay_with_duh_poll(2000,endsongplayer);
	
	clear(screen);
	
	for(int lines=55; lines<58; lines++)
	{
		textlength=endinglines[lines].length();
		
		for(int individualchars=0; individualchars<textlength+1; individualchars++)
		{
			textprintf_ex(screen,gamefont,195,((lines-47)*22),makecol16(255,255,255),0,endinglines[lines].substr(0,individualchars).c_str());
			delay_with_duh_poll(20,endsongplayer);
		}
	}
	
	delay_with_duh_poll(2000,endsongplayer);
	
	for(float volumereduce=((float) music_volume)/10; volumereduce>-0.05f; volumereduce=volumereduce-0.05f)
	{
		al_duh_set_volume(endsongplayer,volumereduce);
		delay_with_duh_poll(100,endsongplayer);
	}
	
	al_pause_duh(endsongplayer);
	
	alt_fade_out(screen,1);
	
	check_if_criteria_fulfilled();
		
	remove_int(add_counter);
	game_active=false;
};

void delay_with_duh_poll(int delaylength, AL_DUH_PLAYER *duhtopoll)
{
	install_int_ex(add_counter,MSEC_TO_TIMER(4));
	
	counter=0;
	
	do {
		al_poll_duh(duhtopoll);
		
		if(input_pressed()&&keyreleased)
		{
			install_int_ex(add_counter,MSEC_TO_TIMER(1));
		}

		if(!input_pressed())
		{
			keyreleased=true;
		}
	}
	while(counter<delaylength);
		
	remove_int(add_counter);	
}

void add_counter()
{
	counter++;	
}

END_OF_FUNCTION(add_counter);

void counter_reset()
{
	counter=0;
}
