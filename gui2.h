/*
 *      gui2.h
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


#ifndef __WIMP_GUI2_H
#define __WIMP_GUI2_H

#include <gtk/gtk.h>
#include <xine.h>

#include <X11/Xlib.h>
#include <gdk/gdkx.h>
#include <gdk/gdkkeysyms.h>

#include <sys/mman.h>

#include "definitions.c"
#include "xine.h"

#include "util.h"
#include "config.h"

#include "search.h"
#include "gui.h"

private static gint timestamp = 0 ;
gboolean wimp_gui2_track_window_state ( 	GtkWidget * widget,
										GdkEventWindowState * event,
										WimpGUI * gui );
void * wimp_gui2_init ( WimpGUI * wimp ) ;
void wimp_gui2_toggle_pause ( WimpGUI * gui ) ;
void wimp_gui2_move_controls_away ( WimpGUI * gui ) ;
void wimp_gui2_dance ( WimpGUI * gui ) ;
gboolean wimp_gui2_quit ( WimpGUI * gui ) ;
void wimp_gui2_zoom_100 ( WimpGUI * gui ) ;
void wimp_gui2_zoom_2x ( WimpGUI * gui ) ;
void wimp_gui2_zoom_x_by_2 ( WimpGUI * gui ) ;
void wimp_gui2_zoom_3x ( WimpGUI * gui ) ;
void wimp_gui2_jump_to ( WimpGUI * gui ) ;

static gboolean gui_init = false ;
void wimp_gui2_search_update ( WimpGUI * gui ) ;
void wimp_search_row_activated ( 	GtkTreeView * tree_view,
									GtkTreePath * tree_path,
									GtkTreeViewColumn * column,
									WimpGUI * gui ) ;

void wimp_search_load_images ( WimpGUI * gui ) ;
private private void fire_event ( void ) ;
void wimp_search_search ( WimpGUI * gui ) ;
void wimp_gui2_handle_search_video ( WimpGUI * gui, gchar * url ) ;
void wimp_gui2_search_shew_source ( void ) ;
void wimp_search_reload ( WimpGUI * gui ) ;
GtkMenu * wimp_search_create_menu ( WimpGUI * gui ) ;
bool wimp_gui2_search_popup_menu ( GtkWidget * widget, GdkEventButton * event, WimpGUI * gui ) ;
void wimp_gui2_search_open ( WimpGUI * gui ) ;
void wimp_gui2_search_browse_favorites ( WimpGUI * gui ) ;
void wimp_gui2_search_add_to_favorites ( WimpGUI * gui ) ;
void wimp_gui2_search_delete_favorite ( WimpGUI * gui ) ;
private void activate_my_dialog ( GtkDialog * dialog ) ;
#endif
