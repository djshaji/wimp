/*
 *      gui2.c
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

#include "gui2.h"

void * wimp_gui2_init ( WimpGUI * wimp )
{
	in ;
	
	wimp -> main_window = gtk_window_new ( GTK_WINDOW_TOPLEVEL ) ;
	gtk_window_move ( wimp -> main_window, 40, 20 ) ;
	
	gtk_window_resize ( wimp -> main_window, 400, 350 ) ;
	gtk_window_set_title ( wimp -> main_window, "Wimp Video" ) ;

	GtkDrawingArea * area = gtk_event_box_new () ;
	gtk_container_add ( wimp -> main_window, area ) ;
	
	gtk_event_box_set_visible_window ( area, no ) ;
	
	GtkLabel * lx = gtk_label_new ( null ) ;
	gtk_label_set_markup ( lx, 	"<span font_desc=\"24\"><b>[ wimp ]</b></span>"
								"\n\n<b>Now loading the WiMPlayer ...</b>"
								"\nAre <b>you</b> ready ?\n\n\n\n\n"
								"DJ Shaji [ "
								"<span size=\"small\"><b>djshaji@gmail.com"
								"</b></span> ]" ) ;
	
	gtk_container_add ( area, lx ) ;
	
	gtk_widget_show_all ( wimp -> main_window ) ;
	wimp_update_gui () ;
	
	wimp -> controls_window = gtk_window_new ( GTK_WINDOW_TOPLEVEL ) ;
	wimp -> playlist_window = gtk_window_new ( GTK_WINDOW_TOPLEVEL ) ;
	
	GtkWidget * seekbar = gtk_hscale_new ( null ) ;
	GtkButton * play = gtk_button_new () ;
	
	gtk_button_set_relief ( play, GTK_RELIEF_NONE ) ;
	gtk_container_add ( play, gtk_image_new_from_stock ( GTK_STOCK_MEDIA_PLAY, GTK_ICON_SIZE_DIALOG )) ;
	
	GtkFrame * f1 = gtk_frame_new ( null ); 
	gtk_container_add ( f1, play ) ;
	
	gtk_range_set_range ( seekbar, 0.0, 1.0 ) ;
	gtk_range_set_increments ( seekbar, 0.01, 0.1 ) ;
	
	gtk_scale_set_draw_value ( seekbar, no ) ;
	wimp -> seekbar = seekbar ;
	
	GtkHBox * box1 = gtk_hbox_new ( no, 2 ) ;
	gtk_container_add ( wimp -> controls_window , box1 ) ;

	GtkVBox * box2 = gtk_vbox_new ( no, 0 ) ;
	gtk_box_pack_start ( box2, seekbar, no, no, 5 ) ;
	
	GtkToolbar * toolbar = gtk_toolbar_new () ;
	gtk_box_pack_start ( box2, toolbar, no, no, 0 ) ;
	
	GtkToolbar * t2 = gtk_toolbar_new () ;
	gtk_box_pack_start ( box2, t2, no, no, 0 ) ;
	
	GtkToolButton * show_video = gtk_tool_button_new ( null, "Video" ) ;
	GtkToolButton * show_playlist = gtk_tool_button_new ( null, "Playlist" ),
				  * show_log = gtk_tool_button_new ( null, null ) ;
	
	GtkLabel * ll = gtk_label_new ( null ),
			 * lv = gtk_label_new ( null ),
			 * lp = gtk_label_new ( null ) ;
			 
	gtk_label_set_markup ( ll, "<span size=\"small\"><b>Video</b></span>" ) ;
	gtk_label_set_markup ( lv, "<span size=\"small\"><b>Playlist</b></span>" ) ;
	
	gtk_label_set_markup ( lp, "<span size=\"small\"><b>Log</b></span>" ) ;
	
	gtk_tool_button_set_label_widget ( show_video, ll ) ;
	gtk_tool_button_set_label_widget ( show_playlist, lv ) ;
	
	gtk_tool_button_set_label_widget ( show_log, lp ) ;
	
	GtkToolItem * l = gtk_separator_tool_item_new () ;
	gtk_tool_item_set_expand ( l, on ) ;
	
	gtk_separator_tool_item_set_draw ( l, no ) ;
	gtk_toolbar_insert ( t2, l, -1 ) ;
	
	gtk_tool_item_set_is_important ( show_video, on ) ;
	gtk_tool_item_set_is_important ( show_playlist, on ) ;
	gtk_tool_item_set_is_important ( show_log, on ) ;
	
	gtk_toolbar_insert ( t2, show_video, -1 ) ;
	gtk_toolbar_insert ( t2, show_playlist, -1 ) ;
	gtk_toolbar_insert ( t2, show_log, -1 ) ;
	
	g_signal_connect_swapped ( show_video, "clicked", gtk_widget_show, wimp -> main_window ) ;
	g_signal_connect_swapped ( show_playlist, "clicked", gtk_widget_show, wimp -> playlist_window ) ;
	g_signal_connect_swapped ( show_log, "clicked", wimp_show_log, wimp -> playlist_window ) ;
	
	gtk_box_pack_start ( box1, f1, no, no, 0 ) ;
	gtk_box_pack_start ( box1, box2, yes, yes, 0 ) ;
	
	gtk_toolbar_set_icon_size ( toolbar, GTK_ICON_SIZE_SMALL_TOOLBAR ) ;
	gtk_window_resize ( wimp -> controls_window, 400, 80 ) ;
	
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
	
	GtkSeparatorToolItem * sep1 = gtk_separator_tool_item_new (),
						 * sep2 = gtk_separator_tool_item_new () ;
						 
	gtk_separator_tool_item_set_draw ( sep1, no ) ;
	gtk_separator_tool_item_set_draw ( sep2, no ) ;
	
	gtk_tool_item_set_expand ( sep1, on ) ;
	//gtk_tool_item_set_expand ( sep2, on ) ;
	
	//gtk_toolbar_insert ( toolbar, sep1, -1 ) ;
	//gtk_toolbar_insert ( toolbar, play, -1 ) ;
	gtk_toolbar_insert ( toolbar, pause, -1 ) ;
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
	
	gtk_toolbar_insert ( toolbar, sep2, -1 ) ;
	wimp -> volume = gtk_hscale_new ( null ) ;
	
	GtkToolItem * holder = gtk_tool_item_new () ;
	
	gtk_container_add ( holder, wimp -> volume ) ;
	gtk_toolbar_insert ( toolbar, holder, -1 ) ;
	
	gtk_tool_item_set_expand ( holder, yes ) ;
	gtk_scale_set_draw_value ( wimp -> volume, no ) ;
	
	gtk_range_set_range ( wimp -> volume, 1, 100 ) ;
	gtk_range_set_increments ( wimp -> volume, 1, 10 ) ;
	
	
	wimp -> status = gtk_statusbar_new () ;
	gtk_statusbar_set_has_resize_grip ( wimp -> status, no ) ;
	
	gtk_box_pack_start ( box2, wimp -> status, no, no, 0 ) ;
	
	g_signal_connect_swapped ( ff, "clicked", wimp_gui_fast_forward, wimp ) ;
	g_signal_connect_swapped ( rev, "clicked", wimp_gui_rewind, wimp ) ;

	g_signal_connect_swapped ( play, "clicked", wimp_gui2_toggle_pause, wimp ) ;
	g_signal_connect_swapped ( pause, "clicked", wimp_gui2_toggle_pause, wimp ) ;
	g_signal_connect_swapped ( stop, "clicked", wimp_gui2_toggle_pause, wimp ) ;
	
	g_signal_connect ( seekbar, "change-value", wimp_gui_seek, wimp ) ;
	g_signal_connect ( seekbar, "button-press-event", wimp_gui_seeker, wimp ) ;
	g_signal_connect_after ( wimp -> volume, "change-value", wimp_gui_enhancements_volume, wimp ) ;
	
		
	/* Playlist */
	GtkWidget * notebook1 = gtk_notebook_new () ;
	GtkWidget * notebook2 = wimp_gui_enhancements ( wimp ) ;
	
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

	gtk_scrolled_window_set_policy ( sw1, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC ) ;
	
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
	
	GtkVBox * vox = gtk_vbox_new ( no, 2 ) ;
	gtk_container_add ( wimp -> playlist_window, vox ) ;
	
	gtk_box_pack_start ( vox, notebook1, yes, yes, 5 ) ;
	gtk_box_pack_start ( vox, notebook2, no, no, 5 ) ;

	GtkTreeModel * godel = gtk_tree_store_new ( 5, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, GDK_TYPE_PIXBUF, G_TYPE_INT ) ;
	GtkIconView * gview = gtk_tree_view_new_with_model ( godel ) ;
	
	gtk_tree_view_set_enable_search ( gview, true ) ;
	gtk_tree_view_set_search_equal_func ( gview, wimp_gui_playlist_search_func, null, null ) ;

	gtk_tree_view_set_headers_visible ( gview, off ) ;
	//gtk_tree_selection_set_mode ( gtk_tree_view_get_selection ( icon_view ), GTK_SELECTION_MULTIPLE ) ;

	GtkCellRenderer * grendererx = gtk_cell_renderer_text_new () ;
	GtkTreeViewColumn * gcolumnx = gtk_tree_view_column_new_with_attributes ( "Playlist",
																			grendererx,
																			"text",
																			0,
																			null ) ;
	
	GtkCellRenderer * grenderery = gtk_cell_renderer_pixbuf_new () ;
	GtkTreeViewColumn * gcolumny = gtk_tree_view_column_new_with_attributes ( "Playlist",
																			grenderery,
																			"pixbuf",
																			3,
																			null ) ;
	
	gtk_tree_view_append_column ( gview, gcolumny ) ;
	gtk_tree_view_append_column ( gview, gcolumnx ) ;
	
	GtkScrolledWindow * gws = gtk_scrolled_window_new ( null, null ) ;
	gtk_container_add ( gws, gview ) ;
	
	GtkVBox * gxo = gtk_vbox_new ( no, 0 ) ;
	gtk_box_pack_start ( gxo, gws, yes, yes, 2 ) ;
	
	wimp -> search_entry = gtk_entry_new () ;
	gtk_entry_set_text ( wimp -> search_entry, "Enter web address" ) ;
	
	gtk_box_pack_start ( gxo, wimp -> search_entry, no, no, 0 ) ;
	
	GtkToolbar * st = gtk_toolbar_new () ;
	gtk_box_pack_start ( gxo, st, no, no, 0 ) ;
	
	GtkToolButton * hm = gtk_tool_button_new_from_stock ( GTK_STOCK_HOME ),
				  * im = gtk_tool_button_new_from_stock ( GTK_STOCK_MISSING_IMAGE ),
				  * op = gtk_tool_button_new_from_stock ( GTK_STOCK_STOP ) ;
	
	gtk_toolbar_insert ( st, hm, -1 ) ;
	gtk_toolbar_insert ( st, im, -1 ) ;
	
	g_signal_connect_swapped ( hm, "clicked", wimp_gui2_search_browse_favorites, wimp ) ;
	wimp -> search_stop = op ;
	
	g_signal_connect_swapped ( im, "clicked", wimp_search_load_images, wimp ) ;
	gtk_toolbar_insert ( st, op, -1 ) ;
	
	g_signal_connect ( op, "clicked", fire_event, null ) ;
	gtk_widget_set_sensitive ( op, no ) ;
	
	GtkToolItem * it = gtk_tool_item_new () ;
	GtkProgressBar * ar = gtk_progress_bar_new () ;
	
	gtk_widget_set_size_request ( ar, 50, 20 ) ;
	
	gtk_container_add ( it, ar ) ;
	gtk_toolbar_insert ( st, it, -1 ) ;
	
	gtk_widget_show_all ( it ) ;
	gtk_tool_item_set_is_important ( it, true ) ;
	
	gtk_tool_item_set_expand ( it, true ) ;
	wimp -> search_bar = ar ;
	
	g_signal_connect_swapped ( wimp -> search_entry, "activate", wimp_search_search, wimp ) ;
	
	gtk_notebook_append_page ( notebook1, gxo, gtk_label_new ( "Web" )) ;
	gtk_scrolled_window_set_policy ( gws, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC ) ;
	
	gtk_widget_show_all ( wimp -> main_window ) ;
	gtk_widget_show_all ( wimp -> playlist_window ) ;
	
	gtk_widget_show_all ( wimp -> controls_window ) ;
	
	g_signal_connect_after ( wimp -> controls_window, "button-press-event", wimp_gui_menu_popup, wimp ) ;
	g_signal_connect ( wimp -> main_window, "key-press-event", wimp_hotkeys_gui, wimp ) ;

	/* Signals */
	
	/* Obsolete */
	wimp -> window = wimp -> main_window ;
	wimp -> video_display = area ;
	
	wimp -> toolbar = toolbar ;
	
	wimp -> pane1  = null ;
	wimp -> pane2 = null ;
	
	wimp -> pane3 = null ;
	wimp -> index = 0 ;
	
	wimp -> play = null ;
	
	wimp -> playlist = playlist ;
	wimp -> store = store ;
	
	wimp -> selection = gtk_tree_view_get_selection ( playlist ) ;
	g_signal_connect_after ( wimp -> main_window, "window-state-event", wimp_gui2_track_window_state, wimp ) ;
	
	g_signal_connect_after ( wimp -> main_window, "button-press-event", wimp_gui_menu_popup, wimp ) ;
	g_signal_connect_after ( wimp -> main_window, "key-press-event", wimp_hotkeys_gui, wimp ) ;
	
	g_signal_connect ( playlist, "row-activated", wimp_gui_playlist_row_activated, wimp ) ;
	g_signal_connect ( playlist, "button-press-event", wimp_gui_playlist_popup_menu, wimp ) ;
	
	g_signal_connect ( gview, "row-activated", wimp_search_row_activated, wimp ) ;
	
	g_signal_connect ( toolbar, "key-press-event", wimp_hotkeys_gui, wimp ) ;
	g_signal_connect ( seekbar, "key-press-event", wimp_hotkeys_gui, wimp ) ;

	g_signal_connect ( wimp -> controls_window, "key-press-event", wimp_hotkeys_gui, wimp ) ;
	g_signal_connect_after ( wimp -> playlist_window, "key-press-event", wimp_hotkeys_gui, wimp ) ;
	
	wimp -> fullscreen = off ;
	wimp -> controls_visible = on ;
	
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

	gtk_drag_dest_set ( wimp -> playlist_window, GTK_DEST_DEFAULT_ALL, & target1, 0, GDK_ACTION_COPY ) ;
	gtk_drag_dest_add_text_targets ( wimp -> playlist_window ) ;

	g_signal_connect ( wimp -> playlist_window, "drag-data-received", wimp_gui_playlist_drag_drop_replace, wimp ) ;
	g_signal_connect ( playlist, "drag-data-received", wimp_gui_playlist_drag_drop, wimp ) ;

	gint width = 320, height = 240 ;
	gtk_window_get_size ( wimp -> playlist_window, & width, & height ) ;
	
	gtk_window_resize ( wimp -> playlist_window, width, 500 ) ;
	gtk_window_move ( wimp -> playlist_window, gdk_screen_width () - width - 20, 20 ) ;
	
	gtk_window_get_size ( wimp -> controls_window, & width, & height ) ;
	gtk_window_move ( wimp -> controls_window, 40, gdk_screen_height () - height - 80 ) ;
	
	gtk_window_move ( wimp -> main_window, 40, 20 ) ;
	wimp_gui_playlist_load_playlist_default ( wimp ) ;
	
	gtk_window_set_title ( wimp -> playlist_window, "Wimp Playlist" ) ;
	gtk_window_set_title ( wimp -> controls_window, "Wimp Controls" ) ;

	g_signal_connect ( wimp -> main_window, "delete-event", gtk_widget_hide, wimp ) ;
	g_signal_connect ( wimp -> playlist_window, "delete-event", gtk_widget_hide, wimp ) ;
	
	g_signal_connect ( wimp -> main_window, "delete-event", gtk_true, wimp ) ;
	g_signal_connect ( wimp -> playlist_window, "delete-event", gtk_true, wimp ) ;
	
	g_signal_connect_swapped ( wimp -> controls_window, "destroy", wimp_cleanup, wimp ) ;
	g_signal_connect_swapped ( wimp -> main_window, "size_request", wimp_gui_adjust_video_display, wimp ) ;
	
	g_signal_connect ( gview, "button-press-event", wimp_gui2_search_popup_menu, wimp ) ;
	
	wimp -> search = gview ;
	wimp -> mode = TOPLEVEL ;

	wimp -> search_show_thumbnails = no ;
	out ;
}

