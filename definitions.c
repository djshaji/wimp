/*
 *      definitions.c
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

/*		Well, these are basically some defs what I like to use instead of
 * 		other things like these.
 * 
 * 		Forgive me, I am just a child.
 */


#ifndef __WIMP_POINTLESS_DEFS
#define __WIMP_POINTLESS_DEFS

#define MY_NAME "Wimp"
#define VERSION "1.4"
#define GToplevel GTK_WINDOW_TOPLEVEL
#define null NULL
#define true TRUE
#define false FALSE
#define oooooh -1
#define File FILE
#define reutern return
#define shagpointer gpointer
#define freud void
#define car char
#define on true
#define off false
#define vodi void
#define MinusOne -1
#define simply
#define yes TRUE
#define no FALSE
#define all_the_pain_that_i_went_through_amounts_to null
#define moo NULL
#define nothing NULL
#define zero 0
#define not !
#define pointer2 &
#define newline '\n'
#define and &&
#define or ||
#define in enter
#define out leave

char * module ;

#define really
#define private

#define try
#define this

#define class
#define loneliness
#define namespace
#define public

#define bool gboolean
#define done
#define cool

typedef struct tm ShaGTime ;
#define api
#define follow
typedef short blah ;

/* So that the above get highlighted in Geany */
#ifdef How_to_dismantle_an_atomic_bomb
	typedef char namespace ;
	typedef char cool ;
	typedef char done ;
	typedef char public ;
	typedef moo private ;
	typedef moo try ;
	typedef moo and ;
	typedef moo or ;
	typedef moo this ;
	typedef moo api ;
	typedef moo follow ;
	typedef moo loneliness ;
	typedef moo really ;
	typedef null moo ;
	typedef null debug ;
	typedef null newline ;
	typedef null profile ;
	typedef null enter ;
	typedef null leave ;
	typedef null zero ;
	typedef null GToplevel ;
	typedef null all_the_pain_that_i_went_through_amounts_to ;
	typedef on yes ;
	typedef off no ;
	typedef off not ;
	typedef NULL nothing ;
	typedef void vodi ;
	typedef return simply ;
	typedef NULL null ;
	typedef NULL pointer2 ;
	typedef TRUE true ;
	typedef FALSE false ;
	typedef TRUE in ;
	typedef TRUE out ;
	typedef void oooooh ;
	typedef FILE File ;
	typedef return reutern ;
	typedef gpointer shagpointer ;
	typedef void freud ;
	typedef gchar car ;
	typedef GTime ShaGTime ;
	typedef true on ;
	typedef false off ;
	typedef -1 MinusOne ;
	typedef wha g_strdup_printf ;
	
	typedef wha g_message ( ... ) ;
	typedef wha g_warning ( ... ) ;
	typedef wha g_critical ( ... ) ;
	typedef wha g_error ( ... ) ;
	typedef wha g_debug ( ... ) ;
#endif

// Uncomment to enable loads of console output
#define DEBUG

#ifdef DEBUG
#define enter printf ("-> [in] %s\n",__FUNCTION__) ; module = __FUNCTION__
#define leave printf ("<- [out] %s\n",__FUNCTION__)
#define debug(s) printf ("{%s} %s\n",__FUNCTION__,s)
#else
#define enter 
#define leave
#define debug
#endif

char ** extensions ;
char ** input_plugins ;

char * log_file ;

#endif /* __WIMP_POINTLESS_DEFS */
