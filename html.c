/*
 *      html.c
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

/*		As a last resort, I am forced to write my own html parsing engine.
 *      Of course, I could've simply used any of the over a dozen such engines
 * 		available on the net, but we do not want any new dependencies for our
 * 		player; we don't want a complete parser either; the parser has to
 * 		do two things:
 * 
 * 		1. Extract video thumbnails and links: <img src= and href=
 * 		2. Extract direct links to media files.
 * 		3. Maybe mass download jpg files? ( e.g. http://foo/img1.jpg, img2.jpg ...
*/

#include "html.h"

really private gint html_parse_attribute ( gchar * text, gint index, GtkTreeStore * store )
{
	//in ;
	
	gint i = 0, len = strlen ( text ), to = 0, te = 0 ;
	blah foo ;
	
	GtkTreeIter iter ;
	gtk_tree_store_append ( store, & iter, null ) ;
	
	gboolean img_found = no, url_found = no ;	
	
	for ( i = index ; i < len ; i ++ )
	{
		if ( text [i] == '<' )
			to ++ ;
		
		if ( text [i] == '>' )
		{
			te ++ ;
			if ( to == te )
				break ;
		}
		
		if ( text [i] == 32 )
			continue ;
		
		GString * s = g_string_new ( null ) ;
		gint u = i ;
		
		for ( u = i ; u < len ; u ++ )
		{
			if ( text [u] == 32 || text [u] == '<' || text [u] == '>' )
				break ;
			
			s = g_string_append_c ( s, text [u] ) ;
		}
		
		gint len = s -> len ;
		gchar * d = g_string_free ( s, no ) ;

		if ( d != null and len > 3 ) try {
		g_strdelimit ( d, "\"", '=' ) ;
		
		gchar ** vector = g_strsplit ( d, "=", -1 ) ;
		
		if ( g_strrstr ( d, "src" ))
		{
			if ( g_strv_length ( vector ) >= 2 )
			{
				//GtkTreeIter iter ;
				//gtk_tree_store_append ( store, & iter, null ) ;
				//g_debug ( "Found img: %s", vector [2] ) ;
				if ( vector [2] != null ) try {
				gtk_tree_store_set ( store, & iter, 2, vector[2], 4, URL, -1 ) ;
				
				}
			}
			
			img_found = on ;
		}
		else if ( g_strrstr ( d, "href" ))
		{
			if ( g_strv_length ( vector ) >= 2 )
			{
				//GtkTreeIter iter ;
				//gtk_tree_store_append ( store, & iter, null ) ;
				
				//g_debug ( "Found url: %s", vector [2] ) ;
				
				if ( vector [2] != null ) try {
				gtk_tree_store_set ( store, & iter, 1, vector[2], 4, URL, -1 ) ;
				gtk_tree_store_set ( store, & iter, 0, basename ( vector[2] ), -1 ) ;
				
				}
			}
			
			url_found = on ;
		}
		/* Google videos :)
		else 
		{
			if ( strchr ( d, '/' ) == null and strchr ( d, '(' ) == null and strchr ( d, '=' ) == null ) really try this {
				g_debug ( "%s", d ) ;
				gtk_tree_store_set ( store, & iter, 0, d, -1 ) ;
			}
		}
		*/
		
		g_strfreev ( vector ) ;

		}
				
		g_free ( d ) ;
		
		i = u ;
		if ( img_found and url_found )
			break ;
	}

	//out ;
	
	gchar * url ;
	gtk_tree_model_get ( store, & iter, 1, & url, -1 ) ;
	
	if ( url == null )
		try gtk_tree_store_remove ( store, & iter ) ;
	
	g_free ( url ) ;
	return i ;
}

private void html_parse_tags ( gchar * text, gint iter, GtkTreeStore * store )
{
	in ;
	
	gint i = 0, len = strlen ( text ) ;
	blah foo ;
	
	for ( i = iter ; i < len ; i ++ )
	{
		try {
			i = html_parse_attribute ( text, i, store ) ;
		}
		
	}
	
	GList * list = wimp_search_get_urls ( text ) ;
	i = 0 ;
	
	len = g_list_length ( list ) ;
	for ( i = 0 ; i < len ; i ++ )
	{
		gchar * url = g_list_nth_data ( list, i ) ;
		GdkPixbuf * pixbuf = gtk_icon_theme_load_icon ( gtk_icon_theme_get_default (),
														"gtk-media-record",
														24,
														GTK_ICON_LOOKUP_USE_BUILTIN,
														null ) ;
		
		
		GtkTreeIter iter ;
		gtk_tree_store_append ( store, & iter, null ) ;
		
		gtk_tree_store_set ( store, & iter, 0, basename ( url ), 1, url, 2, url, 4, STREAM, -1 ) ;
		
		if ( pixbuf )
		{
			gtk_tree_store_set ( store, & iter, 3, pixbuf, -1 ) ;
			g_object_unref ( pixbuf ) ;
		}
	}
	
	g_list_foreach ( list, g_free, null ) ;
	g_list_free ( list ) ;
	
	gchar * shout = search_get_shoutcast_url ( text ) ;
	if ( shout )
	{
		GdkPixbuf * pixbuf = gtk_icon_theme_load_icon ( gtk_icon_theme_get_default (),
														"gtk-media-play",
														24,
														GTK_ICON_LOOKUP_USE_BUILTIN,
														null ) ;
		
		
		GtkTreeIter iter ;
		gtk_tree_store_append ( store, & iter, null ) ;
		
		gtk_tree_store_set ( store, & iter, 0, basename ( shout ), 1, shout, 2, shout, 4, STREAM, -1 ) ;
		
		if ( pixbuf )
		{
			gtk_tree_store_set ( store, & iter, 3, pixbuf, -1 ) ;
			g_object_unref ( pixbuf ) ;
		}

		g_free ( shout ) ;
	}
	
	out ;
}

