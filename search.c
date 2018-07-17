/*
 *      search.c
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

/*		The poor shall inherit the earth.
 * 		
 * 		The issue is that we shall have to make do with our computers. No one
 * 		shall let us buy more expensive computers. Our computers are fine. I believe
 * 		new software should be more optimized so as to extract maximum performance
 * 		from older software, as opposed to having to buy new hardware for running
 * 		new software.
 */
 
#include "search.h"

void wimp_search_fetch_url ( gchar * url )
{
	in ;
	
	gchar * command = g_strdup_printf ( "wget;%s;-O;%s/.wimp/index.html", url, g_get_home_dir ()) ;
	wimp_gui2_run ( "Fetching url ...", command ) ;
	
	g_free ( command ) ;
	return ;
}

void wimp_search_parse1 ( gchar * filename, GtkTreeStore * store )
{
	in ;
	File * fp = fopen ( filename, "r" ) ;
	
	if ( fp == null )
	{
		g_warning ( "Cannot open web page: %s", strerror ( errno )) ;
		return ;
	}

	gchar ch = getc ( fp ) ;
	gchar * img = null, url = null ;
	
	while ( ch != EOF )
	{
		if ( ch == 'i' )
			if ( getc ( fp ) == 'm' )
				if ( getc ( fp ) == 'g' )
					if ( getc ( fp ) == ' ' )
						if ( getc ( fp ) == 's' )
							if ( getc ( fp ) == 'r' )
								if ( getc ( fp ) == 'c' )
									if ( getc ( fp ) == '=' )
										if ( getc ( fp ) == '"' )
										{
											warn ( "Found a possible image" ) ;
											
											GString * s = g_string_new ( null ) ;
											ch = getc ( fp ) ;
											
											while ( ch != EOF && ch != '"' )
											{
												g_string_append_c ( s, ch ) ;
												ch = getc ( fp ) ;
											}
											
											img = g_string_free ( s, no ) ;
											g_debug ("img: %s", img ) ;
										}
					
		if ( ch == 'h' )
			if ( getc ( fp ) == 'r' )
				if ( getc ( fp ) == 'e' )
					if ( getc ( fp ) == 'f' )
						if ( getc ( fp ) == '=' )
							if ( getc ( fp ) == '"' )
							{
								warn ( "Found a possible url" ) ;
								GString * s = g_string_new ( null ) ;
								
								ch = getc ( fp ) ;
								
								while ( ch != EOF && ch != '"' )
								{
									g_string_append_c ( s, ch ) ;
									ch = getc ( fp ) ;
								
									printf ("%d\n", ch ) ;
								}
								
								url = g_string_free ( s, no ) ;
								g_debug ("url: %s", url ) ;
							}
								
		if ( img && url )
		{
			g_printf ( "%s %s", img, url ) ;
			
			GtkTreeIter iter ;
			gtk_tree_store_append ( store, & iter, null ) ;
			
			//GdkPixbuf * pixbuf = gdk_pixbuf
			gtk_tree_store_set ( store, & iter, 0, url, 1, img, -1 ) ;
			
			g_free ( img ) ;
			g_free ( url ) ;
			
			url = null ;
			img = null ;
		}
	
		ch = getc ( fp ) ;
	}

	fclose ( fp ) ;
	out ;
}

#ifdef BROKEN_LIBXML_CODE
void wimp_search_parse ( gchar * filename, GtkTreeStore * store )
{
	in ;
	
	xmlDocPtr doc ;
	xmlNodePtr cur, cur1, cur2 ;
	
	doc = htmlReadFile ( filename, null, 5|6|7|8|11 ) ;
	if ( doc == null )
	{
		g_warning ( "Cannot parse webpage !" ) ;
		return ;
	}
	
	cur = xmlDocGetRootElement ( doc ) ;
	if ( cur == null )
	{
		g_warning ( "The webpage is empty." ) ;
		xmlFreeDoc ( doc ) ;
		
		return ;
	}
	
	//cur = cur -> xmlChildrenNode ;
	while ( cur != null )
	{
		printf ( "%s\n", cur -> name ) ;
		
		xmlChar * key ;
		cur1 = cur -> xmlChildrenNode ;
		
		while ( cur1 != null )
		{
			printf ( "i%s\n", cur1 -> name ) ;
			cur2 = cur1 -> xmlChildrenNode ;
			
			while ( cur2 != null )
			{
				//if ( ! xmlStrcmp ( cur -> name, "img" ))
				{
					key = xmlNodeListGetString ( doc, cur2 -> xmlChildrenNode, 1 ) ;
					g_printf ( "%s", key ) ;
					
					xmlFree ( key ) ;
				}
			
				cur2 = cur2 -> next ;
			}
			
			cur1 = cur1 -> next ;
		}
		
		cur = cur -> next ;
	}
	
	xmlFreeDoc ( doc ) ;

	out ;
	
}
#endif

