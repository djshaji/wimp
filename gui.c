/*
 *      gui.c
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


#include "gui.h"
#include "util.h"

GtkWidget * wimp_gui_init ( WimpGUI * wimp )
{
	in ;

	GtkWindow * window = gtk_window_new ( GTK_WINDOW_TOPLEVEL ) ;
	GtkVBox * master = gtk_vbox_new ( no, 5 ) ;
	
	g_signal_connect_swapped ( window, "destroy", wimp_cleanup, wimp ) ;
	gtk_container_add ( window, master ) ;
	
	gtk_widget_set_sensitive ( window, no ) ;
	
	GtkHBox * main_widget = gtk_hbox_new ( no, 2 ) ;
	gtk_box_pack_start ( master, main_widget, yes, yes, 2 ) ;
	
	GtkWidget * pane1 = gtk_hpaned_new () ;
	gtk_box_pack_start ( main_widget, pane1, yes, yes, 2 ) ;
	
	//GtkWidget * enhancements = gtk_notebook_new () ;
	GtkWidget * pad_box1 = gtk_vpaned_new () ;
	
	GtkWidget * pane2 = gtk_vpaned_new () ;
	gtk_paned_pack2 ( pane1, pane2, on, yes ) ;
	
	GtkWidget * notebook1 = gtk_notebook_new () ;
	gtk_paned_pack1 ( pane2, notebook1, on, yes ) ;
	
	GtkWidget * notebook2 = wimp_gui_enhancements ( wimp ) ;
	gtk_paned_pack2 ( pane2, notebook2, off, yes ) ;
	
	GtkTreeModel * store = gtk_tree_store_new ( 2, G_TYPE_STRING, G_TYPE_STRING ) ;
	GtkTreeView * playlist = gtk_tree_view_new_with_model ( store ) ;
	
	gtk_tree_view_set_enable_search ( playlist, true ) ;
	gtk_tree_view_set_search_equal_func ( playlist, wimp_gui_playlist_search_func, null, null ) ;

	GtkCellRenderer * renderer = gtk_cell_renderer_text_new () ;
	GtkTreeViewColumn * column = gtk_tree_view_column_new_with_attributes ( "Playlist",
																			renderer,
																			"text",
																			zero,
																			null ) ;
	
	gtk_tree_view_append_column ( playlist, column ) ;
	gtk_tree_view_set_reorderable ( playlist, true ) ;
	
	GtkToolbar * toolbar1 = gtk_toolbar_new () ;
	GtkToolButton * add = gtk_tool_button_new_from_stock ( GTK_STOCK_ADD ),
				  * del = gtk_tool_button_new_from_stock ( GTK_STOCK_DELETE ),
				  * shuffle = gtk_tool_item_new (),
				  * repeat = gtk_tool_item_new (),
				  * clear = gtk_tool_button_new_from_stock ( GTK_STOCK_CLEAR ) ;
	
	wimp -> repeat = gtk_toggle_button_new_with_label ( "R" ) ;
	wimp -> shuffle = gtk_toggle_button_new_with_label ( "S" ) ;
	
	g_signal_connect_swapped ( add, "clicked", wimp_gui_playlist_add_to_playlist, wimp ) ;
	g_signal_connect_swapped ( del, "clicked", wimp_gui_playlist_delete_from_playlist, wimp ) ;
	
	g_signal_connect_swapped ( clear, "clicked", wimp_gui_playlist_clear, wimp ) ;
		
	gtk_container_add ( repeat, wimp -> repeat ) ;
	gtk_container_add ( shuffle, wimp -> shuffle ) ;
	
	gtk_toolbar_insert ( toolbar1, add, -1 ) ;
	gtk_toolbar_insert ( toolbar1, del, -1 ) ;
	
	gtk_toolbar_insert ( toolbar1, repeat, -1 ) ;
	gtk_toolbar_insert ( toolbar1, shuffle, -1 ) ;
	
	gtk_toolbar_insert ( toolbar1, clear, -1 ) ;
	
	GtkWidget * vbox22 = gtk_vbox_new ( no, 2 ) ;

	GtkScrolledWindow * sw1 = gtk_scrolled_window_new ( null, null ) ;
	gtk_box_pack_start ( vbox22, sw1, on, on, 2 ) ;

	gtk_box_pack_start ( vbox22, toolbar1, no, no, 2 ) ;
	gtk_notebook_append_page ( notebook1, vbox22, gtk_label_new ( "Playlist" )) ;
	
	gtk_tree_view_set_headers_visible ( playlist, off ) ;
	gtk_container_add ( sw1, playlist ) ;
	
	GtkTreeModel * model = gtk_tree_store_new ( 4, G_TYPE_STRING, G_TYPE_STRING, GDK_TYPE_PIXBUF, G_TYPE_BOOLEAN ) ;
	GtkIconView * icon_view = gtk_tree_view_new_with_model ( model ) ;
	
	gtk_tree_view_set_enable_search ( icon_view, true ) ;
	gtk_tree_view_set_search_equal_func ( icon_view, wimp_gui_playlist_search_func, null, null ) ;

	gtk_tree_view_set_headers_visible ( icon_view, off ) ;
	gtk_tree_selection_set_mode ( gtk_tree_view_get_selection ( icon_view ), GTK_SELECTION_MULTIPLE ) ;

	GtkCellRenderer * rendererx = gtk_cell_renderer_text_new () ;
	GtkTreeViewColumn * columnx = gtk_tree_view_column_new_with_attributes ( "Playlist",
																			rendererx,
																			"text",
																			1,
																			null ) ;
	
	GtkCellRenderer * renderery = gtk_cell_renderer_pixbuf_new () ;
	GtkTreeViewColumn * columny = gtk_tree_view_column_new_with_attributes ( "Playlist",
																			renderery,
																			"pixbuf",
																			2,
																			null ) ;
	
	gtk_tree_view_append_column ( icon_view, columny ) ;
	gtk_tree_view_append_column ( icon_view, columnx ) ;
	
	GtkScrolledWindow * ws = gtk_scrolled_window_new ( null, null ) ;
	gtk_container_add ( ws, icon_view ) ;
	
	GtkVBox * xo = gtk_vbox_new ( no, 0 ) ;
	gtk_box_pack_start ( xo, ws, yes, yes, 2 ) ;
	
	gtk_notebook_append_page ( notebook1, xo, gtk_label_new ( "Browser" )) ;
	gtk_scrolled_window_set_policy ( ws, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC ) ;
	
	GtkToolbar * bar = gtk_toolbar_new () ;
	GtkToolButton * back = gtk_tool_button_new_from_stock ( GTK_STOCK_GO_BACK ),
				  * up = gtk_tool_button_new_from_stock ( GTK_STOCK_GO_UP ),
				  * home = gtk_tool_button_new_from_stock ( GTK_STOCK_HOME ) ;
	
	gtk_toolbar_insert ( bar, back, -1 ) ;
	gtk_toolbar_insert ( bar, up, -1 ) ;
	
	gtk_toolbar_insert ( bar, home, -1 ) ;
	
	g_signal_connect_swapped ( back, "clicked", wimp_file_manager_go_back, wimp ) ;
	g_signal_connect_swapped ( up, "clicked", wimp_file_manager_go_up, wimp ) ;

	g_signal_connect_swapped ( home, "clicked", wimp_file_manager_go_home, wimp ) ;
	gtk_box_pack_start ( xo, bar, no, no, 2 ) ;
	
	wimp -> browser = icon_view ;
	wimp -> model = model ;
	
	gtk_tree_sortable_set_default_sort_func ( model, wimp_file_manager_sort_by_name, null, null ) ;
	gtk_tree_sortable_set_sort_column_id ( model, GTK_TREE_SORTABLE_DEFAULT_SORT_COLUMN_ID, GTK_SORT_ASCENDING ) ;

	g_signal_connect ( icon_view, "row-activated", wimp_file_manager_activated, wimp ) ;
	g_signal_connect ( icon_view, "button-press-event", wimp_file_manager_menu_popup, wimp ) ;
	
	GtkWidget * video_display = gtk_event_box_new () ;
	gtk_paned_pack1 ( pane1, pad_box1, yes, yes ) ;
	
	GtkVBox * vbox2 = gtk_vbox_new ( no, 2 ) ;
	
	//gtk_box_pack_start ( hbox, vbox2, yes, yes, 0 ) ;
	gtk_scrolled_window_set_policy ( sw1, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC ) ;	
	
	gtk_paned_pack1 ( pad_box1, video_display, yes, yes ) ;
	gtk_paned_pack2 ( pad_box1, vbox2, yes, yes ) ;
	
	//gtk_box_pack_start ( vbox2, enhancements, no, no, 2 ) ;
	gtk_event_box_set_visible_window ( video_display, off ) ;
	
	GtkHRuler * r = gtk_hruler_new () ;
	gtk_box_pack_start ( vbox2, r, no, no, 5 ) ;
	
	GtkWidget * seekbar = gtk_hscale_new ( null ) ;
	gtk_box_pack_start ( vbox2, seekbar, no, no, 5 ) ;
	
	gtk_range_set_range ( seekbar, 0.0, 1.0 ) ;
	gtk_range_set_increments ( seekbar, 0.01, 0.1 ) ;
	
	gtk_scale_set_draw_value ( seekbar, no ) ;
	wimp -> seekbar = seekbar ;
	
	GtkToolbar * toolbar = gtk_toolbar_new () ;
	gtk_box_pack_start ( vbox2, toolbar, no, no, 0 ) ;
	
	gtk_toolbar_set_icon_size ( toolbar, GTK_ICON_SIZE_SMALL_TOOLBAR ) ;
	
	GtkToolButton	* rev = gtk_tool_button_new_from_stock ( GTK_STOCK_MEDIA_REWIND ),
					* ff = gtk_tool_button_new_from_stock ( GTK_STOCK_MEDIA_FORWARD ),
					* next = gtk_tool_button_new_from_stock ( GTK_STOCK_MEDIA_NEXT ),
					* prev = gtk_tool_button_new_from_stock ( GTK_STOCK_MEDIA_PREVIOUS ),
					//* play = gtk_tool_button_new_from_stock ( GTK_STOCK_MEDIA_PLAY ),
					* pause = gtk_tool_button_new_from_stock ( GTK_STOCK_MEDIA_PAUSE ),
					* stop = gtk_tool_button_new_from_stock ( GTK_STOCK_MEDIA_STOP ) ;
	
	GtkToolButton * info = gtk_tool_button_new_from_stock ( GTK_STOCK_PROPERTIES ) ;
	g_signal_connect_swapped ( info, "clicked", wimp_gui_meta_info, wimp ) ;
	
	g_signal_connect_swapped ( next, "clicked", wimp_gui_playlist_next, wimp ) ;
	g_signal_connect_swapped ( prev, "clicked", wimp_gui_playlist_previous, wimp ) ;
	
	GtkImage * image = gtk_image_new_from_stock ( GTK_STOCK_MEDIA_PLAY, GTK_ICON_SIZE_LARGE_TOOLBAR ) ;
	GtkButton * button = gtk_toggle_button_new () ;
	
	GtkToolItem * play = gtk_tool_item_new () ;
	gtk_container_add ( button, image ) ;
	
	gtk_container_add ( play, button ) ;
	gtk_button_set_relief ( button, GTK_RELIEF_NONE ) ;
	
	GtkSeparatorToolItem * sep1 = gtk_separator_tool_item_new (),
						 * sep2 = gtk_separator_tool_item_new () ;
						 
	gtk_separator_tool_item_set_draw ( sep1, no ) ;
	gtk_separator_tool_item_set_draw ( sep2, no ) ;
	
	gtk_tool_item_set_expand ( sep1, on ) ;
	gtk_tool_item_set_expand ( sep2, on ) ;
	
	//gtk_toolbar_insert ( toolbar, sep1, -1 ) ;
	gtk_toolbar_insert ( toolbar, play, -1 ) ;
	//gtk_toolbar_insert ( toolbar, pause, -1 ) ;
	gtk_toolbar_insert ( toolbar, stop, -1 ) ;
	gtk_toolbar_insert ( toolbar, rev, -1 ) ;
	gtk_toolbar_insert ( toolbar, ff, -1 ) ;
	gtk_toolbar_insert ( toolbar, prev, -1 ) ;
	gtk_toolbar_insert ( toolbar, next, -1 ) ;
	gtk_toolbar_insert ( toolbar, gtk_separator_tool_item_new (), -1 ) ;

	gtk_toolbar_insert ( toolbar, info, -1 ) ;

	wimp -> fs = gtk_toggle_tool_button_new_from_stock ( GTK_STOCK_JUMP_TO ) ;
	g_signal_connect ( wimp -> fs, "toggled", wimp_gui_toggle_fullscreen, wimp ) ;
	
	gtk_toolbar_insert ( toolbar, wimp -> fs, -1 ) ;
	gtk_toolbar_insert ( toolbar, gtk_separator_tool_item_new (), -1 ) ;
	
	//gtk_toolbar_insert ( toolbar, sep2, -1 ) ;
	
	wimp -> volume = gtk_hscale_new ( null ) ;
	GtkToolItem * holder = gtk_tool_item_new () ;
	
	gtk_container_add ( holder, wimp -> volume ) ;
	gtk_toolbar_insert ( toolbar, holder, -1 ) ;
	
	gtk_tool_item_set_expand ( holder, yes ) ;
	gtk_scale_set_draw_value ( wimp -> volume, no ) ;
	
	gtk_range_set_range ( wimp -> volume, 1, 100 ) ;
	gtk_range_set_increments ( wimp -> volume, 1, 10 ) ;
	
	g_signal_connect_swapped ( ff, "clicked", wimp_gui_fast_forward, wimp ) ;
	g_signal_connect_swapped ( rev, "clicked", wimp_gui_rewind, wimp ) ;

	g_signal_connect_swapped ( button, "clicked", wimp_gui_toggle_pause, wimp ) ;
	g_signal_connect_swapped ( stop, "clicked", wimp_gui_stop, wimp ) ;
	
	g_signal_connect ( seekbar, "change-value", wimp_gui_seek, wimp ) ;
	g_signal_connect ( seekbar, "button-press-event", wimp_gui_seeker, wimp ) ;
	g_signal_connect_after ( wimp -> volume, "change-value", wimp_gui_enhancements_volume, wimp ) ;
	
	GtkToolbar * mode_selector = null ;//gtk_toolbar_new () ;
	//gtk_box_pack_start ( master, mode_selector, no, no, 2 ) ;
	
	GdkColor black ;
	black.red = 0 ;
	
	black.blue = 0 ;
	black.green = 0 ;
	
	gtk_widget_modify_bg ( video_display, GTK_STATE_NORMAL, & black ) ;
	gtk_paned_set_position ( pane1, 320 ) ;
	
	gtk_paned_set_position ( pane2, 230 ) ;
	gtk_paned_set_position ( pad_box1, 320 ) ;
	
	//GtkHBox * seeker = gtk_hbox_new ( no, 2 ) ;
	//gtk_box_pack_start ( master, seeker, no, no, 2 ) ;
	
	//GtkHBox * control = gtk_hbox_new ( no, 2 ) ;
	//gtk_box_pack_start ( master, control, no, no, 2 ) ;
	
	gtk_window_resize ( window, 550, 400 ) ;
	gtk_widget_show_all ( window ) ;
	
	wimp_update_gui () ;
	
	wimp -> window = window ;
	wimp -> video_display = video_display ;
	
	wimp -> toolbar = toolbar ;
	wimp -> mode_selector = mode_selector ;

	wimp -> pane1  = pane1 ;
	wimp -> pane2 = pane2 ;
	
	wimp -> pane3 = pad_box1 ;
	wimp -> index = 0 ;
	
	wimp -> play = button ;
	//wimp -> enhancements = enhancements ;
	
	wimp -> playlist = playlist ;
	wimp -> store = store ;
	
	//wimp -> browser = notebook1 ;
	wimp -> selection = gtk_tree_view_get_selection ( playlist ) ;
	
	g_signal_connect_swapped ( video_display, "size-allocate", wimp_gui_handle_video_resize, wimp ) ;
	g_signal_connect_after ( window, "window-state-event", wimp_gui_track_window_state, wimp ) ;
	
	g_signal_connect_after ( window, "button-press-event", wimp_gui_menu_popup, wimp ) ;
	g_signal_connect_after ( window, "key-press-event", wimp_hotkeys_gui, wimp ) ;
	
	g_signal_connect ( playlist, "row-activated", wimp_gui_playlist_row_activated, wimp ) ;
	g_signal_connect ( playlist, "button-press-event", wimp_gui_playlist_popup_menu, wimp ) ;
	
	g_signal_connect ( toolbar, "key-press-event", wimp_hotkeys_gui, wimp ) ;
	g_signal_connect ( seekbar, "key-press-event", wimp_hotkeys_gui, wimp ) ;
	
	//g_signal_connect_after ( wimp -> window, "motion-notify-event", wimp_gui_track_pointer, wimp ) ;
	wimp -> fullscreen = off ;
	wimp -> controls_visible = on ;
	
	g_printf ( "Attempting to load default playlist ..." ) ;
	wimp_gui_playlist_load_playlist_default ( wimp ) ;
	g_printf ( " [done]\n" ) ;
	
	// So that when we close the window, our store is not destroyed
	g_signal_connect ( playlist, "destroy", gtk_tree_view_set_model, null ) ;
	
	wimp -> event_id = -1 ;
	
	GtkTargetEntry target [1] ;
	target[0].target = g_strdup ( "text" ) ;
	target[0].info = 69 ;
	
	GtkTargetEntry target1 [1] ;
	target1[0].target = g_strdup ( "text" ) ;
	target1[0].info = 69 ;
	
	gtk_drag_dest_set ( playlist, GTK_DEST_DEFAULT_ALL, & target, 0, GDK_ACTION_COPY ) ;
	gtk_drag_dest_add_text_targets ( playlist ) ;

	gtk_drag_dest_set ( window, GTK_DEST_DEFAULT_ALL, & target1, 0, GDK_ACTION_COPY ) ;
	gtk_drag_dest_add_text_targets ( window ) ;

	g_signal_connect ( window, "drag-data-received", wimp_gui_playlist_drag_drop_replace, wimp ) ;
	g_signal_connect ( playlist, "drag-data-received", wimp_gui_playlist_drag_drop, wimp ) ;

	wimp -> mode = MDI ;
	
	out ;
	return window ;
}

void wimp_gui ( gint argc, gchar * argv [] )
{
	in ;
	
	WimpGUI * wimp = wimp_gui_new () ;
	wimp_gui_init ( wimp ) ;
	
	gtk_widget_show_all ( wimp -> window ) ;
	wimp_gui_update () ;
	
	my_wimp = wimp ;
	
	struct sigaction sig ;
	sig.sa_handler = null ;
	
	sig.sa_sigaction = wimp_sigaction ;
	//sig.sa_mask = SIGKILL ;
	//sig.sa_flags = SA_SIGINFO ;
	
	sigaction ( 35, & sig, null ) ;
	
	gchar * touch = g_build_filename ( g_get_home_dir (), ".wimp", "touch", null ) ;
	wimp -> compatibility = g_file_test ( touch, G_FILE_TEST_EXISTS ) ;
	
	File * fp = fopen ( touch, "w" ) ;
	if ( fp ) fclose ( fp ) ;
	
	g_free ( touch ) ;
	
	wimp -> xine = wimp_xine_new () ;
	wimp_xine_engine_init_x11 ( wimp -> xine, wimp -> video_display ) ;

	if ( wimp -> xine -> vo == null )
	{
		g_warning ( "Unable to open a video driver :(\n\n"
					"Please select another video driver from the "
					"options menu and restart WiMP.\n\n"
					"You won't be able to play nothin\'." ) ;
		
		gtk_widget_set_sensitive ( wimp -> window, on ) ;
		gtk_main () ;
	}

	///> DEBUG
	wimp_gui_zoom_100 ( wimp ) ;
	wimp_gui_adjust_video_display ( wimp ) ;

	//wimp_gui_enhancements_load ( wimp ) ;
	wimp_gui_enhancements_init_from_engine ( wimp ) ;
	
	gtk_widget_set_sensitive ( wimp -> window, on ) ;
	
	if ( wimp -> xine -> config -> memlock )
	{
		if ( wimp -> compatibility )
		{
			if ( wimp_message_question ( "The program did not exit properly the last time. As "
										 "a result, we are running in safe-mode. The following "
										 "feature may cause trouble. Do you want to enable it?\n\n"
										 "Lock memory to physical RAM" ))
				mlockall ( MCL_FUTURE ) ;
			else
				wimp -> xine -> config -> memlock = off ;
		}
		else
			mlockall ( MCL_FUTURE ) ;
			
	}
	
	if ( argc > 1 )
	{
		gint i = 0 ;
		wimp_gui_playlist_clear ( wimp ) ;
		
		for ( i = 1 ; i < argc ; i ++ )
			wimp_gui_playlist_append ( wimp, argv [i] ) ;
	}
	
	//wimp_gui_enhancements_init ( wimp ) ;
	gtk_window_get_size ( wimp -> window, & wimp -> width, & wimp -> height ) ;
	
	wimp_file_manager_browse ( wimp, g_get_home_dir ()) ;
	printf ( "WiMP is up and running.\n" ) ;
	
	if ( wimp_gui_playlist_length ( wimp ))
		wimp_gui_playlist_play ( wimp, null ) ;
	
	gtk_main () ;
	
	out ;
}

WimpGUI * wimp_gui_new ( void )
{
	in ;

	GMemChunk * chunk = g_mem_chunk_create ( WimpGUI, 10, G_ALLOC_ONLY ) ;
	WimpGUI * wimp = g_chunk_new ( WimpGUI, chunk ) ;
	
	wimp -> chunk = chunk ;
	wimp -> enhancements_set = wimp_gui_enhancements_new () ;
	
	wimp -> history = null ;

	out ;
	return wimp ;
}

WimpEnhancements * wimp_gui_enhancements_new ( void )
{
	in ;

	GMemChunk * chunk = g_mem_chunk_create ( WimpEnhancements, 10, G_ALLOC_ONLY ) ;
	WimpEnhancements * wimp = g_chunk_new ( WimpEnhancements, chunk ) ;
	
	out ;
	return wimp ;
}
	
void wimp_gui_destroy ( WimpGUI * wimp )
{
	gtk_widget_destroy ( wimp -> window ) ;
	wimp_xine_destroy ( wimp -> xine ) ;
	
	g_mem_chunk_destroy ( wimp -> chunk ) ;
}

static void dest_size_cb ( Xine * data, int video_width, int video_height, double video_pixel_aspect,
                         int *dest_width, int *dest_height, double *dest_pixel_aspect)  {
	*dest_width        = data -> width;
	*dest_height       = data -> height;
	*dest_pixel_aspect = data -> pixel_aspect;
}

/* this will be called by xine when it's about to draw the frame */
static void frame_output_cb ( Xine* data, int video_width, int video_height,
                            double video_pixel_aspect, int *dest_x, int *dest_y,
                            int *dest_width, int *dest_height, 
                            double *dest_pixel_aspect, int *win_x, int *win_y) {
/*
  *dest_x            = data -> x_offset ;
  *dest_y            = data -> y_offset ;
  *win_x             = data -> xpos;
  *win_y             = data -> ypos;
  *dest_width        = data -> width;
  *dest_height       = data -> height;
  *dest_pixel_aspect = data -> pixel_aspect;
*/
  *dest_x            = 0 ;
  *dest_y            = 0 ;
  *win_x             = 0 ; 
  *win_y             = 0 ;
  *dest_width        = data -> width;
  *dest_height       = data -> height;
  *dest_pixel_aspect = data -> pixel_aspect;
  
 
  /*
  g_debug ( "%d %d %d %d %d %d %f", data -> x_offset,
								 data -> y_offset,
								 data -> xpos,
								 data -> ypos,
								 data -> width,
								 data -> height,
								 data -> pixel_aspect ) ;
								 
								 */
}