gboolean wimp_gui2_track_window_state ( 	GtkWidget * widget,
										GdkEventWindowState * event,
										WimpGUI * gui )
{
	in ;
	if ( ! gui_init )
		return ;
	
	if ( event -> new_window_state == GDK_WINDOW_STATE_FULLSCREEN )
	{
		gui -> fullscreen = true ;
		
		gui -> xine -> width = gdk_screen_width () ;
		gui -> xine -> height = gdk_screen_height () ;
		
		//wimp_gui2_move_controls_away ( gui ) ;
	}
	else
	{
		gui -> fullscreen = false ;
		
		gint width = 320, height = 240 ;
		gtk_window_get_size ( gui -> main_window, & width, & height ) ;
		
		wimp_update_gui () ;
		
		gui -> xine -> width = width - 230 ;
		gui -> xine -> height = height - 80 ;
		
		wimp_gui_adjust_video_display ( gui ) ;
		wimp_update_gui () ;
		
	}
		
	out ;
	return false ;
}

void wimp_gui2 ( gint argc, gchar * argv [] )
{
	in ;
	
	WimpGUI * wimp = wimp_gui_new () ;
	wimp_gui2_init ( wimp ) ;
	
	wimp -> xine = wimp_xine_new () ;
	wimp_xine_engine_init_x11 ( wimp -> xine, wimp -> video_display ) ;
	
	wimp_update_gui () ;

	if ( wimp -> xine -> vo == null )
	{
		g_warning ( "Unable to open a video driver :(\n\n"
					"Please select another video driver from the "
					"options menu and restart WiMP.\n\n"
					"You won't be able to play nothin\'." ) ;
		
		gtk_main () ;
	}

	wimp_gui_enhancements_init_from_engine ( wimp ) ;
	gtk_window_get_size ( wimp -> main_window, & wimp -> xine -> width, & wimp -> xine -> height ) ;
	
	if ( argc > 1 )
	{
		gint i = 0 ;
		wimp_gui_playlist_clear ( wimp ) ;
		
		for ( i = 1 ; i < argc ; i ++ )
			wimp_gui_playlist_append ( wimp, argv [i] ) ;
	}
	
	wimp_update_gui () ;

	wimp_file_manager_browse ( wimp, g_get_home_dir ()) ;
	wimp_update_gui () ;

	wimp_gui2_search_update ( wimp ) ;
	wimp_update_gui () ;

	printf ( "WiMP is up and running.\n" ) ;
	
	xine_event_create_listener_thread ( wimp -> xine -> event_queue, wimp_xine_event_listener, wimp ) ;
	gtk_widget_destroy ( gtk_bin_get_child ( wimp -> main_window )) ;
	
	gtk_window_present ( wimp -> controls_window ) ;
	gtk_widget_hide ( wimp -> main_window ) ;
	
	gui_init = true ;
	
	//if ( wimp_gui_playlist_length ( wimp ))
		//wimp_gui_playlist_play ( wimp, null ) ;
	
	gtk_main () ;
	out ;
}

