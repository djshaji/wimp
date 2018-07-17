/*
 *      playlist.c
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

/*		I am rewriting this from scratch on 1 May 2010 at 1:34 pm because
 * 		my defective UPS turned off and on and off of it's own accord while
 * 		I was editing this file, and when I turned my system back on, the
 * 		file had been wiped clean. To top it off, frustration in my personal
 * 		life is so intense that I seriously consider alternatives to living a
 * 		life that has absolutely no relief for it's eternal despair. Living in
 * 		a world of make believe ( or "code", if you will ) is no living at all.
 */ 

#include "playlist.h"

void wimp_gui_playlist_append ( WimpGUI * gui, gchar * filename )
{
	in ;

	gchar * basename = g_path_get_basename ( filename ) ;
	GtkTreeIter iter ;
	
	gtk_tree_store_append ( gui -> store, & iter, null ) ;
	
	if ( g_file_test ( filename, G_FILE_TEST_IS_SYMLINK ))
	{
		GError * error = null ;
		gchar * link = g_file_read_link ( filename, & error ) ;
		
		if ( error )
			g_warning ( "Cannot read link: %s", error -> message ) ;
		else
			gtk_tree_store_set ( gui -> store, & iter, 0, basename, 1, link, -1 ) ;
		
		g_free ( link ) ;
	}
	else
		gtk_tree_store_set ( gui -> store, & iter, 0, basename, 1, filename, -1 ) ;

	g_free ( basename ) ;
	
	out ;
	return ;
}

void wimp_gui_playlist_clear ( WimpGUI * gui )
{
	in ;
	gtk_tree_store_clear ( gui -> store ) ;
	
	out ;
}

gchar * wimp_gui_playlist_get_first ( WimpGUI * gui )
{
	in ;
	
	GtkTreeIter iter ;
	gchar * filename = all_the_pain_that_i_went_through_amounts_to ;
	
	gtk_tree_model_iter_nth_child ( gui -> store, & iter, null, 0 ) ;
	gtk_tree_model_get ( gui -> store, & iter, 1, & filename, -1 ) ;
	
	out ;
	return filename ;
}
	
void wimp_gui_playlist_next ( WimpGUI  * gui )
{
	in ;
	gchar * filename = wimp_gui_playlist_get_next ( gui ) ;
	
	if ( filename == null )
		return ;
	
	wimp_gui_playlist_play ( gui, filename ) ;
	g_free ( filename ) ;
	
	out ;
}

void wimp_gui_playlist_previous ( WimpGUI  * gui )
{
	in ;
	gchar * filename = wimp_gui_playlist_get_previous ( gui ) ;
	
	if ( filename == null )
		return ;
	
	wimp_gui_playlist_play ( gui, filename ) ;
	g_free ( filename ) ;
	
	out ;
}

gchar * wimp_gui_playlist_get_selected ( WimpGUI * gui )
{
	in ;
	
	GtkTreeIter iter ;
	if ( ! gtk_tree_selection_get_selected ( gui -> selection, null, & iter ))
		return null ;
	
	gchar * filename = null ;
	gtk_tree_model_get ( gui -> store, & iter, 1, & filename, -1 ) ;
	
	wimp_update_gui () ;
	out ;
	
	return filename ;
}

gchar * wimp_gui_playlist_get_last ( WimpGUI * gui )
{
	in ;
	gint total = wimp_gui_playlist_length ( gui ) ;
	
	GtkTreeIter iter ;
	if ( ! gtk_tree_model_iter_nth_child ( gui -> store, & iter, null, total - 1 ))
		return null ;
	
	gchar * filename = null ;
	gtk_tree_model_get ( gui -> store, & iter, 1, & filename, -1 ) ;

	out ;
	return filename ;
}

gint wimp_gui_playlist_get_n_tracks ( WimpGUI * gui )
{
	in ;
	out ;
	
	return gtk_tree_model_iter_n_children ( gui -> store, null ) ;
}