void wimp_xine_vo_init_x11 ( Xine * xine, GtkWidget * video_display )
{
	in ;
	
	if ( ! XInitThreads ())
		g_warning ( "XInitThreads () failed !" ) ;
	
	if ( ! GTK_WIDGET_REALIZED ( video_display ))
		gtk_widget_realize ( video_display ) ;
	
	wimp_gui_update () ;
	
	xine -> display = XOpenDisplay ( null ) ;
	xine -> screen = XDefaultScreen ( xine -> display ) ;
	
	xine -> visual.display = xine -> display ;
	xine -> visual.screen = xine -> screen ;
	
	XLockDisplay ( xine -> display ) ;
	xine -> xwindow = GDK_WINDOW_XWINDOW ( gtk_widget_get_parent_window ( video_display )) ;
	
	XUnlockDisplay ( xine -> display ) ;
	xine -> visual.d = xine -> xwindow ;
	
	xine -> visual.dest_size_cb = dest_size_cb ;
	xine -> visual.frame_output_cb = frame_output_cb ;
	
	xine -> visual.user_data = xine ;
	XSelectInput ( xine -> display, xine -> xwindow, INPUT_MOTION ) ;
	
	gint res_h = ( DisplayWidth ( xine -> display, xine -> screen ) * 1000 / DisplayWidthMM ( xine -> display, xine -> screen )) ;
	gint res_v = ( DisplayHeight ( xine -> display, xine -> screen ) * 1000 / DisplayHeightMM ( xine -> display, xine -> screen )) ;
	
	///> Debug: Do these values really mean anything ?
	
	xine -> width = 20 ;
	xine -> height = 40 ;
	
	xine -> pixel_aspect = res_v / res_h ;
	
	xine -> xpos = 0 ;
	xine -> ypos = 0 ;
	
	xine -> x_offset = 0 ;
	xine -> y_offset = 0 ;

	xine -> vo = xine_open_video_driver ( xine -> xine, xine -> config -> vo, XINE_VISUAL_TYPE_X11, & xine -> visual ) ;
	if ( xine -> vo == null )
		g_critical ( "Cannot open video driver !" ) ;
	
	gchar * ext = xine_get_file_extensions ( xine -> xine ) ;
	g_debug ( "Supported file types: %s", ext ) ;
	
	extensions = g_strsplit ( ext, " ", -1 ) ;
	input_plugins = xine_get_browsable_input_plugin_ids  ( xine -> xine ) ;
	
	gint i = 0, len = g_strv_length ( input_plugins ) ;
	printf ( "Input plugins:\n" ) ;
	
	for ( i = 0 ; i < len  ; i ++ )
		printf ( "\t%s\n", input_plugins [i] ) ;
	
	out ;
}