void wimp_gui2_toggle_pause ( WimpGUI * gui )
{
	in ;
	
	if ( xine_get_status ( gui -> xine -> stream ) == XINE_STATUS_PLAY )
	{
		gint pos = 0, length = 0, pos_stream = 0, total_length = 0 ;
		xine_get_pos_length ( gui -> xine -> stream, & pos_stream, & pos, & length ) ;
		
		gui -> xine -> paused_at = pos ;
		xine_stop ( gui -> xine -> stream ) ;
	
		if ( gtk_main_level ())
			gtk_main_quit () ;
		
		gtk_main () ;
	}
	else if ( xine_get_status ( gui -> xine -> stream ) == XINE_STATUS_IDLE )
		wimp_gui_playlist_play ( gui, null ) ;
	else
	{
		wimp_xine_x11_play ( gui,  gui -> xine -> paused_at ) ;
	
	}
	
	out ;
}

void wimp_gui2_move_controls_away ( WimpGUI * gui )
{
	in ;
	gint x = 0, y = 0, i = 0 ;
	
	gtk_window_get_position ( gui -> controls_window, & x, & y ) ;
	
	for ( i = y ; i < gdk_screen_height () + 50 ; i = i + 10 )
	{
		gtk_window_move ( gui -> controls_window, x, i ) ;
		g_usleep ( 10000 ) ;
		
		wimp_xine_x11_expose ( gui ) ;
		wimp_update_gui () ;
	}

	gtk_window_get_position ( gui -> playlist_window, & x, & y ) ;
	
	for ( i = x ; i < gdk_screen_width () + 50 ; i  = i + 10 )
	{
		gtk_window_move ( gui -> playlist_window, i, y ) ;
		g_usleep ( 10000 ) ;
		
		wimp_xine_x11_expose ( gui ) ;
		wimp_update_gui () ;
	}
	
	out ;
}

