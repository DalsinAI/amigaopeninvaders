/***************************************************************************
 *            collision.cc
 *
 *  Fri Jun  1 00:18:24 2007
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
#include "headers/pmask.h"
#include "headers/declare.h"
#include "headers/logic.h"
#endif

#ifndef ALLEGRO_LINUX
#include "../include/pmask.h"
#include "../include/declare.h"
#endif

extern item bulletposition, ufoposition, shieldunits[4], shipposition;

extern BITMAP *ship, *alien[4][4], *projectile, *ufo;
extern item wave[9][5];
extern item enemybullets[25];

extern int score;
extern SAMPLE *explode;
extern int sfx_volume;

extern int ufos_shot_in_a_row, missed_in_a_row;

extern int posx;
extern BITMAP *shield_copies[3];

extern particle explosionbits[500];
extern int current_particle;

void create_bitmasks()
{
	shipposition.mask=create_allegro_pmask(ship);
	wave[1][1].mask=create_allegro_pmask(alien[0][0]);
	enemybullets[1].mask=create_allegro_pmask(projectile);
	ufoposition.mask=create_allegro_pmask(ufo);
	bulletposition.mask=create_allegro_pmask(projectile);
};


void collision_detection()
{	
	//Aliens and projectiles
	
	for(int wave_check_x=0; wave_check_x<8; wave_check_x++)
	{
		for(int wave_check_y=0; wave_check_y<4; wave_check_y++)
		{
			if(bulletposition.alive==alive&&wave[wave_check_x][wave_check_y].alive==alive&&check_pmask_collision(wave[1][1].mask,bulletposition.mask,wave[wave_check_x][wave_check_y].xpos,wave[wave_check_x][wave_check_y].ypos,bulletposition.xpos,bulletposition.ypos))
			{
				wave[wave_check_x][wave_check_y].alive=dying;
				wave[wave_check_x][wave_check_y].deathtime=2;
				wave[wave_check_x][wave_check_y].alpha=500;
				wave[wave_check_x][wave_check_y].explosionstage=0;
				bulletposition.alive=dead;
				score=score+100;
				ufos_shot_in_a_row=0;
				missed_in_a_row=0;
				play_sample(explode,sfx_volume,128,1000,0);
				
				for(int newparticles=0; newparticles<10; newparticles++)
				{
					explosionbits[current_particle].active=true;
					explosionbits[current_particle].x=wave[wave_check_x][wave_check_y].xpos+32;
					explosionbits[current_particle].y=wave[wave_check_x][wave_check_y].ypos+32;
					explosionbits[current_particle].xthrust=40-rand()%80;
					explosionbits[current_particle].ythrust=-rand()%20;
					switch(wave_check_y)
					{
						case 0: explosionbits[current_particle].color=makecol16(50,0,220); break;
						case 1: explosionbits[current_particle].color=makecol16(0,220,50); break;
						case 2: explosionbits[current_particle].color=makecol16(0,220,50); break;
						case 3: explosionbits[current_particle].color=makecol16(210,210,215); break;
					}
					explosionbits[current_particle].life=600;
					current_particle++;
					
					if(current_particle>499)
					{
						current_particle=0;
					}
				}
			};
			
			if(wave[wave_check_x][wave_check_y].alive==dying)
			{
				wave[wave_check_x][wave_check_y].delay--;
			};
			
			if(wave[wave_check_x][wave_check_y].delay==0&&wave[wave_check_x][wave_check_y].alive==dying)
			{
				wave[wave_check_x][wave_check_y].alive=dead;
			};
		};
	};
	
	//Ship and projectiles
	
	for(int bullet_check=0; bullet_check<21; bullet_check++)
	{
		if(enemybullets[bullet_check].alive==alive&&check_pmask_collision(shipposition.mask,enemybullets[1].mask,posx,530,enemybullets[bullet_check].xpos,enemybullets[bullet_check].ypos))
		{
			death_sequence();
			posx=20;
			reset_enemies_position();
		};
	};
	
	//UFO and projectiles
	
	if(bulletposition.alive==alive&&ufoposition.alive==alive&&check_pmask_collision(bulletposition.mask,ufoposition.mask,bulletposition.xpos,bulletposition.ypos,ufoposition.xpos,ufoposition.ypos))
	{
		ufoposition.alive=dying;
		ufoposition.deathtime=2;
		ufoposition.alpha=500;
		ufoposition.explosionstage=0;
		bulletposition.alive=dead;
		score=score+500;
		ufos_shot_in_a_row++;
		missed_in_a_row=0;
		play_sample(explode,sfx_volume,128,500,0);
		
		for(int newparticles=0; newparticles<30; newparticles++)
		{
			explosionbits[current_particle].active=true;
			explosionbits[current_particle].x=ufoposition.xpos+32;
			explosionbits[current_particle].y=ufoposition.ypos+32;
			explosionbits[current_particle].xthrust=40-rand()%80;
			explosionbits[current_particle].ythrust=-rand()%20;
			explosionbits[current_particle].color=makecol16(200,200,200);
			explosionbits[current_particle].life=600;
			current_particle++;
					
			if(current_particle>499)
			{
				current_particle=0;
			}
		}
	};
		
	if(ufoposition.alive==dying)
	{
		ufoposition.delay--;
	};
	
	if(ufoposition.delay==0&&ufoposition.alive==dying)
	{
		ufoposition.alive=dead;
	};
	
	//Aliens and shields
	
	for(int wave_check_x=0; wave_check_x<8; wave_check_x++)
	{
		for(int wave_check_y=0; wave_check_y<5; wave_check_y++)
		{
			if(wave[wave_check_x][wave_check_y].alive==alive&&wave[wave_check_x][wave_check_y].ypos>375)
			{
				for(int clear_shields=0; clear_shields<3; clear_shields++)
				{
					clear_to_color(shield_copies[clear_shields],makecol16(255,0,255));
				};
			};
		};
	};
	
	//Aliens and ship
	
	for(int wave_check_x=0; wave_check_x<8; wave_check_x++)
	{
		for(int wave_check_y=0; wave_check_y<5; wave_check_y++)
		{
			if(wave[wave_check_x][wave_check_y].alive==alive&&check_pmask_collision(wave[1][1].mask,shipposition.mask,wave[wave_check_x][wave_check_y].xpos,wave[wave_check_x][wave_check_y].ypos,posx,530))
			{
				death_sequence();
				posx=20;
				reset_enemies_position();
			};
		};
	};
	
	
	//Aliens and bottom of screen
	
	for(int wave_check_x=0; wave_check_x<8; wave_check_x++)
	{
		for(int wave_check_y=0; wave_check_y<5; wave_check_y++)
		{
			if(wave[wave_check_x][wave_check_y].alive==alive&&wave[wave_check_x][wave_check_y].ypos>SCREEN_H-80)
			{
				death_sequence();
				posx=20;
				reset_enemies_position();
			};
		};
	};
	
	//Projectiles and shields
	
	for(int shieldmasks=0; shieldmasks<3; shieldmasks++)
	{		
		shieldunits[shieldmasks].mask=create_allegro_pmask(shield_copies[shieldmasks]);
		
		if(bulletposition.alive==alive&&check_pmask_collision(bulletposition.mask,shieldunits[shieldmasks].mask,bulletposition.xpos,bulletposition.ypos,(shieldmasks*270)+60,420))
		{
			bulletposition.alive=dead;
			rectfill(shield_copies[shieldmasks],bulletposition.xpos-((shieldmasks*270)+60)+24,bulletposition.ypos-420,bulletposition.xpos-((shieldmasks*270)+60)+40,bulletposition.ypos-420+64,makecol16(255,0,255));
			
			for(int newparticles=0; newparticles<3; newparticles++)
			{
				explosionbits[current_particle].active=true;
				explosionbits[current_particle].x=bulletposition.xpos+32;
				explosionbits[current_particle].y=bulletposition.ypos;
				explosionbits[current_particle].xthrust=40-rand()%80;
				explosionbits[current_particle].ythrust=-rand()%20;
				explosionbits[current_particle].color=makecol16(0,230,230);
				explosionbits[current_particle].life=10;
				current_particle++;
						
				if(current_particle>499)
				{
					current_particle=0;
				}
			}
			
			missed_in_a_row=0;
		}
		
		for(int enemy_shots=0; enemy_shots<21; enemy_shots++)
		{
			if(enemybullets[enemy_shots].alive==alive&&check_pmask_collision(enemybullets[1].mask,shieldunits[shieldmasks].mask,enemybullets[enemy_shots].xpos,enemybullets[enemy_shots].ypos,(shieldmasks*270)+60,420))
			{
				enemybullets[enemy_shots].alive=dead;
				rectfill(shield_copies[shieldmasks],enemybullets[enemy_shots].xpos-((shieldmasks*270)+60)+24,enemybullets[enemy_shots].ypos-420,enemybullets[enemy_shots].xpos-((shieldmasks*270)+60)+40,enemybullets[enemy_shots].ypos-420+64,makecol16(255,0,255));
				
				for(int newparticles=0; newparticles<3; newparticles++)
				{
					explosionbits[current_particle].active=true;
					explosionbits[current_particle].x=enemybullets[enemy_shots].xpos+32;
					explosionbits[current_particle].y=enemybullets[enemy_shots].ypos+32;
					explosionbits[current_particle].xthrust=40-rand()%80;
					explosionbits[current_particle].ythrust=-rand()%20;
					explosionbits[current_particle].color=makecol16(0,230,230);
					explosionbits[current_particle].life=10;
					current_particle++;
				}
					
				if(current_particle>499)
				{
					current_particle=0;
				}
			};				
		};
		
		destroy_pmask(shieldunits[shieldmasks].mask);		
	};
	
	//Projectiles and projectiles
	
	for(int enemy_shots=0; enemy_shots<21; enemy_shots++)
	{
		if(enemybullets[enemy_shots].alive==alive&&check_pmask_collision(enemybullets[1].mask,bulletposition.mask,bulletposition.xpos,bulletposition.ypos,enemybullets[enemy_shots].xpos,enemybullets[enemy_shots].ypos))
		{
			bulletposition.alive=dead;
			enemybullets[enemy_shots].alive=dead;
			
			for(int newparticles=0; newparticles<3; newparticles++)
			{
				explosionbits[current_particle].active=true;
				explosionbits[current_particle].x=enemybullets[enemy_shots].xpos+32;
				explosionbits[current_particle].y=enemybullets[enemy_shots].ypos+64;
				explosionbits[current_particle].xthrust=40-rand()%80;
				explosionbits[current_particle].ythrust=-rand()%20;
				explosionbits[current_particle].color=makecol16(0,230,230);
				explosionbits[current_particle].life=10;
				current_particle++;
			}

			if(current_particle>499)
			{
				current_particle=0;
			}
		}
	}
	
};
