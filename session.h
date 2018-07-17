/*
 *      session.h
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


#ifndef __SESSION_H
#define __SESSION_H

#include <gtk/gtk.h>
#include "definitions.c"

#include "gui.h"
#include "xine.h"

typedef enum _Action {
	ActionOpen = 1,
	ActionEnqueue = 2,
	ActionClose = 3,
	ActionNext = 4,
	ActionPrev = 5,
	ActionPlay = 6,
	ActionPause = 7,
	ActionStop = 8,
	ActionUnknown = 9
} Action ;

#define SIGEVENT 35

void wimp_log_pid ( void ) ;
gint wimp_get_pid ( void ) ;
void wimp_session_send_event ( gchar * filename, Action action ) ;

#endif