void wimp_xine_x11_expose ( WimpGUI * wimp )
{
	if ( ! GTK_WIDGET_IS_SENSITIVE ( wimp -> seekbar ))
		gtk_widget_set_sensitive ( wimp -> seekbar, on ) ;
	
	wimp_gui_update () ;

	XEvent xevent;
	int    got_event;

	g_usleep ( 10000 ) ;
	XLockDisplay ( wimp -> xine -> display ) ;

	got_event = XCheckMaskEvent ( wimp -> xine -> display, INPUT_MOTION, & xevent ) ;
	XUnlockDisplay( wimp -> xine -> display ) ;

	if ( ! got_event )
		return;

	switch ( xevent.type )
	{
		case Expose:
			if ( xevent.xexpose.count != 0 )
				break;
			
			xine_port_send_gui_data ( wimp -> xine -> vo, XINE_GUI_SEND_EXPOSE_EVENT, & xevent ) ;
			break;
	
		case ConfigureNotify:
		{
			wimp_gui_adjust_video_display ( wimp ) ;
			xine_port_send_gui_data ( wimp -> xine -> vo, XINE_GUI_SEND_EXPOSE_EVENT, & xevent ) ;
			
			return ;
			
			XConfigureEvent * cev = (XConfigureEvent * ) & xevent ;
			Window           tmp_win;

			wimp -> xine -> width  = cev -> width;
			wimp -> xine -> height = cev -> height;

			if (( cev -> x == 0 ) && ( cev -> y == 0 )) {
			  XLockDisplay ( wimp -> xine -> display ) ;
			  XTranslateCoordinates ( wimp -> xine -> display, cev -> window,
									DefaultRootWindow ( cev -> display),
									0, 0, & wimp -> xine -> xpos, & wimp -> xine -> ypos, & tmp_win ) ;
			  XUnlockDisplay ( wimp -> xine -> display ) ;
			} else {
			  wimp -> xine -> xpos = cev -> x ;
			  wimp -> xine -> ypos = cev -> y ;
			}
			}
			break;

		case MotionNotify:
		if ( wimp -> fullscreen )
		{
			if ( wimp -> mode == MDI )
			{
				XMotionEvent * cev = ( XMotionEvent * ) & xevent ;
				if ( cev -> y_root > gdk_screen_height () - 90 || cev -> x_root > gdk_screen_width () - 150 )
				{
					if ( ! wimp -> controls_visible )
						wimp_gui_fullscreen_show_controls ( wimp ) ;
				}
				else
				{
					if ( wimp -> controls_visible )
						wimp_gui_fullscreen_hide_controls ( wimp ) ;
				}
			}
			//else
			//{
				//gtk_window_present ( wimp -> playlist_window ) ;
				//gtk_window_present ( wimp -> controls_window ) ;
			//}
			break ;
		}
		
		break ;
			
		default:
			xine_port_send_gui_data ( wimp -> xine -> vo, XINE_GUI_SEND_EXPOSE_EVENT, & xevent ) ;
			break ;
	}
}	

void wimp_xine_x11_play ( WimpGUI * wimp, gint seek )
{
	in ;
	
	if ( wimp -> mode == MDI ) {
	if ( ! gtk_toggle_button_get_active ( wimp -> play ))
		gtk_toggle_button_set_active ( wimp -> play, on ) ;
	}
	
	xine_play ( wimp -> xine -> stream, 0, seek ) ;
	debug ( "xine_play" ) ;
	
	gint pos = 0, length = 0, pos_stream = 0, total_length = 0 ;
	wimp -> xine -> timestamp = gtk_get_current_event_time () ;
	
	gfloat value = 0.0 ;
			
	while ( xine_get_status ( wimp -> xine -> stream ) == XINE_STATUS_PLAY )
	{
		xine_get_pos_length ( wimp -> xine -> stream, & pos_stream, & pos, & length ) ;
		wimp_xine_x11_expose ( wimp ) ;
		
		wimp -> xine -> pos_stream = pos_stream ;
		wimp -> xine -> pos_time = pos ;
		
		value = (( gfloat ) pos / ( gfloat ) length ) ;
		
		wimp -> xine -> length = length ;
		gtk_range_set_value ( wimp -> seekbar, value ) ;
		
		//g_printf ( "%d %d\n", pos, wimp -> xine -> osd_shown ) ;
		/*
		if ( pos - wimp -> xine -> osd_shown > 2000 )
		{
			xine_osd_clear ( wimp -> xine -> osd ) ;
			xine_osd_hide ( wimp -> xine -> osd, wimp -> xine -> vpts ) ;
		
		}
		*/
	
	
		
		//g_debug ( "pos_stream: %d, pos_time: %d, length: %d, value: %f", pos_stream, pos, length, value ) ;
	}
	
	if ( xine_get_status ( wimp -> xine -> stream ) == XINE_STATUS_IDLE )
	{
		gtk_widget_queue_draw ( wimp -> window ) ;
		wimp_update_gui () ;
	}
	
	out ;
}

static inline void wimp_gui_update ( void )
{
	while ( gtk_events_pending ())
		gtk_main_iteration () ;

	return ;
}