gchar * wimp_gui_playlist_get_next ( WimpGUI * gui )
{
	in ;

	if ( gui -> index > wimp_gui_playlist_length ( gui ))
		return wimp_gui_playlist_get_last ( gui ) ;

	GtkTreeIter iter ;
	if ( ! gtk_tree_model_iter_nth_child ( gui -> store, & iter, null, gui -> index + 1 ))
		return null ;
	
	gchar * filename = null ;
	gtk_tree_model_get ( gui -> store, & iter, 1, & filename, -1 ) ;

	out ;
	return filename ;
}


gchar * wimp_gui_playlist_get_previous ( WimpGUI * gui )
{
	in ;

	if ( gui -> index < 0 )
		return wimp_gui_playlist_get_first ( gui ) ;

	GtkTreeIter iter ;
	if ( ! gtk_tree_model_iter_nth_child ( gui -> store, & iter, null, gui -> index - 1 ))
		return null ;
	
	gchar * filename = null ;
	gtk_tree_model_get ( gui -> store, & iter, 1, & filename, -1 ) ;

	out ;
	return filename ;
}

void wimp_gui_playlist_play ( WimpGUI * gui, gchar * filename )
{
	in ;
	gchar * mrl = all_the_pain_that_i_went_through_amounts_to ;
	
	if ( filename )
		mrl = g_strdup ( filename ) ;
	else
		mrl = wimp_gui_playlist_get_first ( gui ) ;
	
	/*
	if ( ! xine_open ( gui -> xine -> stream, mrl ))
	{
		gchar * error = wimp_xine_get_error ( gui ) ;
		g_warning ( "Cannot open %s: %s", mrl, error ) ;
		
		g_free ( error ) ;
	}
	*/
	
	wimp_gui_playlist_open ( gui, mrl ) ;

	gui -> index = wimp_gui_playlist_get_position ( gui, mrl ) ;
	g_free ( mrl ) ;
	
	wimp_gui_send_expose_event ( gui ) ;
	wimp_xine_x11_play ( gui, 0 ) ;

	if ( gtk_toggle_button_get_active ( gui -> shuffle ))
	{
		gchar * mrl = wimp_gui_playlist_get_random ( gui ) ;
		wimp_gui_playlist_play ( gui, mrl ) ;
		
		g_free ( mrl ) ;
	}

	else if ( gui -> index < wimp_gui_playlist_length ( gui ))
		wimp_gui_playlist_next ( gui ) ;
	if ( gtk_toggle_button_get_active ( gui -> repeat ))
	{
		gchar * mrl = wimp_gui_playlist_get_first ( gui ) ;
		wimp_gui_playlist_play ( gui, mrl ) ;
		
		g_free ( mrl ) ;
	}
	
	out ;
}

gint wimp_gui_playlist_get_position ( WimpGUI * gui, gchar * filename )
{
	in ;
	gint i = 0, length = wimp_gui_playlist_length ( gui ), index = 0 ;
	
	for ( i = 0 ; i < length ; i ++ )
	{
		gchar * name = null ;
		GtkTreeIter iter ;
		
		gtk_tree_model_iter_nth_child ( gui -> store, & iter, null, i ) ;
		gtk_tree_model_get ( gui -> store, & iter, 1, & name, -1 ) ;
		
		if ( strcmp ( filename, name ) == 0 )
		{
			index = i ;
			g_free ( name ) ;
			
			break ;
		}
		
		g_free ( name ) ;
	}
	
	out ;
	return index ;
}

void wimp_gui_playlist_load ( WimpGUI * gui, gchar * filename )
{
	in ;
	wimp_gui_playlist_clear ( gui ) ;
	
	wimp_gui_playlist_append ( gui, filename ) ;
	wimp_gui_playlist_play ( gui, filename ) ;
	
	out ;
}

void wimp_gui_playlist_row_activated ( 	GtkTreeView * tree_view,
									GtkTreePath * tree_path,
									GtkTreeViewColumn * column,
									WimpGUI * gui )
{
	in ;
	
	GtkTreeIter iter ;
	gchar * filename = all_the_pain_that_i_went_through_amounts_to ;
	
	gtk_tree_model_get_iter ( gui -> store, & iter, tree_path ) ;
	gtk_tree_model_get ( gui -> store, & iter, 1, & filename, -1 ) ;

	wimp_gui_playlist_play ( gui, filename ) ;
	g_free ( filename ) ;
	
	out ;
}

