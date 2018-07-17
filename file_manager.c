/*
 *      file_manager.c
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


#include "file_manager.h"

GdkPixbuf * wimp_file_manager_get_icon ( gchar * icon )
{
	GdkPixbuf * pixbuf = gtk_icon_theme_load_icon (	gtk_icon_theme_get_default (),
										icon,
										32,
										GTK_ICON_LOOKUP_USE_BUILTIN,
										null ) ;
	
	if ( pixbuf == null )
	{
		pixbuf = gtk_icon_theme_load_icon (	gtk_icon_theme_get_default (),
										"gtk-file",
										32,
										GTK_ICON_LOOKUP_USE_BUILTIN,
										null ) ;
	
	}
	
	return pixbuf ;
}
										

void wimp_file_manager_browse ( WimpGUI * gui, gchar * folder )
{
	in ;
	gtk_tree_store_clear ( gui -> model ) ;
	
	GDir * dir = null ;
	GError * error = null ;
	
	dir = g_dir_open ( folder, 0, & error ) ;
	if ( error )
	{
		g_warning ( "Cannot open folder: %s", error -> message ) ;
		return ;
	}
	
	gchar * name = g_dir_read_name ( dir ) ;
	while ( name )
	{
		if ( name [0] == '.' )
		{
			name = g_dir_read_name ( dir ) ;
			continue ;
		}
		
		gchar * path = g_build_filename ( folder, name, null ) ;
		GdkPixbuf * pixbuf = null ;
		
		gboolean is_dir = false ;
		
		if ( g_file_test ( path, G_FILE_TEST_IS_DIR ))
		{
			pixbuf = wimp_file_manager_get_icon ( "gtk-directory" ) ;
			is_dir = true ;
		}
		else if ( wimp_file_is_suitable_for_playback ( path ))
			pixbuf = wimp_file_manager_get_icon ( "gnome-mime-video" ) ;
		else
			pixbuf = wimp_file_manager_get_icon ( "gtk-file" ) ;
		
		GtkTreeIter iter ;
		gtk_tree_store_append ( gui -> model, & iter, null ) ;
		
		gtk_tree_store_set ( gui -> model, & iter, 0, path, 1, name, 2, pixbuf, 3, is_dir, -1 ) ;
		g_free ( path ) ;
		
		g_object_unref ( pixbuf ) ;
		name = g_dir_read_name ( dir ) ;
	}
	
	if ( g_list_length ( gui -> history ) < 20 )
		gui -> history = g_list_append ( gui -> history, g_strdup ( folder )) ;
	else
	{
		g_free ( g_list_nth_data ( gui -> history, 0 )) ;
		
		gui -> history = g_list_remove ( gui -> history, g_list_nth_data ( gui -> history, 0 )) ;
		gui -> history = g_list_append ( gui -> history, g_strdup ( folder )) ;
	}
	
	g_dir_close ( dir ) ;
	out ;
}
	
gint wimp_file_manager_sort_func ( gchar * data1, gchar * data2 )
{
	gint result = 0 ;
	gchar * basename1 = g_path_get_basename ( data1 ) ;
	gchar * basename2 = g_path_get_basename ( data2 ) ;
	
	gchar * name1 = g_utf8_strdown ( basename1, -1 ) ;
	gchar * name2 = g_utf8_strdown ( basename2, -1 ) ;
	
	gint len1 = strlen ( name1 ), len2 = strlen ( name2 ) ;
	gint i = 0, len = 0 ;
	if ( len2 < len1 )
		len = len2 ;
	else if ( len1 < len2 )
		len = len2 ;
	else if ( len1 == len2 )
		len = len1 ;
	
	for ( i = 0 ; i < len ; i ++ )
	{
		if ( name1 [i] < name2 [i] )
		{
			result = -1 ;
			break ;
		}
		else if ( name2 [i] < name1 [i] )
		{
			result = 1 ;
			break ;
		}
		else
			result = 0 ;
	}
	
	g_free ( name1 ) ;
	g_free ( name2 ) ;
	
	g_free ( basename1 ) ;
	g_free ( basename2 ) ;
	
	return result ;
}

gint wimp_file_manager_sort_by_name ( GtkTreeModel * store, GtkTreeIter * iter1, GtkTreeIter * iter2 )
{
	gchar * name1 = null ;
	gchar * name2 = null ;
	
	gtk_tree_model_get ( store, iter1, 1, & name1, -1 ) ;
	gtk_tree_model_get ( store, iter2, 1, & name2, -1 ) ;
	
	gboolean is_dir1 = false, is_dir2 = false ;

	gtk_tree_model_get ( store, iter1, 1, & name1, 3, & is_dir1, -1 ) ;
	gtk_tree_model_get ( store, iter2, 1, & name2, 3, & is_dir2, -1 ) ;
	
	gint result = 0 ;
	if ( is_dir1 && is_dir2 )
		result = wimp_file_manager_sort_func ( name1, name2 ) ;
	else if ( is_dir1 && ! is_dir2 )
		result = -1 ;
	else if ( ! is_dir1 && is_dir2 )
		result = 1 ;
	else
		result = wimp_file_manager_sort_func ( name1, name2 ) ;
	
	g_free ( name1 ) ;
	g_free ( name2 ) ;
	
	return result ;
}

void wimp_file_manager_activated ( GtkWidget * widget, GtkTreePath * path, GtkTreeViewColumn * arg2, WimpGUI * gui )
{
	in ;
	
	GtkTreeIter iter ;
	gtk_tree_model_get_iter ( gui -> model, & iter, path ) ;
	
	gchar * filename = null ;
	gboolean is_dir = false ;
	
	gtk_tree_model_get ( gui -> model, & iter, 0, & filename, 3, & is_dir, -1 ) ;
	
	if ( is_dir )
		wimp_file_manager_browse ( gui, filename ) ;
	else
	{
		wimp_gui_playlist_clear ( gui ) ;
		wimp_gui_playlist_append ( gui, filename ) ;
		
		wimp_gui_playlist_play ( gui, filename ) ;
	}
	
	g_free ( filename ) ;
	out ;
}

gchar * wimp_file_manager_get_parent ( WimpGUI * gui )
{
	in ;
	
	/*
	GtkTreeIter iter ;
	gtk_tree_model_get_iter_first ( gui -> model, & iter ) ;
	
	gchar * filename = null ;
	gtk_tree_model_get ( gui -> model, & iter, 0, & filename, -1 ) ;
	
	gchar * dirname = g_path_get_dirname ( filename ) ;
	g_free ( filename ) ;
	
	*/
	
	gchar * dirname = g_strdup ( g_list_nth_data ( gui -> history, g_list_length ( gui -> history ) - 1 )) ;
	
	out ;
	return dirname ;
}
		
