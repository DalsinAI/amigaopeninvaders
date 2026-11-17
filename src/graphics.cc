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

#include "headers/declare.h"
#include "headers/graphics.h"


using namespace std;

FONT *gamefont;

BITMAP *display;
BITMAP *ship;
BITMAP *projectile;
BITMAP *alien[4][4];
BITMAP *death[11];
BITMAP *ufo;
BITMAP *shield_original;
BITMAP *shield_copies[3];
BITMAP *logo;
BITMAP *level2bk[2];
BITMAP *level3bk;
BITMAP *level4bk[2];
BITMAP *level5bk;
BITMAP *level6bk;
BITMAP *level7bk;
BITMAP *level9bk[3];
BITMAP *level11bk;
BITMAP *level12bk;
BITMAP *level13bk;
BITMAP *level14bk;
BITMAP *messagebuffer;

RLE_SPRITE *rock_background[2];
RLE_SPRITE *grid_background;
RLE_SPRITE *cloud_background[2];
RLE_SPRITE *planet_background;
RLE_SPRITE *meteor_background;
RLE_SPRITE *moon_background;
RLE_SPRITE *gummibear_background[3];
RLE_SPRITE *lightning_background;
RLE_SPRITE *city_background;
RLE_SPRITE *fortress_background;

RLE_SPRITE *rle_ship, *rle_projectile, *rle_alien[4][4], *rle_death[11], *rle_ufo;

particle explosionbits[500];
int current_particle=0;

extern int level;
extern int todisplayx, todisplayy;
extern startype stars[500], clouds[4], meteors[10], gummis[30];
extern item wave[9][5];
extern item shipposition;
extern int posx;
extern item bulletposition, ufoposition;
extern int projlimit;
extern item enemybullets[25];
extern int score, lives;
extern int bkoffsetx, bkoffsety;
extern bool game_active, paused;

extern SAMPLE *shoot, *explode;

int scrollposition;
int current_alien_frame=0, animdelay=0;

extern void load_data_files();

