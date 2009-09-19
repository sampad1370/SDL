/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2009 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/
#include "SDL_config.h"

/*
	Milan Xbios video functions

	Patrice Mandin
*/

#include <mint/cookie.h>

#include "SDL_xbios.h"
#include "SDL_xbios_milan.h"

#ifndef Validmode
#define Validmode(mode)	\
	(short)trap_14_ww((short)0x5f,(short)(mode))
#endif

#define NUM_PREDEFINED_MODES 7

typedef struct {
	Uint16 width, height;
} predefined_mode_t;

static const predefined_mode_t mode_list[NUM_PREDEFINED_MODES]={
	{640,400},
	{640,480},
	{800,608},
	{1024,768},
	{1152,864},
	{1280,1024},
	{1600,1200}	
};

static const Uint8 mode_bpp[4]={
	8, 15, 16, 32
};

void SDL_XBIOS_ListMilanModes(_THIS, int actually_add)
{
	int i;

	/* Read infos about current mode */ 

	/* Read validated predefined modes */
	for (i=0; i<NUM_PREDEFINED_MODES; i++) {
		int j;
		Uint16 deviceid = 0x1000 + (i<<4);

		for (j=1; j<4; j++) {
			if (Validmode(deviceid + j)) {
				xbiosmode_t modeinfo;
				
				modeinfo.number = deviceid + j;
				modeinfo.width = mode_list[i].width;
				modeinfo.height = mode_list[i].height;
				modeinfo.depth = mode_bpp[j-1];
				modeinfo.doubleline = SDL_FALSE;

				SDL_XBIOS_AddMode(this, actually_add, &modeinfo);
			}
		}
	}

	/* Read custom created modes */
}
