/*
 *      util.c
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


#include "util.h"

void wimp_log_handler ( const gchar * log_domain, GLogLevelFlags log_level,
						const gchar * message, gpointer user_data )
{
	wimp_log_to_file ( message ) ;
	
	GtkMessageType type = GTK_MESSAGE_INFO ;
	switch ( log_level )
	{
		case G_LOG_LEVEL_ERROR:
		case G_LOG_LEVEL_CRITICAL:
			type = GTK_MESSAGE_ERROR ;
			g_printf ( "error in %s: %s\n", module, message ) ;

			break ;
		case G_LOG_LEVEL_WARNING:
			type = GTK_MESSAGE_WARNING ;
			g_printf ( "warning in %s: %s\n", module, message ) ;

			break ;
		case G_LOG_LEVEL_DEBUG:
		#ifdef DEBUG
			g_printf ( "debug in %s: %s\n", module, message ) ;
		#endif
			return ;
		default:
			break ;
	}
	
	GtkWidget * dialog = all_the_pain_that_i_went_through_amounts_to ;
	GtkButtonsType buttons = GTK_BUTTONS_OK ;
	
	GtkDialogFlags flags = GTK_DIALOG_MODAL ;
	dialog = gtk_message_dialog_new ( null,
									  flags,
									  type,
									  buttons,
									  "%s: %s",
									  module,
									  message ) ;
	
	if ( log_level == G_LOG_LEVEL_CRITICAL )
	{
		gtk_dialog_add_button ( dialog, "Abort", -10 ) ;
		GtkButton * log = gtk_button_new_with_label ( "View log" ) ;
		
		GtkDialog * vdialog = dialog ;
		gtk_box_pack_start ( vdialog -> action_area, log, no, no, 4 ) ;
		g_signal_connect ( log, "clicked", wimp_show_log, null ) ;
	
		gtk_widget_show_all ( dialog ) ;
	}
		
	
	gint result = gtk_dialog_run ( dialog ) ;
	
	if ( log_level == G_LOG_LEVEL_CRITICAL )
		if ( result == -10 )
			abort () ;
			
	gtk_widget_destroy ( dialog ) ;
}
												  
void wimp_message_box_info ( gchar * message )
{
	g_message ( "%s", message ) ;
	g_printf ( "[ WIMP ] message: %s\n" ) ;
}

void wimp_message_warning ( gchar * message )
{
	g_warning ( "%s", message ) ;
	g_printf ( "[ WIMP ] warning: %s\n" ) ;
}

void wimp_message_error ( gchar * message )
{
	g_critical ( "%s", message ) ;
	g_printf ( "[ WIMP ] error: %s\n" ) ;
}

gboolean wimp_message_question ( gchar * message )
{
	GtkDialog * dialog = gtk_message_dialog_new ( null,
												  GTK_DIALOG_MODAL,
												  GTK_MESSAGE_QUESTION,
												  GTK_BUTTONS_YES_NO,
												  "%s",
												  message ) ;
	
	GtkResponseType type = gtk_dialog_run ( dialog ) ;
	gtk_widget_destroy ( dialog ) ;
	
	if ( type == GTK_RESPONSE_YES )
		return true ;
	else
		return false ;
	
}


void wimp_update_gui ( void )
{
	while ( gtk_events_pending ())
		gtk_main_iteration () ;

	return ;
}

void wimp_startup ( gint argc, gchar ** argv )
{
	in ;
	
	gchar * wimp_dir = g_build_filename ( g_get_home_dir (), ".wimp", null ) ;
	gchar * playlist_dir = g_build_filename ( wimp_dir, "playlists", null ) ;
	
	log_file = g_build_filename ( wimp_dir, "log", null ) ;
	remove ( log_file ) ;
	
	gboolean first_time = off ;
	
	if ( ! g_file_test ( wimp_dir, G_FILE_TEST_EXISTS ))
	{
		g_mkdir ( wimp_dir, 0777 ) ;
		first_time = on ;
	}

	if ( ! g_file_test ( playlist_dir, G_FILE_TEST_EXISTS ))
		g_mkdir ( playlist_dir, 0777 ) ;
	
	g_free ( wimp_dir ) ;
	g_free ( playlist_dir ) ;
	
	gchar * fav_dir = g_build_filename ( g_get_home_dir (), ".wimp", "favorites", null ) ;
	if ( ! g_file_test ( fav_dir, G_FILE_TEST_IS_DIR ))
		g_mkdir ( fav_dir, 0777 ) ;
	
	g_free ( fav_dir ) ;

	/*	For now, we shall have no session support, because it Hangs
	* 	us
	*
	gchar * old = g_strdup_printf ( "/proc/%d", wimp_get_pid ()) ;
	if ( g_file_test ( old, G_FILE_TEST_EXISTS ))
	{
		g_debug ( "A wimp is already running ( pid %d )", wimp_get_pid ()) ;
		if ( argc > 1 )
			wimp_session_send_event ( argv [1], ActionOpen ) ;
		
		gint i = 0 ;
		for ( i = 2 ; i < argc ; i ++ )
		{
			g_usleep ( 50000 ) ;
			wimp_session_send_event ( argv [1], ActionEnqueue ) ;
		}
		
		exit ( 2 ) ;
	}
	
	wimp_log_pid () ;
	g_free ( old ) ;
	
	
	*/
	
	gtk_init ( & argc, & argv ) ;
	g_set_application_name ( MY_NAME ) ;
	
	g_log_set_handler ( null, G_LOG_LEVEL_DEBUG | G_LOG_LEVEL_INFO | G_LOG_LEVEL_MESSAGE | G_LOG_LEVEL_WARNING | G_LOG_LEVEL_CRITICAL | G_LOG_LEVEL_ERROR , wimp_log_handler, null ) ;	
	g_log_set_handler ( "Gtk", G_LOG_LEVEL_DEBUG | G_LOG_LEVEL_INFO | G_LOG_LEVEL_MESSAGE | G_LOG_LEVEL_WARNING | G_LOG_LEVEL_CRITICAL | G_LOG_LEVEL_ERROR , wimp_log_handler, null ) ;	

	g_printf ( 	"---> WiMP user release 1 version %s\n", VERSION ) ;
	g_printf (	"---> Welcome\n" ) ;

	g_printf ( "---> Xine library version %s\n", xine_get_version_string ()) ;

	signal ( SIGSEGV, wimp_sighandler ) ;
	
	/* Heh :) */
	//gint u = 0 ;
	//for ( u = 1 ; u < 32 ; u ++ )
		//signal ( u, wimp_sighandler ) ;
	
	if ( first_time )
	{
		g_message ( "Hullo %s !\n\n"
					"So it finally worked fur ya, eh :) Sorry "
					"for all the bad versions that didn\'t worked! "
					"\n\Happy usage !", getenv ( "USER" )) ;
			
		gchar * playlist = g_build_filename ( g_get_home_dir (), ".wimp", "playlist.m3u", null ) ;
		FILE * fp = fopen ( playlist, "w" ) ;
		
		fclose ( fp ) ;
		g_free ( playlist ) ;

		playlist = g_build_filename ( g_get_home_dir (), ".wimp", "index.html", null ) ;
		fp = fopen ( playlist, "w" ) ;
		
		fclose ( fp ) ;
		g_free ( playlist ) ;
	}
	
	time_t t ;
	time ( & t ) ;
	
	wimp_log_to_file ( asctime ( localtime ( & t ))) ;
	out ;
}