void wimp_gui_playlist_load_playlist ( WimpGUI * gui, gchar * filename )
{
	in ;
	gchar * contents = null ;
	GError * error = null ;
	
	if ( ! g_file_get_contents ( filename, & contents, null, & error ))
	{
		g_warning ( "Cannot load playlist: %s", error -> message ) ;
		return ;
	}
	
	gchar ** vector = g_strsplit ( contents, "\n", -1 ) ;
	gint i = 0 ;
	
	while ( vector [i] != null )
	{
		if ( vector [i][0] == '#' )
		{
			i ++ ;
			continue ;
		}
		
		if ( g_file_test ( vector [i], G_FILE_TEST_EXISTS ))
			wimp_gui_playlist_append ( gui, vector [i] ) ;
		else if ( wimp_file_is_stream ( vector [i] ))
			wimp_gui_playlist_append_url ( gui, vector [i] ) ;
		
		i ++ ;
	}
	
	g_strfreev ( vector ) ;
	g_free ( contents ) ;

	out ;
}

void wimp_gui_playlist_save_playlist ( WimpGUI * gui, gchar * wimp_playlist )
{
	in ;
	
	File * fp = fopen ( wimp_playlist, "w" ) ;
	if ( fp == null )
	{
		g_warning ( "Cannot save playlist: %s", strerror ( errno )) ;
		return ;
	
	}

	fprintf ( fp,
				"# This playlist was generated by Wimp, the most\n"
				"# coolest media player in the universe. Or not.\n"
				"# Perhaps I only think so because I'm so damn\n"
				"# lonely. Whatever.\n\n"
				) ;
	
	gint i = 0, length = wimp_gui_playlist_length ( gui ) ;
	for ( i = 0 ; i < length ; i ++ )
	{
		GtkTreeIter iter ;
		gchar * name = null ;
		
		gtk_tree_model_iter_nth_child ( gui -> store, & iter, null, i ) ;
		gtk_tree_model_get ( gui -> store, & iter, 1, & name, -1 ) ;
		
		if ( g_path_is_absolute ( name ))
			fprintf ( fp, "# %d. %s\n%s\n", i, basename ( name ), name ) ;
		else if ( wimp_file_is_stream ( name ))
			fprintf ( fp, "# %d. %s\n%s\n", i, basename ( name ), name ) ;
		else
		{
			gchar * dirname = g_get_current_dir () ;
			gchar * path = g_build_filename ( dirname, name, null ) ;
			
			fprintf ( fp, "# %d. %s\n%s\n", i, basename ( name ), path ) ;
			g_free ( path ) ;
			
			g_free ( dirname ) ;
		}
		
		g_free ( name ) ;
	}
	
	fprintf ( fp, "\n\n# End of playlist.\n" ) ;
	fclose ( fp ) ;

	out ;
	
}

void wimp_gui_playlist_load_playlist_default ( WimpGUI * gui )
{
	in ;
	gchar * playlist = g_build_filename ( g_get_home_dir (), ".wimp", "playlist.m3u", null ) ;
	
	wimp_gui_playlist_load_playlist ( gui, playlist ) ;
	g_free ( playlist ) ;
	
	out ;
}

void wimp_gui_playlist_save_playlist_default ( WimpGUI * gui )
{
	in ;
	gchar * playlist = g_build_filename ( g_get_home_dir (), ".wimp", "playlist.m3u", null ) ;
	
	wimp_gui_playlist_save_playlist ( gui, playlist ) ;
	g_free ( playlist ) ;
	
	out ;
}