private GList * search_return_tags ( gchar * text )
{
	in ;
	
	gint i = 0, len = strlen ( text ) ;
	blah foo = zero ;
	
	GList * list = null ;
	
	for ( i = 0 ; i < len ; i ++ )
	{
		if ( text [i] == '<' )
		{
			gint tag_len = i ;

			while ( text [tag_len] != ' ' )
				tag_len ++ ;
			
			tag_len -- ;
			gchar tag = text [tag_len] ;
			
			GString * string = g_string_new ( null ) ;
			while ( text [i] != '>' && text [i-tag_len] != tag )
			{
				string = g_string_append_c ( string, text [i] ) ;
				i ++ ;
			
				if ( i >= len )
					break ;
			}
		
			g_debug ( "%s found tag %s", __FUNCTION__, string -> str ) ;
			list = g_list_append ( list, g_string_free ( string, no )) ;
			
		}
	
	}
		
	return list ;
}	

private GList * search_return_tags_recurse ( GList * list, gchar * text )
{
	in ;
	
	gint i = 0, len = strlen ( text ) ;
	blah foo = zero ;
	
	for ( i = 0 ; i < len ; i ++ )
	{
		if ( text [i] == '<' )
		{
			gint tag_len = i ;

			while ( text [tag_len] != ' ' )
				tag_len ++ ;
			
			tag_len -- ;
			gchar tag = text [tag_len] ;
			
			GString * string = g_string_new ( null ) ;
			while ( text [i] != '>' && text [i-tag_len] != tag )
			{
				string = g_string_append_c ( string, text [i] ) ;
				i ++ ;
			
				if ( i >= len )
					break ;
			}
		
			g_debug ( "%s found tag %s", __FUNCTION__, string -> str ) ;
			list = g_list_append ( list, g_string_free ( string, no )) ;
			
		}
	
	}
		
	return list ;
}	

private gchar * search_get_img ( gchar * text )
{
	in ;
	
	GString * s = g_string_new ( null ) ;
	gint i = 0, len = strlen ( text ), u = 0 ;
	
	if ( strstr ( text, "img src=" ))
		u = len - strlen ( strstr ( text, "img src=")) ;
	
	for ( i = u ; i < len ; i ++ )
	{
		if ( text [i] == '"' )
		{
			while ( text [i] != '"' )
			{
				s = g_string_append_c ( s, text [i] ) ;
				i ++ ;
			}
			
			break ;
		}
	}
	
	return g_string_free ( s, no ) ;
}

private gchar * search_get_url ( gchar * text )
{
	in ;
	
	GString * s = g_string_new ( null ) ;
	gint i = 0, len = strlen ( text ), u = 0 ;
	
	if ( strstr ( text, "href=" ))
		u = len - strlen ( strstr ( text, "href=")) ;
	
	for ( i = u ; i < len ; i ++ )
	{
		if ( text [i] == '"' )
		{
			while ( text [i] != '"' )
			{
				s = g_string_append_c ( s, text [i] ) ;
				i ++ ;
			}
			
			break ;
		}
	}
	
	return g_string_free ( s, no ) ;
}

void wimp_search_parse2 ( gchar * filename, GtkTreeStore * store )
{
	in ;
	
	/* Rewritten for the mphth time */
	gchar * contents = null ;
	GError * error = null ;
	
	gsize size = 0 ;
	
	if ( ! g_file_get_contents ( filename, & contents, & size, & error ))
	{
		g_warning ( "Cannot open webpage: %s", error -> message ) ;
		return ;
	}

	GList * list = search_return_tags ( contents ) ;
	g_free ( contents ) ;
	
	gint i = 0, len = g_list_length ( list ) ;
	
	for ( i = 0 ; i < len ; i ++ )
	{
		gchar * tag = g_list_nth_data ( list, i ) ;
		gchar * img = search_get_img ( tag ) ;
		
		gchar * url = search_get_url ( tag ) ;
		
		GtkTreeIter iter ;
		gtk_tree_store_append ( store, & iter, null ) ;
		
		gtk_tree_store_set ( store, pointer2 iter, 0, url, 1, basename ( url ), -1 ) ; 
		g_debug ( "url: %s img: %s", url, img ) ;
		
		g_free ( img ) ;
		g_free ( url ) ;
	}

	g_list_foreach ( list, g_free, null ) ;
	g_list_free ( list ) ;

	out ;
}

