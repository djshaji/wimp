/*
 *      xine.h
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


#ifndef __XINE_H
#define __XINE_H

#include <gtk/gtk.h>
#include <xine.h>

#include <X11/Xlib.h>
#include "definitions.c"

#include "config.h"
#define INPUT_MOTION (ExposureMask | ButtonPressMask | KeyPressMask | \
                      ButtonMotionMask | StructureNotifyMask |        \
                      PropertyChangeMask | PointerMotionMask)


typedef struct _Xine {
	xine_t * xine ;
	xine_stream_t * stream ;
	
	Config * config ;
	
	xine_audio_port_t * ao ;
	xine_video_port_t * vo ;
	
	x11_visual_t visual ;
	xine_event_queue_t * event_queue ;
	
	xine_osd_t * osd ;
	gint64 osd_shown, vpts ;
	
	Display * display ;
	
	gint screen ;
	Window xwindow ;
	
	gdouble pixel_aspect ;
	gint width, height, xpos, ypos,
		 x_offset, y_offset ;

	GMemChunk * chunk ;
	
	/* Now, this is for the internal seeking mechanism. We
	*  don't want to rely too much on this though, or
	*  there might be ambiguity in the seek process */
	
	gint32 pos_time, pos_stream, paused_at, length ;
	glong timestamp ;
} Xine ;

Xine * wimp_xine_new ( void ) ;
void wimp_xine_destroy ( Xine * xine ) ;

void wimp_xine_engine_init ( Xine * xine ) ;
void wimp_xine_engine_init_x11 ( Xine * xine, GtkWidget * video_display ) ;

void wimp_xine_load ( Xine * xine, gchar * filename ) ;
void wimp_xine_cleanup ( Xine * xine ) ;
#endif /* __XINE_H */