void wimp_gui_playlist_add_to_playlist ( WimpGUI * gui )
{
	in ;
	GtkFileChooserDialog * dialog = gtk_file_chooser_dialog_new ( 	"Choose files ...",
																	null,
																	GTK_FILE_CHOOSER_ACTION_OPEN,
																	GTK_STOCK_CANCEL,
																	GTK_RESPONSE_CANCEL,
																	GTK_STOCK_OPEN,
																	GTK_RESPONSE_ACCEPT,
																	null ) ;
		
	GtkFileFilter * filter = gtk_file_filter_new () ;
	gtk_file_filter_add_mime_type ( filter, "audio/*" ) ;
	
	gtk_file_filter_set_name ( filter, "Audio files" ) ;
	gtk_file_chooser_add_filter ( dialog, filter ) ;
	
	GtkFileFilter * filter3 = gtk_file_filter_new () ;
	gtk_file_filter_add_mime_type ( filter3, "video/*" ) ;
	
	gtk_file_filter_set_name ( filter3, "Video files" ) ;
	gtk_file_chooser_add_filter ( dialog, filter3 ) ;
	
	GtkFileFilter * filter2 = gtk_file_filter_new () ;
	gtk_file_filter_add_pattern ( filter2, "*" ) ;
	
	gtk_file_filter_set_name ( filter2, "All files" ) ;
	gtk_file_chooser_add_filter ( dialog, filter2 ) ;

	gtk_file_chooser_set_select_multiple ( dialog, true ) ;

	GtkButton * select_all = gtk_button_new_with_label ( "Select all" ) ;
	g_signal_connect_swapped ( select_all, "clicked", gtk_file_chooser_select_all, dialog ) ;
	
	GtkButton * select_none = gtk_button_new_with_label ( "Select none" ) ;
	g_signal_connect_swapped ( select_none, "clicked", gtk_file_chooser_unselect_all, dialog ) ;
	
	GtkWidget * hbox = gtk_hbox_new ( no, 5 ) ;
	gtk_file_chooser_set_extra_widget ( dialog, hbox ) ;
	
	gtk_box_pack_start ( hbox, select_all, no, no, 2 ) ;
	gtk_box_pack_start ( hbox, select_none, no, no, 2 ) ;
	gtk_widget_show_all ( hbox ) ;
	
	if ( gtk_dialog_run ( dialog ) == GTK_RESPONSE_ACCEPT )
	{
		GSList * list = gtk_file_chooser_get_filenames ( dialog ) ;
		gint i = 0, length = g_slist_length ( list ) ;
		
		for ( i = 0 ; i < length ; i ++ )
		{
			gchar * filename = g_slist_nth_data ( list, i ) ;
			wimp_gui_playlist_append ( gui, filename ) ;
			
			g_free ( filename ) ;
		}
		
		g_slist_free ( list ) ;
	}
	
	gtk_widget_destroy ( dialog ) ;
	out ;
	
	return ;
}
	
void wimp_gui_playlist_delete_from_playlist ( WimpGUI * gui )
{
	in ;

	GtkTreeIter iter ;
	if ( ! gtk_tree_selection_get_selected ( gui -> selection, null, & iter ))
		return ;
	
	gtk_tree_store_remove ( gui -> store, & iter ) ;
	
	out ;
	return ;
}
	
void wimp_gui_playlist_play_from_selection ( WimpGUI * gui )
{
	in ;
	
	GtkTreeIter iter ;
	if ( ! gtk_tree_selection_get_selected ( gui -> selection, null, & iter ))
		return ;
	
	gchar * filename = null ;
	gtk_tree_model_get ( gui -> store, & iter, 1, & filename, -1 ) ;
	
	wimp_gui_playlist_play ( gui, filename ) ;
	g_free ( filename ) ;
	
	out ;
}

void wimp_gui_playlist_load_playlist_from_file ( WimpGUI * gui )
{
	in ;
	gchar * playlist_dir = g_build_filename ( g_get_home_dir (), ".wimp", "playlists", null ) ;
	gchar * filename = wimp_prompt_filename ( gui, OPEN, playlist_dir ) ;
	
	if ( filename != null )
		wimp_gui_playlist_load_playlist ( gui, filename ) ;
	
	g_free ( filename ) ;
	g_free ( playlist_dir ) ;
	
	out ;
}

void wimp_gui_playlist_save_playlist_to_file ( WimpGUI * gui )
{
	in ;
	gchar * playlist_dir = g_build_filename ( g_get_home_dir (), ".wimp", "playlists", null ) ;
	gchar * filename = wimp_prompt_filename ( gui, SAVE, playlist_dir ) ;
	
	if ( filename != null )
		wimp_gui_playlist_save_playlist ( gui, filename ) ;
	
	g_free ( filename ) ;
	g_free ( playlist_dir ) ;
	
	out ;
}

