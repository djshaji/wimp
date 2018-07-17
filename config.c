/*
 *      config.c
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


#include "config.h"

void wimp_config_load ( Config * config, gchar * filename )
{
	in ;
	
	GKeyFile * file = g_key_file_new () ;
	GError * error = null ;
	
	g_key_file_load_from_file ( file, filename, G_KEY_FILE_NONE, & error ) ;
	if ( error )
	{
		g_critical ( "Cannot load config file %s: %s", filename, error -> message ) ;
		return ;
	}
	
	config -> vo = g_key_file_get_string ( file, "xine", "vo", & error ) ;
	if ( error )
	{
		g_debug ( "config -> vo: %s", error -> message ) ;
		
		config -> vo = g_strdup ( "auto" ) ;
		error = null ;
	}

	config -> ao = g_key_file_get_string ( file, "xine", "ao", & error ) ;
	if ( error )
	{
		g_debug ( "config -> ao: %s", error -> message ) ;
		config -> ao = g_strdup ( "auto" ) ;
		error = null ;
	}

	config -> emergency_exit_enabled = g_key_file_get_boolean ( file, "wimp", "emergency_exit_enabled", & error ) ;
	if ( error )
	{
		g_debug ( "config -> emergency_exit_enabled : %s", error -> message ) ;
		
		config -> emergency_exit_enabled = on ;
		error = null ;
	}

	config -> memlock = g_key_file_get_boolean ( file, "experimental", "memlock", & error ) ;
	if ( error )
	{
		g_debug ( "config -> emergency_exit_enabled : %s", error -> message ) ;
		
		config -> memlock = off ;
		error = null ;
	}

	config -> safe_search = g_key_file_get_boolean ( file, "search", "safe_search", & error ) ;
	if ( error )
	{
		g_debug ( "config -> safe_search : %s", error -> message ) ;
		
		config -> safe_search = on ;
		error = null ;
	}

	config -> thumbnail_size = g_key_file_get_boolean ( file, "search", "thumbnail_size", & error ) ;
	if ( error )
	{
		g_debug ( "config -> thumbnail_size : %s", error -> message ) ;
		
		config -> thumbnail_size = 32 ;
		error = null ;
	}
	
	g_key_file_free ( file ) ;
	out ;
}

Config * wimp_config_new ( void )
{
	GMemChunk * chunk = g_mem_chunk_create ( Config, 10, G_ALLOC_ONLY ) ;
	Config * wimp = g_chunk_new ( Config, chunk ) ;
	
	return wimp ;
}
	
void wimp_config_save ( Config * config, gchar * filename )
{
	in ;
	
	GKeyFile * file = g_key_file_new () ;
	GError * error = null ;
	
	g_key_file_load_from_file ( file, filename, G_KEY_FILE_NONE, & error ) ;
	g_key_file_set_boolean ( file, "wimp", "emergency_exit_enabled", config -> emergency_exit_enabled ) ;

	g_key_file_set_boolean ( file, "experimental", "memlock", config -> memlock ) ;

	g_key_file_set_string ( file, "xine", "vo", config -> vo ) ;
	g_key_file_set_string ( file, "xine", "ao", config -> ao ) ;
	
	g_key_file_set_boolean ( file, "search", "safe_search", config -> safe_search ) ;
	g_key_file_set_boolean ( file, "search", "thumbnail_size", config -> thumbnail_size ) ;
	
	gchar * data = g_key_file_to_data ( file, null, null ) ;
	g_key_file_free ( file ) ;
	
	File * fp = fopen ( filename, "w" ) ;
	fprintf ( fp, "%s", data ) ;
	
	fclose ( fp ) ;
	g_free ( data ) ;
	
	out ;
}
	
void wimp_config_save_default ( gchar * filename )
{
	in ;
	
	GKeyFile * file = g_key_file_new () ;
	GError * error = null ;
	
	g_key_file_load_from_file ( file, filename, G_KEY_FILE_NONE, & error ) ;
	g_key_file_set_boolean ( file, "wimp", "emergency_exit_enabled", true ) ;

	g_key_file_set_string ( file, "xine", "vo", "auto" ) ;
	g_key_file_set_string ( file, "xine", "ao", "auto" ) ;
	
	g_key_file_set_boolean ( file, "experimental", "memlock", off ) ;
	g_key_file_set_boolean ( file, "search", "safe_search", on ) ;
	
	g_key_file_set_boolean ( file, "search", "thumbnail_size", 32 ) ;
	
	gchar * data = g_key_file_to_data ( file, null, null ) ;
	g_key_file_free ( file ) ;
	
	File * fp = fopen ( filename, "w" ) ;
	fprintf ( fp, "%s", data ) ;
	
	fclose ( fp ) ;
	g_free ( data ) ;
	
	out ;
}
	