void wimp_file_manager_go_up ( WimpGUI * gui )
{
	in ;
	
	gchar * parent = wimp_file_manager_get_parent ( gui ) ;
	gchar * dirname = g_path_get_dirname ( parent ) ;
	
	g_free ( parent ) ;
	wimp_file_manager_browse ( gui, dirname ) ;
	
	g_free ( dirname ) ;
	out ;
}

void wimp_file_manager_go_back ( WimpGUI * gui )
{
	in ;
	
	gchar * parent = wimp_file_manager_get_parent ( gui ) ;
	gint pos = g_list_position ( gui -> history, g_list_find_custom ( gui -> history, parent, strcmp )) ;
	
	if ( pos == -1 )
	{
		g_free ( parent ) ;
		g_warning ( "An internal error occurred. %s", __FUNCTION__ ) ;
		
		return ;
	}
	
	if ( pos > 0 )
		pos -- ;
	
	wimp_file_manager_browse ( gui, g_list_nth_data ( gui -> history, pos )) ;
	g_free ( parent ) ;
	
	out ;
}

void wimp_file_manager_go_home ( WimpGUI * gui )
{
	in ;
	wimp_file_manager_browse ( gui, g_get_home_dir ()) ;
	
	out ;
}

void wimp_file_manager_play_selected ( WimpGUI * gui )
{
	in ;
	
	GList * list = null ;
	list = gtk_tree_selection_get_selected_rows ( gtk_tree_view_get_selection ( gui -> browser ), null ) ;
	
	gint i = 0, length = g_list_length ( list ) ;
	gboolean playlist_cleared = no,
			 started_to_play_well_see_its_not_really_that_easy_for_me_to_call_her_and_if_i_did_call_her_anyway_it_wouldnt_make_much_of_a_difference_because_i_dont_think_shes_right_for_me_and_anyway_if_i_can_get_over_HER_i_can_most_definitely_get_over_her = no ;
	
	for ( i = 0 ; i < length ; i ++ )
	{
		GtkTreeIter iter ;
		gtk_tree_model_get_iter ( gui -> model, & iter, g_list_nth_data ( list, i )) ;
		
		gchar * filename = null ;
		gtk_tree_model_get ( gui -> model, & iter, 0, & filename, -1 ) ;
		
		if ( g_file_test ( filename, G_FILE_TEST_IS_DIR ))
			wimp_file_manager_browse ( gui, filename ) ;
		else
		{
			if ( ! playlist_cleared )
			{
				wimp_gui_playlist_clear ( gui ) ;
				playlist_cleared = yes ;
			}
			
			wimp_gui_playlist_append ( gui, filename ) ;
			started_to_play_well_see_its_not_really_that_easy_for_me_to_call_her_and_if_i_did_call_her_anyway_it_wouldnt_make_much_of_a_difference_because_i_dont_think_shes_right_for_me_and_anyway_if_i_can_get_over_HER_i_can_most_definitely_get_over_her = true ;
		}
		
		g_free ( filename ) ;
	}
	
	g_list_foreach ( list, gtk_tree_path_free, null ) ;
	g_list_free ( list ) ;
	
	if ( started_to_play_well_see_its_not_really_that_easy_for_me_to_call_her_and_if_i_did_call_her_anyway_it_wouldnt_make_much_of_a_difference_because_i_dont_think_shes_right_for_me_and_anyway_if_i_can_get_over_HER_i_can_most_definitely_get_over_her )
		wimp_gui_playlist_play ( gui, null ) ;
	
	out ;
}