gchar * wimp_format_time ( gint pos, gint length )
{
	in ;
	
	gint mins = pos / 60000 ;
	gint sex = ( pos / 1000 ) - ( mins * 60 ) ;
	
	gint mins1 = length / 60000, sex1 = ( length / 1000 ) - ( mins1 * 60 ) ;
	gchar * display = g_strdup_printf ( "%d:%d / %d:%d", mins, sex, mins1, sex1 ) ;
	
	out ;
	return display ;
}

void wimp_sighandler ( gint signum )
{
	switch ( signum )
	{
		case SIGTERM:
			g_critical ( "Caught SIGTERM! Trying to exit ..." ) ;
			break ;
		default:
			g_critical ( "%s in module:\n %s.", g_strsignal ( signum ), module ) ;
	
	}
}

gboolean wimp_file_is_suitable_for_playback ( gchar * filename )
{
	gchar * ext = g_strdup ( strrchr ( filename, '.' )) ;
	
	// No extension
	if ( ext == null )
		return true ;
	
	gchar * down = g_ascii_strdown ( ext, -1 ) ;
	
	gint i = 0 ;
	gboolean result = false ;
	
	while ( extensions [i] != null )
	{
		if ( g_strrstr ( down, extensions [i] ) != null )
		{
			result = true ;
			break ;
		}
		
		i ++ ;
	}
	
	g_free ( down ) ;
	g_free ( ext ) ;
	
	return result ;
}
	