void wimp_gui_adjust_video_display ( WimpGUI * gui )
{
	in ;

	if ( gui -> mode == TOPLEVEL )
	{
		gtk_window_get_size ( gui -> main_window, & gui -> xine -> width, & gui  -> xine -> height ) ;
		return ;
	}
	
	if ( ! GTK_WIDGET_REALIZED ( gui -> video_display ))
		//gtk_widget_realize ( gui -> video_display ) ;
	{
		g_critical ( "! GTK_WIDGET_REALIZED ( gui -> video_display )" ) ;

		out ;
		return ;
	}

	
	static gint event_id = 0 ;
	if ( event_id == gui -> event_id )
		return ;
	else event_id = gui -> event_id ;
	
	if ( gui -> fullscreen )
	{
		gui -> xine -> width  = gdk_screen_width () ;
		gui -> xine -> height = gdk_screen_height () ;
		
		return ;
	}
	
	GtkRequisition qu1, qu2, qu3 ;
	gtk_widget_size_request ( gui -> toolbar, & qu1 ) ;
	
	//gtk_widget_size_request ( gui -> mode_selector, & qu2 ) ;
	gtk_widget_size_request ( gui -> pane1, & qu3 ) ;
	
	gui -> xine -> width = gtk_paned_get_position ( gui -> pane1 ) ;
	gui -> xine -> height = gtk_paned_get_position ( gui -> pane3 ) ;
	
	gui -> xine -> x_offset = 10 ;
	gui -> xine -> y_offset = qu1.height + 10 ;
	
	////gtk_widget_queue_draw ( gui -> window ) ;
	wimp_update_gui () ;
	
	GtkRequisition hello ;
	gtk_widget_size_request ( gui -> video_display, & hello ) ;
	
	g_debug ( "%d x %d", hello.width, hello.height ) ;
	
	
	//g_debug ( "%d x %d", qu3.width, qu3.height ) ;
	out ;
}
	
void wimp_gui_zoom_100 ( WimpGUI * gui )
{
	//xine_current_frame_data_t data ;
	//xine_get_current_frame_data ( gui -> xine -> stream, & data, 0 ) ;
	
	//gui -> xine -> width = data.width ;
	//gui -> xine -> height = data.height ;
	
	gui -> xine -> width = xine_get_param ( gui -> xine -> stream, XINE_PARAM_VO_WINDOW_WIDTH ) ;
	gui -> xine -> height = xine_get_param ( gui -> xine -> stream, XINE_PARAM_VO_WINDOW_HEIGHT ) ;
	
	gtk_widget_set_size_request ( gui -> video_display, gui -> xine -> width, gui -> xine -> height ) ;
	xine_port_send_gui_data ( gui -> xine -> vo, XINE_GUI_SEND_DRAWABLE_CHANGED, gui -> xine -> xwindow ) ;
}

void wimp_gui_handle_video_resize ( WimpGUI * gui )
{
	//in ;
	wimp_gui_adjust_video_display ( gui ) ;	
	
	//out ;
}

void wimp_gui_fast_forward ( WimpGUI * gui )
{
	in ;
	
	g_debug ( "timestamp: %d, current_time: %d", gui -> xine -> timestamp, gtk_get_current_event_time () ) ;
	if ( gui -> xine -> timestamp == gtk_get_current_event_time () )
	{
		return ;
	}
	
	wimp_xine_x11_play ( gui, gui -> xine -> pos_time + 100000 ) ;
	out ;
}

void wimp_gui_rewind ( WimpGUI * gui )
{
	in ;
	
	g_debug ( "timestamp: %d, current_time: %d", gui -> xine -> timestamp, gtk_get_current_event_time () ) ;
	if ( gui -> xine -> timestamp == gtk_get_current_event_time () )
		return ;
		
	/*
	gint pos_stream = 0, pos_time = 0, length_time = 0 ;
	
	xine_get_pos_length (	gui -> xine -> stream,
							& pos_stream,
							& pos_time,
							& length_time ) ;

	
	*/ 
	
	wimp_xine_x11_play ( gui, gui -> xine -> pos_time - 100000 ) ;
	out ;
}

void wimp_gui_toggle_pause ( WimpGUI * gui )
{
	in ;
	
	/*
	if ( xine_get_status ( gui -> xine -> stream ) == XINE_STATUS_IDLE )
	{
		g_debug ( "No file loaded." ) ;
		wimp_gui_playlist_play ( gui, null ) ;
		
		out ;
		return ;
	}
	*/
	
	g_debug ( "Paused at: %d", gui -> xine -> paused_at ) ;
	
	if ( gui -> xine -> timestamp == gtk_get_current_event_time () )
		return ;
	
	/*
	if ( xine_get_status ( gui -> xine -> stream ) == XINE_STATUS_STOP )
	{
		wimp_xine_x11_play ( gui, 0 ) ;
		out ;
		return ;
	}
	*/
	
	if ( ! gtk_toggle_button_get_active ( gui -> play ))
	{
		gui -> xine -> paused_at = gui -> xine -> pos_time ;
		xine_stop ( gui -> xine -> stream ) ;
	}
	else
	{
		g_debug ( "time: %d", gui -> xine -> pos_time ) ;
		wimp_xine_x11_play ( gui, gui -> xine -> paused_at ) ;
	}
	
	out ;
}

vodi wimp_gui_stop ( WimpGUI * gui )
{
	in ;
	
	xine_stop ( gui -> xine -> stream ) ;
	xine_close ( gui -> xine -> stream ) ;
	
	gtk_widget_queue_draw ( gui -> window ) ;
	
	gui -> xine -> timestamp = gtk_get_current_event_time () ;
	gtk_toggle_button_set_active ( gui -> play, off ) ;

	gui -> xine -> pos_stream = 0 ;
	gui -> xine -> pos_time = 0 ;
	
	gui -> xine -> paused_at = 0 ;
	gui -> xine -> timestamp = gtk_get_current_event_time () ;

	wimp_update_gui () ;
	
	out ;
}

void wimp_gui_stop1 ( GtkWidget * widget, GtkWidget * window )
{
	in ;
	
	gtk_widget_queue_draw ( window ) ;
	wimp_update_gui () ;
	
	out ;
}

gboolean wimp_gui_seek ( GtkWidget * widget, GtkScrollType * scroll, gdouble seek, WimpGUI * gui )
{
	in ;
	
	gint seek_to = seek * ( gdouble ) gui -> xine -> length  ;
	g_debug ( "seek: %f, seek_to: %d", seek, seek_to ) ;
	
	gtk_widget_set_sensitive ( widget, true ) ;
	wimp_xine_x11_play ( gui, seek_to ) ;
	
	return true ;
	out ;
}

gboolean wimp_gui_toggle_fullscreen ( GtkToggleToolButton * button, WimpGUI * gui )
{
	in ;
	//wimp_gui_pause ( gui ) ;
	
	if ( gui -> mode == TOPLEVEL )
	{
		if ( gui -> fullscreen )
			gtk_window_unfullscreen ( gui -> main_window ) ;
		else
			gtk_window_fullscreen ( gui -> main_window ) ;
		
		return ;
	}
	
	if ( gtk_toggle_tool_button_get_active ( button ))
	{
		gtk_window_fullscreen ( gui -> window ) ;
		wimp_gui_fullscreen_hide_controls ( gui ) ;
	}
	else
	{
		gtk_window_unfullscreen ( gui -> window ) ;
		wimp_gui_fullscreen_show_controls ( gui ) ;
	}

	//wimp_gui_unpause ( gui ) ;
	out ;
}

void wimp_gui_fullscreen_show_controls ( WimpGUI * gui )
{
	in ;
	gint i = 0, y = gdk_screen_height (), w = gdk_screen_width () ;
	
	gtk_paned_set_position ( gui -> pane1, w - 230 ) ;
	gtk_paned_set_position ( gui -> pane3, y  - 84 ) ;
	
	gui -> xine -> width = gdk_screen_width () - 230 ;
	gui -> xine -> height = gdk_screen_height () - 84 ;

	/*
	for ( i = y ; i > y - 60 ; i = i - 20 )
	{
		gtk_paned_set_position ( gui -> pane3, i ) ;
		wimp_update_gui () ;
		
		//g_usleep ( 1000 ) ;
	}
	
	for ( i = w ; i > w - 60 ; i = i - 20 )
	{
		gtk_paned_set_position ( gui -> pane1, i ) ;
		wimp_update_gui () ;
		
		//g_usleep ( 1000 ) ;
	}
	
	*/
	gui -> controls_visible = on ;
	out ;
}
	
void wimp_gui_fullscreen_hide_controls ( WimpGUI * gui )
{
	in ;

	gint i = 0, y = gdk_screen_height (), z = gtk_paned_get_position ( gui -> pane3 ),
		 w = gdk_screen_width (), u = gtk_paned_get_position ( gui -> pane1 ) ;
	
	gtk_paned_set_position ( gui -> pane1, w ) ;
	gtk_paned_set_position ( gui -> pane3, y ) ;
	
	gui -> xine -> width = gdk_screen_width () ;
	gui -> xine -> height = gdk_screen_height () ;

	/*
	for ( i = z ; i <= y ; i = i + 20 )
	{
		gtk_paned_set_position ( gui -> pane3, i ) ;
		wimp_update_gui () ;
		
		//g_usleep ( 10000 ) ;
		xine_port_send_gui_data ( gui -> xine -> vo, XINE_GUI_SEND_DRAWABLE_CHANGED, (void *) gui -> xine -> xwindow );

	}
	
	for ( i = u ; i <= w ; i = i + 20 )
	{
		gtk_paned_set_position ( gui -> pane1, i ) ;
		wimp_update_gui () ;
		
		//g_usleep ( 10000 ) ;
	}
	*/
	
	gui -> controls_visible = off ;
	out ;
}
	
void wimp_gui_pause ( WimpGUI * gui )
{
	gui -> xine -> paused_at = gui -> xine -> pos_time ;
	xine_stop ( gui -> xine -> stream ) ;
}

void wimp_gui_unpause ( WimpGUI * gui )
{
	wimp_xine_x11_play ( gui, gui -> xine -> paused_at ) ;
}

gboolean wimp_gui_track_window_state ( 	GtkWidget * widget,
										GdkEventWindowState * event,
										WimpGUI * gui )
{
	in ;
	
	gui -> event_id = gtk_get_current_event_time () ;
	if ( event -> new_window_state == GDK_WINDOW_STATE_FULLSCREEN )
	{
		gui -> fullscreen = true ;
		
		gui -> xine -> width = gdk_screen_width () ;
		gui -> xine -> height = gdk_screen_height () ;
		
	}
	else
	{
		gui -> fullscreen = false ;
		
		gint width = gui -> width, height = gui -> height ;
		gtk_window_get_size ( gui -> window, & width, & height ) ;
		
		g_debug ( "Adapting to %d x %d", width, height ) ;
		
		wimp_update_gui () ;
		
		gtk_paned_set_position ( gui -> pane3, height - 80 ) ;
		gtk_paned_set_position ( gui -> pane1, width - 230 ) ;
	
		gui -> xine -> width = width - 230 ;
		gui -> xine -> height = height - 80 ;
		
		wimp_update_gui () ;
		
		//gtk_widget_set_size_request ( gui -> video_display, width - 150, height - 80 ) ;
		//wimp_gui_adjust_video_display ( gui ) ;
		
		g_debug ( "Size is %d x %d", width, height ) ;
		wimp_update_gui () ;
	}
		
	out ;
	return false ;
}

