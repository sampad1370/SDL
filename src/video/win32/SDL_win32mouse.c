/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2006 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Sam Lantinga
    slouken@libsdl.org
*/

#if (_WIN32_WINNT < 0x0501)
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

#include "SDL_config.h"

#include "SDL_win32video.h"

#include "../../events/SDL_mouse_c.h"

#include <wintab.h>
#define PACKETDATA ( PK_X | PK_Y | PK_BUTTONS | PK_NORMAL_PRESSURE)
#define PACKETMODE 0
#include <pktdef.h>

extern HANDLE* mice;

extern int total_mice;

extern int tablet;

void
WIN_InitMouse(_THIS)
{
	int index=0;
	RAWINPUTDEVICELIST *deviceList=NULL;
	int devCount=0;
	int i;
	int tmp=0;
	char* buffer=NULL;
	char* tab="wacom";

    SDL_VideoData *data = (SDL_VideoData *) _this->driverdata;

	if(GetRawInputDeviceList(NULL,&devCount,sizeof(RAWINPUTDEVICELIST)))
	{
		return;
	}
	else
	{
		deviceList = SDL_malloc(sizeof(RAWINPUTDEVICELIST)*devCount);
	}

	GetRawInputDeviceList(deviceList,&devCount,sizeof(RAWINPUTDEVICELIST));

	mice = SDL_malloc(devCount*sizeof(HANDLE));

	for(i=0;i<devCount;++i)
	{
		int j;
		int k;
		char *default_device_name="Pointing device xx";
		const char *reg_key_root = "System\\CurrentControlSet\\Enum\\";
		char *device_name=SDL_malloc(256*sizeof(char));
		char *key_name=NULL;
		char *tmp_name=NULL;
		LONG rc = 0;
		HKEY hkey;
		DWORD regtype = REG_SZ;
		DWORD out=256*sizeof(char);
		SDL_Mouse mouse;
		int l;
		if(deviceList[i].dwType!=RIM_TYPEMOUSE)
		{
			continue;
		}
		if(GetRawInputDeviceInfoA(deviceList[i].hDevice, RIDI_DEVICENAME, NULL, &tmp)<0)
		{
			continue;
		}
		buffer = SDL_malloc((tmp+1)*sizeof(char));
		key_name = SDL_malloc(tmp + sizeof(reg_key_root)*sizeof(char));

		if(GetRawInputDeviceInfoA(deviceList[i].hDevice, RIDI_DEVICENAME, buffer, &tmp)<0)
		{
			continue;
		}

		buffer+=4;
		tmp-=4;

		tmp_name=buffer;
		for(j=0;j<tmp;++j)
		{
			if(*tmp_name=='#')
			{
				*tmp_name='\\';
			}
			else if(*tmp_name=='{')
			{
				break;
			}
			++tmp_name;
		}
		*tmp_name='\0';

	SDL_memcpy(key_name, reg_key_root, SDL_strlen (reg_key_root));
    SDL_memcpy(key_name + (SDL_strlen (reg_key_root)), buffer, j + 1);
    
	rc = RegOpenKeyExA(HKEY_LOCAL_MACHINE, key_name, 0, KEY_READ, &hkey);
    
	if (rc != ERROR_SUCCESS)
	{
		SDL_memcpy(device_name, default_device_name, SDL_strlen(default_device_name));
	}

    rc = RegQueryValueExA(hkey, "DeviceDesc", NULL, &regtype, device_name, &out);
    RegCloseKey(hkey);
    if (rc != ERROR_SUCCESS)
	{
        SDL_memcpy(device_name, default_device_name, SDL_strlen(default_device_name));
    } 
		mice[index]=deviceList[i].hDevice;
		SDL_zero(mouse);
		SDL_SetIndexId(index,index);
		l=SDL_strlen(device_name);
		if(tablet==-1)
		{
			for(j=0;j<l-5;++j)
			{
				for(k=0;k<5;++k)
				{
					if(tab[k]!=SDL_tolower((unsigned char)device_name[j+k]))
					{
						break;
					}
				}
				if(k==5)
				{
					tablet=index;
					break;
				}
			}
		}
		if(tablet==index)
		{
			AXIS pressure;
			WTInfo(WTI_DEVICES,DVC_NPRESSURE, &pressure);
			data->mouse = SDL_AddMouse(&mouse, index,device_name,pressure.axMax,pressure.axMin);
		}
		else
		{
			data->mouse = SDL_AddMouse(&mouse, index,device_name,0,0);
		}
		++index;

		SDL_free(buffer);
		SDL_free(key_name);
	}
	total_mice=index;
	SDL_free(deviceList);
}

void
WIN_QuitMouse(_THIS)
{
	int i;
    SDL_VideoData *data = (SDL_VideoData *) _this->driverdata;
	for(i=0;i<total_mice;++i)
	{
		SDL_DelMouse(i);
	}
}

/* vi: set ts=4 sw=4 expandtab: */

