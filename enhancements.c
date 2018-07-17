/*
 *      enhancements.c
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


#include "enhancements.h"

void wimp_gui_enhancements_volume ( GtkRange * range, GtkScrollType scroll, gdouble value, WimpGUI * gui )
{
	in ;
	xine_set_param ( gui -> xine -> stream, XINE_PARAM_AUDIO_VOLUME, value ) ;
	
	out ;
}

void wimp_gui_enhancements_volume_up ( WimpGUI * gui )
{
	in ;
	
	gint volume = gtk_range_get_value ( gui -> volume ) ;
	volume = volume + 3 ;
	
	gtk_range_set_value ( gui -> volume, volume ) ;
	xine_set_param ( gui -> xine -> stream, XINE_PARAM_AUDIO_VOLUME, volume ) ;
	
	g_debug ( "Volume: %d", volume ) ;
	
	out ;
}

void wimp_gui_enhancements_volume_down ( WimpGUI * gui )
{
	in ;

	gint volume = gtk_range_get_value ( gui -> volume ) ;
	volume = volume - 3 ;
	
	gtk_range_set_value ( gui -> volume, volume ) ;
	xine_set_param ( gui -> xine -> stream, XINE_PARAM_AUDIO_VOLUME, volume ) ;
	
	g_debug ( "Volume: %d", volume ) ;
	
	out ;
}

/*
GtkWidget * wimp_gui_enhancements_new ( WimpGUI * gui )
{
	GMemChunk * chunk = g_mem_chunk_create ( Enhancements, 10, G_ALLOC_ONLY ) ;
	Enhancements * enhancements = g_chunk_new ( Enhancements, chunk ) ;

	enhancements -> audio = g_chunk_new ( Audio, chunk ) ;
	enhancements -> video = g_chunk_new ( Video, chunk ) ;
	
	enhancements -> audio -> speed = gtk_hsca
*/