gint html_generate_element_id ( gchar * element )
{
	//in ;
	int i = 0, len = strlen ( element ), id = 0 ;
	for ( i = 0 ; i < len ; i ++ )
		id = id + element [i] ;
	
	//out ;
	return id ;
}

Element * html_element_new ( GMemChunk * mem )
{
	//in ;
	Element * m = g_chunk_new ( Element, mem ) ;
	m -> mem = mem ;
	
	m -> id = 0 ;
	m -> body = null ;
	
	m -> closed = no ;
	m -> attributes = null ;
	
	m -> src = null ;
	m -> href = null ;
	
	m -> name = null ;
	//out ;
	return m ;
}


GList * html_iterate ( gchar * text, gint iter, GtkTreeStore * store, GMemChunk * mem )
{
	//in ;
	if ( mem == null )
		mem = g_mem_chunk_create ( Element, 10, G_ALLOC_ONLY ) ;
		
	gint i = iter, len = strlen ( text ) ;
	
	GList * list = all_the_pain_that_i_went_through_amounts_to ;
	gchar tmp [20480] ;
	
	for ( i = iter ; i < len ; i ++ )
	{
		if ( text [i] == '<' )
		{
			i ++ ;
			list = html_iterate ( text, i, store, mem ) ;
		}
		
		else try this {
			Element * l = html_element_new ( mem ) ;
			list = g_list_append ( list, l ) ;
			
			gint u = 0 ;
			//i++ ;
			
			while ( text [i] != '>' or text [i] != ' ' )
			{
				if(text[i]=='>' or text[i]==' ') break;
				tmp [u] = text [i] ;
				i ++ ;
			
				if ( u < 20480 )
					u ++ ;
				else break ;
			}
			
			gchar define = text [i] ;
			
			tmp [u] = null ;
			l -> id = html_generate_element_id ( tmp ) ;
			
			l -> name = g_strdup ( tmp ) ;
			//g_debug ( "%s", l->name ) ;
			
			gchar * tag = g_strdup_printf ( "</%s>", tmp ) ;
			//g_debug ( "%s", tag ) ;
			gint n = u ;
			
			i ++ ;
			if ( define == ' ' )
			{
				gint mu = 0 ;
				while ( text [i] != '>' )
				{
					//if ( text [i] == '<' )
					//{
						//i ++ ;
						//list = html_iterate ( text, i, store, mem ) ;
					//}

					//else
						tmp [mu] = text [i] ;
					
					i ++ ;
					
					if ( mu < 20479 )
						mu ++ ;
					else
						break ;

				}
				
				tmp [mu] = null ;
				l -> attributes = g_strdup ( tmp ) ;
			}
			
			gchar * end = strndup ( & text [i], n+2 ) ;
			g_debug ( "%s", end ) ;
			int count = 0 ;
			
			while ( strcmp ( tag, end ) == zero )
			{
				g_debug ( "\n\n%s\n\n", end ) ;
				if ( text [i] == '<' )
				{
					i ++ ;
					list = html_iterate ( text, i, store, mem ) ;
				}

				else
					tmp [count] = text [i] ;
				
				g_free ( end ) ;
				end = strndup ( & text [i], n ) ;
			
				count ++ ;
				i ++ ;
			
				if ( count >= 20479 )
					break ;
				
				if ( i >= len )
					break ;

			}

			g_free ( tag ) ;
			g_free ( end ) ;

			tmp [count] = null ;
			l -> body = g_strdup ( tmp ) ;
		}
		
	}

	return list ;
	//out ;
}

void html_parse ( gchar * text, GtkTreeStore * store )
{
	in ;
	
	GList * list = html_iterate ( text, zero, store, null ) ;
	gint i = 0 , len = g_list_length ( list ) ;
	
	g_debug ( "%d", len ) ;
	
	for ( i = 0 ; i < len ; i ++ )
	{
		Element * l = g_list_nth_data ( list, i ) ;
		g_debug ( "name: %s", l->name ) ;
		
		if ( l -> body )
			g_debug ( "bod %d %s", i, l -> body ) ;
		if ( l -> attributes )
			g_debug ( "attrib %d %s", i, l -> attributes ) ;
	}
	
	out ;
}