void wimp_file_manager_enqueue ( WimpGUI * gui )
{
	in ;
	
	GList * list = null ;
	list = gtk_tree_selection_get_selected_rows ( gtk_tree_view_get_selection ( gui -> browser ), null ) ;
	
	gint i = 0, length = g_list_length ( list ) ;
	
	for ( i = 0 ; i < length ; i ++ )
	{
		GtkTreeIter iter ;
		gtk_tree_model_get_iter ( gui -> model, & iter, g_list_nth_data ( list, i )) ;
		
		gchar * filename = null ;
		gtk_tree_model_get ( gui -> model, & iter, 0, & filename, -1 ) ;
		
		wimp_gui_playlist_append ( gui, filename ) ;
		g_free ( filename ) ;
	}
	
	g_list_foreach ( list, gtk_tree_path_free, null ) ;
	g_list_free ( list ) ;
	
	out ;
}

void wimp_file_manager_delete ( WimpGUI * gui )
{
	in ;
	
	GList * list = null ;
	list = gtk_tree_selection_get_selected_rows ( gtk_tree_view_get_selection ( gui -> browser ), null ) ;
	
	gint i = 0, length = g_list_length ( list ) ;
	
	gchar * m = g_strdup_printf ( "Are you sure you want to delete %d file(s) ?", length ) ;	
	if ( ! wimp_message_question ( m ))
	{
		g_list_foreach ( list, gtk_tree_path_free, null ) ;
		g_list_free ( list ) ;

		g_free ( m ) ;
		return ;
	}
	
	g_free ( m ) ;
	
	for ( i = 0 ; i < length ; i ++ )
	{
		GtkTreeIter iter ;
		gtk_tree_model_get_iter ( gui -> model, & iter, g_list_nth_data ( list, i )) ;
		
		gchar * filename = null ;
		gtk_tree_model_get ( gui -> model, & iter, 0, & filename, -1 ) ;
		
		if ( remove ( filename ) != 0 )
			g_warning ( "Unable to delete: %s", strerror ( errno )) ;
		else
			g_debug ( "Removed %s", filename ) ;
		
		g_free ( filename ) ;
	}

	g_list_foreach ( list, gtk_tree_path_free, null ) ;
	g_list_free ( list ) ;

	wimp_file_manager_refresh ( gui ) ;
	out ;
}