gchar * wimp_array_to_char ( gchar ** vector )
{
	GString * string = g_string_new ( null ) ;
	gint i = 0, count = 0 ;
	
	while ( vector [i] != null )
	{
		g_string_append ( string, vector [i] ) ;
		
		if ( count > 5 )
		{
			g_string_append ( string, "\n" ) ;
			count = 0 ;
		}
		else
			g_string_append ( string, " " ) ;
		
		i ++ ;
		count ++ ;
	}
	
	return g_string_free ( string, no ) ;
}

gchar * wimp_string_to_nl ( gchar * string )
{
	gint i = 0, len = strlen ( string ), count = 0 ;
	GString * s = g_string_new ( null ) ;
	
	for ( i = 0 ; i < len ; i ++ )
	{
		g_string_append_c ( s, string [i] ) ;
		if ( count > 30 )
		{
			g_string_append ( s, "\n" ) ;
			count = 0 ;
		}
		
		count ++ ;
	}
	
	return g_string_free ( s, no ) ;
}

GList * wimp_xdnd_to_list ( gchar * text )
{
	in ;
	g_debug ( "%s", text ) ;
	
	gchar ** vector = g_strsplit ( text, "\n", -1 ) ;
	gint i = 0 ;
	
	GList * list = null ;
	while ( vector [i] != null )
	{
		gchar * path = strndup ( vector [i], strlen ( vector [i] ) - 1 ) ;
		gint length = strlen ( path ) ;
		
		gint u = 0, count = 0 ;
		
		for ( u = 7 ; u < length ; u ++ )
		{
			path [count] = vector [i][u] ;
			count ++ ;

			if ( vector [i][u] == '%' )
				u = u + 2 ;
			
		}
		
		g_strdelimit ( path, "%", ' ' ) ;
		path [count] = null ;
		
		g_debug ( "Converted %s to %s", vector [i], path ) ;
		
		if ( length > 2 )
			list = g_list_append ( list, path ) ;
		else
			g_free ( path ) ;
		
		i ++ ;
	}
	
	g_strfreev ( vector ) ;
	return list ;
}

void wimp_message_box_detailed_info ( gchar * message )
{
	
	GtkTextBuffer * buffer = gtk_text_buffer_new ( null ) ;
	GtkTextIter iter ;
	
	gtk_text_buffer_get_iter_at_line ( buffer, & iter, 0 ) ;
	gtk_text_buffer_insert ( buffer, & iter, message, -1 ) ;
	
	GtkTextView * view = gtk_text_view_new_with_buffer ( buffer ) ;
	GtkScrolledWindow * sw = gtk_scrolled_window_new ( null, null ) ;
	
	//gtk_text_view_set_cursor_visible ( view, no ) ;
	gtk_container_add ( sw, view ) ;
	
	gtk_text_view_set_wrap_mode ( view, GTK_WRAP_WORD ) ;
	gtk_text_view_set_editable ( view, no ) ;
	
	GtkDialog * dialog = gtk_dialog_new_with_buttons ( 	"Info",
														null,
														GTK_DIALOG_MODAL,
														"Ok",
														1,
														null ) ;

	gtk_box_pack_start ( dialog -> vbox, sw, yes, yes, 2 ) ;
	gtk_window_resize ( dialog, 550, 400 ) ;

	gtk_scrolled_window_set_policy ( sw, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC ) ;
	gtk_widget_show_all ( dialog ) ;
	
	gtk_dialog_run ( dialog ) ;
	gtk_widget_destroy ( dialog ) ;
	
}

