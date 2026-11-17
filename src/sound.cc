/***************************************************************************
 *            sound.cc
 *
 *  Fri Jun  1 00:13:31 2007
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

#include "config.h"
#include "headers/declare.h"

SAMPLE *shoot, *explode, *newhighscore, *screenshotsaved, *welcome;
SAMPLE *letsgo, *gameisover, *listhiscores;

int music_volume;
int sfx_volume;

void check_if_hiscore_beaten(int passedscore, int passedhiscore)
{
	static bool hiscore_beaten=false;
	
	if(passedscore>passedhiscore)
	{
		if(!hiscore_beaten)
		{
			play_sample(newhighscore,sfx_volume,128,1000,0);
			hiscore_beaten=true;
		};
	}
}