gboolean wimp_gui_track_pointer ( GtkWidget * widget, GdkEventMotion * event, WimpGUI * gui )
{
	in ;
	
	if ( gui -> fullscreen )
	{
		if (( event -> x_root > gdk_screen_width () / 4 ) && ( event -> y_root > gdk_screen_height () / 4 ))
		{
			if ( ! gui -> controls_visible )
				wimp_gui_fullscreen_show_controls ( gui ) ;
		}
		else
		{
			if ( gui -> controls_visible )
				wimp_gui_fullscreen_hide_controls ( gui ) ;
		}
	
	}

	out ;

	return on ;
}

gboolean wimp_hotkeys_gui ( GtkWidget * widget, GdkEventKey * event, WimpGUI * gui )
{
	in ;
	
	//g_debug ( "%d", event -> keyval ) ;
	/*
	if ( GTK_IS_RANGE ( gtk_window_get_focus ( gui -> window )))
		return false ;
	*/
	
	if ( GTK_WIDGET_HAS_FOCUS ( gui -> volume ))
		return false ;
	
	//if ( GTK_WIDGET_HAS_FOCUS ( gui -> playlist ))
	//{
		//if ( event -> keyval == GDK_Delete )
		//{
			//if ( GTK_WIDGET_HAS_FOCUS ( gui -> playlist ))
			//{
				//wimp_gui_playlist_delete_from_playlist ( gui ) ;
			//}
			
			//return true ;
		//}
		
		//return false ;
	//}
		
	switch ( event -> keyval )
	{
		case GDK_f:
			if ( gui -> mode == MDI )
				gtk_toggle_tool_button_set_active ( gui -> fs, ! gtk_toggle_tool_button_get_active ( gui -> fs )) ;
			else
			{
				if ( gui -> fullscreen )
					gtk_window_unfullscreen ( gui -> main_window ) ;
				else
					gtk_window_fullscreen ( gui -> main_window ) ;
			}
			break ;
		case GDK_q:
			///> For fun
			wimp_cleanup ( gui ) ;
			break ;
		case GDK_Escape:
			if ( gui -> xine -> config -> emergency_exit_enabled )
				g_error ( "Emergency exit." ) ;
			break ;
			
		case GDK_Left:
		case GDK_KP_Left:
			wimp_xine_x11_play ( gui, gui -> xine -> pos_time - 30000 ) ;
			break ;
		case GDK_KP_Right:
		case GDK_Right:
			wimp_xine_x11_play ( gui, gui -> xine -> pos_time + 30000 ) ;
			break ;
		case GDK_Up:
		case GDK_KP_Up:
			wimp_xine_x11_play ( gui, gui -> xine -> pos_time + 100000 ) ;
			break ;
		case GDK_Down:
		case GDK_KP_Down:
			wimp_xine_x11_play ( gui, gui -> xine -> pos_time - 100000 ) ;
			break ;
		case GDK_Page_Down:
			wimp_xine_x11_play ( gui, gui -> xine -> pos_time - 1000000 ) ;
			break ;
		case GDK_Page_Up:
			wimp_xine_x11_play ( gui, gui -> xine -> pos_time + 1000000 ) ;
			break ;
		case GDK_space:
		case GDK_5:
		case GDK_p:
			if ( gui -> mode == MDI )
				gtk_toggle_button_set_active ( gui -> play, ! gtk_toggle_button_get_active ( gui -> play )) ;
			else
				wimp_gui2_toggle_pause ( gui ) ;
			break ;
		case GDK_t:
			//wimp_gui_osd_message ( gui, "Hello" ) ;
			break ;
		case GDK_s:
			wimp_gui_playlist_save_playlist_default ( gui ) ;
			break ;
		case GDK_i:
			wimp_gui_meta_info ( gui ) ;
			break ;
		case 65438:
			wimp_file_manager_go_up ( gui ) ;
			break ;
		case 65437:
			wimp_file_manager_play_selected ( gui ) ;
			break ;
		case GDK_asterisk:
		case GDK_plus:
		case GDK_equal:
		case GDK_KP_Add:
			wimp_gui_enhancements_volume_up ( gui ) ;
			break ;
		case GDK_slash:
		case GDK_KP_Subtract:
		case GDK_minus:
		case GDK_hyphen:
			wimp_gui_enhancements_volume_down ( gui ) ;
			break ;
		case GDK_k:
			wimp_gui_config ( gui ) ;
			break ;
		case 65383:
			if ( GTK_WIDGET_HAS_FOCUS ( gui -> browser ))
			{
				GtkMenu * menu = wimp_file_manager_menu_create ( gui ) ;
				g_signal_connect_swapped ( menu, "selection-done", gtk_widget_destroy, menu ) ;
				
				gtk_menu_popup ( menu, null, null, null, null, null, gtk_get_current_event_time ()) ;
			}
			else if ( GTK_WIDGET_HAS_FOCUS ( gui -> playlist ))
			{
				GtkMenu * menu = wimp_gui_playlist_menu_create ( gui ) ;
				g_signal_connect_swapped ( menu, "selection-done", gtk_widget_destroy, menu ) ;
				
				gtk_menu_popup ( menu, null, null, null, null, null, gtk_get_current_event_time ()) ;
			}
			else if ( GTK_WIDGET_HAS_FOCUS ( gui -> search ))
			{
				GtkMenu * menu = wimp_search_create_menu ( gui ) ;
				g_signal_connect_swapped ( menu, "selection-done", gtk_widget_destroy, menu ) ;
				
				gtk_menu_popup ( menu, null, null, null, null, null, gtk_get_current_event_time ()) ;
			}
			else
			{
				GtkMenu * menu = wimp_gui_menu_create ( gui ) ;
				g_signal_connect_swapped ( menu, "selection-done", gtk_widget_destroy, menu ) ;
				
				gtk_menu_popup ( menu, null, null, null, null, null, gtk_get_current_event_time ()) ;
			}
			
			break ;
		case GDK_Insert:
			wimp_file_manager_enqueue ( gui ) ;
			break ;
		case GDK_d:
			wimp_gui2_dance ( gui ) ;
			break ;
		case '1':
			wimp_gui2_zoom_100 ( gui ) ;
			break ;
		case '2':
			wimp_gui2_zoom_2x ( gui ) ;
			break ;
		case '`':
			wimp_gui2_zoom_x_by_2 ( gui ) ;
			break ;
		case '3':
			wimp_gui2_zoom_3x ( gui ) ;
			break ;
		case 'j':
			wimp_gui2_jump_to ( gui ) ;
			break ;
		case 'l':
			wimp_show_log () ;
			break ;
		default:
			out ;
			
			return false ;
			break ;
	}

	return true ;
	out ;
}

void wimp_gui_osd_message ( WimpGUI * gui, gchar * message )
{
	in ;
	
	gui -> xine -> vpts = xine_get_current_vpts ( gui -> xine -> stream ) ;
	gui -> xine -> osd_shown = gui -> xine -> pos_time ;
	
	xine_osd_clear ( gui -> xine -> osd ) ;
	
	xine_osd_draw_text ( gui -> xine -> osd, 10, 0, message, 0 ) ;
	xine_osd_show ( gui -> xine -> osd, gui -> xine -> vpts ) ;
	
	out ;
}

void wimp_gui_osd_indicate_position ( WimpGUI * gui )
{
	in ;
	
	gchar * display = wimp_format_time ( gui -> xine -> pos_time, gui -> xine -> length ) ;
	wimp_gui_osd_message ( gui, display ) ;
	
	g_free ( display ) ;
	out ;
}
	
void wimp_gui_load ( WimpGUI * gui, gchar * filename )
{
	in ;
	xine_close ( gui -> xine -> stream ) ;
	
	gui -> xine -> pos_stream = 0 ;
	gui -> xine -> pos_time = 0 ;
	
	gui -> xine -> paused_at = 0 ;
	gui -> xine -> timestamp = gtk_get_current_event_time () ;
	
	gui -> xine -> length = 0 ;

	if ( filename == null )
		g_critical ( "Filename is null !!!" ) ;
	
	else
	{
		wimp_gui_playlist_clear ( gui ) ;
		wimp_gui_playlist_append ( gui, filename ) ;
	}
		
	
	out ;
}
	
void wimp_gui_send_expose_event ( WimpGUI * gui )
{
	in ;
	
	XExposeEvent event ;
	event.type = Expose ;
	
	event.send_event = TRUE ;
	event.display = gdk_x11_display_get_xdisplay ( gdk_display_get_default ()) ;
	
	event.window = gui -> xine -> xwindow ;
	event.x = 0 ;
	
	event.y = 0 ;
	
	gint x = 0, y = 0 ;
	gtk_window_get_size ( gui -> window, & x, & y ) ;
	
	event.width = x ;
	event.height = y ;

	event.count = 0 ;
	xine_port_send_gui_data ( gui -> xine -> vo, XINE_GUI_SEND_EXPOSE_EVENT, & event ) ;
	
	gtk_widget_queue_draw ( gui -> window ) ;
	wimp_update_gui () ;
	
	out ;
}
void wimp_cleanup ( WimpGUI * gui )
{
	in ;
	
	//wimp_gui_enhancements_save ( gui ) ;
	
	wimp_gui_playlist_save_playlist_default ( gui ) ;
	wimp_xine_cleanup ( gui -> xine ) ;
	
	gchar * config = g_build_filename ( g_get_home_dir (), ".wimp", "wimp.cfg", null ) ;
	wimp_config_save ( gui -> xine -> config, config ) ;
	
	g_free ( config ) ;
	
	//while ( gtk_main_level () > 1 )
		//gtk_main_quit () ;
	
	gchar * touch = g_build_filename ( g_get_home_dir (), ".wimp", "touch", null ) ;
	remove ( touch ) ;
	
	exit ( 0 ) ;
}

gchar * wimp_prompt_filename ( WimpGUI * gui, GtkFileChooserAction action, gchar * dirname )
{	
	GtkFileChooserDialog * dialog = gtk_file_chooser_dialog_new ( 	"Choose a file ...",
																	null,
																	action,
																	GTK_STOCK_CANCEL,
																	GTK_RESPONSE_CANCEL,
																	GTK_STOCK_OK,
																	GTK_RESPONSE_ACCEPT,
																	null ) ;
	
	if ( dirname )
		gtk_file_chooser_set_current_folder ( dialog, dirname ) ;
	
	GtkFileFilter * filter2 = gtk_file_filter_new () ;
	gtk_file_filter_add_pattern ( filter2, "*" ) ;
	
	gtk_file_filter_set_name ( filter2, "All files" ) ;
	gtk_file_chooser_add_filter ( dialog, filter2 ) ;

	gchar * filename = null ;
	if ( gtk_dialog_run ( dialog ) == GTK_RESPONSE_ACCEPT )
		filename = gtk_file_chooser_get_filename ( dialog ) ;
	
	gtk_widget_destroy ( dialog ) ;
	return filename ;
}