void wimp_gui2_run ( gchar * title, gchar * command )
{
	in ;
	
	GtkDialog * dialog = gtk_dialog_new_with_buttons ( "Please wait ...",
														null,
														GTK_DIALOG_MODAL,
														null ) ;
	
	GtkLabel * label = gtk_label_new ( null ) ;
	GtkProgressBar * bar = gtk_progress_bar_new () ;
	
	gchar * header = gtk_label_new ( null ) ;
	gchar * markup = g_strdup_printf ( "<span size=\"large\"><b>%s</b></span>", title ) ;
	
	gtk_label_set_markup ( header, markup ) ;
	g_free ( markup ) ;
	
	gtk_box_pack_start ( dialog -> vbox, header, no, no, 5 ) ;
	gtk_box_pack_start ( dialog -> vbox, label, no, no, 2 ) ;
	
	gtk_box_pack_start ( dialog -> vbox, bar, no, no, 5 ) ;
	gtk_widget_show_all ( dialog ) ;
	
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
		
		gtk_label_set_label ( label, string -> str ) ;
		g_string_free ( string, on ) ;
		
		gtk_progress_bar_pulse ( bar ) ;
		
		wimp_update_gui () ;
		g_usleep ( 10000 ) ;
		
		ch = getc ( fd ) ;
		if ( ch == EOF )
			break ;
	}

	g_free ( proc ) ;
	
	g_strfreev ( vector ) ;
	fclose ( fd ) ;
	
	g_spawn_close_pid ( pid ) ;
	gtk_widget_destroy ( dialog ) ;
	
	out ;
}

void wimp_log_to_file ( gchar * text )
{
	File * fp = fopen ( log_file, "a+" ) ;
	fprintf ( fp, "%s\n", text ) ;
	
	fclose ( fp ) ;
}

void wimp_show_log ( vodi )
{
	in ;

	gchar * contents = null ;
	GError * error = null ;
	
	gsize size = 0 ;
	
	if ( ! g_file_get_contents ( log_file, & contents, & size, & error ))
	{
		g_warning ( "Cannot open log: %s", error -> message ) ;
		return ;
	}

	wimp_message_box_detailed_info ( contents ) ;
	g_free ( contents ) ;
	
	out ;
}

GdkPixbuf * wimp_pixbuf_new_from_file_at_scale ( gchar * filename, gint width, gint height, GError ** error )
{
	in ;
	gint requested_width = width, requested_height = height ;
	
	GdkPixbuf * pixbuf = gdk_pixbuf_new_from_file ( filename, error ) ;
	gint image_width = gdk_pixbuf_get_width ( pixbuf ),
		 image_height = gdk_pixbuf_get_height ( pixbuf ) ;
	
	if ( image_width == width && image_height == height )
		return pixbuf ;
	
	gfloat aspect = ( gfloat ) image_width / ( gfloat ) image_height ;
	if ( image_width > image_height )
	{
		width = ( gfloat ) height * aspect ;
		
		if ( width > requested_width )
		{
			width = requested_width ;
			height  = ( gfloat ) width / aspect ;
		}
		
	}
	else if ( image_height > image_width )
	{			
		width = ( gfloat ) height * aspect ;
		/* height = ( gfloat ) width / aspect ; */
	}
	else if ( image_width == image_height )
		width = height ;
	
	GdkPixbuf * pixbuf1 = gdk_pixbuf_scale_simple ( pixbuf, width, height, GDK_INTERP_BILINEAR ) ;
	g_object_unref ( pixbuf ) ;	

	out ;
	return pixbuf1 ;
}

void wimp_clipboard_set ( gchar * filename )
{
	in ;
	
	GtkClipboard * clip ;
	clip = gtk_clipboard_get ( GDK_NONE ) ;

	gtk_clipboard_set_text ( clip, filename, oooooh ) ;

	out ;
	return ;
}

bool wimp_file_is_stream ( gchar * filename )
{
	in ;
	
	/*  I still do not know all the different *types* of streams
	* 	out there, or even how many types of streams xine supports.
	* 	
	* 	Here goes nothing :)
	*/
	
	bool res = false ;
	if ( strstr ( filename, "mms://" ))
		return true ;
	
	if ( strstr ( filename, "rtsp://" ))
		return true ;
	
	//if ( strstr ( filename, "http://" ))
		//return true ;
	
	out ;
	return false ;
}

