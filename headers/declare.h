/***************************************************************************
 *            declare.h
 *
 *  Tue Jun  5 01:25:13 2007
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
 
enum{alive, dying, dead};

#define ALLEGRO_LOGO PACKAGE_DATA_DIR"/"PACKAGE"/allegro_logo.pcx"
#define GAME_FONT PACKAGE_DATA_DIR"/"PACKAGE"/arcade_font.pcx"
#define BIG_FONT PACKAGE_DATA_DIR"/"PACKAGE"/arcade_font_big.pcx"
#define SPIKE_IMAGE PACKAGE_DATA_DIR"/"PACKAGE"/spike.pcx"
#define JAMYSKIS_LOGO PACKAGE_DATA_DIR"/"PACKAGE"/jamyskis_logo.pcx"
#define LINUX_LOGO PACKAGE_DATA_DIR"/"PACKAGE"/linux.pcx"
#define GFX_SHIP PACKAGE_DATA_DIR"/"PACKAGE"/ship.pcx"
#define GFX_ALIEN_TOP PACKAGE_DATA_DIR"/"PACKAGE"/invader_top.pcx"
#define GFX_ALIEN_MIDDLE PACKAGE_DATA_DIR"/"PACKAGE"/invader_middle.pcx"
#define GFX_ALIEN_BOTTOM PACKAGE_DATA_DIR"/"PACKAGE"/invader_bottom.pcx"
#define GFX_EXPLOSION PACKAGE_DATA_DIR"/"PACKAGE"/explosion.pcx"
#define GFX_BULLET PACKAGE_DATA_DIR"/"PACKAGE"/bullet.pcx"
#define GFX_UFO PACKAGE_DATA_DIR"/"PACKAGE"/ufo.pcx"
#define SHIELD_GRAPHIC PACKAGE_DATA_DIR"/"PACKAGE"/shield.pcx"
#define OPEN_INVADERS_LOGO PACKAGE_DATA_DIR"/"PACKAGE"/oi_logo.pcx"
#define LEVEL_TWO_BACKGROUND_LEFT PACKAGE_DATA_DIR"/"PACKAGE"/level2bk.pcx"
#define LEVEL_TWO_BACKGROUND_RIGHT PACKAGE_DATA_DIR"/"PACKAGE"/level2bk2.pcx"
#define LEVEL_THREE_BACKGROUND PACKAGE_DATA_DIR"/"PACKAGE"/level3bk.pcx"
#define LEVEL_FOUR_BACKGROUND_ONE PACKAGE_DATA_DIR"/"PACKAGE"/level4bk.pcx"
#define LEVEL_FOUR_BACKGROUND_TWO PACKAGE_DATA_DIR"/"PACKAGE"/level4bk2.pcx"
#define LEVEL_FIVE_BACKGROUND PACKAGE_DATA_DIR"/"PACKAGE"/level5bk.pcx"
#define LEVEL_SIX_BACKGROUND PACKAGE_DATA_DIR"/"PACKAGE"/level6bk.pcx"
#define LEVEL_SEVEN_BACKGROUND PACKAGE_DATA_DIR"/"PACKAGE"/level7bk.pcx"
#define LEVEL_NINE_BACKGROUND_RED PACKAGE_DATA_DIR"/"PACKAGE"/level9bk1.pcx"
#define LEVEL_NINE_BACKGROUND_GREEN PACKAGE_DATA_DIR"/"PACKAGE"/level9bk2.pcx"
#define LEVEL_NINE_BACKGROUND_YELLOW PACKAGE_DATA_DIR"/"PACKAGE"/level9bk3.pcx"
#define LEVEL_ELEVEN_BACKGROUND PACKAGE_DATA_DIR"/"PACKAGE"/level11bk.pcx"
#define LEVEL_TWELVE_BACKGROUND PACKAGE_DATA_DIR"/"PACKAGE"/level12bk.pcx"
#define LEVEL_THIRTEEN_BACKGROUND PACKAGE_DATA_DIR"/"PACKAGE"/level13bk.pcx"
#define LEVEL_FOURTEEN_BACKGROUND PACKAGE_DATA_DIR"/"PACKAGE"/level14bk.pcx"
#define SHOOT_SOUND PACKAGE_DATA_DIR"/"PACKAGE"/shoot.wav"
#define DESTROY_SOUND PACKAGE_DATA_DIR"/"PACKAGE"/destroy.wav"
#define NEW_HIGH_SCORE_SOUND PACKAGE_DATA_DIR"/"PACKAGE"/new_high_score.wav"
#define SCREENSHOT_SOUND PACKAGE_DATA_DIR"/"PACKAGE"/screenshotsaved.wav"
#define WELCOME_SOUND PACKAGE_DATA_DIR"/"PACKAGE"/welcome.wav"
#define LETS_GO_SOUND PACKAGE_DATA_DIR"/"PACKAGE"/letsgo.wav"
#define GAME_IS_OVER_SOUND PACKAGE_DATA_DIR"/"PACKAGE"/gameisover.wav"
#define LIST_HISCORES_SOUND PACKAGE_DATA_DIR"/"PACKAGE"/listhiscores.wav"
#define TITLE_MOVE_SOUND PACKAGE_DATA_DIR"/"PACKAGE"/move.wav"
#define TITLE_SELECT_SOUND PACKAGE_DATA_DIR"/"PACKAGE"/select.wav"
#define TITLE_SONG PACKAGE_DATA_DIR"/"PACKAGE"/titlesong.mod"
#define TITLE_CREDITS_SOUND PACKAGE_DATA_DIR"/"PACKAGE"/credits.wav"
#define TITLE_OPTIONS_SOUND PACKAGE_DATA_DIR"/"PACKAGE"/options.wav"
#define OPTIONS_CHICKEN_SOUND PACKAGE_DATA_DIR"/"PACKAGE"/chicken.wav"
#define OPTIONS_INSANE_SOUND PACKAGE_DATA_DIR"/"PACKAGE"/insane.wav"
#define TITLE_GOODBYE_SOUND PACKAGE_DATA_DIR"/"PACKAGE"/goodbye.wav"
#define GAME_SONG PACKAGE_DATA_DIR"/"PACKAGE"/gamesong.mod"
#define ENDING_SONG PACKAGE_DATA_DIR"/"PACKAGE"/endsong.mod"
#define HISCORE_SONG PACKAGE_DATA_DIR"/"PACKAGE"/hiscore.mod"

#ifdef ALLEGRO_LINUX
#include "headers/pmask.h"
#else
#include "../include/pmask.h"
#endif


struct item
{
	int xpos;
	int ypos;
	int alive;
	int deathtime;
	int delay;
	int explosionstage;
	int alpha;
	PMASK *mask;
};

struct startype
{
	int x;
	int y;
	int speed;
	int color;
	int size;
	int brightness;
};

struct particle
{
	bool active;
	int x;
	int y;
	int xthrust;
	int ythrust;
	int life;
	int color;
};
