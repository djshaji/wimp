/*
 *      config.h
 *
 *      Copyright 2010 Shaji <djshaji@gmail.com>
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */


#ifndef __CONFIG_H
#define __CONFIG_H
#include <gtk/gtk.h>
#include "definitions.c"

typedef struct _Config {
	gchar * ao, * vo ;
	gboolean emergency_exit_enabled ;
	
	gboolean safe_search ;
	gint thumbnail_size ;
	gboolean memlock ;
} Config ;

void wimp_config_load ( Config * config, gchar * filename ) ;
Config * wimp_config_new ( void ) ;
void wimp_config_save ( Config * config, gchar * filename ) ;
void wimp_config_save_default ( gchar * filename ) ;
#endif
