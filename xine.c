/*
 *      wim
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

#include "xine.h"

Xine * wimp_xine_new ( void )
{
	enter ;	

	GMemChunk * chunk = g_mem_chunk_create ( Xine, 10, G_ALLOC_ONLY ) ;
	Xine * xine = g_chunk_new ( Xine, chunk ) ;
	
	xine -> chunk = chunk ;

	leave ;
	return xine ;
}
	
void wimp_xine_destroy ( Xine * xine )
{
	xine_dispose ( xine -> stream ) ;
	xine_close_audio_driver ( xine -> xine, xine -> ao ) ;
	
	xine_close_video_driver ( xine -> xine, xine -> vo ) ;
	xine_exit ( xine -> xine ) ;
	
	g_mem_chunk_destroy ( xine -> chunk ) ;
}

void wimp_xine_engine_init ( Xine * xine )
{
	in ;
	g_debug ( "Creating xine object ..." ) ;
	
	xine -> xine = xine_new () ;
	gchar * xine_config = g_build_filename ( g_get_home_dir (), ".wimp", "xine.cfg", null ) ;

	g_debug ( "Using config file %s", xine_config ) ;

	xine_config_load ( xine -> xine, xine_config ) ;
	g_free ( xine_config ) ;
	
	g_debug ( "Initializing xine ..." ) ;
	xine_init ( xine -> xine ) ;
	
	xine -> ao = xine_open_audio_driver ( xine -> xine, "alsa", null ) ;
	xine -> stream = xine_stream_new ( xine -> xine, xine -> ao, null ) ;
	
	xine -> event_queue = xine_event_new_queue ( xine -> stream ) ;
	//xine_event_create_listener_thread ( xine -> event_queue, wimp_xine_event_listener, NULL ) ;
	
	xine -> pos_stream = 0 ;
	xine -> pos_time = 0 ;
	
	xine -> paused_at = 0 ;
	xine -> timestamp = gtk_get_current_event_time () ;

	xine -> length = 0 ;

	g_debug ( "Initializing OSD ..." ) ;
	xine -> osd = xine_osd_new ( xine -> stream, 0, 0, 400, 300 ) ;
	
	xine_osd_set_font ( xine -> osd, "/usr/share/fonts/bitstream-vera/VeraMono.ttf", 18 ) ;
	xine_osd_set_text_palette ( xine -> osd, XINE_TEXTPALETTE_WHITE_NONE_TRANSLUCID, XINE_TEXTPALETTE_WHITE_NONE_TRANSLUCID ) ;	
	
	g_debug ( "xine initialized !" ) ;
	out ;
}

void wimp_xine_load ( Xine * xine, gchar * filename )
{
	in ;
	xine_close ( xine -> stream ) ;
	
	xine -> pos_stream = 0 ;
	xine -> pos_time = 0 ;
	
	xine -> paused_at = 0 ;
	xine -> timestamp = gtk_get_current_event_time () ;
	
	xine -> length = 0 ;

	if ( filename == null )
		g_critical ( "Filename is null !!!" ) ;
	
	else if ( ! xine_open ( xine -> stream, filename ))
		g_critical ( "Failed to load file %s with error code %d", filename, xine_get_error ( xine -> stream )) ;

	out ;
}


void wimp_xine_engine_init_x11 ( Xine * xine, GtkWidget * video_display )
{
	in ;
	g_debug ( "Creating xine object ..." ) ;
	
	gtk_widget_realize ( video_display ) ;
	
	xine -> xine = xine_new () ;
	gchar * xine_config = g_build_filename ( g_get_home_dir (), ".wimp", "xine.cfg", null ) ;

	g_debug ( "Using config file %s", xine_config ) ;

	xine_config_load ( xine -> xine, xine_config ) ;
	g_free ( xine_config ) ;
	
	g_debug ( "Initializing xine ..." ) ;
	xine_init ( xine -> xine ) ;
	
	xine -> config = wimp_config_new () ;
	
	gchar * config = g_build_filename ( g_get_home_dir (), ".wimp", "wimp.cfg", null ) ;
	if ( ! g_file_test ( config, G_FILE_TEST_EXISTS ))
		wimp_config_save_default ( config ) ;
	
	wimp_config_load ( xine -> config, config ) ;
	g_free ( config ) ;
	
	g_debug ( "Using audio driver %s and video driver %s", xine -> config -> ao, xine -> config -> vo ) ;
	
	xine -> ao = xine_open_audio_driver ( xine -> xine, xine -> config -> ao, null ) ;
	wimp_xine_vo_init_x11 ( xine, video_display ) ;

	if ( xine -> vo )
		xine -> stream = xine_stream_new ( xine -> xine, xine -> ao, xine -> vo ) ;
	else
		xine -> stream = xine_stream_new ( xine -> xine, xine -> ao, null ) ;

	xine -> event_queue = xine_event_new_queue ( xine -> stream ) ;
	
	if ( xine -> vo ) {
	xine_port_send_gui_data ( xine -> vo, XINE_GUI_SEND_DRAWABLE_CHANGED, (void *) xine -> xwindow );
	xine_port_send_gui_data ( xine -> vo, XINE_GUI_SEND_VIDEOWIN_VISIBLE, (void *) 1);
	}
	
	/* OSD is diabled, for now
	g_debug ( "Initializing OSD ..." ) ;
	xine -> osd = xine_osd_new ( xine -> stream, 0, 0, 400, 300 ) ;
	
	xine_osd_set_font ( xine -> osd, "/usr/share/fonts/bitstream-vera/VeraMoBd.ttf", 24 ) ;
	xine_osd_set_text_palette ( xine -> osd, XINE_TEXTPALETTE_WHITE_NONE_TRANSLUCID, XINE_TEXTPALETTE_WHITE_NONE_TRANSLUCID ) ;
	
	*/
	xine -> osd_shown = 0 ;
	xine -> vpts = 0 ;
	
	xine -> pos_stream = 0 ;
	xine -> pos_time = 0 ;
	
	xine -> paused_at = 0 ;
	xine -> timestamp = gtk_get_current_event_time () ;
	
	xine -> length = 0 ;

	g_debug ( "xine initialized !" ) ;
	out ;
}

void wimp_xine_cleanup ( Xine * xine )
{
	in ;

	xine_stop ( xine -> stream ) ;
	xine_close ( xine -> stream ) ;
	
	xine_dispose ( xine -> stream ) ;
	
	xine_close_video_driver ( xine -> xine, xine -> vo ) ;
	xine_close_audio_driver ( xine -> xine, xine -> ao ) ;
	
	gchar * config = g_build_filename ( g_get_home_dir (), ".wimp", "xine.cfg", null ) ;
	xine_config_save ( xine -> xine, config ) ;
	
	g_free ( config ) ;
	xine_exit ( xine -> xine ) ;

	out ;
}