void display_background()
{	
	if(level==1||level==6)
	{
		for(int displaystars=0; displaystars<400; displaystars++)
	 	{
			todisplayx=stars[displaystars].x;
   			todisplayy=stars[displaystars].y;
			
   			rectfill(display,todisplayx,todisplayy,todisplayx+stars[displaystars].size,todisplayy+stars[displaystars].size,stars[displaystars].color);

   			if(!paused)
			{
				stars[displaystars].y=stars[displaystars].y+stars[displaystars].speed;
			}
			
   			if(stars[displaystars].y>600)
   			{
   				stars[displaystars].x=rand()%SCREEN_W;
   				stars[displaystars].y=0;
   				stars[displaystars].speed=(rand()%10)+1;
				stars[displaystars].brightness=rand()%100;
				stars[displaystars].color=makecol16((125+rand()%120-stars[displaystars].brightness),255-stars[displaystars].brightness,(125+rand()%120)-stars[displaystars].brightness);
				stars[displaystars].size=(rand()%2);
   			};
   		};
	}
	
	if(level==2)
	{
		for(int displayrocks=0; displayrocks<(SCREEN_H/250)+2; displayrocks++)
		{
			draw_rle_sprite(display,rock_background[0],0,(displayrocks*250)-bkoffsety);
			draw_rle_sprite(display,rock_background[1],SCREEN_W-250,(displayrocks*250)-bkoffsety);
		};
		
		if(!paused)
		{	
			bkoffsety--;
		}
		
		if(bkoffsety<0)
		{
			bkoffsety=250;
		}		
	};
	
	if(level==3)
	{
		for(int displaygridx=0; displaygridx<6; displaygridx++)
		{
			for(int displaygridy=0; displaygridy<5; displaygridy++)
			{
				draw_rle_sprite(display,grid_background,displaygridx*250,(displaygridy*250)-bkoffsety);
			}
		}
		
		if(!paused)
		{
			bkoffsety--;
		}		
		
		if(bkoffsety<0)
		{
			bkoffsety=250;
		}
	}
	
	if(level==4||level==8)
	{
		if(level==4)
		{
			clear_to_color(display,makecol16(0,190,160));
		}
		
		if(level==8)
		{
			clear_to_color(display,makecol16(0,0,40));
		}
		
		for(int displayclouds=0; displayclouds<3; displayclouds++)
	 	{
			todisplayx=clouds[displayclouds].x;
   			todisplayy=clouds[displayclouds].y;
   			draw_rle_sprite(display,cloud_background[clouds[displayclouds].color],clouds[displayclouds].x,clouds[displayclouds].y);
			
			if(!paused)
			{
				clouds[displayclouds].x=clouds[displayclouds].x+clouds[displayclouds].speed;
			}
			
			if(clouds[displayclouds].x>SCREEN_W)
   			{
   				clouds[displayclouds].x=-250;
   				clouds[displayclouds].y=rand()%400;
   				clouds[displayclouds].speed=(rand()%3)+1;
   			};
   		};
	}
	
	if(level==5)
	{
		for(int displaystars=0; displaystars<100; displaystars++)
		{
			putpixel(display,stars[displaystars].x,stars[displaystars].y,makecol16(180,180,180));
		}
		
		draw_rle_sprite(display,planet_background,400,100);
	}
	
	if(level==6)
	{
		for(int displaymeteors=0; displaymeteors<10; displaymeteors++)
	 	{
			todisplayx=meteors[displaymeteors].x;
   			todisplayy=meteors[displaymeteors].y;
   			draw_rle_sprite(display,meteor_background,meteors[displaymeteors].x,meteors[displaymeteors].y);
			
			if(!paused)
			{
				meteors[displaymeteors].y=meteors[displaymeteors].y+meteors[displaymeteors].speed;
			}
				
			if(meteors[displaymeteors].y>SCREEN_H)
   			{
   				meteors[displaymeteors].x=rand()%SCREEN_W;
   				meteors[displaymeteors].y=-300;
   				meteors[displaymeteors].speed=(rand()%7)+3;
   			};
   		};
	}
	
	if(level==7||level==12||level==14)
	{
		for(int displaystars=0; displaystars<100; displaystars++)
		{
			int shade=rand()%100+155;
			putpixel(display,stars[displaystars].x,stars[displaystars].y,makecol16(shade,shade,shade));
		}
		
		switch(level)
		{
			case 7: draw_rle_sprite(display,moon_background,400,100); break;
			case 12: draw_rle_sprite(display,city_background,0,300); break;
		}
	}
	
	if(level==9)
	{
		for(int displaygummis=0; displaygummis<30; displaygummis++)
	 	{
			todisplayx=gummis[displaygummis].x;
   			todisplayy=gummis[displaygummis].y;
   			draw_rle_sprite(display,gummibear_background[gummis[displaygummis].color],gummis[displaygummis].x,gummis[displaygummis].y);
			
			if(!paused)
			{
				gummis[displaygummis].y=gummis[displaygummis].y+gummis[displaygummis].speed;
			}
			
			if(gummis[displaygummis].y>SCREEN_H)
   			{
   				gummis[displaygummis].x=rand()%SCREEN_W;
   				gummis[displaygummis].y=-300;
   				gummis[displaygummis].speed=(rand()%7)+3;
				gummis[displaygummis].color=rand()%3;
   			};
   		};
	}
	
	if(level==11||level==14)
	{
		int chance_of_bad_weather = rand()%100;
		int position_of_bolt = rand()%(SCREEN_W-level11bk->w);
		
		if(chance_of_bad_weather>96&&!paused)
		{
			clear_to_color(display,makecol16(220,220,255));
			draw_rle_sprite(display,lightning_background,position_of_bolt,0);
		}
		else
		{
			if(level==11)
			{
				clear_to_color(display,makecol16(0,0,40));
			}
		}			
	}
	
	if(level==13)
	{
		blit(level13bk,display,0,0,0,0,800,600);
	}
	
	if(level==14)
	{
		draw_rle_sprite(display,fortress_background,200,100);
	}		
}


