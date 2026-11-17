/***************************************************************************
 *            logic.h
 *
 *  Fri Jun  1 00:09:56 2007
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

int activate_logic();
void reset_enemies_state();
bool game_still_active();
void move_automatic_items();
void process_enemy_projectiles();
void process_ufo();
void death_sequence();
void check_for_next_level();
void check_if_extra_life_due();
void check_if_game_over();
void update_logic();
void reset_enemies_position();
void update_particles();
void update_explosions();
void update_animation();