GtkWidget * wimp_gui_enhancements ( WimpGUI * gui )
{
	in ;
	WimpGUI * wimp = gui ;
	
	GtkWidget 	* mute = gtk_check_button_new_with_label ( "Mute" ),
				* amp_level = gtk_hscale_new ( null ),
				* comp_level = gtk_hscale_new ( null ),
				* amp_mute = gtk_check_button_new_with_label ( "Amp mute" ) ;
	
	gtk_widget_set_size_request ( amp_level, 100, 30 ) ;
	gtk_widget_set_size_request ( comp_level, 100, 30 ) ;
	
	gtk_range_set_range ( amp_level, 0, 200 ) ;
	gtk_range_set_increments ( amp_level, 1, 10 ) ;
	
	gtk_range_set_range ( comp_level, 0, 200 ) ;
	gtk_range_set_increments ( comp_level, 1, 10 ) ;
	
	gtk_scale_set_draw_value ( amp_level, no ) ;
	gtk_scale_set_draw_value ( comp_level, no ) ;
	
	GtkWidget 	* contrast = gtk_hscale_new ( null ),
				* saturation = gtk_hscale_new ( null ),
				* brightness = gtk_hscale_new ( null ),
				* hue = gtk_hscale_new ( null ) ;
	
	gtk_widget_set_size_request ( contrast, 100, 30 ) ;
	gtk_range_set_range ( contrast, 0, MAX ) ;
	gtk_range_set_increments ( contrast, 1000, 6000 ) ;
	gtk_scale_set_draw_value ( contrast, no ) ;
	
	gtk_widget_set_size_request ( brightness, 100, 30 ) ;
	gtk_range_set_range ( brightness, 0, MAX ) ;
	gtk_range_set_increments ( brightness, 1000, 6000 ) ;
	gtk_scale_set_draw_value ( brightness, no ) ;
	
	gtk_widget_set_size_request ( hue, 100, 30 ) ;
	gtk_range_set_range ( hue, 0, MAX ) ;
	gtk_range_set_increments ( hue, 1000, 6000 ) ;
	gtk_scale_set_draw_value ( hue, no ) ;
	
	gtk_widget_set_size_request ( saturation, 100, 30 ) ;
	gtk_range_set_range ( saturation, 0, MAX ) ;
	gtk_range_set_increments ( saturation, 1000, 6000 ) ;
	gtk_scale_set_draw_value ( saturation, no ) ;
	
	GtkWidget 	* aspect_ratio = gtk_combo_box_new_text (),
				* zoom_x = gtk_hscale_new ( null ),
				* zoom_y = gtk_hscale_new ( null ),
				* tv_mode = gtk_check_button_new_with_label ( "TV mode" ) ;
	
	gtk_widget_set_size_request ( zoom_x, 100, 30 ) ;
	gtk_range_set_range ( zoom_x, 0, 100 ) ;
	gtk_range_set_increments ( zoom_x, 1, 10 ) ;
	gtk_scale_set_draw_value ( zoom_x, no ) ;

	gtk_widget_set_size_request ( zoom_y, 100, 30 ) ;
	gtk_range_set_range ( zoom_y, 0, 100 ) ;
	gtk_range_set_increments ( zoom_y, 1, 10 ) ;
	gtk_scale_set_draw_value ( zoom_y, no ) ;

	GtkWidget 	* vbox1 = gtk_vbox_new ( no, 2 ),
				* vbox2 = gtk_vbox_new ( no, 2 ),
				* vbox3 = gtk_vbox_new ( no, 2 ),
				* vbox4 = gtk_vbox_new ( no, 2 ),
				* vbox5 = gtk_vbox_new ( no, 2 ),
				* vbox6 = gtk_vbox_new ( no, 2 ),
				* hbox1 = gtk_hbox_new ( no, 2 ),
				* hbox2 = gtk_hbox_new ( no, 2 ),
				* hbox3 = gtk_hbox_new ( no, 2 ) ;
	
	gtk_box_pack_start ( vbox1, comp_level, no, no, 0 ) ;
	gtk_box_pack_start ( vbox1, gtk_label_new ( "Comp level" ), no, no, 2 ) ;
	gtk_box_pack_start ( vbox1, amp_level, no, no, 0 ) ;
	gtk_box_pack_start ( vbox1, gtk_label_new ( "Amp level" ), no, no, 2 ) ;
	gtk_box_pack_start ( vbox2, mute, no, no, 2 ) ;
	gtk_box_pack_start ( vbox2, amp_mute, no, no, 2 ) ;
	gtk_box_pack_start ( vbox3, brightness, no, no, 0 ) ;
	gtk_box_pack_start ( vbox3, gtk_label_new ( "Brightness" ), no, no, 0 ) ;
	gtk_box_pack_start ( vbox3, contrast, no, no, 0 ) ;
	gtk_box_pack_start ( vbox3, gtk_label_new ( "Contrast" ), no, no, 0 ) ;
	gtk_box_pack_start ( vbox4, saturation, no, no, 0 ) ;
	gtk_box_pack_start ( vbox4, gtk_label_new ( "Saturation" ), no, no, 0 ) ;
	gtk_box_pack_start ( vbox4, hue, no, no, 0 ) ;
	gtk_box_pack_start ( vbox4, gtk_label_new ( "Hue" ), no, no, 0 ) ;
	
	gtk_box_pack_start ( vbox5, zoom_x, no, no, 2 ) ;
	gtk_box_pack_start ( vbox5, gtk_label_new ( "Zoom X" ), no, no, 0 ) ;
	gtk_box_pack_start ( vbox5, zoom_y, no, no, 2 ) ;
	gtk_box_pack_start ( vbox5, gtk_label_new ( "Zoom Y" ), no, no, 0 ) ;
	gtk_box_pack_start ( vbox6, aspect_ratio, no, no, 2 ) ;
	gtk_box_pack_start ( vbox6, gtk_label_new ( "Aspect ratio" ), no, no, 0 ) ;
	gtk_box_pack_start ( vbox6, tv_mode, no, no, 2 ) ;
	gtk_box_pack_start ( hbox1, vbox1, no, no, 2 ) ;
	gtk_box_pack_start ( hbox1, vbox2, no, no, 2 ) ;
	gtk_box_pack_start ( hbox2, vbox3, no, no, 2 ) ;
	gtk_box_pack_start ( hbox2, vbox4, no, no, 2 ) ;
	gtk_box_pack_start ( hbox3, vbox5, no, no, 2 ) ;
	gtk_box_pack_start ( hbox3, vbox6, no, no, 2 ) ;
	
	gint i = 0 ;
	while ( aspect_ratios [i] != null )
	{
		gtk_combo_box_append_text ( aspect_ratio, aspect_ratios [i] ) ;
		i ++ ;
	}
	
	gtk_combo_box_set_active ( aspect_ratio, 0 ) ;
	
	GMemChunk * chunk = g_mem_chunk_create ( Equalizer, 10, G_ALLOC_ONLY ) ;
	GtkWidget * eq_box = gtk_hbox_new ( no, 0 ) ;
	
	gui -> eq = null ;
	
	for ( i = 18 /* heh :) */ ; i < 28 ; i ++ )
	{
		Equalizer * eq = wimp_gui_enhancements_equalizer ( gui, i, chunk ) ;
		gtk_box_pack_start ( eq_box, eq -> scale, no, no, 1 ) ;
	
		gui -> eq = g_list_append ( gui -> eq, eq -> scale ) ;
	}

	GtkNotebook * notebook = gtk_notebook_new () ;
	gtk_notebook_append_page ( notebook, eq_box, gtk_label_new ( "Equalizer" )) ;
	
	gtk_notebook_append_page ( notebook, hbox1, gtk_label_new ( "Audio" )) ;
	
	gtk_notebook_append_page ( notebook, hbox2, gtk_label_new ( "Video" )) ;
	gtk_notebook_append_page ( notebook, hbox3, gtk_label_new ( "Misc" )) ;
		
	gtk_widget_show_all ( notebook ) ;
	
	wimp -> mute = mute ;
	wimp -> amp_level = amp_level ;
	wimp -> comp_level = comp_level ;
	wimp -> amp_mute = amp_mute ;
	
	g_signal_connect_after ( amp_level, "change-value", wimp_gui_enhancements_handle_amp_level, wimp ) ;
	g_signal_connect_after ( comp_level, "change-value", wimp_gui_enhancements_handle_comp_level, wimp ) ;

	g_signal_connect_after ( amp_mute, "toggled", wimp_gui_enhancements_handle_amp_mute, wimp ) ;
	g_signal_connect_after ( mute, "toggled", wimp_gui_enhancements_handle_mute, wimp ) ;

	wimp -> brightness = brightness ;
	wimp -> contrast = contrast ;
	wimp -> hue = hue ;
	wimp -> saturation = saturation ;
	
	g_signal_connect_after ( brightness, "change-value", wimp_gui_enhancements_handle_brightness, wimp ) ;
	g_signal_connect_after ( contrast, "change-value", wimp_gui_enhancements_handle_contrast, wimp ) ;
	g_signal_connect_after ( hue, "change-value", wimp_gui_enhancements_handle_hue, wimp ) ;
	g_signal_connect_after ( saturation, "change-value", wimp_gui_enhancements_handle_saturation, wimp ) ;

	wimp -> aspect_ratio = aspect_ratio ;
	wimp -> zoom_x = zoom_x ;
	wimp -> zoom_y = zoom_y ;
	wimp -> tv_mode = tv_mode ;
	
	wimp -> big_audio_label = gtk_label_new ( null ) ;
	gtk_box_pack_start ( vbox2, wimp -> big_audio_label, no, no, 2 ) ;
	
	g_signal_connect_after ( zoom_x, "change-value", wimp_gui_enhancements_handle_zoom_x, wimp ) ;
	g_signal_connect_after ( zoom_y, "change-value", wimp_gui_enhancements_handle_zoom_y, wimp ) ;

	g_signal_connect_after ( aspect_ratio, "changed", wimp_gui_enhancements_handle_aspect_ratio, wimp ) ;
	g_signal_connect_after ( tv_mode, "toggled", wimp_gui_enhancements_handle_tv_mode, wimp ) ;

	out ;
	return notebook ;
}