// Inititialise display and reserve memory for sprites

void display_setup(int screenmode)
{	
	set_color_depth(16);
	
	switch(screenmode)
	{
		//case 0: set_gfx_mode(GFX_XDGA2,800,600,0,0); break;
		//case 1: set_gfx_mode(GFX_XDGA2,800,600,0,0); break;
		//case 2: set_gfx_mode(GFX_XDGA2,800,600,0,0); break;
		//case 3: set_gfx_mode(GFX_TEXT,800,600,0,0); break;
		
		case 0: set_gfx_mode(GFX_AUTODETECT,800,600,0,0); break;
		case 1: set_gfx_mode(GFX_AUTODETECT_FULLSCREEN,800,600,0,0); break;
		case 2: set_gfx_mode(GFX_AUTODETECT_WINDOWED,800,600,0,0); break;
		case 3: set_gfx_mode(GFX_TEXT,800,600,0,0); break;
	};
}
	
void define_sprites()
{	
	display=create_bitmap(800,600);
	
	for(int copies=0; copies<3; copies++)
	{
		shield_copies[copies]=create_bitmap(140,88);
	};

	load_data_files();
	
	rle_ship=get_rle_sprite(ship);
	
	for(int aliens=0; aliens<4; aliens++)
	{
		for(int animframes=0; animframes<4; animframes++)
		{
			rle_alien[aliens][animframes]=get_rle_sprite(alien[aliens][animframes]);
		}
	}
	
	for(int deathanim=0; deathanim<11; deathanim++)
	{
		rle_death[deathanim]=get_rle_sprite(death[deathanim]);
	}
	
	rle_projectile=get_rle_sprite(projectile);
	rle_ufo=get_rle_sprite(ufo);
		
	rock_background[0]=get_rle_sprite(level2bk[0]);
	rock_background[1]=get_rle_sprite(level2bk[1]);
	grid_background=get_rle_sprite(level3bk);
	cloud_background[0]=get_rle_sprite(level4bk[0]);
	cloud_background[1]=get_rle_sprite(level4bk[1]);
	planet_background=get_rle_sprite(level5bk);
	meteor_background=get_rle_sprite(level6bk);
	moon_background=get_rle_sprite(level7bk);
	for(int gummisback=0; gummisback<3; gummisback++)
	{gummibear_background[gummisback]=get_rle_sprite(level9bk[gummisback]);}
	lightning_background=get_rle_sprite(level11bk);
	city_background=get_rle_sprite(level12bk);	
	fortress_background=get_rle_sprite(level14bk);
};