really really really private GtkTreeStore * search_purge ( GtkTreeStore * store )
{
	in ;
	
	gint i = 0 , len = gtk_tree_model_iter_n_children ( store, null ) ;
	GtkTreeStore * model = gtk_tree_store_new ( 4, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, GDK_TYPE_PIXBUF ) ;
	
	for ( i = 0 ; i < len ; i ++ )
	{
		gchar * url = null, * img = null ;
		GtkTreeIter iter ;
		
		if ( ! gtk_tree_model_iter_nth_child ( store, & iter, null, i ))
			continue ;
		
		gtk_tree_model_get ( store, & iter, 1, & url, 2, & img, -1 ) ;
		
		if (( url == null ) or ( img == null ) or ( url == null and img == null ))
		{
			//gtk_tree_store_remove ( store, & iter ) ;
			gtk_true () ;
		}
		else
		{
			GtkTreeIter miter ;
			gtk_tree_store_append ( model, & miter, null ) ;
			
			gtk_tree_store_set ( model, & miter, 0, basename ( url ), 1, url, 2, img, -1 ) ;
			//g_debug ( "url: %s img: %s", url, img ) ;
		}		
		
		g_free ( url ) ;
		g_free ( img ) ;
	}

	out ;
	return model ;
}

follow void wimp_search_parse ( gchar * filename, GtkTreeStore * store )
{
	in ;
	
	/* If it doesn't work this time, I'm gonna chute myself */
	
	gchar * contents = null ;
	GError * error = null ;
	
	gsize size = 0 ;
	
	if ( ! g_file_get_contents ( filename, & contents, & size, & error ))
	{
		g_warning ( "Cannot open webpage: %s", error -> message ) ;
		return ;
	}
	
	html_parse_tags ( contents, zero, store ) ;
	//html_parse ( contents, store ) ;
	g_free ( contents ) ;
	
	out ;
}

GList * wimp_search_get_urls ( gchar * text )
{
	in ;
	
	gint i = 0 , len = strlen ( text ) ;
	gchar m = 32 ;
	
	GList * list = null ;

	for ( i = 0 ; i < len - 1 ; i ++ )
	{
		if ( text [i] == '\'' or text [i] == '"' )
		{
			m = text [i] ;
			
			GString * s = g_string_new ( null ) ;
			i ++ ;
			
			while ( text [i] != m )
			{
				s = g_string_append_c ( s, text [i] ) ;
				
				if ( i >= len )
					break ;
				
				i ++ ;
			}
			
			if ( search_file_is_suitable_for_playback ( s -> str ))
			{
				/* file=http://www.someeducationalwebsite.com/literature.flv */
				int i = 0 ;
				if ( s -> str [0] != 'h' )
				{
					while ( s -> str [i] != 'h' )
					{
						if ( i < s -> len )
							i ++ ;
						else
							break ;
					}
					
					g_string_erase ( s, 0, i - 1 ) ;
				}
				
				//g_debug ( "Parsed url %s", s -> str ) ;
				list = g_list_append ( list, g_string_free ( s, no )) ;
			}
			else
				g_string_free ( s, yes ) ;
			
		
			i ++ ;
		}

	}
	
	out ;
	return list ;
}

really private gboolean search_file_is_suitable_for_playback ( gchar * filename )
{
	gchar * ext = g_strdup ( strrchr ( filename, '.' )) ;
	
	if ( ext == null )
		return false ;
	
	gchar * down = g_ascii_strdown ( ext, -1 ) ;
	
	gint i = 0 ;
	gboolean result = false ;
	
	while ( search_extensions [i] != null )
	{
		if ( g_strrstr ( down, search_extensions [i] ) != null )
		{
			result = true ;
			break ;
		}
		
		i ++ ;
	}
	
	g_free ( down ) ;
	g_free ( ext ) ;
	
	return result ;
}
	
cool gchar * search_get_shoutcast_url ( gchar * txt )
{
	in ;
	
	GKeyFile * f = g_key_file_new () ;
	if ( ! g_key_file_load_from_data ( f, txt, strlen ( txt ), G_KEY_FILE_NONE, null ))
	{
		g_key_file_free ( f ) ;
		return null ;
	}
	
	gchar * name = g_key_file_get_string ( f, "playlist", "File1", null ) ;
	g_key_file_free ( f ) ;
	
	return name ;
}