gchar * wimp_xine_get_error ( WimpGUI * gui )
{
	in ;
	gchar * error = null ;
	
	switch ( xine_get_error ( gui -> xine -> stream ))
	{
		case 0:
			error = g_strdup_printf ( "No error." ) ;
			break ;
		case 1:
			error = g_strdup_printf ( "No input plugin." ) ;
			break ;
		case 2:
			error = g_strdup_printf ( "No demuxer plugin." ) ;
			break ;
		case 3:
			error = g_strdup_printf ( "Demuxing failed." ) ;
			break ;
		case 4:
			error = g_strdup_printf ( "Malformed mrl." ) ;
			break ;
		case 5:
			error = g_strdup_printf ( "Input failed." ) ;
			break ;
		default:
			error = g_strdup_printf ( "Unknown error." ) ;
			break ;
	}
	
	out ;
	return error ;
}

void wimp_gui_meta_info ( WimpGUI * gui )
{
	in ;
	GtkTreeStore * store = gtk_tree_store_new ( 2,
												G_TYPE_STRING,
												G_TYPE_STRING ) ;
	
	GtkTreeView * tree_view = gtk_tree_view_new_with_model ( store ) ;
	
	GtkCellRenderer * renderer1 = gtk_cell_renderer_text_new () ;
	GtkTreeViewColumn * column1 = gtk_tree_view_column_new_with_attributes ( "Playlist",
																			renderer1,
																			"text",
																			zero,
																			null ) ;
	GtkCellRenderer * renderer2 = gtk_cell_renderer_text_new () ;
	GtkTreeViewColumn * column2 = gtk_tree_view_column_new_with_attributes ( "Playlist",
																			renderer2,
																			"text",
																			1,
																			null ) ;
	
	gtk_tree_view_append_column ( tree_view, column1 ) ;
	gtk_tree_view_append_column ( tree_view, column2 ) ;
	
	gint i = 0 ;
	for ( i = 0 ; i < 13 ; i ++ )
	{
		GtkTreeIter iter ;
		gtk_tree_store_append ( store, & iter, null ) ;
		
		gtk_tree_store_set ( store, & iter, 0, xine_meta_tags [i], 1, xine_get_meta_info ( gui -> xine -> stream, i ), -1 ) ;
	}
	
	GtkDialog * dialog = gtk_dialog_new_with_buttons (	"Meta Info",
														null,
														GTK_DIALOG_MODAL,
														"Okay",
														1,
														null ) ;
	
	GtkScrolledWindow * sw = gtk_scrolled_window_new ( null, null ) ;
	gtk_container_add ( sw, tree_view ) ;
	
	gtk_tree_view_set_headers_visible ( tree_view, no ) ;
	
	gtk_box_pack_start ( dialog -> vbox, sw, yes, yes, 2 ) ;
	gtk_widget_set_size_request ( dialog, 400, 300 ) ;
	
	gtk_scrolled_window_set_policy ( sw, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC ) ;
	gtk_widget_show_all ( dialog ) ;
	
	gtk_dialog_run ( dialog ) ;
	gtk_widget_destroy ( dialog ) ;
	
	out ;
}

void wimp_gui_adjust_video_display1 ( WimpGUI * gui )
{
	in ;
	
	GtkRequisition qu ;
	gtk_widget_size_request ( gui -> video_display, & qu ) ;
	
	if ( qu.width < 100 )
		qu.width = 420 ; // ?
	if ( qu.height < 100 )
		qu.height = 300 ;
	
	gui -> xine -> width = qu.width ;
	gui -> xine -> height = qu.height ;
	
	out ;
}

GtkMenu * wimp_gui_menu_create ( WimpGUI * gui )
{
	in ;
	
	GtkMenu * menu = gtk_menu_new () ;
	GtkMenuItem * about = gtk_menu_item_new_with_label ( "About Wimp" ),
				* open = gtk_menu_item_new_with_label ( "Open file" ),
				* open_folder = gtk_menu_item_new_with_label ( "Open folder" ),
				* fullscreen = gtk_menu_item_new_with_label ( "Fullscreen" ),
				* exit = gtk_menu_item_new_with_label ( "Exit" ),
				* options = gtk_menu_item_new_with_label ( "Options" ),
				* dance = gtk_menu_item_new_with_label ( "Dance !" ),
				* zoom1 = gtk_menu_item_new_with_label ( "Original size" ),
				* zoom2 = gtk_menu_item_new_with_label ( "Zoom 2x" ),
				* zoom3 = gtk_menu_item_new_with_label ( "Zoom 3x" ),
				* zoom4 = gtk_menu_item_new_with_label ( "Half size" ) ;
	
	if ( gui -> fullscreen )
	{
		gtk_label_set_label ( gtk_bin_get_child ( fullscreen ), "Exit fullscreen" ) ;
		gtk_widget_set_sensitive ( zoom1, no ) ;
		
		gtk_widget_set_sensitive ( zoom2, no ) ;
		gtk_widget_set_sensitive ( zoom3, no ) ;
		
		gtk_widget_set_sensitive ( zoom4, no ) ;
	}
	
	#include "gui2.h"
	
	g_signal_connect_swapped ( zoom1, "activate", wimp_gui2_zoom_100, gui ) ;
	g_signal_connect_swapped ( zoom2, "activate", wimp_gui2_zoom_2x, gui ) ;
	
	g_signal_connect_swapped ( zoom3, "activate", wimp_gui2_zoom_3x, gui ) ;
	g_signal_connect_swapped ( zoom4, "activate", wimp_gui2_zoom_x_by_2, gui ) ;
	
	g_signal_connect_swapped ( about, "activate", wimp_about, gui ) ;
	g_signal_connect_swapped ( options, "activate", wimp_gui_config, gui ) ;

	g_signal_connect_swapped ( open, "activate", wimp_gui_load_prompt, gui ) ;
	g_signal_connect_swapped ( open_folder, "activate", wimp_gui_load_folder_prompt, gui ) ;

	g_signal_connect_swapped ( exit, "activate", wimp_cleanup, gui ) ;
	g_signal_connect_swapped ( fullscreen, "activate", wimp_gui_fullscreen, gui ) ;
	
	g_signal_connect_swapped ( dance, "activate", wimp_gui2_dance, gui ) ;
	
	gtk_menu_shell_append ( menu, about ) ;
	gtk_menu_shell_append ( menu, gtk_separator_menu_item_new ()) ;
	
	gtk_menu_shell_append ( menu, open ) ;
	gtk_menu_shell_append ( menu, open_folder ) ;
	
	gtk_menu_shell_append ( menu, gtk_separator_menu_item_new ()) ;
	gtk_menu_shell_append ( menu, fullscreen ) ;
	gtk_menu_shell_append ( menu, dance ) ;
	
	gtk_menu_shell_append ( menu, gtk_separator_menu_item_new ()) ;
	gtk_menu_shell_append ( menu, zoom4 ) ;
	
	gtk_menu_shell_append ( menu, zoom1 ) ;
	gtk_menu_shell_append ( menu, zoom2 ) ;
	
	gtk_menu_shell_append ( menu, zoom3 ) ;
	gtk_menu_shell_append ( menu, gtk_separator_menu_item_new ()) ;
	
	gtk_menu_shell_append ( menu, options ) ;
	
	gtk_menu_shell_append ( menu, exit ) ;
	gtk_widget_show_all ( menu ) ;
	
	out ;
	return menu ;
}

void wimp_gui_menu_popup ( GtkWidget * widget, GdkEventButton * event, WimpGUI * gui )
{
	in ;
	
	//g_debug ( "%d", event -> type ) ;
	if ( event -> button == 1 )
	{
		if ( event -> type == GDK_2BUTTON_PRESS )
		{
			gtk_toggle_tool_button_set_active ( gui -> fs, ! gtk_toggle_tool_button_get_active ( gui -> fs )) ;
		}
		
		return ;
	}
	
	if ( event -> button == 2 )
		reutern ;
	
	GtkMenu * menu = wimp_gui_menu_create ( gui ) ;
	g_signal_connect_swapped ( menu, "selection-done", gtk_widget_destroy, menu ) ;
	
	gtk_menu_popup ( menu, null, null, null, null, null, gtk_get_current_event_time ()) ;
	out ;
}

void wimp_gui_fullscreen ( WimpGUI * gui )
{
	in ;
	
	gtk_toggle_tool_button_set_active ( gui -> fs, ! gtk_toggle_tool_button_get_active ( gui -> fs )) ;
	out ;
}

void wimp_about1 ( WimpGUI * gui )
{
	in ;
	
	GtkDialog * dialog = gtk_about_dialog_new () ;
	gtk_about_dialog_set_name ( dialog, MY_NAME ) ;
	
	gtk_about_dialog_set_version ( dialog, VERSION ) ;
	gtk_about_dialog_set_copyright ( dialog, "This is free software, and you are welcome to redistribute\nit "
											 "under certain conditions." ) ;
	
	gtk_about_dialog_set_comments ( dialog, "An easy to use and robust media player for Linux." ) ;
	gtk_about_dialog_set_website ( dialog, "djshaji@gmail.com" ) ;
	
	gchar * authors [] = {
		"Shaji <djshaji@gmail.com>",
		null
	} ;
	
	GtkButton * xine = gtk_button_new_with_label ( "Xine engine info" ) ;
	gtk_box_pack_end ( dialog -> action_area, xine, no, no, 5 ) ;
	
	g_signal_connect_swapped ( xine, "clicked", wimp_xine_info, gui ) ;
	
	gtk_about_dialog_set_authors ( dialog, authors ) ;
	gtk_widget_show_all ( dialog ) ;
	
	gtk_dialog_run ( dialog ) ;
	
	gtk_widget_destroy ( dialog ) ;
	out ;
}

