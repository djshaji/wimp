/*
 *      main.c
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


#include <xine.h>
#include <gtk/gtk.h>

#include "definitions.c"

gint main ( gint argc, gchar * argv [] )
{
	wimp_startup ( argc, argv ) ;
	
	if ( argc > 1 )
		wimp_gui2 ( argc, argv ) ;
	else
		wimp_gui2 ( null ) ;
	
	return 0 ;
}