GtkMenu * wimp_file_manager_menu_create ( WimpGUI * gui )
{
	in ;
	GtkMenuItem * play = gtk_menu_item_new_with_label ( "Play" ),
				* en = gtk_menu_item_new_with_label ( "Enqueue" ),
				* del = gtk_menu_item_new_with_label ( "Delete" ),
				* open_with = gtk_menu_item_new_with_label ( "Open with" ),
				* prop = gtk_menu_item_new_with_label ( "Properties" ) ;
				

	g_signal_connect_swapped ( play, "activate", wimp_file_manager_play_selected , gui ) ;
	g_signal_connect_swapped ( en, "activate", wimp_file_manager_enqueue, gui ) ;

	g_signal_connect_swapped ( del, "activate", wimp_file_manager_delete, gui ) ;
	g_signal_connect_swapped ( open_with, "activate", wimp_file_manager_open_with, gui ) ;

	g_signal_connect_swapped ( prop, "activate", wimp_file_manager_properties, gui ) ;

	GtkMenu * menu = gtk_menu_new () ;
	gtk_menu_shell_append ( menu, play ) ;
	
	gtk_menu_shell_append ( menu, en ) ;
	gtk_menu_shell_append ( menu, gtk_separator_menu_item_new ()) ;
	
	gtk_menu_shell_append ( menu, open_with ) ;
	gtk_menu_shell_append ( menu, gtk_separator_menu_item_new ()) ;
	
	gtk_menu_shell_append ( menu, del ) ;
	gtk_menu_shell_append ( menu, prop ) ;
	
	gtk_widget_show_all ( menu ) ;
	return menu ;
}

gboolean wimp_file_manager_menu_popup ( GtkWidget * widget, GdkEventButton * event, WimpGUI * wimp )
{
	in ;
	
	if ( event -> button == 1 )
	{
		return false ;
	}
	
	gint x = 0, y = 0 ;
	gdk_window_get_pointer ( gtk_tree_view_get_bin_window ( wimp -> browser ), & x, & y, null ) ;
	
	GtkTreePath * tree_path ;
	if ( gtk_tree_view_get_path_at_pos ( wimp -> browser, x, y, & tree_path, null, null, null ))
	{
		gtk_tree_selection_select_path ( gtk_tree_view_get_selection ( wimp -> browser ), tree_path ) ;
		gtk_tree_path_free ( tree_path ) ;
	}
	else
		gtk_tree_selection_unselect_all ( gtk_tree_view_get_selection ( wimp -> browser )) ;
	
	GtkMenu * menu = wimp_file_manager_menu_create ( wimp ) ;
	g_signal_connect_swapped ( menu, "selection-done", gtk_widget_destroy, menu ) ;
	
	gtk_menu_popup ( menu, null, null, null, null, null, gtk_get_current_event_time ()) ;
	out ;
	
	return true ;
}

