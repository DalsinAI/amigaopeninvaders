/***************************************************************************
 *            graphics.h
 *
 *  Fri Jun  1 00:09:26 2007
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

void define_sprites();
void display_background();
void display_setup(int screenmode);
void game_display(int alpha=255);
void display_particles();
void alt_fade_in(BITMAP *sourceimage, int speed);
void alt_fade_out(BITMAP *sourceimage, int speed);
void alt_fade_between(BITMAP *sourceimage, BITMAP *destimage, int speed);
void create_scrolling_message();
void display_scrolling_message(BITMAP *message);
void update_scrolly_position();