gboolean wimp_gui_enhancements_handle_amp_level ( GtkRange * range, GtkScrollType scroll, gdouble value, WimpGUI * gui )
{
	in ;
	
	gchar * markup = g_strdup_printf ( "<span font_desc=\"36\"><b>%.0f</b></span>", value ) ;
	gtk_label_set_markup ( gui -> big_audio_label, markup ) ;
	g_free ( markup ) ;
	
	xine_set_param ( gui -> xine -> stream, XINE_PARAM_AUDIO_AMP_LEVEL, value ) ;
	gui -> enhancements_set -> amp_level = value ;
	
	out ;
}	

gboolean wimp_gui_enhancements_handle_comp_level ( GtkRange * range, GtkScrollType scroll, gdouble value, WimpGUI * gui )
{
	in ;
	g_debug ( "%f", value ) ;
	
	gchar * markup = g_strdup_printf ( "<span font_desc=\"36\"><b>%.0f</b></span>", value ) ;
	gtk_label_set_markup ( gui -> big_audio_label, markup ) ;
	g_free ( markup ) ;
	xine_set_param ( gui -> xine -> stream, XINE_PARAM_AUDIO_COMPR_LEVEL, value ) ;
	gui -> enhancements_set -> comp_level = value ;
	
	out ;
}	

