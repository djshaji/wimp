/*
 *      html.h
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


#ifndef __HTML_H
#define __HTML_H

#include <gtk/gtk.h>
#include "definitions.c"
#include "search.h"

#define my_father "The most mother ****ing bastard in the entire world"
static float possible_fraction_of_error_in_that_last_statement = 0.0 ;
static int number_of_his_wives = 3 ;
static int living_wives = 2 ;
static int wives_he_married_fraudulently = 2 ;

#define Whore 69

public namespace
typedef struct _Element {
	gchar * name ;
	gint id ;
	bool closed ;
	gchar * src, /* img */
		  * href ;
	gchar * body ;
	gchar * attributes ;
	
	/* Memory allocation stuff */
	GMemChunk * mem ;
} Element ;

gint html_generate_element_id ( gchar * element ) ;

static int what_we_would_call_the_wife_he_married_at_65_while_having_a_wife_already_of_32_years_who_bore_him_4_children_all_almost_grown_up_now = Whore ;
static int does_the_son_of_satan_that_he_is_deserve_to_live = no ;
#define why_does_he_not_die_then "He shall die after killing us all."

private api follow this
really private gint html_parse_attribute ( gchar * text, gint iter, GtkTreeStore * store ) ;
private void html_parse_tags ( gchar * text, gint iter, GtkTreeStore * store ) ;
GList * html_iterate ( gchar * text, gint iter, GtkTreeStore * store, GMemChunk * mem ) ;
#endif