void wimp_gui2_dance ( WimpGUI * gui )
{
	in ;
	
	gint i = 0 ;
	GList * list = null ;
	
	list = g_list_append ( list, gui -> main_window ) ;
	list = g_list_append ( list, gui -> playlist_window ) ;
	
	list = g_list_append ( list, gui -> controls_window ) ;
	for ( i = 0 ; i < 100 ; i ++ )
	{
		gint u = 0 ;
		for ( u = 0 ; u < 3 ; u ++ )
		{
			gint x = 0, y = 0 ;
			gtk_window_get_position ( g_list_nth_data ( list, u ), & x, & y ) ;
			
			gboolean add = g_random_boolean () ;
			if ( add )
				gtk_window_move ( g_list_nth_data ( list, u ), g_random_int_range ( x - 1, x + 5 ), g_random_int_range ( y - 1, y + 5 )) ;
			else
				gtk_window_move ( g_list_nth_data ( list, u ), g_random_int_range ( x- 5, x + 1 ), g_random_int_range ( y - 5, y + 1 )) ;
			
			wimp_update_gui () ;
		}	
		
		g_usleep ( 10000 ) ;
	}
	
	g_list_free ( list ) ;
	out ;
}

gboolean wimp_gui2_quit ( WimpGUI * gui )
{
	in ;
	
	if ( wimp_message_question ( "Are you sure you want to quit ?" ))
		if ( wimp_message_question ( "Sure ?" ))
			if ( wimp_message_question ( "Really? I mean, well ..." ))
				if ( wimp_message_question ( "QUIT ?" ))
					if ( wimp_message_question ( "REALLY this time ?" ))
						wimp_cleanup ( gui ) ;
	
	g_message ( "Good choice. I knew you wasn\'t no quitter." ) ;
	return true ;
}

void wimp_gui2_zoom_100 ( WimpGUI * gui )
{
	in ;
	
	if ( gui -> fullscreen )
		return ;
	
	if ( ! xine_get_stream_info ( gui -> xine -> stream, XINE_STREAM_INFO_HAS_VIDEO ))
		return ;
		
	gui -> xine -> width = xine_get_stream_info ( gui -> xine -> stream, XINE_STREAM_INFO_VIDEO_WIDTH ) ;
	gui -> xine -> height = xine_get_stream_info ( gui -> xine -> stream, XINE_STREAM_INFO_VIDEO_HEIGHT ) ;

	gtk_window_resize ( gui -> main_window, gui -> xine -> width, gui -> xine -> height ) ;
	out ;
}

void wimp_gui2_zoom_2x ( WimpGUI * gui )
{
	in ;
	
	if ( gui -> fullscreen )
		return ;

	if ( ! xine_get_stream_info ( gui -> xine -> stream, XINE_STREAM_INFO_HAS_VIDEO ))
		return ;

	gui -> xine -> width = xine_get_stream_info ( gui -> xine -> stream, XINE_STREAM_INFO_VIDEO_WIDTH ) * 2 ;
	gui -> xine -> height = xine_get_stream_info ( gui -> xine -> stream, XINE_STREAM_INFO_VIDEO_HEIGHT ) * 2 ;

	gtk_window_resize ( gui -> main_window, gui -> xine -> width, gui -> xine -> height ) ;
	out ;
}

void wimp_gui2_zoom_x_by_2 ( WimpGUI * gui )
{
	in ;
	
	if ( gui -> fullscreen )
		return ;

	if ( ! xine_get_stream_info ( gui -> xine -> stream, XINE_STREAM_INFO_HAS_VIDEO ))
		return ;

	gui -> xine -> width = xine_get_stream_info ( gui -> xine -> stream, XINE_STREAM_INFO_VIDEO_WIDTH ) / 2 ;
	gui -> xine -> height = xine_get_stream_info ( gui -> xine -> stream, XINE_STREAM_INFO_VIDEO_HEIGHT ) / 2 ;

	gtk_window_resize ( gui -> main_window, gui -> xine -> width, gui -> xine -> height ) ;
	out ;
}