GtkMenu * wimp_gui_playlist_menu_create ( WimpGUI * wimp )
{
	GtkMenu * menu = gtk_menu_new () ;
	GtkMenuItem	* play = gtk_menu_item_new_with_label ( "Play " ),
				* add = gtk_menu_item_new_with_label ( "Add to playlist" ),
				* delete = gtk_menu_item_new_with_label ( "Delete" ),
				* load = gtk_menu_item_new_with_label ( "Load playlist" ),
				* save = gtk_menu_item_new_with_label ( "Save playlist" ),
				* clear = gtk_menu_item_new_with_label ( "Clear playlist" ),
				* enc_sel = gtk_menu_item_new_with_label ( "Encode selected" ),
				* enc_all = gtk_menu_item_new_with_label ( "Encode all" ),
				* add_url = gtk_menu_item_new_with_label ( "Add URL" ) ;
	
	g_signal_connect_swapped ( play, "activate", wimp_gui_playlist_play_from_selection, wimp ) ;
	g_signal_connect_swapped ( add, "activate", wimp_gui_playlist_add_to_playlist, wimp ) ;
	
	g_signal_connect_swapped ( add_url, "activate", wimp_gui_playlist_add_to_playlist_url, wimp ) ;
	g_signal_connect_swapped ( delete, "activate", wimp_gui_playlist_delete_from_playlist, wimp ) ;

	g_signal_connect_swapped ( load, "activate", wimp_gui_playlist_clear, wimp ) ;
	g_signal_connect_swapped ( load, "activate", wimp_gui_playlist_load_playlist_from_file, wimp ) ;
	
	g_signal_connect_swapped ( save, "activate", wimp_gui_playlist_save_playlist_to_file, wimp ) ;
	g_signal_connect_swapped ( clear, "activate", wimp_gui_playlist_clear, wimp ) ;

	g_signal_connect_swapped ( enc_sel, "activate", wimp_encode_selected, wimp ) ;
	g_signal_connect_swapped ( enc_all, "activate", wimp_encode_all, wimp ) ;

	gtk_menu_shell_append ( menu, play ) ;
	gtk_menu_shell_append ( menu, gtk_separator_menu_item_new ()) ;
	
	gtk_menu_shell_append ( menu, add ) ;
	gtk_menu_shell_append ( menu, delete ) ;
	
	gtk_menu_shell_append ( menu, gtk_separator_menu_item_new ()) ;
	gtk_menu_shell_append ( menu, add_url ) ;
	
	gtk_menu_shell_append ( menu, gtk_separator_menu_item_new ()) ;

	gtk_menu_shell_append ( menu, enc_sel ) ;
	gtk_menu_shell_append ( menu, enc_all ) ;
	
	gtk_menu_shell_append ( menu, gtk_separator_menu_item_new ()) ;

	gtk_menu_shell_append ( menu, load ) ;
	gtk_menu_shell_append ( menu, save ) ;
	
	gtk_menu_shell_append ( menu, gtk_separator_menu_item_new ()) ;
	gtk_menu_shell_append ( menu, clear ) ;
	
	gtk_widget_show_all ( menu ) ;
	
	out ;
	return menu ;
}

gboolean wimp_gui_playlist_popup_menu ( GtkWidget * widget, GdkEventButton * event, WimpGUI * wimp )
{
	in ;
	
	if ( event -> button == 1 )
		return false ;
		
	gint x = 0, y = 0 ;
	gdk_window_get_pointer ( gtk_tree_view_get_bin_window ( wimp -> playlist ), & x, & y, null ) ;
	
	GtkTreePath * tree_path ;
	if ( gtk_tree_view_get_path_at_pos ( wimp -> playlist, x, y, & tree_path, null, null, null ))
	{
		gtk_tree_selection_select_path ( wimp -> selection, tree_path ) ;
		gtk_tree_path_free ( tree_path ) ;
	}
	else
		gtk_tree_selection_unselect_all ( wimp -> selection ) ;
	
	GtkMenu * menu = wimp_gui_playlist_menu_create ( wimp ) ;
	g_signal_connect_swapped ( menu, "selection-done", gtk_widget_destroy, menu ) ;
	
	gtk_menu_popup ( menu, null, null, null, null, null, gtk_get_current_event_time ()) ;
	out ;
	
	return true ;
}