void wimp_xine_info ( WimpGUI * gui )
{
	in ;
	
	GtkTreeStore * store = gtk_tree_store_new ( 2,
												G_TYPE_STRING,
												G_TYPE_STRING ) ;
	
	gchar * ext = wimp_string_to_nl ( xine_get_file_extensions  ( gui -> xine -> xine )) ;
	
	GtkTreeIter iter ;
	gtk_tree_store_append ( store, & iter, null ) ;
	
	gtk_tree_store_set ( store, & iter, 0, "Supported formats: ", 1, ext, -1 ) ;
	
	gchar * mime = wimp_string_to_nl ( xine_get_mime_types ( gui -> xine -> xine )) ;
	GtkTreeIter iter1 ;
	gtk_tree_store_append ( store, & iter1, null ) ;
	
	gtk_tree_store_set ( store, & iter1, 0, "Supported mime types: ", 1, mime, -1 ) ;
	
	gchar ** vo1 = xine_list_video_output_plugins ( gui -> xine -> xine ) ;
	gchar * vo = wimp_array_to_char ( vo1 ) ;

	gchar ** ao1 = xine_list_audio_output_plugins ( gui -> xine -> xine ) ;	
	gchar * ao = wimp_array_to_char ( ao1 ) ;
	
	GtkTreeIter iter3 ;
	gtk_tree_store_append ( store, & iter3, null ) ;
	
	gtk_tree_store_set ( store, & iter3, 0, "Video outputs available: ", 1, vo, -1 ) ;
	
	GtkTreeIter iter4 ;
	gtk_tree_store_append ( store, & iter4, null ) ;
	
	gtk_tree_store_set ( store, & iter4, 0, "Audio outputs available: ", 1, ao, -1 ) ;

	gchar ** demuxers1 = xine_list_demuxer_plugins ( gui -> xine -> xine ) ;
	gchar * demuxers = wimp_array_to_char ( demuxers1 ) ;

	gchar ** input1 = xine_list_input_plugins ( gui -> xine -> xine ) ;
	gchar * input = wimp_array_to_char ( input1 ) ;
	
	GtkTreeIter iter41 ;
	gtk_tree_store_append ( store, & iter41, null ) ;
	
	gtk_tree_store_set ( store, & iter41, 0, "Demuxers available: ", 1, demuxers, -1 ) ;

	GtkTreeIter iter5 ;
	gtk_tree_store_append ( store, & iter5, null ) ;
	
	gtk_tree_store_set ( store, & iter5, 0, "Inputs available: ", 1, input, -1 ) ;

	gchar  ** adec1 = xine_list_audio_decoder_plugins ( gui -> xine -> xine ) ;
	gchar * adec = wimp_array_to_char ( adec1 ) ;

	gchar ** vdec1 = xine_list_video_decoder_plugins ( gui -> xine -> xine ) ;
	gchar * vdec = wimp_array_to_char ( vdec1 ) ;

	GtkTreeIter iter6 ;
	gtk_tree_store_append ( store, & iter6, null ) ;
	
	gtk_tree_store_set ( store, & iter6, 0, "Video decoders available: ", 1, vdec, -1 ) ;

	GtkTreeIter iter7 ;
	gtk_tree_store_append ( store, & iter7, null ) ;
	
	gtk_tree_store_set ( store, & iter7, 0, "Audio decoders available: ", 1, adec, -1 ) ;

	g_free ( ao ) ;
	g_free ( vo ) ;
	
	g_free ( demuxers ) ;
	g_free ( input ) ;
	
	g_free ( adec ) ;
	g_free ( vdec ) ;
	
	g_free ( ext ) ;
	g_free ( mime ) ;

	GtkTreeView * tree_view = gtk_tree_view_new_with_model ( store ) ;
	
	GtkCellRenderer * renderer1 = gtk_cell_renderer_text_new () ;
	GtkTreeViewColumn * column1 = gtk_tree_view_column_new_with_attributes ( "Playlist",
																			renderer1,
																			"text",
																			zero,
																			null ) ;
	GtkCellRenderer * renderer2 = gtk_cell_renderer_text_new () ;
	GtkTreeViewColumn * column2 = gtk_tree_view_column_new_with_attributes ( "Playlist",
																			renderer2,
																			"text",
																			1,
																			null ) ;
	
	gtk_tree_view_append_column ( tree_view, column1 ) ;
	gtk_tree_view_append_column ( tree_view, column2 ) ;
	
	GtkDialog * dialog = gtk_dialog_new_with_buttons (	"Xine engine info",
														null,
														GTK_DIALOG_MODAL,
														"Okay",
														1,
														null ) ;
	
	GtkScrolledWindow * sw = gtk_scrolled_window_new ( null, null ) ;
	gtk_container_add ( sw, tree_view ) ;
	
	gtk_tree_view_set_headers_visible ( tree_view, no ) ;
	
	gtk_box_pack_start ( dialog -> vbox, sw, yes, yes, 2 ) ;
	gtk_widget_set_size_request ( dialog, 550, 400 ) ;
	
	gtk_scrolled_window_set_policy ( sw, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC ) ;
	gtk_widget_show_all ( dialog ) ;
	
	gtk_dialog_run ( dialog ) ;
	gtk_widget_destroy ( dialog ) ;
	
	out ;
}

gboolean wimp_gui_seeker ( GtkWidget * widget, GdkEventButton * event, WimpGUI * gui )
{
	gtk_widget_set_sensitive ( widget, no ) ;
	return false ;
}

private void set_thumbnails ( GtkWidget * widget, WimpGUI * gui )
{
	gui -> search_show_thumbnails = gtk_toggle_button_get_active ( widget ) ;
}

private void set_thumbnails_size ( GtkWidget * combo, WimpGUI * gui )
{
	gchar * text = gtk_combo_box_get_active_text ( combo ) ;
	gui -> xine -> config -> thumbnail_size = atoi ( text ) ;
	
	g_free ( text ) ;
}

void wimp_gui_config ( WimpGUI * gui )
{
	in ;
	GtkDialog * dialog = gtk_dialog_new_with_buttons ( "Preferences",
													   null,
													   GTK_DIALOG_MODAL,
													   "Okay",
													   1,
													   null ) ;
	
	GtkTable * table = gtk_table_new ( 1, 1, no ) ;
	GtkTable * table1 = gtk_table_new ( 1, 1, no ) ;
	
	GtkFrame * frame1 = gtk_frame_new ( "Drivers" ) ;
	gtk_container_add ( frame1, table1 ) ;
	
	GtkComboBox * vo = gtk_combo_box_new_text () ;
	
	GtkTable * table2 = gtk_table_new ( 1, 1, no ) ;
	GtkFrame * frame2 = gtk_frame_new ( "Misc" ) ;
	
	gtk_container_add ( frame2, table2 ) ;
	
	GtkTable * table3 = gtk_table_new ( 1, 1, no ) ;
	GtkFrame * frame3 = gtk_frame_new ( "Experimental features" ) ;
	
	GtkCheckButton * memlock = gtk_check_button_new () ;
	gtk_toggle_button_set_active ( memlock, gui -> xine -> config -> memlock ) ;
	
	GtkHBox * box = gtk_hbox_new ( no, 2 ) ;
	
	GtkFrame * frame4 = gtk_frame_new ( "Search options" ) ;
	GtkTable * tab = gtk_table_new ( 1, 1, no ) ;
	
	gtk_container_add ( frame4, tab ) ;
	
	GtkCheckButton * tn = gtk_combo_box_new_text () ;
	GtkCheckButton * st = gtk_check_button_new_with_label ( "Show thumbnails" ) ;
	
	gtk_table_attach ( tab, st, 0, 3, 0, 1, GTK_FILL, GTK_SHRINK, 5, 5 ) ;
	gtk_table_attach ( tab, gtk_label_new ( "Thumbnail size" ), 0, 1, 2, 3, GTK_SHRINK, GTK_SHRINK, 5, 5 ) ;
	
	gtk_table_attach ( tab, tn, 2, 3, 2, 3, GTK_SHRINK, GTK_SHRINK, 10, 5 ) ;
		
	gint u = 0 ;
	while ( thumbnail_sizes [u] != null )
	{
		gtk_combo_box_append_text ( tn, thumbnail_sizes [u] ) ;
		u ++ ;
	}
	
	g_signal_connect ( tn, "changed", set_thumbnails_size, gui ) ;
	g_signal_connect ( st, "toggled", set_thumbnails, gui ) ;
	
	GtkVBox * vbox = gtk_vbox_new ( no, 2 ) ;
	gtk_box_pack_start ( vbox, box, no, no, 2 ) ;
	
	gtk_box_pack_start ( box, gtk_image_new_from_stock ( GTK_STOCK_DIALOG_WARNING, GTK_ICON_SIZE_BUTTON ), no, no, 5 ) ;
	gtk_box_pack_start ( box, gtk_label_new ( "Lock memory pages\nto physical RAM" ), no, no, 2 ) ;
	
	gtk_box_pack_start ( vbox, memlock, no, no, 2 ) ;
	gtk_container_add ( frame3, vbox ) ;
	
	gtk_box_pack_start ( vbox, frame4, no, no, 2 ) ;
	
	gtk_combo_box_append_text ( vo, "auto" ) ;
	gtk_combo_box_set_active ( vo, 0 ) ;
	
	gchar ** ov = xine_list_video_output_plugins ( gui -> xine -> xine ) ;
	gint i = 0 ;
	
	while ( ov [i] != null )
	{
		gtk_combo_box_append_text ( vo, ov [i] ) ;
		if ( strcmp ( ov [i], gui -> xine -> config -> vo ) == 0 )
			gtk_combo_box_set_active ( vo, i + 1 ) ;
		
		i ++ ;
	}

	
	GtkComboBox * ao = gtk_combo_box_new_text () ;
	gtk_combo_box_append_text ( ao, "auto" ) ;
	
	gtk_combo_box_set_active ( ao, 0 ) ;
	
	gchar ** oa = xine_list_audio_output_plugins ( gui -> xine -> xine ) ;
	i = 0 ;
	
	while ( oa [i] != null )
	{
		gtk_combo_box_append_text ( ao, oa [i] ) ;
		if ( strcmp ( oa [i], gui -> xine -> config -> ao ) == 0 )
			gtk_combo_box_set_active ( ao, i + 1 ) ;
		
		i ++ ;
	}

	gtk_table_attach ( table1, gtk_label_new ( "Video output" ), 0, 1, 0, 1,GTK_SHRINK, GTK_SHRINK, 5, 5 ) ;
	gtk_table_attach ( table1, vo, 2, 3, 0, 1,GTK_SHRINK, GTK_SHRINK, 5, 5 ) ;

	gtk_table_attach ( table1, gtk_label_new ( "Audio output" ), 0, 1, 2, 3,GTK_SHRINK, GTK_SHRINK, 5, 5 ) ;
	gtk_table_attach ( table1, ao, 2, 3, 2, 3,GTK_SHRINK, GTK_SHRINK, 5, 5 ) ;

	gtk_table_attach ( table, frame1, 0, 1, 0, 1, GTK_FILL, GTK_FILL, 2, 2 ) ;
	gtk_table_attach ( table, frame2, 0, 1, 2, 3, GTK_FILL, GTK_FILL, 2, 2 ) ;
	
	gtk_table_attach ( table, frame3, 2, 3, 0, 3, GTK_FILL, GTK_FILL, 2, 2 ) ;
	
	GtkCheckButton * check = gtk_check_button_new_with_label ( "Enable emergency\nexit strategy" ) ;
	gtk_toggle_button_set_active ( check, gui -> xine -> config -> emergency_exit_enabled ) ;

	GtkButton * reset = gtk_button_new_with_label ( "Reset settings" ) ;
	
	gtk_table_attach ( table2, check, 0, 1, 0, 1,GTK_SHRINK, GTK_SHRINK, 5, 5 ) ;
	gtk_box_pack_start ( dialog -> vbox, table, yes, yes, 5 ) ;
	
	gtk_table_attach ( table2, reset, 2, 3, 0, 1, GTK_SHRINK, GTK_SHRINK, 5, 5 ) ;

	g_signal_connect_after ( vo, "changed", wimp_gui_config_set_vo, gui ) ;
	g_signal_connect_after ( ao, "changed", wimp_gui_config_set_ao, gui ) ;
	
	g_signal_connect ( check, "toggled", wimp_gui_config_set_emergency, gui ) ;
	g_signal_connect_swapped ( reset, "clicked", wimp_gui_config_reset, gui ) ;

	g_signal_connect ( memlock, "toggled", wimp_gui_config_memlock, gui ) ;
	g_signal_connect_swapped ( reset, "clicked", gtk_dialog_response, dialog ) ;

	gtk_widget_show_all ( dialog ) ;
	gtk_dialog_run ( dialog ) ;
	
	gtk_widget_destroy ( dialog ) ;
	
	static gboolean ot = true ;
	if ( ot )
	{
		g_message ( "You have to restart WiMP for some settings to take effect." ) ;
		ot = no ;
	}
	
	out ;
}

