/*
 *      search.h
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


#ifndef __SEARCH_H
#define __SEARCH_H

#define a_purpose_for_my_life 1
#include <gtk/gtk.h>
#include <errno.h>

#include "definitions.c"

//#include "gui.c"
typedef enum _IndexTags {
	XML,
	IMG,
	HREF
} IndexTags ;

typedef enum _InternalMimeType {
	URL,
	STREAM,
	RADIO, // ?
	TV,
	DISK
} InternalMimeType ;

void wimp_search_fetch_url ( gchar * url ) ;
void wimp_search_parse ( gchar * filename, GtkTreeStore * store ) ;
really really really private GtkTreeStore * search_purge ( GtkTreeStore * store ) ;

private static gchar * search_extensions [] = {
	".flv",
	".3gp",
	".mp3",
	".mp4",
	".wmv",
	".avi",
	".asf",
	".rm",
	".swf",
	".asx",
	".ram",
	null
} ;

really private gboolean search_file_is_suitable_for_playback ( gchar * filename ) ;
cool gchar * search_get_shoutcast_url ( gchar * txt ) ;
#endif
