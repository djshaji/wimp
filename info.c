/*
 *      info.c
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

static char * faq = {
	"[ Wimp FAQ ]\n\n"
	
	"Q. [new] What is the Podcast tab ?\n"
	"A. The podcast module is a \"selective\" browser for sites "
	"that have \"directly accessible\" media content. This means, that "
	"if a site provides a link to, say, mp3 files, you can browse "
	"and play media directly from the player. Of course, we will "
	"support more sites later. Patches welcome :)\n\n"
	
	"Q. [new] How do I play internet media ?\n"
	"A. Either\n"
	"\t1. On the Podcasts tab, enter a url in the address bar. You will "
	"see an icon next to a playable stream found.\n"
	"\t2. On the playlist tab, right click the playlist and click on "
	"add url, and add a url :)\n\n"
	"Q. Why Wimp ?\n"
	"A. Why not.\n\n"

	"Q. When I use the equalizer, volume goes way low.\n"
	"A. The Xine Amp kicks in to prevent clipping (?). Use the "
	   "\"Amp level\" on the \"Audio\" tab to boost volume.\n\n"

	"Q. Where are the equalizer presets ?\n"
	"A. Equalizer presets are only available in the full version "
	   "of wimp, which costs ten trillion dollars.\n\n"
	  
	"Q. Which video / audio output driver should I use ?\n"
	"A. Use xv + alsa for optimum performance, or select \"auto\" "
	   "to let xine decide what's best.\n\n"

	"Q. Who's this lady you keep referring to?\n"
	"A. I'm not gonna tell you.\n\n"

	"Q. How many years have it been ?\n"
	"A. Six, maybe seven; I've really no memory left.\n\n"
	
	"Q. What's the system requirement?\n"
	"A. Anything that can run X. 32 MB of RAM, 350 MHz processor, "
	   "couple of MB's of free space ...\n\n"

	"Q. How come so low?\n"
	"A. That's what I have. \n\n"

	"Q. But my foo player requires 10GB of ram. Surely it must have "
	   "better features?\n"
	"A. Surely.\n\n"

	"Q. If you've used Gtk+-2.0 and libxine, both high level libraries, "
	   "brilliantly object oriented in native C, how much did you contribute "
	   "to this project on your own ?\n"
	"A. Umm ....\n\n"

	"Q. My god the player runs stable and fast. Can I send you a picture "
	   "of myself as a thank you gift? I'm a russian super model.\n"
	"A. Finally!\n\n"

	"Q. Your player is stupid.\n"
	"A. Hm.\n\n"

	"Q. Did she really not call you on April the 18th ?\n"
	"A. Swear to god she did not. And, I waited for 3 years, right next "
	   "to the phone.\n\n"

	"Q. What about LADSPA and some really cool audio effects that\'ll make "
	   "my average speakers sound nicer.\n"
	"A. Unfortunately, xine has no LADSPA support as yet. You have the "
	   "compressor for now. Either I'll wait for somebody to put in ladspa "
	   "in xine, or try to implement it myself.\n\n"

	"Q. What does \"TV Mode\" do?\n"
	"A. I wished I knew.\n\n"

	"Q. I want more features.\n"
	"A. You'll have them soon.\n\n"

	"Q. Now that I've used it, the player seems really usable.\n"
	"A. Hm.\n\n"

	"Q. It don't play ( properly ) some codec.\n"
	"A. That's a libxine thing. Update your xine libraries.\n\n"

	"Q. Can wimp open my media player's playlists?\n"
	"A. You can try.\n\n"

	"Q. What's the \"emergency exit\" strategy?\n"
	"A. It aborts the player immediately, irrespective of whatever it\'s "
	   "doing. So, if you have a slow system, and you're watching some HD "
	   "video size 720x480 and, well, hit escape and it goes away.\n"
	   "No gaurantees ( try the realtime Linux patch ), but it should make "
	   "the player exit immediately.\n\n"
	   
} ;

static char * readme = {
	
	"Introduction\n"
	"---------------\n"
	"WiMP is a robust and easy to use media player for Linux and "
	"perhaps other POSIX like systems. It aims for stability and "
	"low system resources as requirements.\n\n"

	"System Requirements\n"
	"---------------------\n"
	"It should run on anything that can run X. 32 MB RAM, 400 MHz "
	"processor, some free space. I developed it on 256 MB RAM, "
	"1.5 GHz Pentium 4, Fedora Core 4.\n\n"

	"Software Requirements\n"
	"------------------------\n"
	"Gtk+-2.0\n"
	"Libxine\n\n"

	"It should work on old ( ~ 2004 ) distros. I developed it on "
	"Fedora Core 4.\n\n"

	"Compiling and Running\n"
	"------------------------\n"
	"To compile, you'll need devel packages of everything from "
	"Xorg to Gtk+-2.0 and libxine\n\n"

	"to compile, do\n"
	"$ make\n"

	"to run, do\n"
	"$ ./wimp\n\n"

	"There is no need to \"install\" it anywhere. Just move the "
	"wimp binary to ~/bin and you should be good to go.\n\n"

	"Bugs\n"
	"-----\n"
	"Of course. And, some of them are complimentary.\n"
	"Mail 'em to me. I'll take a look.\n\n"

	"About the author\n"
	"------------------\n"
	"I'm Shaji, and I'm a post graduate english literature  "
	"student.\n"
		
} ;

static char * keyboard_shortcuts = {
	"Media Player ( when the playlist and browser are NOT selected ):\n"
	"Left arrow: Rewind\n"
	"Right arrow: Fast forward\n"
	"Up arrow: Really fast forward\n"
	"Down arrow: Really Rewind\n\n"
	
	"(Special) J: jump to time\n\n"
	
	"Zoom:\n"
	"1: Original size\n"
	"2: Double size\n"
	"3: Triple size\n"
	"`: Half size\n\n"
	
	"F: Fullscreen\n"
	"Page up / down: More forward / rewind\n\n"
	"Browser:\n"
	"Keypad 0: Up folder\n"
	"Keypad 5: Load files\n"
	"Insert : Append to playlist\n\n"
	"Playlist:\n"
	"Delete: Remove from playlist\n\n"
	"Other:\n"
	"Q: Quit\n"
	"Esc: abort ()\n\n"
	"Apart from these, just start typing in the playlist and "
	"browser tabs to begin in-line search."
	
} ;