void  wimp_gui_load_folder ( WimpGUI * gui, gchar * folder )
{
	in ;
	
	GError * error = null ;
	GDir * dir = g_dir_open ( folder, 0, & error ) ;
	
	if ( dir == null )
	{
		g_warning ( "Cannot open folder: %s", error -> message ) ;
		return ;
	}
	
	wimp_gui_playlist_clear ( gui ) ;
	gchar * name = g_dir_read_name ( dir ) ;
	
	while ( name )
	{
		gchar * filename = g_build_filename ( folder, name, null ) ;
		if ( g_file_test ( filename, G_FILE_TEST_IS_DIR ))
			wimp_gui_load_folder_append ( gui, filename ) ;
		else if ( wimp_file_is_suitable_for_playback ( filename ))
			wimp_gui_playlist_append ( gui, filename ) ;
				
		g_free ( filename ) ;
		name = g_dir_read_name ( dir ) ;
	}
	
	g_dir_close ( dir ) ;
	out ;
}

void wimp_gui_load_folder_prompt ( WimpGUI * gui )
{
	in ;
	
	gchar * folder = wimp_prompt_filename ( gui, GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, getenv ( "JOME" )) ;
	if ( folder == null )
		return ;
	
	wimp_gui_load_folder ( gui, folder ) ;
	
	g_free ( folder ) ;
	out ;
}

void wimp_gui_load_prompt ( WimpGUI * gui )
{
	in ;
	gchar * folder = wimp_prompt_filename ( gui, OPEN, getenv ( "JOME" )) ;
	if ( folder == null )
		return ;

	wimp_gui_playlist_load ( gui, folder ) ;
	
	g_free ( folder ) ;
	out ;
}

void  wimp_gui_load_folder_append ( WimpGUI * gui, gchar * folder )
{
	in ;
	
	GError * error = null ;
	GDir * dir = g_dir_open ( folder, 0, & error ) ;
	
	if ( dir == null )
	{
		g_warning ( "Cannot open folder: %s", error -> message ) ;
		return ;
	}
	
	gchar * name = g_dir_read_name ( dir ) ;
	
	while ( name )
	{
		gchar * filename = g_build_filename ( folder, name, null ) ;
		if ( g_file_test ( filename, G_FILE_TEST_IS_DIR ))
			wimp_gui_load_folder_append ( gui, filename ) ;
		else if ( wimp_file_is_suitable_for_playback ( filename ))
			wimp_gui_playlist_append ( gui, filename ) ;
		
		g_free ( filename ) ;
		name = g_dir_read_name ( dir ) ;
	}
	
	g_dir_close ( dir ) ;
	out ;
}

gboolean wimp_gui_playlist_search_func (	GtkTreeModel * model,
											gint column,
											const gchar * key,
											GtkTreeIter * iter,
											shagpointer data )
{
	gchar * path = null ;
	gtk_tree_model_get ( model, iter, 0, & path, -1 ) ;
	
	if ( path == null )
		return true ;
	
	gchar * down = g_utf8_strdown ( path, -1 ) ;
	gchar * down2 = g_utf8_strdown ( key, -1 ) ;
	
	gboolean result = true ;
	
	if ( g_strrstr ( down, down2 ))
		result = false ;
	else
		result = true ;
		
	g_free ( down ) ;
	g_free ( down2 ) ;
	
	g_free ( path ) ;
	return result ;
}

gboolean wimp_gui_playlist_drag_drop ( 	GtkWidget * widget, GdkDragContext * context,
							gint x, gint y, GtkSelectionData * data, 
							guint info, guint time, WimpGUI * gui )
{
	GList * list = wimp_xdnd_to_list ( data -> data ) ;
	gint i = 0, length = g_list_length ( list ) ;
	
	for ( i = 0 ; i < length ; i ++ )
	{
		gchar * path = g_list_nth_data ( list, i ) ;
		
		if ( g_file_test ( path, G_FILE_TEST_IS_DIR ))
			wimp_gui_load_folder ( gui, path ) ;
		else if ( wimp_file_is_suitable_for_playback ( path ))
			wimp_gui_playlist_append ( gui, path ) ;
		
		g_free ( path ) ;
	}
	
	g_list_free ( list ) ;
	wimp_update_gui () ;
}