gboolean wimp_gui_enhancements_handle_mute ( GtkWidget * button, WimpGUI * gui )
{
	in ;
	xine_set_param ( gui -> xine -> stream, XINE_PARAM_AUDIO_MUTE, gtk_toggle_button_get_active ( button )) ;
	
	out ;
}	

gboolean wimp_gui_enhancements_handle_amp_mute ( GtkWidget * button, WimpGUI * gui )
{
	in ;
	xine_set_param ( gui -> xine -> stream, XINE_PARAM_AUDIO_AMP_MUTE, gtk_toggle_button_get_active ( button )) ;
	
	out ;
}	

gboolean wimp_gui_enhancements_handle_brightness ( GtkRange * range, GtkScrollType scroll, gdouble value, WimpGUI * gui )
{
	in ;
	if ( value > MAX || value < 0 )
		return ;
		
	xine_set_param ( gui -> xine -> stream, XINE_PARAM_VO_BRIGHTNESS, value ) ;
	
	gui -> enhancements_set -> brightness = value ;
	out ;
}	

gboolean wimp_gui_enhancements_handle_contrast ( GtkRange * range, GtkScrollType scroll, gdouble value, WimpGUI * gui )
{
	in ;
	if ( value > MAX || value < 0 )
		return ;
		
	xine_set_param ( gui -> xine -> stream, XINE_PARAM_VO_CONTRAST, value ) ;
	
	gui -> enhancements_set -> contrast = value ;
	out ;
}	

gboolean wimp_gui_enhancements_handle_hue ( GtkRange * range, GtkScrollType scroll, gdouble value, WimpGUI * gui )
{
	in ;
	if ( value > MAX || value < 0 )
		return ;
		
	xine_set_param ( gui -> xine -> stream, XINE_PARAM_VO_HUE, value ) ;
	
	gui -> enhancements_set -> hue = value ;
	out ;
}	

gboolean wimp_gui_enhancements_handle_saturation ( GtkRange * range, GtkScrollType scroll, gdouble value, WimpGUI * gui )
{
	in ;
	if ( value > MAX || value < 0 )
		return ;
		
	xine_set_param ( gui -> xine -> stream, XINE_PARAM_VO_SATURATION, value ) ;
	
	gui -> enhancements_set -> saturation = value ;
	out ;
}	

gboolean wimp_gui_enhancements_handle_zoom_x ( GtkRange * range, GtkScrollType scroll, gdouble value, WimpGUI * gui )
{
	in ;

	if ( value > 10 )
		xine_set_param ( gui -> xine -> stream, XINE_PARAM_VO_ZOOM_X, value ) ;
	
	out ;
}	

gboolean wimp_gui_enhancements_handle_zoom_y ( GtkRange * range, GtkScrollType scroll, gdouble value, WimpGUI * gui )
{
	in ;
	if ( value > 10 )
		xine_set_param ( gui -> xine -> stream, XINE_PARAM_VO_ZOOM_Y, value ) ;
	
	out ;
}	

gboolean wimp_gui_enhancements_handle_tv_mode ( GtkWidget * button, WimpGUI * gui )
{
	in ;
	xine_set_param ( gui -> xine -> stream, XINE_PARAM_VO_TVMODE, gtk_toggle_button_get_active ( button )) ;
	
	out ;
}	

gboolean wimp_gui_enhancements_handle_aspect_ratio ( GtkWidget * button, WimpGUI * gui )
{
	in ;
	
	gint ratio = gtk_combo_box_get_active ( button ) ;
	xine_set_param ( gui -> xine -> stream, XINE_PARAM_VO_ASPECT_RATIO, ratio ) ;
	
	out ;
}	