void game_display(int alpha)
{	
	static int flimmer=160;
	BITMAP *shield_aura;
	
	shield_aura=create_bitmap(150,98);
	
	// Display background
	
	display_background();
	
	// Display characters
	
	for(int wavex=0; wavex<8; wavex++)
	{
		for(int wavey=0; wavey<4; wavey++)
		{
			if(wave[wavex][wavey].alive==alive)
			{
				switch(wavey)
				{
					case 0: draw_rle_sprite(display,rle_alien[0][current_alien_frame],wave[wavex][wavey].xpos,wave[wavex][wavey].ypos); break;
					case 1: draw_rle_sprite(display,rle_alien[1][current_alien_frame],wave[wavex][wavey].xpos,wave[wavex][wavey].ypos); break;
					case 2: draw_rle_sprite(display,rle_alien[1][current_alien_frame],wave[wavex][wavey].xpos,wave[wavex][wavey].ypos); break;
					case 3: draw_rle_sprite(display,rle_alien[2][current_alien_frame],wave[wavex][wavey].xpos,wave[wavex][wavey].ypos); break;
				};
			}
			
			if(wave[wavex][wavey].alive==dying)
			{
				set_add_blender(0,0,0,wave[wavex][wavey].alpha);
				draw_trans_sprite(display,death[wave[wavex][wavey].explosionstage/2],wave[wavex][wavey].xpos,wave[wavex][wavey].ypos);
			};
		};
	};
		
	if(animdelay>16)
	{
		current_alien_frame++;
		animdelay=0;
	}
	
	if(current_alien_frame>3)
	{
		current_alien_frame=0;
	}
	
	if(shipposition.alive==alive)
	{
		draw_rle_sprite(display,rle_ship,posx,530);
	}
	else
	{
		draw_rle_sprite(display,rle_death[4],posx,530);
	};
		
	if(bulletposition.alive==alive)
	{
		draw_rle_sprite(display,rle_projectile,bulletposition.xpos,bulletposition.ypos);
	};
	
	if(ufoposition.alive==alive)
	{
		draw_rle_sprite(display,rle_ufo,ufoposition.xpos,ufoposition.ypos);
	};
	
	if(ufoposition.alive==dying)
	{
		set_add_blender(0,0,0,ufoposition.alpha);
		draw_trans_sprite(display,death[ufoposition.explosionstage/2],ufoposition.xpos,ufoposition.ypos);
	};
	
	// Display and manipulate particles
	
	display_particles();
	
	// Display enemy bullets
	
	for(int enemy_shots=0; enemy_shots<projlimit; enemy_shots++)
	{
		if(enemybullets[enemy_shots].alive==alive)
		{
			draw_rle_sprite(display,rle_projectile,enemybullets[enemy_shots].xpos,enemybullets[enemy_shots].ypos);
		}
	};
	
	// Display shields (copies which are modified as bullets collide with them)

	for(int shieldnumber=0; shieldnumber<3; shieldnumber++)
	{
		clear_to_color(shield_aura,makecol16(255,0,255));		
		stretch_sprite(shield_aura,shield_copies[shieldnumber],0,0,150,98);
		
		if(flimmer==160&&!paused)
		{
			flimmer=170;
		}
		else
		{
			flimmer=160;
		}
		set_add_blender(0,0,0,flimmer);
		draw_trans_sprite(display,shield_copies[shieldnumber],(shieldnumber*270)+60,420);
		set_add_blender(0,0,0,flimmer-90);
		draw_trans_sprite(display,shield_aura,(shieldnumber*270)+55,415);
	};
		
	// Display scoreboard
	
	textprintf_ex(display,gamefont,0,0,makecol16(255,255,255),makecol16(255,0,255),"SCORE: %d",score);
	textprintf_centre_ex(display,gamefont,399,0,makecol16(255,255,255),makecol16(255,0,255),"LIVES: %d",lives);
	textprintf_right_ex(display,gamefont,799,0,makecol16(255,255,255),makecol16(255,0,255),"LEVEL: %d",level);
	
	// Display scrolling message
	
	display_scrolling_message(messagebuffer);
	
	// Display final image and clear buffer
	
	if(alpha<255)
	{
		set_trans_blender(0,0,0,255-alpha);
		drawing_mode(DRAW_MODE_TRANS,0,0,0);
		rectfill(display,0,0,(int) SCREEN_W-1,(int) SCREEN_H-1,0);
	}
	else
	{
		drawing_mode(DRAW_MODE_SOLID,0,0,0);
	}
	
	if(paused)
	{
		textout_centre_ex(display, gamefont, "PAUSED", SCREEN_W/2, 300,-1,-1);
	}
	
	blit(display,screen,0,0,0,0,800,600);
	
	clear_to_color(display,0);
	
	destroy_bitmap(shield_aura);
};


void display_particles()
{
	for(int dispparts=0; dispparts<500; dispparts++)
	{
		if(explosionbits[dispparts].active)
		{
			rectfill(display,explosionbits[dispparts].x-1,explosionbits[dispparts].y-1,explosionbits[dispparts].x+1,explosionbits[dispparts].y+1,explosionbits[dispparts].color);
			rect(display,explosionbits[dispparts].x-2,explosionbits[dispparts].y-2,explosionbits[dispparts].x+2,explosionbits[dispparts].y+2,0);		
		}
	}
}	

