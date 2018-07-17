/*
 *      encode.c
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


#include "encode.h"

void wimp_encode_file_gui ( gchar * command, GtkStatusbar * status, GtkProgress * bar )
{
	in ;

	gint pid = 0, stdin = 0, stdout = 0, stderr = 0 ;
	GError * error = null ;
	
	gchar ** vector = g_strsplit ( command, ";", -1 ) ;
	gboolean result = false ;
	
	result = g_spawn_async_with_pipes ( null, vector, null, G_SPAWN_SEARCH_PATH,
										null, null, & pid, & stdin, & stdout,
										& stderr, & error ) ;
	
	if ( error )
		g_warning ( "Cannot spawn process: %s", error -> message ) ;
	
	File *fd = fdopen ( stderr, "r" ) ;
	if ( fd == null )
		g_warning ( "Cannot open file descriptor: %s", strerror ( errno )) ;
	
	gchar ch = fgetc ( fd ) ;
	wimp_update_gui () ;
	
	gchar * proc = g_strdup_printf ( "/proc/%d", pid ) ;
	while ( g_file_test ( proc, G_FILE_TEST_EXISTS ))
	{
		fflush ( fd ) ;
		
		GString * string = g_string_new ( null ) ;
		while ( ch != 13 )
		{
			g_string_append_c ( string, ch ) ;
			ch = getc ( fd ) ;
			
			if ( ch == EOF )
				break ;
		}
		
		gtk_statusbar_push ( status, gtk_statusbar_get_context_id ( status, string -> str ), string -> str ) ;
		g_string_free ( string, on ) ;
		
		gtk_progress_bar_pulse ( bar ) ;
		
		//g_printf ( "%s", message ) ;
		//g_free ( message1 ) ;
		
		wimp_update_gui () ;
		g_usleep ( 10000 ) ;
		
		ch = getc ( fd ) ;
		if ( ch == EOF )
			break ;
		//fflush ( fd ) ;
		//fgets ( & message, 32, fd ) ;
		//fgets ( & message, 32, fd ) ;
	}

	g_free ( proc ) ;
	
	g_strfreev ( vector ) ;
	fclose ( fd ) ;
	
	g_spawn_close_pid ( pid ) ;
	return result ;
}
	
void wimp_encode_selected ( WimpGUI * gui )
{
	in ;
	
	GtkTreeSelection * selection = gui -> selection ;
	GtkTreeModel * model = gui -> store ;
	
	GList * list = gtk_tree_selection_get_selected_rows ( selection, null ) ;
	gint i = 0, length = g_list_length ( list ) ;
	
	if ( list == null )
	{
		g_message ( "No file selected. Select a file first." ) ;
		return ;
	}
	
	GtkDialog * dialog = gtk_dialog_new_with_buttons (	"WiMP Encoder",
														null, //gui -> window,
														GTK_DIALOG_MODAL,
														null,
														0,
														null ) ;
	
	GtkTable * table = gtk_table_new ( 1, 1, no ) ;
	GtkComboBoxEntry * acodec = gtk_combo_box_entry_new_text (),
					 * vcodec = gtk_combo_box_entry_new_text (),
					 * s = gtk_combo_box_entry_new_text (),
					 * container = gtk_combo_box_entry_new_text (),
					 * preset = gtk_combo_box_new_text () ;
	
	GtkSpinButton * b = gtk_spin_button_new_with_range ( 0.0, 10000.0, 1.0 ),
				  * ab = gtk_spin_button_new_with_range ( 0.0, 10000.0, 0.1 ),
				  * ar = gtk_spin_button_new_with_range ( 8000, 192000.0, 1000 ),
				  * r = gtk_spin_button_new_with_range ( 0.0, 1000.0, 1.0 ) ;

	gint d = 0 ;
	while ( video_codecs [d] != null )
	{
		gtk_combo_box_append_text ( vcodec, video_codecs [d] ) ;
		d ++ ;
	}
	
	d = 0 ;
	while ( audio_codecs [d] != null )
	{
		gtk_combo_box_append_text ( acodec, audio_codecs [d] ) ;
		d ++ ;
	}
	
	d = 0 ;
	while ( containers [d] != null )
	{
		gtk_combo_box_append_text ( container, containers [d] ) ;
		d ++ ;
	}
	
	d = 0 ;
	while ( sz [d] != null )
	{
		gtk_combo_box_append_text ( s, sz [d] ) ;
		d ++ ;
	}
	
	
	gtk_table_attach ( table, gtk_label_new ( "Video codec" ), 0, 1, 0, 1, GTK_SHRINK, GTK_SHRINK, 2, 2 ) ;
	gtk_table_attach ( table, vcodec, 2, 3, 0, 1, GTK_FILL, GTK_FILL, 2, 5 ) ;
	
	gtk_table_attach ( table, gtk_label_new ( "Audio codec" ), 0, 1, 2, 3, GTK_SHRINK, GTK_SHRINK, 2, 5 ) ;
	gtk_table_attach ( table, acodec, 2, 3, 2, 3, GTK_FILL, GTK_FILL, 2, 5 ) ;
	
	gtk_table_attach ( table, gtk_label_new ( "Video bitrate" ), 4, 5, 0, 1, GTK_SHRINK, GTK_SHRINK, 10, 2 ) ;
	gtk_table_attach ( table, b, 6, 7, 0, 1, GTK_FILL, GTK_FILL, 2, 5 ) ;
	
	gtk_table_attach ( table, gtk_label_new ( "Audio bitrate" ), 4, 5, 2, 3, GTK_SHRINK, GTK_SHRINK, 10, 5 ) ;
	gtk_table_attach ( table, ab, 6, 7, 2, 3, GTK_FILL, GTK_FILL, 2, 5 ) ;
	
	gtk_table_attach ( table, gtk_label_new ( "Frames / second" ), 0, 1, 4, 5, GTK_SHRINK, GTK_SHRINK, 10, 5 ) ;
	gtk_table_attach ( table, r, 2, 3, 4, 5, GTK_FILL, GTK_FILL, 2, 5 ) ;
	
	gtk_table_attach ( table, gtk_label_new ( "Sample rate" ), 0, 1, 6, 7, GTK_SHRINK, GTK_SHRINK, 10, 5 ) ;
	gtk_table_attach ( table, ar, 2, 3, 6, 7, GTK_FILL, GTK_FILL, 2, 5 ) ;
	
	gtk_table_attach ( table, gtk_label_new ( "Video size" ), 4, 5, 4, 5, GTK_SHRINK, GTK_SHRINK, 10, 5 ) ;
	gtk_table_attach ( table, s, 6, 7, 4, 5, GTK_FILL, GTK_FILL, 2, 5 ) ;

	gtk_table_attach ( table, gtk_label_new ( "File type" ), 4, 5, 6, 7, GTK_SHRINK, GTK_SHRINK, 10, 5 ) ;
	gtk_table_attach ( table, container, 6, 7, 6, 7, GTK_FILL, GTK_FILL, 2, 5 ) ;

	GtkFileChooserButton * button = gtk_file_chooser_button_new ( "Select folder", GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER ) ;
	gtk_box_pack_start ( dialog -> vbox, table, yes, yes, 2 ) ;
	
	gtk_table_attach ( table, gtk_label_new ( "Output folder" ), 0, 1, 8, 9, GTK_SHRINK, GTK_SHRINK, 10, 5 ) ;
	gtk_table_attach ( table, button, 2, 3, 8, 9, GTK_FILL, GTK_FILL, 2, 5 ) ;
	
	gtk_table_attach ( table, gtk_label_new ( "Preset" ), 4, 5, 8, 9, GTK_SHRINK, GTK_SHRINK, 10, 5 ) ;
	gtk_table_attach ( table, preset, 6, 7, 8, 9, GTK_FILL, GTK_FILL, 2, 5 ) ;
	
	Encoder encoder ;
	encoder.vcodec = vcodec ;
	encoder.acodec = acodec ;
	
	d = 0 ;
	while ( preset_names [d] != null )
	{
		gtk_combo_box_append_text ( preset, preset_names [d] ) ;
		d ++ ;
	}
	
	encoder.b = b ;
	encoder.ab = ab ;
	
	encoder.container = container ;
	encoder.s = s ;
	
	encoder.r = r ;
	encoder.ar = ar ;
	
	g_signal_connect ( preset, "changed", wimp_encode_set_preset, & encoder ) ;
	gtk_combo_box_set_active ( preset, 0 ) ;
	
	GtkButton * encode = gtk_dialog_add_button ( dialog, "Encode", 1 ) ;
	GtkButton * close = gtk_dialog_add_button ( dialog, "Close", 0 ) ;
	
	GtkProgressBar * bar = gtk_progress_bar_new () ;
	GtkProgressBar * bar1 = gtk_progress_bar_new () ;

	gtk_table_attach ( table, bar, 0, 3, 10, 11, GTK_FILL, GTK_FILL, 10, 5 ) ;
	gtk_table_attach ( table, bar1, 4, 7, 10, 11, GTK_FILL, GTK_FILL, 2, 5 ) ;

	GtkStatusbar * status = gtk_statusbar_new () ;
	gtk_statusbar_set_has_resize_grip ( status, no ) ;
	
	gtk_table_attach ( table, status, 0, 7, 12, 13, GTK_FILL, GTK_SHRINK, 2, 5 ) ;
	gtk_widget_show_all ( dialog ) ;
	
	if ( gtk_dialog_run ( dialog ) == 0 )
	{
		g_list_foreach ( list, gtk_tree_path_free, null ) ;
		g_list_free ( list ) ;
		
		gtk_widget_destroy ( dialog ) ;
		return ;
	}
	
	gtk_button_set_label ( encode, "Stop" ) ;
	g_signal_connect ( encode, "clicked", wimp_encode_stop, null ) ;

	GList * flies = null, * encoded_flies = null ;
	gtk_widget_destroy ( close ) ;
	
	for ( i = 0 ; i < length ; i ++ )
	{
		GtkTreeIter iter ;
		gtk_tree_model_get_iter ( model, & iter, g_list_nth_data ( list, i )) ;
		
		gchar * name = null ;
		gtk_tree_model_get ( model, & iter, 1, & name, -1 ) ;
		
		if ( name == null )
			continue ;
		
		gchar * ext = gtk_combo_box_get_active_text ( container ),
			  * vc = gtk_combo_box_get_active_text ( vcodec ),
			  * ac = gtk_combo_box_get_active_text ( acodec ),
			  * size = gtk_combo_box_get_active_text ( s ) ;
		
		gchar * folder = gtk_file_chooser_get_filename ( button ) ;
		
		GString * string = g_string_new ( "ffmpeg;-y;-i;" ) ;
		g_string_append_printf ( string, "%s;", name ) ;
		
		g_string_append_printf ( string,
			"-vcodec;%s;-acodec;%s;-s;%s;-b;%dk;-ab;%fk;-r;%d;-ar;%d;"
			"%s/%s.%s",
			vc,
			ac,
			size,
			gtk_spin_button_get_value_as_int ( b ),
			gtk_spin_button_get_value ( ab ),
			gtk_spin_button_get_value_as_int ( r ),
			gtk_spin_button_get_value_as_int ( ar ),
			folder,
			basename ( name ),
			ext ) ;
		
		flies = g_list_append ( flies, g_strdup ( name )) ;
		encoded_flies = g_list_append ( encoded_flies, g_strdup_printf ( "%s/%s.%s", folder, basename ( name ), ext )) ;
			
		g_free ( vc ) ;
		g_free ( ac ) ;
		g_free ( size ) ;
		g_free ( ext ) ;

		g_free ( name ) ;
		g_free ( folder ) ;
		
		gchar * cmdline = g_string_free ( string, no ) ;
		g_debug ( "%s", cmdline ) ;
		
		gchar * text = g_strdup_printf ( "%d of %d", i + 1, length ) ;
		
		gtk_progress_bar_set_text ( bar1, text ) ;
		gtk_progress_bar_set_fraction ( bar1, (( gfloat ) ( i + 1 ) / ( gfloat ) length )) ;
		
		wimp_update_gui () ;
		g_free ( text ) ;
		
		wimp_encode_file_gui ( cmdline, status, bar ) ;
		g_free ( cmdline ) ;
	}
	
	g_list_foreach ( list, gtk_tree_path_free, null ) ;
	g_list_free ( list ) ;

	GString * string = g_string_new ( "Encoding results: \n\n" ) ;
	i = 0 ;
	
	for ( i = 0 ; i < g_list_length ( flies ) ; i ++ )
	{
		g_string_append_printf ( string, "%s -> %s\n",
			g_list_nth_data ( flies, i ),
			g_list_nth_data ( encoded_flies, i )) ;
		
	}

	gtk_widget_destroy ( table ) ;
	gtk_widget_destroy ( encode ) ;

	GtkButton * play = gtk_dialog_add_button ( dialog, "Play", 1 ) ;
	GtkButton * qu = gtk_dialog_add_button ( dialog, "Enqueue", 2 ) ;
	
	gtk_dialog_add_button ( dialog, "Close", 0 ) ;
	
	GtkTextBuffer * buffer = gtk_text_buffer_new ( null ) ;
	GtkTextIter iter ;
	
	gtk_text_buffer_get_iter_at_line ( buffer, & iter, 0 ) ;
	gtk_text_buffer_insert ( buffer, & iter, string -> str, -1 ) ;
	
	GtkTextView * view = gtk_text_view_new_with_buffer ( buffer ) ;
	GtkScrolledWindow * sw = gtk_scrolled_window_new ( null, null ) ;
	
	gtk_text_view_set_cursor_visible ( view, no ) ;
	gtk_container_add ( sw, view ) ;
	
	gtk_text_view_set_wrap_mode ( view, GTK_WRAP_WORD ) ;
	gtk_text_view_set_editable ( view, no ) ;
	
	gtk_box_pack_start ( dialog -> vbox, sw, yes, yes, 2 ) ;
	gtk_scrolled_window_set_policy ( sw, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC ) ;
	
	gtk_widget_show_all ( dialog ) ;
	
	switch ( gtk_dialog_run ( dialog ))
	{
		case 1:
			wimp_gui_playlist_clear ( gui ) ;
		case 2:
			for ( i = 0 ; i < g_list_length ( encoded_flies ) ; i ++ )
			{
				wimp_gui_playlist_append ( gui, g_list_nth_data ( encoded_flies, i )) ;
				//wimp_gui_playlist_play ( gui, g_list_nth_data ( encoded_flies, 0 )) ;
			}
			break ;
		default:
			break ;
	}
	
	gtk_widget_destroy ( dialog ) ;
	
	g_string_free ( string, on ) ;
	g_list_foreach ( flies, g_free, null ) ;
	
	g_list_foreach ( encoded_flies, g_free, null ) ;
	g_list_free ( encoded_flies ) ;
	
	g_list_free ( flies ) ;
}

void wimp_encode_set_preset ( GtkWidget * widget, Encoder * encoder )
{
	in; 
	
	gint preset = gtk_combo_box_get_active ( widget ),
		 i = 0 ;
		 
	gtk_combo_box_set_active ( encoder -> vcodec, presets [preset][0] ) ;
	gtk_combo_box_set_active ( encoder -> acodec, presets [preset][1] ) ;

	gtk_combo_box_set_active ( encoder -> container, presets [preset][2] ) ;
	gtk_combo_box_set_active ( encoder -> s, presets [preset][3] ) ;

	gtk_spin_button_set_value ( encoder -> b, presets [preset][4] ) ;
	gtk_spin_button_set_value ( encoder -> ab, presets [preset][5] ) ;

	gtk_spin_button_set_value ( encoder -> r, presets [preset][6] ) ;
	gtk_spin_button_set_value ( encoder -> ar, presets [preset][7] ) ;

	out ;
}
	
void wimp_encode_stop ( void )
{
	system ( "killall ffmpeg" ) ;
}

void wimp_encode_all ( WimpGUI * gui )
{
	in ;
	
	GtkTreeSelection * selection = gui -> selection ;
	GtkTreeModel * model = gui -> store ;
	
	gint i = 0, length = wimp_gui_playlist_length ( gui ) ;
	
	GtkDialog * dialog = gtk_dialog_new_with_buttons (	"WiMP Encoder",
														null, //gui -> window,
														GTK_DIALOG_MODAL,
														null,
														0,
														null ) ;
	
	GtkTable * table = gtk_table_new ( 1, 1, no ) ;
	GtkComboBoxEntry * acodec = gtk_combo_box_entry_new_text (),
					 * vcodec = gtk_combo_box_entry_new_text (),
					 * s = gtk_combo_box_entry_new_text (),
					 * container = gtk_combo_box_entry_new_text (),
					 * preset = gtk_combo_box_new_text () ;
	
	GtkSpinButton * b = gtk_spin_button_new_with_range ( 0.0, 10000.0, 1.0 ),
				  * ab = gtk_spin_button_new_with_range ( 0.0, 10000.0, 0.1 ),
				  * ar = gtk_spin_button_new_with_range ( 8000, 192000.0, 1000 ),
				  * r = gtk_spin_button_new_with_range ( 0.0, 1000.0, 1.0 ) ;

	gint d = 0 ;
	while ( video_codecs [d] != null )
	{
		gtk_combo_box_append_text ( vcodec, video_codecs [d] ) ;
		d ++ ;
	}
	
	d = 0 ;
	while ( audio_codecs [d] != null )
	{
		gtk_combo_box_append_text ( acodec, audio_codecs [d] ) ;
		d ++ ;
	}
	
	d = 0 ;
	while ( containers [d] != null )
	{
		gtk_combo_box_append_text ( container, containers [d] ) ;
		d ++ ;
	}
	
	d = 0 ;
	while ( sz [d] != null )
	{
		gtk_combo_box_append_text ( s, sz [d] ) ;
		d ++ ;
	}
	
	
	gtk_table_attach ( table, gtk_label_new ( "Video codec" ), 0, 1, 0, 1, GTK_SHRINK, GTK_SHRINK, 2, 2 ) ;
	gtk_table_attach ( table, vcodec, 2, 3, 0, 1, GTK_FILL, GTK_FILL, 2, 5 ) ;
	
	gtk_table_attach ( table, gtk_label_new ( "Audio codec" ), 0, 1, 2, 3, GTK_SHRINK, GTK_SHRINK, 2, 5 ) ;
	gtk_table_attach ( table, acodec, 2, 3, 2, 3, GTK_FILL, GTK_FILL, 2, 5 ) ;
	
	gtk_table_attach ( table, gtk_label_new ( "Video bitrate" ), 4, 5, 0, 1, GTK_SHRINK, GTK_SHRINK, 10, 2 ) ;
	gtk_table_attach ( table, b, 6, 7, 0, 1, GTK_FILL, GTK_FILL, 2, 5 ) ;
	
	gtk_table_attach ( table, gtk_label_new ( "Audio bitrate" ), 4, 5, 2, 3, GTK_SHRINK, GTK_SHRINK, 10, 5 ) ;
	gtk_table_attach ( table, ab, 6, 7, 2, 3, GTK_FILL, GTK_FILL, 2, 5 ) ;
	
	gtk_table_attach ( table, gtk_label_new ( "Frames / second" ), 0, 1, 4, 5, GTK_SHRINK, GTK_SHRINK, 10, 5 ) ;
	gtk_table_attach ( table, r, 2, 3, 4, 5, GTK_FILL, GTK_FILL, 2, 5 ) ;
	
	gtk_table_attach ( table, gtk_label_new ( "Sample rate" ), 0, 1, 6, 7, GTK_SHRINK, GTK_SHRINK, 10, 5 ) ;
	gtk_table_attach ( table, ar, 2, 3, 6, 7, GTK_FILL, GTK_FILL, 2, 5 ) ;
	
	gtk_table_attach ( table, gtk_label_new ( "Video size" ), 4, 5, 4, 5, GTK_SHRINK, GTK_SHRINK, 10, 5 ) ;
	gtk_table_attach ( table, s, 6, 7, 4, 5, GTK_FILL, GTK_FILL, 2, 5 ) ;

	gtk_table_attach ( table, gtk_label_new ( "File type" ), 4, 5, 6, 7, GTK_SHRINK, GTK_SHRINK, 10, 5 ) ;
	gtk_table_attach ( table, container, 6, 7, 6, 7, GTK_FILL, GTK_FILL, 2, 5 ) ;

	GtkFileChooserButton * button = gtk_file_chooser_button_new ( "Select folder", GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER ) ;
	gtk_box_pack_start ( dialog -> vbox, table, yes, yes, 2 ) ;
	
	gtk_table_attach ( table, gtk_label_new ( "Output folder" ), 0, 1, 8, 9, GTK_SHRINK, GTK_SHRINK, 10, 5 ) ;
	gtk_table_attach ( table, button, 2, 3, 8, 9, GTK_FILL, GTK_FILL, 2, 5 ) ;
	
	gtk_table_attach ( table, gtk_label_new ( "Preset" ), 4, 5, 8, 9, GTK_SHRINK, GTK_SHRINK, 10, 5 ) ;
	gtk_table_attach ( table, preset, 6, 7, 8, 9, GTK_FILL, GTK_FILL, 2, 5 ) ;
	
	Encoder encoder ;
	encoder.vcodec = vcodec ;
	encoder.acodec = acodec ;
	
	d = 0 ;
	while ( preset_names [d] != null )
	{
		gtk_combo_box_append_text ( preset, preset_names [d] ) ;
		d ++ ;
	}
	
	encoder.b = b ;
	encoder.ab = ab ;
	
	encoder.container = container ;
	encoder.s = s ;
	
	encoder.r = r ;
	encoder.ar = ar ;
	
	g_signal_connect ( preset, "changed", wimp_encode_set_preset, & encoder ) ;
	gtk_combo_box_set_active ( preset, 0 ) ;
	
	GtkButton * encode = gtk_dialog_add_button ( dialog, "Encode", 1 ) ;
	GtkButton * close = gtk_dialog_add_button ( dialog, "Close", 0 ) ;
	
	GtkProgressBar * bar = gtk_progress_bar_new () ;
	GtkProgressBar * bar1 = gtk_progress_bar_new () ;

	gtk_table_attach ( table, bar, 0, 3, 10, 11, GTK_FILL, GTK_FILL, 10, 5 ) ;
	gtk_table_attach ( table, bar1, 4, 7, 10, 11, GTK_FILL, GTK_FILL, 2, 5 ) ;

	GtkStatusbar * status = gtk_statusbar_new () ;
	gtk_statusbar_set_has_resize_grip ( status, no ) ;
	
	gtk_table_attach ( table, status, 0, 7, 12, 13, GTK_FILL, GTK_SHRINK, 2, 5 ) ;
	gtk_widget_show_all ( dialog ) ;
	
	if ( gtk_dialog_run ( dialog ) == 0 )
	{
		gtk_widget_destroy ( dialog ) ;
		return ;
	}
	
	gtk_button_set_label ( encode, "Stop" ) ;
	g_signal_connect ( encode, "clicked", wimp_encode_stop, null ) ;
		
	GList * flies = null, * encoded_flies = null ;
	gtk_widget_destroy ( close ) ;
	
	for ( i = 0 ; i < length ; i ++ )
	{
		GtkTreeIter iter ;
		if ( ! gtk_tree_model_iter_nth_child ( model, & iter, null, i ))
			continue ;
		
		gchar * name = null ;
		gtk_tree_model_get ( model, & iter, 1, & name, -1 ) ;
		
		if ( name == null )
			continue ;
		
		gchar * ext = gtk_combo_box_get_active_text ( container ),
			  * vc = gtk_combo_box_get_active_text ( vcodec ),
			  * ac = gtk_combo_box_get_active_text ( acodec ),
			  * size = gtk_combo_box_get_active_text ( s ) ;
		
		gchar * folder = gtk_file_chooser_get_filename ( button ) ;
		
		GString * string = g_string_new ( "ffmpeg;-y;-i;" ) ;
		g_string_append_printf ( string, "%s;", name ) ;
		
		g_string_append_printf ( string,
			"-vcodec;%s;-acodec;%s;-s;%s;-b;%dk;-ab;%fk;-r;%d;-ar;%d;"
			"%s/%s.%s",
			vc,
			ac,
			size,
			gtk_spin_button_get_value_as_int ( b ),
			gtk_spin_button_get_value ( ab ),
			gtk_spin_button_get_value_as_int ( r ),
			gtk_spin_button_get_value_as_int ( ar ),
			folder,
			basename ( name ),
			ext ) ;
		
		flies = g_list_append ( flies, g_strdup ( name )) ;
		encoded_flies = g_list_append ( encoded_flies, g_strdup_printf ( "%s/%s.%s", folder, basename ( name ), ext )) ;
			
		g_free ( vc ) ;
		g_free ( ac ) ;
		g_free ( size ) ;
		g_free ( ext ) ;

		g_free ( name ) ;
		g_free ( folder ) ;
		
		gchar * cmdline = g_string_free ( string, no ) ;
		g_debug ( "%s", cmdline ) ;
		
		gchar * text = g_strdup_printf ( "%d of %d", i + 1, length ) ;
		
		gtk_progress_bar_set_text ( bar1, text ) ;
		gtk_progress_bar_set_fraction ( bar1, (( gfloat ) ( i + 1 ) / ( gfloat ) length )) ;

		g_free ( text ) ;

		wimp_encode_file_gui ( cmdline, status, bar ) ;
		g_free ( cmdline ) ;
	}
	
	GString * string = g_string_new ( "Encoding results: \n\n" ) ;
	i = 0 ;
	
	for ( i = 0 ; i < g_list_length ( flies ) ; i ++ )
	{
		g_string_append_printf ( string, "%s -> %s\n",
			g_list_nth_data ( flies, i ),
			g_list_nth_data ( encoded_flies, i )) ;
		
	}

	gtk_widget_destroy ( table ) ;
	gtk_widget_destroy ( encode ) ;

	GtkButton * play = gtk_dialog_add_button ( dialog, "Play", 1 ) ;
	GtkButton * qu = gtk_dialog_add_button ( dialog, "Enqueue", 2 ) ;
	
	gtk_dialog_add_button ( dialog, "Close", 0 ) ;
	
	GtkTextBuffer * buffer = gtk_text_buffer_new ( null ) ;
	GtkTextIter iter ;
	
	gtk_text_buffer_get_iter_at_line ( buffer, & iter, 0 ) ;
	gtk_text_buffer_insert ( buffer, & iter, string -> str, -1 ) ;
	
	GtkTextView * view = gtk_text_view_new_with_buffer ( buffer ) ;
	GtkScrolledWindow * sw = gtk_scrolled_window_new ( null, null ) ;
	
	gtk_text_view_set_cursor_visible ( view, no ) ;
	gtk_container_add ( sw, view ) ;
	
	gtk_text_view_set_wrap_mode ( view, GTK_WRAP_WORD ) ;
	gtk_text_view_set_editable ( view, no ) ;
	
	gtk_box_pack_start ( dialog -> vbox, sw, yes, yes, 2 ) ;
	gtk_scrolled_window_set_policy ( sw, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC ) ;
	
	gtk_widget_show_all ( dialog ) ;
	
	switch ( gtk_dialog_run ( dialog ))
	{
		case 1:
			wimp_gui_playlist_clear ( gui ) ;
		case 2:
			for ( i = 0 ; i < g_list_length ( encoded_flies ) ; i ++ )
			{
				wimp_gui_playlist_append ( gui, g_list_nth_data ( encoded_flies, i )) ;
				//wimp_gui_playlist_play ( gui, g_list_nth_data ( encoded_flies, 0 )) ;
			}
			break ;
		default:
			break ;
	}
	
	gtk_widget_destroy ( dialog ) ;
	
	g_string_free ( string, on ) ;
	g_list_foreach ( flies, g_free, null ) ;
	
	g_list_foreach ( encoded_flies, g_free, null ) ;
	g_list_free ( encoded_flies ) ;
	
	g_list_free ( flies ) ;
}