Equalizer * wimp_gui_enhancements_equalizer ( WimpGUI * gui, gint band, GMemChunk * chunk )
{
	in ;
	
	Equalizer * eq = g_chunk_new ( Equalizer, chunk ) ;
	eq -> band = band ;
	
	eq -> scale = gtk_vscale_new ( null ) ;
	gtk_range_set_range ( eq -> scale, -20.0, 20.0 ) ;
	
	gtk_range_set_increments ( eq -> scale, 1, 10 ) ;
	gtk_scale_set_draw_value ( eq -> scale, no ) ;
	
	gtk_range_set_inverted ( eq -> scale, yes ) ;
	
	eq -> gui = gui ;
	g_signal_connect_after ( eq -> scale, "change-value", wimp_gui_enhancements_equalizer_handler, eq ) ;
	
	out ;
	return eq ;

}

gboolean wimp_gui_enhancements_equalizer_handler ( GtkWidget * widget, GtkScrollType type, gdouble value, Equalizer * eq )
{
	in ;
	xine_set_param ( eq -> gui -> xine -> stream, eq -> band, value ) ;
	
	eq -> gui -> enhancements_set -> eq [eq -> band] = value ;
	
	g_debug ( "Band %d set to %f", eq -> band, value ) ;
	out ;
}

void wimp_gui_enhancements_init_from_engine ( WimpGUI * gui )
{
	in ;
	
	gint i = 0, length = g_list_length ( gui -> eq ) ;
	for ( i = 0 ; i < length ; i ++ )
	{
		gtk_range_set_value ( g_list_nth_data ( gui -> eq, i ), xine_get_param ( gui -> xine -> stream, i + 18 )) ;
	}
	
	gtk_range_set_value ( gui -> brightness, xine_get_param ( gui -> xine -> stream, XINE_PARAM_VO_BRIGHTNESS )) ;
	gtk_range_set_value ( gui -> contrast, xine_get_param ( gui -> xine -> stream, XINE_PARAM_VO_CONTRAST )) ;
	gtk_range_set_value ( gui -> hue, xine_get_param ( gui -> xine -> stream, XINE_PARAM_VO_HUE )) ;
	gtk_range_set_value ( gui -> saturation, xine_get_param ( gui -> xine -> stream, XINE_PARAM_VO_SATURATION )) ;

	gtk_range_set_value ( gui -> amp_level, xine_get_param ( gui -> xine -> stream, XINE_PARAM_AUDIO_AMP_LEVEL )) ;
	gtk_range_set_value ( gui -> comp_level, xine_get_param ( gui -> xine -> stream, XINE_PARAM_AUDIO_COMPR_LEVEL )) ;

	gtk_toggle_button_set_active ( gui -> mute, xine_get_param ( gui -> xine -> stream, XINE_PARAM_AUDIO_MUTE )) ;
	gtk_toggle_button_set_active ( gui -> amp_mute, xine_get_param ( gui -> xine -> stream, XINE_PARAM_AUDIO_AMP_MUTE )) ;
	
	gtk_range_set_value ( gui -> zoom_x, xine_get_param ( gui -> xine -> stream, XINE_PARAM_VO_ZOOM_X )) ;
	gtk_range_set_value ( gui -> zoom_y, xine_get_param ( gui -> xine -> stream, XINE_PARAM_VO_ZOOM_Y )) ;

	gtk_range_set_value ( gui -> volume, xine_get_param ( gui -> xine -> stream, XINE_PARAM_AUDIO_VOLUME )) ;
	out ;
}

void wimp_gui_enhancements_init ( WimpGUI * gui )
{
	in ;
	
	gint i = 0, length = g_list_length ( gui -> eq ) ;
	for ( i = 0 ; i < length ; i ++ )
	{
		gtk_range_set_value ( g_list_nth_data ( gui -> eq, i ), gui -> enhancements_set -> eq [i] ) ;
	}
	
	gtk_range_set_value ( gui -> brightness, gui -> enhancements_set -> brightness ) ;
	gtk_range_set_value ( gui -> contrast, gui -> enhancements_set -> contrast ) ;
	gtk_range_set_value ( gui -> hue, gui -> enhancements_set -> hue ) ;
	gtk_range_set_value ( gui -> saturation, gui -> enhancements_set -> saturation ) ;

	gtk_range_set_value ( gui -> amp_level, gui -> enhancements_set -> amp_level ) ;
	gtk_range_set_value ( gui -> comp_level, gui -> enhancements_set -> comp_level ) ;

	gtk_toggle_button_set_active ( gui -> mute, gui -> enhancements_set -> amp_mute ) ;
	gtk_toggle_button_set_active ( gui -> amp_mute, gui -> enhancements_set -> mute ) ;
	
	gtk_range_set_value ( gui -> zoom_x, xine_get_param ( gui -> xine -> stream, XINE_PARAM_VO_ZOOM_X )) ;
	gtk_range_set_value ( gui -> zoom_y, xine_get_param ( gui -> xine -> stream, XINE_PARAM_VO_ZOOM_Y )) ;

	gtk_range_set_value ( gui -> volume, xine_get_param ( gui -> xine -> stream, XINE_PARAM_AUDIO_VOLUME )) ;
	out ;
}

