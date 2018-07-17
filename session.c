/*
 *      session.c
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


#include "session.h"

void wimp_session_send_event ( gchar * filename, Action action )
{
	in ;
	
	gchar * file = g_build_filename ( g_get_home_dir (), ".wimp", "connect", null ) ;
	GKeyFile * key = g_key_file_new () ;
	
	if ( g_path_is_absolute ( filename ))
		g_key_file_set_string ( key, "action", "mrl", filename ) ;
	else
	{
		gchar * dir = g_get_current_dir () ;
		gchar * name = g_build_filename ( dir, filename, null ) ;
		
		g_key_file_set_string ( key, "action", "mrl", name ) ;
		
		g_free ( dir ) ;
		g_free ( name ) ;
	}
	
	g_key_file_set_integer ( key, "action", "action", action ) ;
	
	File * fp = fopen ( file, "w" ) ;
	if ( fp == null )
	{
		g_warning ( "Cannot open channel to host: %s", strerror ( errno )) ;
		g_free ( file ) ;
		
		g_key_file_free ( key ) ;
		return ;
	}
	
	gchar * data = g_key_file_to_data ( key, null, null ) ;
	fprintf ( fp, "%s", data ) ;
	
	g_free ( data ) ;
	g_free ( file ) ;
	
	g_key_file_free ( key ) ;
	fclose ( fp ) ;
	
	kill ( wimp_get_pid (), SIGEVENT ) ;

	out ;
}

gint wimp_get_pid ( void )
{
	in ;
	
	#ifdef DEBUG
	g_printf ( "List of functions called follow ...\n" ) ;
	error_at_line ( 0, 0, "session.c", 74, "g_getenv (\"HOME\")" ) ;
	#endif

	gchar * home = g_get_home_dir () ;
	if ( home == null )
	{
		g_printf ( "HOME is not set. There might be a somepin\' wrong with ya distro ..." ) ;
		home = "/tmp" ;
		
		mkdir ( "/tmp/.wimp", 0777 ) ;
	}
	
	#ifdef DEBUG
	error_at_line ( 0, 0, "session.c", 88, "g_build_filename ()" ) ;
	#endif
	gchar * f = g_build_filename ( home, ".wimp", "pid", null ) ;
	
	#ifdef DEBUG
	error_at_line ( 0, 0, "session.c", 91, "g_file_test ()" ) ;
	#endif
	if ( ! g_file_test ( f, G_FILE_TEST_EXISTS ))
	{
		g_message ( "Hello %s!\n\n"
					"Welcome to WiMP. Please read the included documentation "
					"to make your program experience even more complete. Have fun!", g_getenv ( "USER" )) ;
		
		g_free ( f ) ;
		return ;
	}
	
	gchar * contents = null ;
	gsize length = 0 ;
	
	GError * error = null ;
	
	#ifdef DEBUG
	error_at_line ( 0, 0, "session.c", 107, "g_file_get_contents ()" ) ;
	#endif
	if ( ! g_file_get_contents ( f, & contents, & length, & error ))
	{
		warn ( 22, "Critical error: %s", error -> message ) ;
		
		g_free ( f ) ;
		return ;
	}
	
	gint pid = -1 ;
	
	if ( contents != null )
		pid = atoi ( contents ) ;
	
	g_free ( contents ) ;
	
	g_free ( f ) ;
	return pid ;
}

void wimp_log_pid ( void )
{
	in ;
	
	gchar * f = g_build_filename ( g_get_home_dir (), ".wimp", "pid", null ) ;
	File * fp = fopen ( f, "w" ) ;
	
	if ( fp == null )
	{
		g_free ( f ) ;
		return ;
	}
	
	fprintf ( fp, "%d", getpid ()) ;
	fclose ( fp ) ;
	
	g_free ( f ) ;
	return ;
}