void wimp_gui2_zoom_3x ( WimpGUI * gui )
{
	in ;
	
	if ( gui -> fullscreen )
		return ;

	if ( ! xine_get_stream_info ( gui -> xine -> stream, XINE_STREAM_INFO_HAS_VIDEO ))
		return ;

	gui -> xine -> width = xine_get_stream_info ( gui -> xine -> stream, XINE_STREAM_INFO_VIDEO_WIDTH ) * 3 ;
	gui -> xine -> height = xine_get_stream_info ( gui -> xine -> stream, XINE_STREAM_INFO_VIDEO_HEIGHT ) * 3 ;

	gtk_window_resize ( gui -> main_window, gui -> xine -> width, gui -> xine -> height ) ;
	out ;
}

private void activate_my_dialog ( GtkDialog * dialog )
{
	gtk_dialog_response ( dialog, 1 ) ;
}

void wimp_gui2_jump_to ( WimpGUI * gui )
{
	in ;
	
	GtkDialog * dialog = gtk_dialog_new_with_buttons ( "Jump to",
														null,
														GTK_DIALOG_MODAL,
														null ) ;
	
	GtkButton * ok = gtk_dialog_add_button ( dialog, "Jump", 1 ) ;
	gtk_dialog_add_button ( dialog, "Cancel", 0 ) ;
	
	GtkEntry * entry = gtk_entry_new () ;
	GtkLabel * label = gtk_label_new ( "Enter time in mm:ss format" ) ;
	
	g_signal_connect_swapped ( entry, "activate", activate_my_dialog, dialog ) ;
		
	
	gtk_box_pack_start ( dialog -> vbox, entry, no, no, 2 ) ;
	gtk_box_pack_start ( dialog -> vbox, label, no, no, 2 ) ;
	
	gtk_widget_show_all ( dialog ) ;
	if ( ! gtk_dialog_run ( dialog ))
	{
		gtk_widget_destroy ( dialog ) ;
		return ;
	}
	
	gchar * text = gtk_entry_get_text ( entry ) ;
	if ( text == null || strlen ( text ) == 0 )
	{
		gtk_widget_destroy ( dialog ) ;
		return ;
	}
	
	gchar ** vector = g_strsplit ( text, ":", -1 ) ;
	gint time = 0 ;
	
	if ( g_strv_length ( vector ) > 1 )
	{
		time = ( atoi ( vector [0] ) * 60 ) + atoi ( vector [1] ) ;
	}
	else
		time = atoi ( vector [0] ) ;
	
	g_strfreev ( vector ) ;
	gtk_widget_destroy ( dialog ) ;
	
	wimp_xine_x11_play ( gui, time * 1000 ) ;
	out ;
}

void wimp_gui2_search_update ( WimpGUI * gui )
{
	in ;
	
	GtkTreeStore * model = gtk_tree_view_get_model ( gui -> search ) ;
	gtk_tree_store_clear ( model ) ;
	
	gchar * index = g_build_filename ( g_get_home_dir (), ".wimp", "index.html", null ) ;
	wimp_search_parse ( index, model ) ;
	
	//GtkTreeStore * store = search_purge ( model ) ;
	//gtk_tree_view_set_model ( gui -> search, store ) ;
	
	//g_object_unref ( model ) ;

	if ( gui -> search_show_thumbnails )
		wimp_search_load_images ( gui ) ;
	
	out ;
}


really private gchar * get_toplevel_domain ( gchar * url )
{
	in ;
	
	if ( url == null )
		return null ;
	
	GString * s = g_string_new ( null ) ;
	gint i = 0, len = strlen ( url ), count = 0 ;
	
	for ( i = 0 ; i < len ; i ++ )
	{
		s = g_string_append_c ( s, url [i] ) ;
		
		if ( url [i] == '/' )
			count ++ ;
		
		if ( count == 3 )
			break ;
	}
	
	out ;
	return g_string_free ( s, no ) ;

}
void wimp_search_row_activated ( 	GtkTreeView * tree_view,
									GtkTreePath * tree_path,
									GtkTreeViewColumn * column,
									WimpGUI * gui )
{
	in ;
	
	gchar * url ;
	GtkTreeIter iter ;
	
	InternalMimeType type = URL ;
	
	gtk_tree_model_get_iter ( gtk_tree_view_get_model ( tree_view ), & iter, tree_path ) ;
	gtk_tree_model_get ( gtk_tree_view_get_model ( tree_view ), & iter, 1, & url, 4, & type, -1 ) ;
	
	if ( search_file_is_suitable_for_playback ( url ) or wimp_file_is_stream ( url ) or type == STREAM )
	{
		wimp_gui2_handle_search_video ( gui, url ) ;
		
		g_free ( url ) ;
		return ;
	}
	
	if ( url [0] == '/' )
	{
		gchar * url1 = get_toplevel_domain ( gtk_entry_get_text ( gui -> search_entry )) ;
		gchar * new_url = g_strdup_printf ( "%s%s", url1, url ) ;
		
		g_free ( url1 ) ;
		
		g_free ( url ) ;
		url = g_strdup ( new_url ) ;
		
		g_free ( new_url ) ;
	}
	
	gtk_entry_set_text ( gui -> search_entry, url ) ;
	g_free ( url ) ;
	
	wimp_search_search ( gui ) ;
	return ;
	
	
	gchar * cmd = g_strdup_printf ( "wget;%s;-O;%s/.wimp/index.html", url, g_get_home_dir ()) ;
	
	gchar ** command = g_strsplit ( cmd, ";", -1 ) ;
	g_free ( url ) ;
	
	gint pid = 0, stdin = 0, stdout = 0, stderr = 0 ;
	GError * error = null ;
	
	gboolean result = false ;
	
	result = g_spawn_async_with_pipes ( null, command, null, G_SPAWN_SEARCH_PATH,
										null, null, & pid, null, null,
										null, & error ) ;
	
	if ( error )
		g_warning ( "Cannot spawn process: %s", error -> message ) ;
	
	gchar * proc = g_strdup_printf ( "/proc/%d", pid ) ;
	gint stamp = timestamp ;
	
	while ( g_file_test ( proc, G_FILE_TEST_EXISTS ))
	{
		gtk_progress_bar_pulse ( gui -> search_bar ) ;
		
		wimp_update_gui () ;
		g_usleep ( 100000 ) ;
		
		if ( stamp != timestamp )
		{
			kill ( pid, SIGKILL ) ;
			break ;
		}
	}
	
	g_strfreev ( command ) ;
	g_free ( cmd ) ;

	g_free ( proc ) ;
	
	gtk_widget_set_sensitive ( gui -> search_stop, no ) ;
	gtk_progress_bar_set_fraction ( gui -> search_bar, 0.0 ) ;
	
	g_free ( url ) ;

	GtkTreeStore * model = gtk_tree_view_get_model ( tree_view ) ;
	gtk_tree_store_clear ( model ) ;
	
	gchar * index = g_build_filename ( g_get_home_dir (), ".wimp", "index.html", null ) ;
	wimp_search_parse ( index, model ) ;
	
	if ( gui -> search_show_thumbnails )
		wimp_search_load_images ( gui ) ;
	
	out ;
}