void wimp_file_manager_properties ( WimpGUI * gui )
{
	in ;
	
	GList * list = null ;
	list = gtk_tree_selection_get_selected_rows ( gtk_tree_view_get_selection ( gui -> browser ), null ) ;
	
	gint i = 0, length = g_list_length ( list ) ;
	GString * string = g_string_new ( null ) ;
	
	for ( i = 0 ; i < length ; i ++ )
	{
		GtkTreeIter iter ;
		gtk_tree_model_get_iter ( gui -> model, & iter, g_list_nth_data ( list, i )) ;
		
		gchar * filename = null ;
		gtk_tree_model_get ( gui -> model, & iter, 0, & filename, -1 ) ;
		
		struct stat buf ;
		stat ( filename, & buf ) ;
		
		g_string_append_printf ( string,
			"%s :\n"
			"Size: %.2f MB\n\n",
			filename,
			( gfloat ) buf.st_size / 1000000.0 ) ;
		
		g_free ( filename ) ;
	}

	gchar * info = g_string_free ( string, no ) ;
	wimp_message_box_detailed_info ( info ) ;
	
	g_free ( info ) ;
	
	g_list_foreach ( list, gtk_tree_path_free, null ) ;
	g_list_free ( list ) ;

	out ;
}

gchar * wimp_file_manager_get_selected ( WimpGUI * gui )
{
	in ;
	
	GList * list = null ;
	list = gtk_tree_selection_get_selected_rows ( gtk_tree_view_get_selection ( gui -> browser ), null ) ;
	
	if ( list == null )
		reutern null ;
	
	GtkTreeIter iter ;
	gtk_tree_model_get_iter ( gui -> model, & iter, g_list_nth_data ( list, 0 )) ;
	
	gchar * filename = null ;
	gtk_tree_model_get ( gui -> model, & iter, 0, & filename, -1 ) ;
	
	g_list_foreach ( list, gtk_tree_path_free, null ) ;
	g_list_free ( list ) ;

	out ;
	return filename ;
}

void wimp_file_manager_open_with ( WimpGUI * gui )
{
	in ;
	
	GtkEntry * entry = gtk_entry_new () ;
	GtkDialog * dialog = gtk_dialog_new_with_buttons ( 	"Open with",
														null,
														GTK_DIALOG_MODAL,
														"Open",
														1,
														"Cancel",
														0,
														null ) ;
	
	GtkCheckButton * button = gtk_check_button_new_with_label ( "Run in terminal" ) ;
	GtkHBox * box = gtk_hbox_new ( no, 2 ) ;
	
	gtk_box_pack_start ( box, entry, yes, yes, 2 ) ;
	gtk_box_pack_start ( box, button, no, no, 2 ) ;
	
	gtk_box_pack_start ( dialog -> vbox, box, yes, yes, 2 ) ;
	gtk_widget_show_all ( dialog ) ;
	
	if ( ! gtk_dialog_run ( dialog ))
	{
		gtk_widget_destroy ( dialog ) ;
		return ;
	}
	
	gchar * filename = wimp_file_manager_get_selected ( gui ) ;
	gchar * app = gtk_entry_get_text ( entry ) ;
	
	if ( filename )
	{
		gchar * command = null ;
		if ( gtk_toggle_button_get_active ( button ))
			command = g_strdup_printf ( "xterm -e %s \"%s\" &", app, filename ) ;
		else
			command = g_strdup_printf ( "%s \"%s\" &", app, filename ) ;
		
		system ( command ) ;
		g_free ( command ) ;
	}
	
	g_free ( filename ) ;
	gtk_widget_destroy ( dialog ) ;
	
	out ;
}

void wimp_file_manager_refresh ( WimpGUI * gui )
{
	in ;
	gchar * parent = wimp_file_manager_get_parent ( gui ) ;
	
	wimp_file_manager_browse ( gui, parent ) ;
	g_free ( parent ) ;
	
	out ;
}