gboolean wimp_gui_playlist_drag_drop_replace ( 	GtkWidget * widget, GdkDragContext * context,
							gint x, gint y, GtkSelectionData * data, 
							guint info, guint time, WimpGUI * gui )
{
	GList * list = wimp_xdnd_to_list ( data -> data ) ;
	gint i = 0, length = g_list_length ( list ) ;
	
	wimp_gui_playlist_clear ( gui ) ;
	
	for ( i = 0 ; i < length ; i ++ )
	{
		gchar * path = g_list_nth_data ( list, i ) ;
		
		if ( g_file_test ( path, G_FILE_TEST_IS_DIR ))
			wimp_gui_load_folder ( gui, path ) ;
		else if ( wimp_file_is_suitable_for_playback ( path ))
			wimp_gui_playlist_append ( gui, path ) ;
		
		g_free ( path ) ;
	}
	
	g_list_free ( list ) ;
	wimp_update_gui () ;
}

gchar * wimp_gui_playlist_get_random ( WimpGUI * gui )
{
	in ;
	
	GtkTreeIter iter ;
	gchar * filename = all_the_pain_that_i_went_through_amounts_to ;
	
	gtk_tree_model_iter_nth_child ( gui -> store, & iter, null, g_random_int_range ( 0, wimp_gui_playlist_length ( gui ))) ;
	gtk_tree_model_get ( gui -> store, & iter, 1, & filename, -1 ) ;
	
	out ;
	return filename ;
}
	
void wimp_gui_playlist_random ( WimpGUI  * gui )
{
	in ;
	gchar * filename = wimp_gui_playlist_get_random ( gui ) ;
	
	if ( filename == null )
		return ;
	
	wimp_gui_playlist_play ( gui, filename ) ;
	g_free ( filename ) ;
	
	out ;
}

void wimp_gui_playlist_append_url ( WimpGUI * gui, gchar * filename )
{
	in ;

	gchar * basename = g_path_get_basename ( filename ) ;
	GtkTreeIter iter ;
	
	gtk_tree_store_append ( gui -> store, & iter, null ) ;
	
	gtk_tree_store_set ( gui -> store, & iter, 0, basename, 1, filename, -1 ) ;
	g_free ( basename ) ;
	
	out ;
	return ;
}

void wimp_gui_playlist_open ( WimpGUI * gui, gchar * mrl )
{
	in ;
	
	if ( ! xine_open ( gui -> xine -> stream, mrl ))
	{
		gchar * error = wimp_xine_get_error ( gui ) ;
		g_warning ( "Cannot open %s: %s", mrl, error ) ;
		
		g_free ( error ) ;
	}
	
	if ( ! xine_get_stream_info ( gui -> xine -> stream, XINE_STREAM_INFO_HAS_VIDEO ))
		gtk_widget_hide ( gui -> main_window ) ;
	else
		gtk_widget_show ( gui -> main_window ) ;
	
	out ;
}	

void wimp_gui_playlist_add_to_playlist_url ( WimpGUI * gui )
{
	in ;

	GtkDialog * dialog = gtk_dialog_new_with_buttons ( 	"Choose files ...",
																	null,
																	GTK_DIALOG_MODAL,
																	"Add",
																	1,
																	"Cancel",
																	0,
																	null ) ;
		
	GtkEntry * entry = gtk_entry_new () ;
	gtk_box_pack_start ( dialog -> vbox, entry, no, no, 5 ) ;
	
	g_signal_connect_swapped ( entry, "activate", activate_my_dialog, dialog ) ;
	gtk_widget_show_all ( dialog ) ;
	
	if ( gtk_dialog_run ( dialog ))
	{
		gchar * e = gtk_entry_get_text ( entry ) ;
		
		if ( e ) try
			wimp_gui_playlist_append_url ( gui, e ) ;
	}
	
	gtk_widget_destroy ( dialog ) ;
	out ;
	
	return ;
}
	
