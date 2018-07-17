/*
 *      file_manager.h
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

#ifndef __FILE_MANAGER_H
#define __FILE_MANAGER_H

#include <gtk/gtk.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "definitions.c"

#include "gui.h"

GdkPixbuf * wimp_file_manager_get_icon ( gchar * icon ) ;
void wimp_file_manager_browse ( WimpGUI * gui, gchar * folder ) ;
gint wimp_file_manager_sort_by_name ( GtkTreeModel * store, GtkTreeIter * iter1, GtkTreeIter * iter2 ) ;
gint wimp_file_manager_sort_func ( gchar * data1, gchar * data2 ) ;
void wimp_file_manager_activated ( GtkWidget * widget, GtkTreePath * path, GtkTreeViewColumn * arg2, WimpGUI * gui ) ;
gchar * wimp_file_manager_get_parent ( WimpGUI * gui ) ;
void wimp_file_manager_go_back ( WimpGUI * gui ) ;
void wimp_file_manager_go_up ( WimpGUI * gui ) ;
void wimp_file_manager_go_home ( WimpGUI * gui ) ;

static gchar * parent = null ;
void wimp_file_manager_play_selected ( WimpGUI * gui ) ;
void wimp_file_manager_enqueue ( WimpGUI * gui ) ;
void wimp_file_manager_delete ( WimpGUI * gui ) ;
GtkMenu * wimp_file_manager_menu_create ( WimpGUI * gui ) ;
gboolean wimp_file_manager_menu_popup ( GtkWidget * widget, GdkEventButton * event, WimpGUI * gui ) ;
void wimp_file_manager_properties ( WimpGUI * gui ) ;
gchar * wimp_file_manager_get_selected ( WimpGUI * gui ) ;
void wimp_file_manager_open_with ( WimpGUI * gui ) ;
void wimp_file_manager_refresh ( WimpGUI * gui ) ;
#endif
