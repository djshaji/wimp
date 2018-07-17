/*
 *      encode.h
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


#ifndef __ENCODE_H
#define __ENCODE_H

#include <gtk/gtk.h>
#include "definitions.c"
#include "gui.h"

static gchar * video_codecs [] = {
	"asv1",
	"asv2",
	"bmp",
	"dnxhd",
	"dvvideo",
	"ffv1",
	"ffvhuff",
	"flashsv",
	"flv",
	"gif",
	"h261",
	"h263",
	"h263p",
	"h264",
	"huffyuv",
	"jpegls",
	"mjpeg",
	"mpeg1video",
	"mpeg2video",
	"mpeg4",
	"msmpeg4",
	"msmpeg4v1",
	"msmpeg4v2",
	"pam",
	"pbm",
	"pcx",
	"pgm",
	"pgmyuv",
	"png",
	"ppm",
	"qtrle",
	"rawvideo",
	"roqvideo",
	"rv10",
	"rv20",
	"sgi",
	"snow",
	"svq1",
	"targa",
	"tiff",
	"v210",
	"wmv1",
	"wmv2",
	"zlib",
	"zmbv",
	null
} ;


static gchar * audio_codecs [] = {
	"aac",
	"ac3",
	"adpcm_adx",
	"adpcm_ima_qt",
	"adpcm_ima_wav",
	"adpcm_ms",
	"adpcm_swf",
	"adpcm_yamaha",
	"alac",
	"flac",
	"g726",
	"libfaac",
	"libmp3lame",
	"libopencore_amrnb",
	"mp2",
	"nellymoser",
	"pcm_alaw",
	"pcm_f32be",
	"pcm_f32le",
    "pcm_f64be",
    "pcm_f64le",
    "pcm_mulaw",
    "pcm_s16be",
    "pcm_s16le",
	"pcm_s24be",
	"pcm_s24daud",
	"pcm_s24le",
	"pcm_s32be",
	"pcm_s32le",
	"pcm_s8",
	"pcm_u16be",
	"pcm_u16le",
	"pcm_u24be",
	"pcm_u24le",
	"pcm_u32be",
	"pcm_u32le",
	"pcm_u8",
	"pcm_zork",
	"roq_dpcm",
	"sonic",
	"sonicls",
	"vorbis",
	"wmav1",
	"wmav2",
	null
} ;



static gchar * containers [] = {
	"3g2",
	"3gp",
	"RoQ",
	"ac3",
	"adts",
	"aiff",
	"alaw",
	"amr",
	"au",
	"avi",
	"avm2",
	"dts",
	"dv",
	"dvd",
	"eac3",
	"flac",
	"flv",
	"gif",
	"m4v",
	"matroska",
	"mmf",
	"mov",
	"mp2",
	"mp3",
	"mp4",
	"mpeg",
	"mpeg1video",
	"mpeg2video",
	"mpegts",
	"mulaw",
	"ogg",
	"rawvideo",
	"rm",
	"swf",
	"wav",
	null
} ;


static gchar * sz [] = {
	"sqcif",
	"qcif",
	"cif",
	"4cif",
	"16cif",
	"qqvga",
	"qvga",
	"vga",
	"svga",
	"xga",
	"uxga",
	"qxga",
	"sxga",
	"qsxga",
	"hsxga",
	"wvga",
	"wxga",
	"wsxga",
	"wuxga",
	"woxga",
	"wqsxga",
	"wquxga",
	"whsxga",
	"whuxga",
	"cga",
	"ega",
	"hd480",
	"hd720",
	"hd1080",
	null
} ;

static gint presets [][8] = {
/*	  vc ac  co  s b ab r ar */
	{ 19, 0, 24, 3, 400, 128, 30, 44100 }, // MPEG4
	{ 11, 11, 1, 1, 200, 64, 15, 44100 }, // 3gp
	{ 19, 11, 24, 7, 1200, 128, 30, 44100 }, // Ipod
	{ 19, 11, 24, 7, 1200, 128, 30, 44100 } // PSP
} ;

static gchar * preset_names [] = {
	"Generic mpeg4",
	"Video for Mobile (3gp)",
	"Video for the IPod",
	"Video for the PSP",
	null
} ;

typedef struct _Encoder {
	GtkWidget * vcodec, * acodec, * b, * ab, * r,
				* ar, * container, * s ;
} Encoder ;

void wimp_encode_file_gui ( gchar * command, GtkStatusbar * status, GtkProgress * bar ) ;
void wimp_encode_selected ( WimpGUI * gui ) ;
void wimp_encode_set_preset ( GtkWidget * widget, Encoder * encoder ) ;
void wimp_encode_stop ( void ) ;
void wimp_encode_all ( WimpGUI * gui ) ;
#endif
