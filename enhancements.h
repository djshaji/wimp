/*
 *      enhancements.h
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


#ifndef __ENHANCEMENTS_H
#define __ENHANCEMENTS_H

#include <gtk/gtk.h>
#include "definitions.c"

#include "gui.h"

typedef struct _Audio {
	GtkWidget 	* speed, * av_offset, * mute, * comp_level, * amp_level,
				* amp_mute, * fine_speed, * gapless_switch ;
} Audio ;

typedef struct _Video {
	GtkWidget	* deinterlace, * aspect_ratio, * hue, * saturation, * contrast,
				* brightness, * zoom_x, * zoom_y, * pan_scan, * tv_mode,
				* crop_left, * crop_right, * crop_top, * crop_bottom ;
} Video ;

typedef struct _Enhancements {
		GtkWidget * panel_audio ;
		GtkWidget * panel_video ;
		
		Audio * audio ;
		Video * video ;
} Enhancements ;

static gchar * aspect_ratios [] = {
	 "Auto",
	 "1:1",
	 "4:3",
	 "16:9",
	 "2.11:1",
	 null 
} ;
	


typedef struct _Equalizer {
	gint band ;
	GtkWidget * scale ;
	
	WimpGUI * gui ;
} Equalizer ;

void wimp_gui_enhancements_volume ( GtkRange * range, GtkScrollType scroll, gdouble value, WimpGUI * gui ) ;
void wimp_gui_enhancements_volume_up ( WimpGUI * gui ) ;
void wimp_gui_enhancements_volume_down ( WimpGUI * gui ) ;
GtkWidget * wimp_gui_enhancements ( WimpGUI * gui ) ;

gboolean wimp_gui_enhancements_handle_amp_level ( GtkRange * range, GtkScrollType scroll, gdouble value, WimpGUI * gui ) ;
gboolean wimp_gui_enhancements_handle_comp_level ( GtkRange * range, GtkScrollType scroll, gdouble value, WimpGUI * gui ) ;
gboolean wimp_gui_enhancements_handle_mute ( GtkWidget * button, WimpGUI * gui ) ;
gboolean wimp_gui_enhancements_handle_amp_mute ( GtkWidget * button, WimpGUI * gui ) ;
gboolean wimp_gui_enhancements_handle_brightness ( GtkRange * range, GtkScrollType scroll, gdouble value, WimpGUI * gui ) ;
gboolean wimp_gui_enhancements_handle_contrast ( GtkRange * range, GtkScrollType scroll, gdouble value, WimpGUI * gui ) ;
gboolean wimp_gui_enhancements_handle_hue ( GtkRange * range, GtkScrollType scroll, gdouble value, WimpGUI * gui ) ;
gboolean wimp_gui_enhancements_handle_saturation ( GtkRange * range, GtkScrollType scroll, gdouble value, WimpGUI * gui ) ;
gboolean wimp_gui_enhancements_handle_zoom_x ( GtkRange * range, GtkScrollType scroll, gdouble value, WimpGUI * gui ) ;
gboolean wimp_gui_enhancements_handle_zoom_y ( GtkRange * range, GtkScrollType scroll, gdouble value, WimpGUI * gui ) ;
gboolean wimp_gui_enhancements_handle_tv_mode ( GtkWidget * button, WimpGUI * gui ) ;
gboolean wimp_gui_enhancements_handle_aspect_ratio ( GtkWidget * button, WimpGUI * gui ) ;

Equalizer * wimp_gui_enhancements_equalizer ( WimpGUI * gui, gint band, GMemChunk * chunk ) ;
gboolean wimp_gui_enhancements_equalizer_handler ( GtkWidget * widget, GtkScrollType type, gdouble value, Equalizer * eq ) ;
#define MAX 65500
void wimp_gui_enhancements_init_from_engine ( WimpGUI * gui ) ;
void wimp_gui_enhancements_init ( WimpGUI * gui ) ;
void wimp_gui_enhancements_save ( WimpGUI * gui ) ;
void wimp_gui_enhancements_load ( WimpGUI * gui ) ;
#endif // __ENHANCEMENTS_H