private void gui2_fetch_image ( WimpGUI * gui, gchar * image )
{
	in ;
	
	gchar * cmd = g_strdup_printf ( "wget;%s;-O;%s/.wimp/image", image, g_get_home_dir ()) ;
	gchar ** command = g_strsplit ( cmd, ";", -1 ) ;
	
	gint pid = 0, stdin = 0, stdout = 0, stderr = 0 ;
	GError * error = null ;
	
	gboolean result = false ;
	
	result = g_spawn_async_with_pipes ( null, command, null, G_SPAWN_SEARCH_PATH,
										null, null, & pid, null, null,
										null, & error ) ;
	
	if ( error )
		g_warning ( "Cannot spawn process: %s", error -> message ) ;
	
	gchar * proc = g_strdup_printf ( "/proc/%d", pid ) ;
	gint stamp = timestamp ;
	
	while ( g_file_test ( proc, G_FILE_TEST_EXISTS ))
	{
		gtk_progress_bar_pulse ( gui -> search_bar ) ;
		
		wimp_update_gui () ;
		g_usleep ( 100000 ) ;
		
		if ( ! stamp == timestamp )
		{
			//system ( "killall wget" ) ;
			kill ( pid, SIGKILL ) ;
			break ;
		}
	}
	
	g_free ( cmd ) ;
	g_strfreev ( command ) ;
	
	g_free ( proc ) ;
	out ;
}

void wimp_search_load_images ( WimpGUI * gui )
{
	in ;
	
	gtk_widget_set_sensitive ( gui -> search_stop, on ) ;
	//gtk_widget_set_sensitive ( gui -> search, no ) ;
	
	GtkTreeView * search = gui -> search ;
	GtkTreeStore * store = gtk_tree_view_get_model ( search ) ;
	
	gint i = 0, len = gtk_tree_model_iter_n_children ( store, null ) ;
	gint stamp = timestamp ;
	
	for ( i = 0 ; i < len ; i ++ )
	{
		GtkTreeIter iter ;
		if ( stamp != timestamp )
			break ;
		
		if ( ! gtk_tree_model_iter_nth_child ( store, & iter, null, i ))
			continue ;
		
		gchar * img = null ;
		gtk_tree_model_get ( store, & iter, 2, & img, -1 ) ;
		
		gui2_fetch_image ( gui, img ) ;
		gchar * filename = g_build_filename ( g_get_home_dir (), ".wimp", "image", null ) ;
		
		GdkPixbuf * pixbuf = wimp_pixbuf_new_from_file_at_scale ( filename, gui -> xine -> config -> thumbnail_size, gui -> xine -> config -> thumbnail_size, null ) ;
		gtk_tree_store_set ( store, & iter, 3, pixbuf, -1 ) ;
		
		g_free ( img ) ;
		g_free ( filename ) ;
		
		if ( pixbuf )
			g_object_unref ( pixbuf ) ;
	}
	
	gtk_widget_set_sensitive ( gui -> search_stop, no ) ;
	//gtk_widget_set_sensitive ( gui -> search, on ) ;
	
	gtk_progress_bar_set_fraction ( gui -> search_bar, 0.0 ) ;
	
	out ;
}

private private void fire_event ( void )
{
	timestamp = gtk_get_current_event_time () ;
}

void wimp_search_search ( WimpGUI * gui )
{
	in ;
	
	gtk_widget_set_sensitive ( gui -> search_stop, on ) ;
	//gtk_widget_set_sensitive ( gui -> search, no ) ;
	
	gchar * text = gtk_entry_get_text ( gui -> search_entry ) ;
	if ( text == null )
		return ;
	
	/* The following search string is to be  parsed 
	* http://www.google.com/search?q=u2+vertigo&hl=en&safe=off&sa=G&tbo=u&prmd=vi&source=univ&tbs=vid:1&ei=2-__S8WEDMG1rAe--tDHDg&oi=video_result_group&ct=title&resnum=1&ved=0CEcQqwQwAA
	*/
	
	gchar safe [10] ;
	if ( gui -> xine -> config -> safe_search )
		strcpy ( & safe, "on" ) ;
	else
		strcpy ( & safe, "off" ) ;
	
	gchar * my_text = g_strdup ( text ) ;
	g_strdelimit ( my_text, " ", '+' ) ;
	
	gchar * cmd = null ;
	
	if ( g_strrstr ( text, "." ))
		cmd = g_strdup_printf ( "wget;%s;-O;%s/.wimp/index.html", text, g_get_home_dir ()) ;
	else
		//cmd = g_strdup_printf ( "wget;http://www.google.com/search?q=%s&hl=en&safe=%s&sa=G&tbo=u&prmd=vi&source=univ&tbs=vid:1&ei=2-__S8WEDMG1rAe--tDHDg&oi=video_result_group&ct=title&resnum=1&ved=0CEcQqwQwAA;-O;%s/.wimp/index.html",
		cmd = g_strdup_printf ( "wget;http://www.google.com/search?q=%s&hl=en&safe=%s;-O;%s/.wimp/index.html",
									my_text,
									safe,
									g_get_home_dir ()) ;
	
	gchar ** command = g_strsplit ( cmd, ";", -1 ) ;
	g_free ( my_text ) ;
	
	gint pid = 0, stdin = 0, stdout = 0, stderr = 0 ;
	GError * error = null ;
	
	gboolean result = false ;
	
	result = g_spawn_async_with_pipes ( null, command, null, G_SPAWN_SEARCH_PATH,
										null, null, & pid, null, null,
										null, & error ) ;
	
	if ( error )
		g_warning ( "Cannot spawn process: %s", error -> message ) ;
	
	gchar * proc = g_strdup_printf ( "/proc/%d", pid ) ;
	gint stamp = timestamp ;
	
	while ( g_file_test ( proc, G_FILE_TEST_EXISTS ))
	{
		gtk_progress_bar_pulse ( gui -> search_bar ) ;
		
		wimp_update_gui () ;
		g_usleep ( 100000 ) ;
		
		if ( stamp != timestamp )
		{
			//system ( "killall wget" ) ;
			kill ( pid, SIGKILL ) ;
			break ;
		}
	}
	
	g_free ( cmd ) ;
	g_strfreev ( command ) ;

	g_free ( proc ) ;

	/*
	gchar * index = g_build_filename ( g_get_home_dir (), ".wimp", "index.html", null ) ;

	gchar * contents = null ;
	error = null ;
	
	gsize size = 0 ;
	
	if ( ! g_file_get_contents ( index, & contents, & size, & error ))
	{
		g_warning ( "Cannot open webpage: %s", error -> message ) ;
		return ;
	}
	
	html_parse_tags ( contents, zero, gtk_tree_view_get_model ( gui -> search )) ;
	g_free ( contents ) ;
	
	g_free ( index ) ;
	search_purge ( gtk_tree_view_get_model ( gui -> search )) ;
	
	
	*/
	
	wimp_gui2_search_update ( gui ) ;
	
	gtk_widget_set_sensitive ( gui -> search_stop, no ) ;
	//gtk_widget_set_sensitive ( gui -> search, on ) ;
	
	gtk_progress_bar_set_fraction ( gui -> search_bar, 0.0 ) ;
	
	out ;
}

