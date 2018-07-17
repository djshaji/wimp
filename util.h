/*
 *      util.h
 *
 *      Copyright 2009 Shaji <djshaji@gmail.com>
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

#ifndef __WIMP_UTIL_H
#define __WIMP_UTIL_H

#include <gtk/gtk.h>
#include <xine.h>

#include <signal.h>
#include <X11/Xlib.h>
#include "definitions.c"
#include "session.h"

void wimp_log_handler ( const gchar * log_domain, GLogLevelFlags log_level,
						const gchar * message, gpointer user_data ) ;

void wimp_message_box_info ( gchar * message ) ;
void wimp_message_warning ( gchar * message ) ;
void wimp_message_error ( gchar * message ) ;

gboolean wimp_message_question ( gchar * message ) ;
void wimp_update_gui ( void ) ;
gchar * wimp_format_time ( gint pos, gint length ) ;
void wimp_sighandler ( gint signum ) ;
gboolean wimp_file_is_suitable_for_playback ( gchar * filename );
gchar * wimp_array_to_char ( gchar ** vector ) ;
gchar * wimp_string_to_nl ( gchar * string ) ;
GList * wimp_xdnd_to_list ( gchar * text ) ;
#define OPEN GTK_FILE_CHOOSER_ACTION_OPEN
#define SAVE GTK_FILE_CHOOSER_ACTION_SAVE
void wimp_message_box_detailed_info ( gchar * message ) ;
void wimp_gui2_run ( gchar * title, gchar * command ) ;
void wimp_log_to_file ( gchar * text ) ;
void wimp_show_log ( vodi ) ;
GdkPixbuf * wimp_pixbuf_new_from_file_at_scale ( gchar * filename, gint width, gint height, GError ** error ) ;
void wimp_clipboard_set ( gchar * filename ) ;
bool wimp_file_is_stream ( gchar * filename ) ;
#endif /* __WIMP_UTIL_H */
