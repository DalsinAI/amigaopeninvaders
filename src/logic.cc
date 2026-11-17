/***************************************************************************
 *            logic.cc
 *
 *  Fri Jun  1 00:11:21 2007
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

#include "config.h"
#include "headers/declare.h"
#include "headers/graphics.h"
#include "headers/collision.h"
#include "headers/input.h"
#include "headers/logic.h"
#include "headers/ending.h"
#include "headers/init.h"
#include "headers/sound.h"
#include "headers/unlock.h"

using namespace std;

startype stars[500], clouds[4], meteors[10], gummis[30];
int todisplayx, todisplayy;
item wave[9][5];
item enemybullets[25];
int score, lives, posx, level, wavedirection, delay, furthest_left, furthest_right;
int wx, wy, enemy_count, enemy_projectile_count, projlimit;
int ufo_delay, next_extra_life, difficulty;
extern int music_volume;
extern int sfx_volume;
extern int counter;
int bkoffsetx, bkoffsety;
int game_condition;
int hiscore[10];
char hiscore_names[10][4];
int ufos_shot_in_a_row, missed_in_a_row;


extern item bulletposition, ufoposition, shipposition;
extern PALETTE gamepalette;
extern FONT *gamefont;
extern SAMPLE *newhighscore, *gameisover, *explode;
extern void interrupt_time_control();
extern bool paused;

bool game_active, program_active, check, in_process_of_fading;



void reset_enemies_state()
{
	for(int wavex=0; wavex<8; wavex++)
	{
		for(int wavey=0; wavey<4; wavey++)
		{
			wave[wavex][wavey].alive=alive;
			wave[wavex][wavey].explosionstage=0;
			wave[wavex][wavey].deathtime=1;
		};
	};
};



void move_automatic_items()
{
	int enemy_speed=0, enemy_jump=1;
	
	switch(difficulty)
	{
		case 0:
			enemy_speed=14-level;
			break;
		case 1:
			enemy_speed=12-level;
			if(level>=8)
			{
				enemy_jump=1;
				enemy_speed=(13-level)/2;
			}
			if(level==12)
			{
				enemy_jump=2;
				enemy_speed=2;
			}
			if(level==13)
			{
				enemy_jump=3;
				enemy_speed=2;
			}
			if(level==14)
			{
				enemy_jump=4;
				enemy_speed=2;
			}
			break;
		case 2:
			enemy_speed=10-level;
			enemy_jump=level;
		
			if(enemy_speed<1)
			{
				enemy_speed=1;
			}
			
			if(enemy_jump>4)
			{
				enemy_jump=4;
			}
			break;
		case 3:
			enemy_speed=6-level;
			enemy_jump=level;
		
			if(enemy_speed<1)
			{
				enemy_speed=1;
			}
			
			if(enemy_jump>4&&level<12)
			{
				enemy_jump=4;
			}
			
			if(level>=12)
			{
				enemy_jump=level-7;
			}
			break;
		case 4:
			enemy_speed=5-level;
			enemy_jump=level;
		
			if(enemy_speed<1)
			{
				enemy_speed=1;
			}
			
			if(enemy_jump>6)
			{
				enemy_jump=6;
			}
			
			if(level==14)
			{
				enemy_speed=0;
				enemy_jump=4;
			}
			break;
	}

	
	// Move projectile
		
	if(bulletposition.alive==alive)
	{
		bulletposition.ypos=bulletposition.ypos-6;
		
		// Users on chicken and easy modes get an extra shove with faster bullets
		
		if(difficulty<2)
		{
			bulletposition.ypos=bulletposition.ypos-((3-difficulty)*3);
		}
	};
	
	if(bulletposition.ypos<8&&bulletposition.alive==alive)
	{
		bulletposition.alive=dead;
		missed_in_a_row++;
	};
	
	// Move enemies
	
	if(delay>enemy_speed)
	{
		
		for(int wavex=0; wavex<8; wavex++)
		{
			for(int wavey=0; wavey<4; wavey++)
			{
				wave[wavex][wavey].xpos=wave[wavex][wavey].xpos+(enemy_jump*wavedirection*3);	
			};
		};
		
		for(int wavex=0; wavex<8; wavex++)
		{
			for(int wavey=0; wavey<4; wavey++)
			{
				if(wave[wavex][wavey].xpos>740&&wave[wavex][wavey].alive==alive&&wavedirection==+1)
				{
					for(int wavex=0; wavex<8; wavex++)
					{
						for(int wavey=0; wavey<4; wavey++)
						{
							wave[wavex][wavey].ypos=wave[wavex][wavey].ypos+20;
						};
					};
					wavedirection=-1;
				};
			
				if(wave[wavex][wavey].xpos<0&&wave[wavex][wavey].alive==alive&&wavedirection==-1)
				{
					for(int wavex=0; wavex<8; wavex++)
					{
						for(int wavey=0; wavey<4; wavey++)
						{
						wave[wavex][wavey].ypos=wave[wavex][wavey].ypos+20;
						};
					};
					wavedirection=+1;
				};
			};
		};
		
		delay=0;
	};
	
	delay++;
};

void process_enemy_projectiles()
{
	int randomenemy_x, randomenemy_y, enemy_count;
	
	// Less projectiles for easy modes
	
	projlimit=((difficulty+1)*5)-(14-level);
	
	if(projlimit<3)
	{
		projlimit=3+level;
	}
	
	enemy_count=0;
	
	for(int wavex=0; wavex<8; wavex++)
	{
		for(int wavey=0; wavey<4; wavey++)
		{
			if(wave[wavex][wavey].alive==alive)
			{
				enemy_count++;
			};
		};
	};
		
	for(int enemy_shots=0; enemy_shots<projlimit; enemy_shots++)
	{	
		if(enemybullets[enemy_shots].alive==alive)
		{
			enemybullets[enemy_shots].delay++;
			if(enemybullets[enemy_shots].delay==2)
			{
				enemybullets[enemy_shots].delay=0;
				enemybullets[enemy_shots].ypos=enemybullets[enemy_shots].ypos+3;
			};
		};
		
		if(enemybullets[enemy_shots].ypos>600)
		{
			enemybullets[enemy_shots].alive=dead;
		};
		
		if(enemybullets[enemy_shots].alive==dead)
		{
			randomenemy_x=rand()%8;
			randomenemy_y=rand()%4;
			
			if(wave[randomenemy_x][randomenemy_y].alive==alive&&rand()%1000<10)
			{
				enemybullets[enemy_shots].alive=alive;
				enemybullets[enemy_shots].xpos=wave[randomenemy_x][randomenemy_y].xpos;
				enemybullets[enemy_shots].ypos=wave[randomenemy_x][randomenemy_y].ypos+15;
			};
		};
	};
};

void process_ufo()
{
	ufo_delay++;
	
	if(ufo_delay==5-difficulty)
	{
		ufoposition.xpos=ufoposition.xpos+3;
		ufo_delay=0;
	};
	
	if(ufoposition.alive==dead&&rand()%50000<10)
	{
		ufoposition.alive=alive;
		ufoposition.xpos=0;
	};
	
	if(ufoposition.alive==alive&&ufoposition.xpos>770)
	{
		ufoposition.alive=dead;
	};
};

void death_sequence()
{
	extern AL_DUH_PLAYER *gamesongplayer;
	extern bool life_lost;
	
	remove_int(interrupt_time_control);	
	shipposition.alive=dead;
	bulletposition.alive=dead;
	ufoposition.alive=dead;
	game_display();
	lives--;
	life_lost=true;
	reset_enemies_position();
	shipposition.alive=alive;	
	posx=20;
	
	if(lives>0&&level<15)
	{
		alt_fade_out(screen,5);
	
		for(int fadein=0; fadein<256; fadein=fadein+5)
		{
			game_display(fadein);
			al_poll_duh(gamesongplayer);
		}
		install_int_ex(interrupt_time_control,BPS_TO_TIMER(60));
	};	
};



void check_for_next_level()
{
	bool lightning_white=false;
	extern AL_DUH_PLAYER *gamesongplayer;
	extern BITMAP *messagebuffer;
	extern int scrollposition;
	
	// Check if next level needed
	
	enemy_count=0;
		
	for(int wavex=0; wavex<8; wavex++)
	{
		for(int wavey=0; wavey<4; wavey++)
		{
			if(wave[wavex][wavey].alive!=dead)
			{
				enemy_count++;
			};
		};
	};
	
	if(enemy_count==0)
	{
		remove_int(interrupt_time_control);
		level++;
		scrollposition=SCREEN_W;
		
		if(level<15)
		{
			if(projlimit>15)
			{
				projlimit=15;
			};

			reset_enemies_position();
			reset_enemies_state();
			reset_shields();
			bulletposition.alive=dead;
		
			wavedirection=1;
		
			alt_fade_out(screen,5);
	
			posx=20;
		
			for(int fadein=0; fadein<256; fadein=fadein+5)
			{
				game_display(fadein);
				al_poll_duh(gamesongplayer);
			}

			install_int_ex(interrupt_time_control,BPS_TO_TIMER(60));
		}
		else
		{
			al_pause_duh(gamesongplayer);			
			counter_reset();
			install_int_ex(add_counter,MSEC_TO_TIMER(1));
				
			for(int lightning_flashes=0; lightning_flashes<SCREEN_W*1.5; lightning_flashes=lightning_flashes+5)
			{
				if(rand()%120<5)
				{
					lightning_white=true;
				}
				else
				{
					lightning_white=false;
				}
				
				if(lightning_white)
				{
					play_sample(explode,sfx_volume,128,3000,0);
				}
				
				do
				{
					if(lightning_white)
					{
						circlefill(screen,bulletposition.xpos,bulletposition.ypos,lightning_flashes,makecol16(255,255,255));						
					}
					else
					{
						circlefill(screen,bulletposition.xpos,bulletposition.ypos,lightning_flashes,makecol16(100,255,255));
					}
				
				}
				while(counter<45);
				
				counter=0;
			}
			
			alt_fade_out(screen,2);
			game_ending();
		}
		
		create_scrolling_message();
		display_scrolling_message(messagebuffer);
	}
}

void check_if_extra_life_due()
{
	if(score>=next_extra_life)
	{
		lives++;
		next_extra_life=next_extra_life+20000;
	};
};

void check_if_game_over()
{
	BITMAP *gameoverdisplay;
	extern AL_DUH_PLAYER *gamesongplayer;
	
	gameoverdisplay=create_bitmap(800,600);
	
	if(lives==0||level>14)
	{
		al_pause_duh(gamesongplayer);
		clear_to_color(gameoverdisplay,0);
		textprintf_centre_ex(gameoverdisplay,gamefont,399,270,0,0,"GAME OVER");
		play_sample(gameisover,sfx_volume,128,1000,0);
		alt_fade_between(screen,gameoverdisplay,1);
		rest(200);
		alt_fade_out(gameoverdisplay,7);
		game_active=false;
		lives=3;
	};
	
	destroy_bitmap(gameoverdisplay);	
};


void update_logic()
{
	extern AL_DUH_PLAYER *gamesongplayer;
	
	read_input();
	al_poll_duh(gamesongplayer);
	
	if(!paused)
	{
		move_automatic_items();
		update_scrolly_position();
		process_enemy_projectiles();
		process_ufo();
		check_if_criteria_fulfilled();
		update_animation();
		update_explosions();
		update_particles();
		collision_detection();
		check_if_hiscore_beaten(score, hiscore[9]);
		check_for_next_level();
		check_if_extra_life_due();
		check_if_game_over();
	}
};


void reset_enemies_position()
{
	for(int wavex=0; wavex<8; wavex++)
	{
		for(int wavey=0; wavey<4; wavey++)
		{
			wave[wavex][wavey].xpos=20+(wavex*65);
			wave[wavex][wavey].ypos=90+(wavey*50);
		};
	};
	
	for(int enemy_shots=0; enemy_shots<20; enemy_shots++)
	{
		enemybullets[enemy_shots].alive=dead;
	};
};

void update_particles()
{
	extern particle explosionbits[500];
	
	for(int dispparts=0; dispparts<500; dispparts++)
	{
		if(!paused)
		{			
			explosionbits[dispparts].life--;
		
			if(explosionbits[dispparts].life<=0)
			{
				explosionbits[dispparts].active=false;
			}
			
			explosionbits[dispparts].x=explosionbits[dispparts].x+explosionbits[dispparts].xthrust;
			explosionbits[dispparts].y=explosionbits[dispparts].y+explosionbits[dispparts].ythrust;
		
			explosionbits[dispparts].ythrust++;
			
			if(explosionbits[dispparts].xthrust<-2)
			{
				explosionbits[dispparts].xthrust=explosionbits[dispparts].xthrust/2;
			}
		
			if(explosionbits[dispparts].xthrust>2)
			{
				explosionbits[dispparts].xthrust=explosionbits[dispparts].xthrust/2;
			}
			
			if(explosionbits[dispparts].y>SCREEN_H)
			{
				explosionbits[dispparts].active=false;
			}
		}
	}
}


void update_explosions()
{	
	for(int wavex=0; wavex<8; wavex++)
	{
		for(int wavey=0; wavey<4; wavey++)
		{
			if(!paused&&wave[wavex][wavey].alive==dying)
			{
				wave[wavex][wavey].deathtime--;
				wave[wavex][wavey].alpha=wave[wavex][wavey].alpha-11;
	
				if(wave[wavex][wavey].deathtime<0)
				{
					wave[wavex][wavey].explosionstage++;
					wave[wavex][wavey].deathtime=1;
				}		
			}
			
			if(wave[wavex][wavey].explosionstage>20)
			{
				wave[wavex][wavey].alive=dead;
			}
		}
	}
	
	if(!paused)
	{
		ufoposition.deathtime--;
		ufoposition.alpha=ufoposition.alpha-21;
				
		if(ufoposition.deathtime<0)
		{
			ufoposition.explosionstage++;
			ufoposition.deathtime=1;
		}
				
		if(ufoposition.explosionstage>20)
		{
			ufoposition.alive=dead;
		}
	}

}

void update_animation()
{
	extern int animdelay, current_alien_frame;
	
	if(!paused)
	{
		animdelay++;
	}
}