void wimp_gui2_handle_search_video ( WimpGUI * gui, gchar * url )
{
	in ;
	
	GtkDialog * dialog = gtk_dialog_new_with_buttons ( "Video source",
														null,
														GTK_DIALOG_MODAL,
														"Play",
														1,
														"Enqueue",
														2,
														"Copy url",
														3,
														//"Download",
														//4,
														"Cancel",
														0,
														null ) ;
	
	GtkWidget * xo = gtk_hbox_new ( no, 2 ) ;
	
	GtkLabel * label = gtk_label_new ( null ) ;
	gchar * markup = g_markup_printf_escaped (
		"The following is a <b>video</b> url:\n\n"
		"%s\n\n"
		"What do <b>you </b> want to do?\n"
		"<span size=\"small\">Hint: Copy the url to the "
		"clipboard and use a tool like <b>wget</b> to "
		"download the file. Alternatively, you can <b>"
		"watch</b> the video in the player.</span>", url ) ;
	
	gtk_label_set_markup ( label, markup ) ;
	g_free ( markup ) ;
	
	gtk_label_set_line_wrap ( label, true ) ;
	gtk_label_set_width_chars ( label, 50 ) ;
	
	gtk_label_set_selectable ( label, yes ) ;
	
	gtk_box_pack_start ( dialog -> vbox, xo, no, no, 5 ) ;
	gtk_box_pack_start ( xo, gtk_image_new_from_stock ( GTK_STOCK_DIALOG_INFO, GTK_ICON_SIZE_DIALOG ), no, no, 5 ) ;
	
	gtk_box_pack_start ( xo, label, no, no, 2 ) ;
	gtk_widget_show_all ( dialog ) ;
	
	switch ( gtk_dialog_run ( dialog ))
	try {
		case 0:
		default:
			gtk_widget_destroy ( dialog ) ;
			return ;
		case 1:
			wimp_gui_playlist_clear ( gui ) ;
		case 2:
			wimp_gui_playlist_append_url ( gui, url ) ;
			break ;
		case 3:
			wimp_clipboard_set ( url ) ;
			break ;
	}
	
	gtk_widget_destroy ( dialog ) ;
	out ;
}

void wimp_gui2_search_shew_source ( void )
{
	in ;
	
	gchar * contents = null ;
	GError * error = null ;
	
	gchar * filename = g_build_filename ( g_get_home_dir (), ".wimp", "index.html", null ) ;
	gsize size = 0 ;
	
	if ( ! g_file_get_contents ( filename, & contents, & size, & error ))
	{
		g_warning ( "Cannot open webpage: %s", error -> message ) ;
		return ;
	}
	
	wimp_message_box_detailed_info ( contents ) ;
	g_free ( contents ) ;
	
	g_free ( filename ) ;
	out ;
}
	
void wimp_search_reload ( WimpGUI * gui )
{
	in ;
	wimp_search_search ( gui ) ;
	
	out ;
}
	
GtkMenu * wimp_search_create_menu ( WimpGUI * gui )
{
	in ;
	
	GtkMenu * menu = gtk_menu_new () ;
	GtkMenuItem * reload = gtk_menu_item_new_with_label ( "Reload" ),
				* src = gtk_menu_item_new_with_label ( "View source" ),
				* add = gtk_menu_item_new_with_label ( "Add to favorites" ),
				* bro = gtk_menu_item_new_with_label ( "Browse favorites" ),
				* open = gtk_menu_item_new_with_label ( "Open" ) ;
	
	gtk_menu_shell_append ( menu, open ) ;
	gtk_menu_shell_append ( menu, gtk_separator_menu_item_new ()) ;
	
	gtk_menu_shell_append ( menu, add ) ;
	gtk_menu_shell_append ( menu, bro ) ;
	
	gchar * t = gtk_entry_get_text ( gui -> search_entry ) ;
	if ( t )
	{
		if ( t[0] == '/' )
		{
			GtkMenuItem * l = gtk_menu_item_new_with_label ( "Delete favorite" ) ;
			g_signal_connect_swapped ( l, "activate", wimp_gui2_search_delete_favorite, gui ) ;
			
			gtk_menu_shell_append ( menu, gtk_separator_menu_item_new ()) ;
			gtk_menu_shell_append ( menu, l ) ;
			
			gtk_widget_set_sensitive ( add, no ) ;
			gtk_widget_set_sensitive ( bro, no ) ;
		}
	}
	
	gtk_menu_shell_append ( menu, gtk_separator_menu_item_new ()) ;
	
	gtk_menu_shell_append ( menu, src ) ;
	gtk_menu_shell_append ( menu, reload ) ;
	
	g_signal_connect_swapped ( open, "activate", wimp_gui2_search_open, gui ) ;
	g_signal_connect_swapped ( add, "activate", wimp_gui2_search_add_to_favorites, gui ) ;
	
	g_signal_connect_swapped ( bro, "activate", wimp_gui2_search_browse_favorites, gui ) ;
	
	g_signal_connect ( src, "activate", wimp_gui2_search_shew_source, null ) ;
	g_signal_connect_swapped ( reload, "activate", wimp_search_reload, gui ) ;
	
	gtk_widget_show_all ( menu ) ;

	out ;
	return menu ;
}

