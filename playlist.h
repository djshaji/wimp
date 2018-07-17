/*
 *      playlist.h
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

#ifndef __PLAYLIST_H
#define __PLAYLIST_H

#include <gtk/gtk.h>
#include <xine.h>

#define _GNU_SOURCE
#include <string.h>
#include <errno.h>

#include "definitions.c"
#include "gui.h"
#include "encode.h"

void wimp_gui_playlist_append ( WimpGUI * gui, gchar * filename ) ;
void wimp_gui_playlist_clear ( WimpGUI * gui ) ;
gchar * wimp_gui_playlist_get_first ( WimpGUI * gui ) ;
void wimp_gui_playlist_next ( WimpGUI  * gui ) ;
void wimp_gui_playlist_previous ( WimpGUI * gui ) ;
gchar * wimp_gui_playlist_get_selected ( WimpGUI * gui ) ;
gchar * wimp_gui_playlist_get_last ( WimpGUI * gui ) ;
gint wimp_gui_playlist_get_n_tracks ( WimpGUI * gui ) ;
gchar * wimp_gui_playlist_get_next ( WimpGUI * gui ) ;
gchar * wimp_gui_playlist_get_previous ( WimpGUI * gui ) ;
void wimp_gui_playlist_play ( WimpGUI * gui, gchar * filename ) ;
gint wimp_gui_playlist_get_position ( WimpGUI * gui, gchar * filename ) ;
void wimp_gui_playlist_load ( WimpGUI * gui, gchar * filename ) ;

void wimp_gui_playlist_row_activated ( 	GtkTreeView * tree_view,
									GtkTreePath * tree_path,
									GtkTreeViewColumn * column,
									WimpGUI * gui ) ;

void wimp_gui_playlist_load_playlist ( WimpGUI * gui, gchar * filename ) ;
void wimp_gui_playlist_save_playlist ( WimpGUI * gui, gchar * wimp_playlist ) ;
void wimp_gui_playlist_load_playlist_default ( WimpGUI * gui ) ;
void wimp_gui_playlist_save_playlist_default ( WimpGUI * gui ) ;
void wim_gui_playlist_add_to_playlist ( WimpGUI * gui ) ;
void wimp_gui_playlist_delete_from_playlist ( WimpGUI * gui ) ;
void wimp_gui_playlist_play_from_selection ( WimpGUI * gui ) ;
void wimp_gui_playlist_load_playlist_from_file ( WimpGUI * gui ) ;
void wimp_gui_playlist_save_playlist_to_file ( WimpGUI * gui ) ;
void wimp_gui_playlist_add_to_playlist ( WimpGUI * gui ) ;
GtkMenu * wimp_gui_playlist_menu_create ( WimpGUI * gui ) ;
gboolean wimp_gui_playlist_popup_menu ( GtkWidget * widget, GdkEventButton * event, WimpGUI * wimp ) ;
void wimp_gui_load_folder_prompt ( WimpGUI * gui ) ;
void wimp_gui_load_prompt ( WimpGUI * gui ) ;
void  wimp_gui_load_folder_append ( WimpGUI * gui, gchar * folder ) ;
gboolean wimp_gui_playlist_search_func (	GtkTreeModel * model,
											gint column,
											const gchar * key,
											GtkTreeIter * iter,
											shagpointer data ) ;
gboolean wimp_gui_playlist_drag_drop ( 	GtkWidget * widget, GdkDragContext * context,
							gint x, gint y, GtkSelectionData * data, 
							guint info, guint time, WimpGUI * gui ) ;
#define wimp_gui_playlist_length wimp_gui_playlist_get_n_tracks
gboolean wimp_gui_playlist_drag_drop_replace ( 	GtkWidget * widget, GdkDragContext * context,
							gint x, gint y, GtkSelectionData * data, 
							guint info, guint time, WimpGUI * gui ) ;
gchar * wimp_gui_playlist_get_random ( WimpGUI * gui ) ;
void wimp_gui_playlist_random ( WimpGUI  * gui ) ;
void wimp_gui_playlist_append_url ( WimpGUI * gui, gchar * filename ) ;
void wimp_gui_playlist_open ( WimpGUI * gui, gchar * filename ) ;
void wimp_gui_playlist_add_to_playlist_url ( WimpGUI * gui ) ;
#endif
