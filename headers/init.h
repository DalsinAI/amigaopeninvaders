/***************************************************************************
 *            init.h
 *
 *  Fri Jun  1 00:10:15 2007
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

void load_data_files();
void load_hiscore();
void save_hiscore();
void load_config();
void save_config();
void initialise_game();
void predefine_variables();
void reset_shields();
BITMAP *oi_load_graphic(std::string filename, std::string defsymbol);
SAMPLE *oi_load_sfx(std::string filename, std::string defsymbol);