bool wimp_gui2_search_popup_menu ( GtkWidget * widget, GdkEventButton * event, WimpGUI * gui )
{
	in ;

	if ( event -> button != 3 )
		return false ;
	
	GtkMenu * menu = wimp_search_create_menu ( gui ) ;
	g_signal_connect_swapped ( menu, "selection-done", gtk_widget_destroy, menu ) ;
	
	gtk_menu_popup ( menu, null, null, null, null, null, gtk_get_current_event_time ()) ;
	out ;
	
	return true ;
}

void wimp_gui2_search_open ( WimpGUI * gui )
{
	in ;

	GtkTreeIter iter ;
	GtkTreeStore * store = null ;
	
	if ( ! gtk_tree_selection_get_selected ( gtk_tree_view_get_selection ( gui -> search ), & store, & iter ))
		return null ;
	
	gchar * filename = null ;
	gtk_tree_model_get ( store, & iter, 1, & filename, -1 ) ;
	
	gtk_entry_set_text ( gui -> search_entry, filename ) ;
	g_free ( filename ) ;
	
	wimp_search_search ( gui ) ;
	out ;
}

void wimp_gui2_search_add_to_favorites ( WimpGUI * gui )
{
	in ;

	GtkTreeIter iter ;
	GtkTreeStore * store = null ;
	
	if ( ! gtk_tree_selection_get_selected ( gtk_tree_view_get_selection ( gui -> search ), & store, & iter ))
		return null ;
	
	gchar * filename = null ;
	gchar * img = null ;
	
	gtk_tree_model_get ( store, & iter, 1, & filename, 2, & img, -1 ) ;
	
	if ( filename == null )
	{
		g_free ( img ) ;
		return ;
	}
	
	gchar * f = g_build_filename ( g_get_home_dir (), ".wimp", "favorites", basename ( filename ), null ) ;
	GKeyFile * ff = g_key_file_new () ;
	
	if ( filename [0] != '/' )
		g_key_file_set_string ( ff, "favorite", "url", filename ) ;
	else
	{
		gchar * l = g_strdup_printf ( "%s%s", gtk_entry_get_text ( gui -> search_entry ), filename ) ;
		g_key_file_set_string ( ff, "favorite", "url", l ) ;
		
		g_free ( l ) ;
	}
	
	g_key_file_set_string ( ff, "favorite", "img", img ) ;
	
	gchar * data = g_key_file_to_data ( ff, null, null ) ;
	
	File * fp = fopen ( f, "w" ) ;
	fprintf ( fp, "%s", data ) ;
	
	fclose ( fp ) ;
	
	g_free ( data ) ;
	g_free ( f ) ;
	
	g_key_file_free ( ff ) ;
	g_free ( filename ) ;
	
	g_free ( img ) ;
	out ;
}

void wimp_gui2_search_browse_favorites ( WimpGUI * gui )
{
	in ;
	
	gchar * fav_dir = g_build_filename ( g_get_home_dir (), ".wimp", "favorites", null ) ;
	GError * error = null ;
	
	GDir * dir = g_dir_open ( fav_dir, 0, & error ) ;
	
	if ( dir == null )
	{
		g_warning ( "Cannot open folder: %s", error -> message ) ;
		g_free ( fav_dir ) ;
		
		return ;
	}
	
	gchar * name = g_dir_read_name ( dir ) ;
	gtk_tree_store_clear ( gtk_tree_view_get_model ( gui -> search )) ;
	
	GtkTreeStore * store = gtk_tree_view_get_model ( gui -> search ) ;
	gtk_entry_set_text ( gui -> search_entry, fav_dir ) ;
	
	while ( name )
	{
		gchar * filename = g_build_filename ( fav_dir, name, null ) ;
		GKeyFile * file = g_key_file_new () ;
		
		if ( g_key_file_load_from_file ( file, filename, G_KEY_FILE_NONE, & error ))
		{
			gchar * filename = g_key_file_get_string ( file, "favorite", "url", null ) ;
			gchar * img = g_key_file_get_string ( file, "favorite", "img", null ) ;
			
			if ( filename )
			{
				GtkTreeIter iter ;
				gtk_tree_store_append ( store, & iter, null ) ;
				
				gtk_tree_store_set ( store, & iter, 0, basename ( filename ), 1, filename, 2, img, -1 ) ;
			}
			
			g_free ( filename ) ;
			g_free ( img ) ;
		}
		
		g_key_file_free ( file ) ;
		g_free ( filename ) ;
		
		name = g_dir_read_name ( dir ) ;
	}
	
	g_dir_close ( dir ) ;
	g_free ( fav_dir ) ;

	out ;
}

void wimp_gui2_search_delete_favorite ( WimpGUI * gui )
{
	in ;
	
	GtkTreeIter iter ;
	GtkTreeStore * store = all_the_pain_that_i_went_through_amounts_to ;
	if ( ! gtk_tree_selection_get_selected ( gtk_tree_view_get_selection ( gui -> search ), & store, & iter ))
		return null ;
	
	gchar * fav_dir = g_build_filename ( g_get_home_dir (), ".wimp", "favorites", null ) ;
	GError * error = null ;
	
	GDir * dir = g_dir_open ( fav_dir, 0, & error ) ;
	
	if ( dir == null )
	{
		g_warning ( "Cannot open folder: %s", error -> message ) ;
		g_free ( fav_dir ) ;
		
		return ;
	}
	
	gchar * filename = null ;
	gtk_tree_model_get ( store, & iter, 1, & filename, -1 ) ;
	
	if ( filename == null )
		return ;
	
	gchar * name = g_dir_read_name ( dir ) ;
	while ( name )
	{
		gchar * f = g_build_filename ( fav_dir, name, null ) ;
		GKeyFile * file = g_key_file_new () ;
		
		if ( g_key_file_load_from_file ( file, f, G_KEY_FILE_NONE, & error ))
		{
			gchar * f2 = g_key_file_get_string ( file, "favorite", "url", null ) ;
			if ( strcmp ( f2, filename ) == zero )
			{
				gchar * m = g_strdup_printf ( "Remove this favorite? %s", basename ( f )) ;
				if ( wimp_message_question ( m ))
				{
					if ( g_remove ( f ))
						g_warning ( "Cannot remove %s !", f ) ;
						
					gtk_tree_store_remove ( store, & iter ) ;
				}
				
				g_free ( m ) ;
			}
			
			g_free ( f2 ) ;
		}
		
		g_key_file_free ( file ) ;
		g_free ( f ) ;
		
		name = g_dir_read_name ( dir ) ;
	}
	
	g_dir_close ( dir ) ;
	g_free ( filename ) ;
	
	g_free ( fav_dir ) ;
	out ;
}		

