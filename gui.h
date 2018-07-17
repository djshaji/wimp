/*
 *      gui.h
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


#ifndef __WIMP_GUI_H
#define __WIMP_GUI_H

#include <gtk/gtk.h>
#include <xine.h>

#include <X11/Xlib.h>
#include <gdk/gdkx.h>
#include <gdk/gdkkeysyms.h>

#include <sys/mman.h>

#include "definitions.c"
#include "xine.h"

#include "util.h"
#include "config.h"

static gchar * thumbnail_sizes [] = {
	"32",
	"48",
	"64",
	"96",
	"128",
	"160",
	"192",
	"224",
	"256",
	null
} ;

typedef struct _WimpEnhancements {
	gint 
		amp_level,
		comp_level,
		contrast,
		saturation,
		brightness,
		hue,
		zoom_x,
		zoom_y,
		eq [10] ;
	
	gboolean mute,
			 amp_mute ;
} WimpEnhancements ;

typedef enum _WimpMode {
	MDI,
	TOPLEVEL,
	NONE
} WimpMode ;

typedef struct _WimpGUI  {
	WimpMode mode ;
	GtkWindow * window ;
	GtkWidget * video_display ;
	
	gboolean fullscreen, controls_visible ;
	GtkWidget * browser ;
	
	GList * history ;
	GtkToolButton * search_stop ;

	GtkWidget * search_entry ;
	GtkWidget * search_bar ;
	
	GtkWidget * playlist ;
	GtkTreeModel * store ;
	
	GtkListStore * model ;
	GtkStatusbar * status ;
	
	GtkTreeSelection * selection ;
	gint index ;
	
	GtkToolbar * toolbar, * mode_selector ;
	GtkPaned * pane1, * pane2, * pane3 ;
	
	GMemChunk * chunk ;
	Xine * xine ;
	
	GtkWidget * enhancements ;
	GtkToggleButton * play ;

	GtkToggleToolButton * fs ;
	GtkWidget * seekbar, * volume ;
	
	GList * eq ;
	GtkWidget 	* mute,
				* amp_level,
				* comp_level,
				* amp_mute ;
	
	GtkWidget 	* contrast,
				* saturation,
				* brightness,
				* hue ;
		
	GtkWidget 	* aspect_ratio,
				* zoom_x,
				* zoom_y,
				* tv_mode ;
	
	GtkWidget * big_audio_label ;
	gint paned1_pos, paned2_pos, paned3_pos ;
	
	gint width, height ;
	gint event_id ;
	
	gboolean compatibility ;
	WimpEnhancements * enhancements_set ;
	
	GtkToggleButton * shuffle, * repeat ;
	GtkDialog * controls_window, * playlist_window ;
	GtkWidget * main_window ;
	GtkWidget * search ;

	gboolean search_show_thumbnails ;
} WimpGUI ;

#include "file_manager.h"
WimpGUI * my_wimp ;

GtkWidget * wimp_gui_init ( WimpGUI * wimp ) ;

void wimp_gui ( gint argc, gchar * argv [] ) ;
WimpGUI * wimp_gui_new ( void ) ;
void wimp_gui_destroy ( WimpGUI * wimp ) ;

void wimp_xine_vo_init_x11 ( Xine * xine, GtkWidget * video_display ) ;

static void dest_size_cb ( Xine * data, int video_width, int video_height, double video_pixel_aspect,
                         int *dest_width, int *dest_height, double *dest_pixel_aspect) ;

static void frame_output_cb ( Xine * data, int video_width, int video_height,
                            double video_pixel_aspect, int *dest_x, int *dest_y,
                            int *dest_width, int *dest_height, 
                            double *dest_pixel_aspect, int *win_x, int *win_y) ;


void wimp_xine_x11_expose ( WimpGUI * wimp ) ;
void wimp_xine_x11_play ( WimpGUI * wimp, gint seek ) ;

void wimp_gui_adjust_video_display ( WimpGUI * gui ) ;
static inline void wimp_gui_update ( void ) ;

void wimp_gui_zoom_100 ( WimpGUI * gui ) ;

void wimp_gui_handle_video_resize ( WimpGUI * gui ) ;

void wimp_gui_fast_forward ( WimpGUI * gui ) ;
void wimp_gui_rewind ( WimpGUI * gui ) ;
void wimp_gui_toggle_pause ( WimpGUI * gui ) ;
void wimp_gui_stop1 ( GtkWidget * widget, GtkWidget * window ) ;
vodi wimp_gui_stop ( WimpGUI * gui ) ;
gboolean wimp_gui_seek ( GtkWidget * widget, GtkScrollType * scroll, gdouble seek, WimpGUI * gui ) ;
gboolean wimp_gui_toggle_fullscreen ( GtkToggleToolButton * button, WimpGUI * gui ) ;

void wimp_gui_fullscreen_show_controls ( WimpGUI * gui ) ;
void wimp_gui_fullscreen_hide_controls ( WimpGUI * gui ) ;

void wimp_gui_pause ( WimpGUI * gui ) ;
void wimp_gui_unpause ( WimpGUI * gui ) ;

gboolean wimp_gui_track_window_state ( 	GtkWidget * widget,
										GdkEventWindowState * event,
										WimpGUI * gui ) ;

gboolean wimp_gui_track_pointer ( GtkWidget * widget, GdkEventMotion * event, WimpGUI * gui ) ;
gboolean wimp_hotkeys_gui ( GtkWidget * widget, GdkEventKey * event, WimpGUI * gui ) ;
void wimp_gui_osd_message ( WimpGUI * gui, gchar * message ) ;
void wimp_gui_osd_indicate_position ( WimpGUI * gui ) ;

void wimp_gui_playlist_append ( WimpGUI * gui, gchar * filename ) ;
void wimp_gui_playlist_clear ( WimpGUI * gui ) ;
void wimp_gui_send_expose_event ( WimpGUI * gui ) ;
void wimp_cleanup ( WimpGUI * gui ) ;
gchar * wimp_prompt_filename ( WimpGUI * gui, GtkFileChooserAction action, gchar * dirname ) ;
gchar * wimp_xine_get_error ( WimpGUI * gui ) ;

static gchar * xine_meta_tags [] = {
	"Title",
	"Comment",
	"Artist",
	"Genre",
	"Album",
	"Year",
	"Video codec",
	"Audio codec",
	"System layer",
	"Input plugin",
	"CD Index / Disc ID",
	"Track number",
	"Composer",
	null
} ;

void wimp_gui_meta_info ( WimpGUI * gui ) ;
GtkMenu * wimp_gui_menu_create ( WimpGUI * gui ) ;
void wimp_gui_menu_popup ( GtkWidget * widget, GdkEventButton * event, WimpGUI * gui ) ;
void wimp_gui_fullscreen ( WimpGUI * gui ) ;
void wimp_about ( WimpGUI * gui ) ;

gboolean wimp_gui_seeker ( GtkWidget * widget, GdkEventButton * event, WimpGUI * gui ) ;
void wimp_xine_info ( WimpGUI * gui ) ;
#include "playlist.h"
#include "enhancements.h"
void wimp_gui_config ( WimpGUI * gui ) ;
void wimp_gui_config_set_vo ( GtkWidget * widget, WimpGUI * gui ) ;
void wimp_gui_config_set_ao ( GtkWidget * widget, WimpGUI * gui ) ;
void wimp_gui_config_reset ( WimpGUI * gui ) ;
void wimp_gui_config_set_emergency ( GtkWidget * widget, WimpGUI * gui ) ;
void wimp_gui_config_memlock ( GtkWidget * widget, WimpGUI * gui ) ;
WimpEnhancements * wimp_gui_enhancements_new ( void ) ;
void wimp_session_get_event ( WimpGUI * gui ) ;
void wimp_sigaction ( gint signum, siginfo_t * t, void * data ) ;
void wimp_gui_set_status ( WimpGUI * gui, gchar * message ) ;
void wimp_xine_event_listener ( WimpGUI * data, xine_event_t * event ) ;

#include "gui2.h"
#endif /* __WIMP_GUI_H */
