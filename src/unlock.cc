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

#include "headers/declare.h"
#include "headers/unlock.h"
#include "headers/ending.h"
#include "headers/init.h"

using namespace std;

int unlockables_unlocked[35];

void check_if_criteria_fulfilled()
{
	extern int level, difficulty, score, lives, ufos_shot_in_a_row;
	extern int missed_in_a_row, start_level;
	extern bool life_lost;
	extern item wave[9][5];
	
	if(level==2)
	{
		unlock_secret(0);
	}
	
	if(level==3&&difficulty>1&&start_level==1)
	{
		unlock_secret(1);
	}

	if(level==4&&difficulty>1&&start_level==1)
	{
		unlock_secret(2);
	}

	if(level==5&&difficulty>1&&start_level==1)
	{
		unlock_secret(3);
	}

	if(level==6&&difficulty>1&&start_level==1)
	{
		unlock_secret(4);
	}

	if(level==7&&difficulty>1&&start_level==1)
	{
		unlock_secret(5);
	}

	if(level==8&&difficulty>1&&start_level==1)
	{
		unlock_secret(6);
	}

	if(level==9&&difficulty>1&&start_level==1)
	{
		unlock_secret(7);
	}

	if(level==10&&difficulty>1&&start_level==1)
	{
		unlock_secret(8);
	}

	if(level==11&&difficulty>1&&start_level==1)
	{
		unlock_secret(9);
	}

	if(level==12&&difficulty>1&&start_level==1)
	{
		unlock_secret(10);
	}

	if(level==13&&difficulty>1&&start_level==1)
	{
		unlock_secret(11);
	}

	if(level==14&&difficulty>1&&start_level==1)
	{
		unlock_secret(12);
	}
	
	if(level==15&&difficulty>1&&start_level==1)
	{
		unlock_secret(13);
	}
	
	if(level==15&&difficulty==0&&start_level==1)
	{
		unlock_secret(14);
	}

	if(level==15&&difficulty==1&&start_level==1)
	{
		unlock_secret(15);
	}

	if(level==15&&difficulty==2&&start_level==1)
	{
		unlock_secret(16);
	}

	if(level==15&&difficulty==3&&start_level==1)
	{
		unlock_secret(17);
	}

	if(level==15&&difficulty==4&&start_level==1)
	{
		unlock_secret(18);
	}
	
	if(score>=10000&&difficulty>0)
	{
		unlock_secret(19);
	}
	
	if(score>=30000&&difficulty>0)
	{
		unlock_secret(20);
	}
	
	if(score>=60000&&difficulty>0)
	{
		unlock_secret(21);
	}
	
	if(score>=100000)
	{
		unlock_secret(22);
	}
	
	if(lives==5&&difficulty>1)
	{
		unlock_secret(23);
	}
	
	if(level==15&&!life_lost&&difficulty==1&&start_level==1)
	{
		unlock_secret(24);
	}
	
	if(level==15&&!life_lost&&difficulty==2&&start_level==1)
	{
		unlock_secret(25);
	}
	
	if(level==15&&!life_lost&&difficulty==3&&start_level==1)
	{
		unlock_secret(26);
	}
	
	if(level==15&&!life_lost&&difficulty==4&&start_level==1)
	{
		unlock_secret(27);
	}
	
	if(ufos_shot_in_a_row==3)
	{
		unlock_secret(28);
	}
	
	if(missed_in_a_row==5&&difficulty>1&&level>7)
	{
		unlock_secret(29);
	}
	
	// Check for square formation
	
	bool secret30_valid=true;
	
	for(int checkrows=0; checkrows<8; checkrows++)
	{
		if(wave[checkrows][0].alive==dead||wave[checkrows][3].alive==dead)
		{
			secret30_valid=false;
		}
	}
	
	for(int checkcolumns=0; checkcolumns<4; checkcolumns++)
	{
		if(wave[0][checkcolumns].alive==dead||wave[7][checkcolumns].alive==dead)
		{
			secret30_valid=false;
		}
	}
	
	for(int checkrows=1; checkrows<7; checkrows++)
	{
		for(int checkcolumns=1; checkcolumns<3; checkcolumns++)
		{
			if(wave[checkrows][checkcolumns].alive==alive)
			{
				secret30_valid=false;
			}
		}
	}
	
	// Check for plus formation
	
	bool secret31_valid=true;
	
	for(int horiz=0; horiz<8; horiz++)
	{
		if(wave[horiz][1].alive==dead)
		{
			secret31_valid=false;
		}
	}
	
	for(int vert=0; vert<4; vert++)
	{
		if(wave[3][vert].alive==dead||wave[4][vert].alive==dead)
		{
			secret31_valid=false;
		}
	}
	
	for(int checkrows=0; checkrows<8; checkrows++)
	{
		for(int checkcolumns=0; checkcolumns<4; checkcolumns++)
		{
			if(wave[checkrows][checkcolumns].alive==alive&&!(checkcolumns==1||checkrows==3||checkrows==4))
			{
				secret31_valid=false;
			}
		}
	}
	
	// Check for cross formation
	
	bool secret32_valid=true;
	
	for(int wavey=0; wavey<8; wavey++)
	{
		if(wave[wavey][wavey/2].alive==dead||wave[7-wavey][wavey/2].alive==dead)
		{
			secret32_valid=false;
		}
	};
	
	for(int wavex=2; wavex<6; wavex++)
	{
		if(wave[wavex][0].alive==alive||wave[wavex][3].alive==alive)
		{
			secret32_valid=false;
		}
	}
	
	for(int side=0; side<7; side=side+6)
	{
		for(int checkrows=0; checkrows<2; checkrows++)
		{
			for(int checkcolumns=1; checkcolumns<3; checkcolumns++)
			{
				if(wave[checkrows+side][checkcolumns].alive==alive)
				{
					secret32_valid=false;
				}
			}
		}
	}
	
	// Check for chequered formation
	
	bool secret33_valid=true;
	
	int testtable[]={alive, dead,  alive, dead,  alive, dead, alive, dead,
						 dead, alive, dead, alive, dead ,alive,dead, alive, 
						 alive, dead,  alive, dead,  alive, dead, alive, dead, 
						 dead, alive, dead, alive, dead ,alive,dead, alive};

	int tableposition=0;
	
	for(int wavey=0; wavey<4; wavey++)
	{
		for(int wavex=0; wavex<8; wavex++)
		{
			if(wave[wavex][wavey].alive!=testtable[tableposition])
			{
				secret33_valid=false;
			}
			tableposition++;
		}
	};
	
	if(!secret33_valid)
	{
		secret33_valid=true;
		
		int testtable[]={dead, alive, dead, alive, dead ,alive,dead, alive, 
					 alive, dead,  alive, dead,  alive, dead, alive, dead, 
					 dead, alive, dead, alive, dead ,alive,dead, alive,
		 			alive, dead,  alive, dead,  alive, dead, alive, dead};
	
		tableposition=0;
				 
		for(int wavey=0; wavey<4; wavey++)
		{
			for(int wavex=0; wavex<8; wavex++)
			{
				if(wave[wavex][wavey].alive!=testtable[tableposition])
				{
					secret33_valid=false;
				}
				tableposition++;
			}
		};
	}
	
	
	// Check if only bottom row intact
	
	bool secret34_valid=true;
	
	for(int checkrows=0; checkrows<8; checkrows++)
	{
		for(int checkcolumns=0; checkcolumns<3; checkcolumns++)
		{
			if(wave[checkrows][checkcolumns].alive==alive)
			{
				secret34_valid=false;
			}
		}
	}
	
	for(int checkrows=0; checkrows<8; checkrows++)
	{
		if(wave[checkrows][3].alive!=alive)
		{
			secret34_valid=false;
		}
	}
	
	if(secret30_valid)
	{
		unlock_secret(30);
	}
	
	if(secret31_valid)
	{
		unlock_secret(31);
	}
	
	if(secret32_valid)
	{
		unlock_secret(32);
	}
	
	if(secret33_valid)
	{
		unlock_secret(33);
	}
	
	if(secret34_valid)
	{
		unlock_secret(34);
	}
}