void wimp_gui_config_set_vo ( GtkWidget * widget, WimpGUI * gui )
{
	in ;
	
	gchar * vo = gtk_combo_box_get_active_text ( widget ) ;
	if ( gui -> xine -> config -> vo ) ;
		g_free ( gui -> xine -> config -> vo ) ;
	
	gchar * config = g_build_filename ( g_get_home_dir (), ".wimp", "xine.cfg", null ) ;
	gui -> xine -> config -> vo = vo ;

	wimp_config_save ( gui -> xine -> config, config ) ;
	g_free ( config ) ;

	out ;
}

void wimp_gui_config_set_ao ( GtkWidget * widget, WimpGUI * gui )
{
	in ;
	
	gchar * ao = gtk_combo_box_get_active_text ( widget ) ;
	if ( gui -> xine -> config -> ao ) ;
		g_free ( gui -> xine -> config -> ao ) ;
	
	gchar * config = g_build_filename ( g_get_home_dir (), ".wimp", "xine.cfg", null ) ;
	gui -> xine -> config -> ao = ao ;

	wimp_config_save ( gui -> xine -> config, config ) ;
	g_free ( config ) ;

	out ;
}

void wimp_gui_config_set_emergency ( GtkWidget * widget, WimpGUI * gui )
{
	in ;

	gui -> xine -> config -> emergency_exit_enabled = gtk_toggle_button_get_active ( widget ) ;
	gchar * config = g_build_filename ( g_get_home_dir (), ".wimp", "xine.cfg", null ) ;
	wimp_config_save ( gui, config ) ;
	
	g_free ( config ) ;
out ;
}

void wimp_gui_config_reset ( WimpGUI * gui )
{
	in ;
	
	static gint warn = 0 ;
	if ( warn == 0 )
		if ( ! wimp_message_question ( "All custom settings will be lost. Sure you want to continue ?" ))
			return ;
	
	warn ++ ;

	gchar * config = g_build_filename ( g_get_home_dir (), ".wimp", "xine.cfg", null ) ;
	wimp_config_save_default ( config ) ;
	
	g_free ( config ) ;

	g_free ( gui -> xine -> config -> vo ) ;
	g_free ( gui -> xine -> config -> ao ) ;
	
	gui -> xine -> config -> ao = g_strdup ( "auto" ) ;
	gui -> xine -> config -> vo = g_strdup ( "auto" ) ;
	
	out ;
}

void wimp_gui_config_memlock ( GtkWidget * widget, WimpGUI * gui )
{
	in ;
	
	static gboolean warn = on ;
	if ( warn )
	{
		g_warning (	"Hm.\n\n"
					"Well, okay. Maybe. Maybe not. Whatever. Really? Perhaps not.\n"
					"This \"feature\" will tell the system to lock our pages in memory "
					"and not page them out. Good thing is, our song shall remain playing "
					"even under heavy load. Bad news is, your system ( might ) run a little "
					"slow(er). Try it. You might like it." ) ;
		
		warn = no ;
	}
	
	gui -> xine -> config -> memlock = gtk_toggle_button_get_active ( widget ) ;
	if ( gui -> xine -> config -> memlock )
	{
		if ( mlockall ( MCL_FUTURE ) != 0 )
			g_critical ( "Cannot lock memory: %s", strerror ( errno )) ;
	}
	else
		munlockall () ;
	
	out ;
}

void wimp_session_get_event ( WimpGUI * gui )
{
	in ;
	
	gchar * file = g_build_filename ( g_get_home_dir (), ".wimp", "connect", null ) ;
	GKeyFile * key = g_key_file_new () ;
	
	GError * error = null ;
	g_key_file_load_from_file ( key, file, G_KEY_FILE_NONE, & error ) ;
	
	if ( error )
	{
		g_warning ( "Cannot interpret incoming message: %s", error -> message ) ;
		g_free ( file ) ;
		
		return ;
	}
	
	Action action = g_key_file_get_integer ( key, "action", "action", null ) ;
	gchar * mrl = g_key_file_get_string ( key, "action", "mrl", null ) ;

	switch ( action )
	{
		case ActionPlay:
			gtk_toggle_button_toggled ( gui -> play ) ;
			break ;
		case ActionEnqueue:
			wimp_gui_playlist_append ( gui, mrl ) ;
			break ;
		case ActionOpen:
			wimp_gui_playlist_clear ( gui ) ;
			wimp_gui_playlist_append ( gui, mrl ) ;
			
			wimp_gui_playlist_play ( gui, mrl ) ;
			break ;
		case ActionNext:
			wimp_gui_playlist_next ( gui ) ;
			break ;
		case ActionPrev:
			wimp_gui_playlist_previous ( gui ) ;
			break ;
		default:
			g_message ( "Action %d not yet implemented.", action ) ;
	}
	
	g_free ( mrl ) ;
	g_free ( file ) ;
	
	g_key_file_free ( key ) ;
	out ;
}

void wimp_sigaction ( gint signum, siginfo_t * t, void * data )
{
	in ;
	wimp_session_get_event ( my_wimp ) ;
	
	out ;
}

#include "info.c"
void wimp_about ( WimpGUI * gui )
{
	in ;
	
	GtkTextView * ifaq = gtk_text_view_new (),
				* ireadme = gtk_text_view_new (),
				* ikeys = gtk_text_view_new () ;
	
	gtk_text_view_set_cursor_visible ( ifaq, no ) ;
	gtk_text_view_set_editable ( ifaq, no ) ;
	
	gtk_text_view_set_cursor_visible ( ireadme, no ) ;
	gtk_text_view_set_editable ( ireadme, no ) ;

	gtk_text_view_set_cursor_visible ( ikeys, no ) ;
	gtk_text_view_set_editable ( ikeys, no ) ;

	gtk_text_view_set_wrap_mode ( ifaq, GTK_WRAP_WORD ) ;
	gtk_text_view_set_wrap_mode ( ikeys, GTK_WRAP_WORD ) ;

	gtk_text_view_set_wrap_mode ( ireadme, GTK_WRAP_WORD ) ;

	GtkScrolledWindow * sw = gtk_scrolled_window_new ( null, null ),
					  * sw1 = gtk_scrolled_window_new ( null, null ),
					  * sw2 = gtk_scrolled_window_new ( null, null ) ;

	gtk_container_add ( sw, ireadme ) ;
	gtk_container_add ( sw1, ifaq ) ;
	
	gtk_container_add ( sw2, ikeys ) ;
	
	GtkTextIter iter1, iter2, iter3 ;
	gtk_text_buffer_get_iter_at_line ( gtk_text_view_get_buffer ( ifaq ), & iter1, 0 ) ;

	gtk_text_buffer_get_iter_at_line ( gtk_text_view_get_buffer ( ireadme ), & iter2, 0 ) ;
	gtk_text_buffer_get_iter_at_line ( gtk_text_view_get_buffer ( ikeys ), & iter3, 0 ) ;

	gtk_text_buffer_insert ( gtk_text_view_get_buffer ( ifaq ), & iter1, faq, -1 ) ;
	gtk_text_buffer_insert ( gtk_text_view_get_buffer ( ireadme ), & iter2, readme, -1 ) ;

	gtk_text_buffer_insert ( gtk_text_view_get_buffer ( ikeys ), & iter3, keyboard_shortcuts, -1 ) ;

	GtkDialog * dialog = gtk_dialog_new_with_buttons ( 	"About",
														null,
														GTK_DIALOG_MODAL,
														"Cool.",
														1,
														null ) ;
	
	
	GdkColor b ;
	b.red = 255 * 257;
	b.green = 255 * 257;
	b.blue = 255 * 257;
	
	GtkNotebook * note = gtk_notebook_new () ;
	gtk_box_pack_start ( dialog -> vbox, note, yes, yes, 2 ) ;
	
	//gtk_widget_modify_bg ( note, GTK_STATE_NORMAL, & b ) ;
	//gtk_widget_modify_bg ( dialog, GTK_STATE_NORMAL, & b ) ;
	
	gtk_notebook_append_page ( note, sw, gtk_label_new ( "Readme" )) ;
	
	gtk_notebook_append_page ( note, sw1, gtk_label_new ( "FAQ" )) ;
	gtk_notebook_append_page ( note, sw2, gtk_label_new ( "Keyboard Shortcuts" )) ;
	
	gtk_scrolled_window_set_policy ( sw, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC ) ;
	gtk_scrolled_window_set_policy ( sw1, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC ) ;
	
	gtk_scrolled_window_set_policy ( sw2, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC ) ;
	gtk_window_resize ( dialog, 500, 350 ) ;
	
	gtk_widget_show_all ( dialog ) ;
	
	gtk_dialog_run ( dialog ) ;
	gtk_widget_destroy ( dialog ) ;
	
	out ;
}

void wimp_gui_set_status ( WimpGUI * gui, gchar * message )
{
	in ;
	
	gint id = gtk_statusbar_get_context_id ( gui -> status, message ) ;
	gtk_statusbar_push ( gui -> status, id, message ) ;
	
	out ;
}

void wimp_xine_event_listener ( WimpGUI * data, xine_event_t * event )
{
	switch ( event -> type )
	{
		case XINE_EVENT_PROGRESS:
			{
				xine_progress_data_t *pevent = (xine_progress_data_t *) event->data;
				printf("%s [%d%%]\n", pevent->description, pevent->percent);
				
				gchar * msg = g_strdup_printf ( "[%d%%] %s", pevent -> percent, pevent -> description ) ;
				wimp_gui_set_status ( data , msg ) ;
				
				g_free ( msg ) ;
				wimp_update_gui () ;
			}
			break ;
		default:
			break ;
	}
}