void wimp_gui_enhancements_save ( WimpGUI * gui )
{
	in ;
	
	gchar * config = g_build_filename ( g_get_home_dir (), ".wimp", "wimp.cfg", null ) ;
	GKeyFile * file = g_key_file_new () ;

	GError * error = null ;
	g_key_file_load_from_file ( file, config, G_KEY_FILE_NONE, & error ) ;
	
	if ( error )
	{
		g_warning ( "Cannot open config file: %s", error -> message ) ;
		return ;
	}
	
	gchar * s = null ;
	gint i = 0 ;
	
	for ( i = 0 ; i < 10 ; i ++ )
	{
		s = g_strdup_printf ( "band%d", i ) ;
		g_key_file_set_integer ( file, "eq", s, gui -> enhancements_set -> eq [i] ) ;
		
		g_free ( s ) ;
	}
	
	g_key_file_set_integer ( file, "video", "brightness", gui -> enhancements_set -> brightness ) ;
	g_key_file_set_integer ( file, "video", "contrast", gui -> enhancements_set -> contrast ) ;
	g_key_file_set_integer ( file, "video", "hue", gui -> enhancements_set -> hue ) ;
	g_key_file_set_integer ( file, "video", "saturation", gui -> enhancements_set -> saturation ) ;

	g_key_file_set_integer ( file, "audio", "amp_level", gui -> enhancements_set -> amp_level ) ;
	g_key_file_set_integer ( file, "audio", "comp_level", gui -> enhancements_set -> comp_level ) ;

	File * fp = fopen ( config, "w" ) ;
	if ( fp == null )
	{
		g_warning ( "Cannot open config: %s", strerror ( errno )) ;
		g_free ( config ) ;
		
		g_key_file_free ( file ) ;
		return ;
	}

	gchar * data = g_key_file_to_data ( file, null, null ) ;
	fprintf ( fp, "%s", data ) ;
	
	fclose ( fp ) ;
	g_free ( config ) ;
	
	g_key_file_free ( file ) ;
	out ;
}

void wimp_gui_enhancements_load ( WimpGUI * gui )
{
	in ;
	
	gchar * config = g_build_filename ( g_get_home_dir (), ".wimp", "wimp.cfg", null ) ;
	GKeyFile * file = g_key_file_new () ;

	GError * error = null ;
	g_key_file_load_from_file ( file, config, G_KEY_FILE_NONE, & error ) ;
	
	if ( error )
	{
		g_warning ( "Cannot open config file: %s", error -> message ) ;
		return ;
	}
	
	gchar * s = null ;
	gint i = 0 ;
	
	for ( i = 0 ; i < 10 ; i ++ )
	{
		s = g_strdup_printf ( "band%d", i ) ;
		gui -> enhancements_set -> eq [i] = g_key_file_get_integer ( file, "eq", s, null ) ;
		
		g_free ( s ) ;
	}
	
	gui -> enhancements_set -> brightness = g_key_file_get_integer ( file, "video", "brightness", null ) ;
	gui -> enhancements_set -> contrast = g_key_file_get_integer ( file, "video", "contrast", null ) ;
	gui -> enhancements_set -> hue = g_key_file_get_integer ( file, "video", "hue", null ) ;
	gui -> enhancements_set -> saturation = g_key_file_get_integer ( file, "video", "saturation", null ) ;

	gui -> enhancements_set -> amp_level = g_key_file_get_integer ( file, "audio", "amp_level", null ) ;
	gui -> enhancements_set -> comp_level = g_key_file_get_integer ( file, "audio", "comp_level", null ) ;

	g_free ( config ) ;
	g_key_file_free ( file ) ;
	
	out ;
}