void unlock_secret(int id)
{
	extern FONT *gamefont;
	extern void interrupt_time_control();
	extern AL_DUH_PLAYER *gamesongplayer;
		
	string unlockabletexts[]= {
		"THE UNLOCKABLE MESSAGE",
		"LEVEL 2",
		"LEVEL 3",
		"LEVEL 4",
		"LEVEL 5",
		"LEVEL 6",
		"LEVEL 7",
		"LEVEL 8",
		"LEVEL 9",
		"LEVEL 10",
		"LEVEL 11",
		"LEVEL 12",
		"LEVEL 13",
		"LEVEL 14",
		"SPIKE THE CHICKEN",
		"INVADERS PONG",
		"INVADERS BREAKOUT",
		"ENDLESS MODE",
		"THE ENDING CREDITS",
		"RETRO GRAPHICS",
		"OPEN INVADERS 0.01 GRAPHICS",
		"OPEN INVADERS 0.05 GRAPHICS",
		"SILLY GRAPHICS",
		"JAMYSKIS DEMO",
		"NAUGHTS & CROSSES SCREENS PT.1",
		"NAUGHTS & CROSSES SCREENS PT.2",
		"MAKING OF OPEN INVADERS PT.1",
		"MAKING OF OPEN INVADERS PT.2",
		"COW ABDUCTION",
		"WILD WEST INVADERS",
		"ALIEN ROMANCE",
		"MATHS WITH THE INVADERS",
		"CAN THE CANS DO THE CAN-CAN?",
		"THE INVADER DISCO DANCE",
		"OLD MEETS NEW",
	};
	
	if(!unlockables_unlocked[id])
	{
		remove_int(interrupt_time_control);
		unlockables_unlocked[id]=true;
		rectfill(screen,(SCREEN_W/2)-300,(SCREEN_H/2)-200,(SCREEN_W/2)+300,(SCREEN_H/2)-100,makecol16(0,0,155));
		rect(screen,(SCREEN_W/2)-300,(SCREEN_H/2)-200,(SCREEN_W/2)+300,(SCREEN_H/2)-100,makecol16(255,255,255));
		
		textout_centre_ex(screen,gamefont,"CONGRATULATIONS!",SCREEN_W/2,(SCREEN_H/2)-190,-1,-1);
		textout_centre_ex(screen,gamefont,"YOU HAVE UNLOCKED:",SCREEN_W/2,(SCREEN_H/2)-160,-1,-1);
		textout_centre_ex(screen,gamefont,unlockabletexts[id].c_str(),SCREEN_W/2,(SCREEN_H/2)-130,-1,-1);
		delay_with_duh_poll(1000,gamesongplayer);
		install_int_ex(interrupt_time_control,BPS_TO_TIMER(60));
		save_hiscore();
	}
}