void alt_fade_in(BITMAP *destimage, int speed)
{
	BITMAP *buffer;
	extern AL_DUH_PLAYER *gamesongplayer;
	
	buffer=create_bitmap(800,600);
	
	clear_to_color(buffer,0);
	
	for(int alpha=0; alpha<256; alpha+=speed)
	{
		set_trans_blender(0,0,0,alpha);
		draw_trans_sprite(buffer,destimage,0,0);
		vsync();
		blit(buffer,screen,0,0,0,0,800,600);
		al_poll_duh(gamesongplayer);
	}
	
	blit(destimage,screen,0,0,0,0,800,600);
	
	destroy_bitmap(buffer);
}

void alt_fade_out(BITMAP *sourceimage, int speed)
{	
	//I'm sure there's a more efficient way of doing this - please let me know
	//if you find it
	
	BITMAP *buffer,*buffer2;
	extern AL_DUH_PLAYER *gamesongplayer;
	extern bool program_active;
	
	buffer=create_bitmap(800,600);
	buffer2=create_bitmap(800,600);
	
	blit(sourceimage,buffer,0,0,0,0,800,600);
	clear_to_color(buffer2,0);
	
	for(int alpha=0; alpha<256; alpha+=speed)
	{
		set_trans_blender(0,0,0,alpha);
		draw_trans_sprite(buffer,buffer2,0,0);
		vsync();
		blit(buffer,screen,0,0,0,0,800,600);
		if(program_active)
		{
			al_poll_duh(gamesongplayer);
		}
	}
	
	clear_to_color(screen,0);
	
	destroy_bitmap(buffer);
	destroy_bitmap(buffer2);
}

void alt_fade_between(BITMAP *sourceimage, BITMAP *destimage, int speed)
{
	BITMAP *buffer;
	
	buffer=create_bitmap(800,600);
	
	blit(sourceimage,buffer,0,0,0,0,800,600);
	
	for(int alpha=0; alpha<256; alpha+=speed)
	{
		set_trans_blender(0,0,0,alpha);
		draw_trans_sprite(buffer,destimage,0,0);
		vsync();
		blit(buffer,screen,0,0,0,0,800,600);
	}
	
	blit(destimage,screen,0,0,0,0,800,600);
	
	destroy_bitmap(buffer);
}

void create_scrolling_message()
{
	std::string message;
	BITMAP *buffer;
	
	switch(level)
	{
		case 1: message="STARS IN YOUR EYES"; break;
		case 2: message="BECAVE YOURSELF"; break;
		case 3: message="OFF THE GRID"; break;
		case 4: message="SKY HIGH"; break;
		case 5: message="BIG ROUND AND SHINY"; break;
		case 6: message="METEOR DOOM"; break;
		case 7: message="ISN'T THE MOON WONDERFUL TONIGHT?"; break;
		case 8: message="SKY DIE"; break;
		case 9: message="SWEET REVENGE"; break;
		case 10: message="I CAN SEE BETTER IN THE DARK"; break;	
		case 11: message="ABSOLUTELY SHOCKING"; break;
		case 12: message="NEW YORK, NEW YORK"; break;
		case 13: message="I WANNA PARTY LIKE IT'S 1985"; break;
		case 14: message="THE OBLIGATORY FINAL BATTLE"; break;
	};
	
	buffer=create_bitmap(message.length()*25,25);
	destroy_bitmap(messagebuffer);
	messagebuffer=create_bitmap((buffer->w)*4,(buffer->h)*4);
		
	clear_to_color(buffer,makecol16(255,0,255));
	
	textout_ex(buffer,gamefont,message.c_str(),0,0,-1,-1);
	
	stretch_blit(buffer,messagebuffer,0,0,buffer->w,buffer->h,0,0,messagebuffer->w,messagebuffer->h);
	
	destroy_bitmap(buffer);	
}

void display_scrolling_message(BITMAP *message)
{
	set_add_blender(0,0,0,100);
	draw_trans_sprite(display,message,scrollposition,30);
}

void update_scrolly_position()
{
	if(scrollposition>-3000)
	{
		scrollposition=scrollposition-6;
	}
}
