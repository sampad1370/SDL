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

/**
 * \file SDL_video.h
 *
 * Header file for access to the SDL raw framebuffer window
 */

#ifndef _SDL_video_h
#define _SDL_video_h

#include "SDL_stdinc.h"
#include "SDL_error.h"
#include "SDL_pixels.h"
#include "SDL_rwops.h"

#include "begin_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
/* *INDENT-OFF* */
extern "C" {
/* *INDENT-ON* */
#endif

/* Transparency definitions: These define alpha as the opacity of a surface */
#define SDL_ALPHA_OPAQUE 255
#define SDL_ALPHA_TRANSPARENT 0

/**
 * \struct SDL_Rect
 *
 * \brief A rectangle, with the origin at the upper left.
 */
typedef struct SDL_Rect
{
    Sint16 x, y;
    Uint16 w, h;
} SDL_Rect;

/**
 * \struct SDL_DisplayMode
 *
 * \brief  The structure that defines a display mode
 *
 * \sa SDL_GetNumDisplayModes()
 * \sa SDL_GetDisplayMode()
 * \sa SDL_GetDesktopDisplayMode()
 * \sa SDL_GetCurrentDisplayMode()
 * \sa SDL_GetClosestDisplayMode()
 * \sa SDL_SetDisplayMode()
 */
typedef struct
{
    Uint32 format;              /**< pixel format */
    int w;                      /**< width */
    int h;                      /**< height */
    int refresh_rate;           /**< refresh rate (or zero for unspecified) */
} SDL_DisplayMode;

/**
 * \typedef SDL_WindowID
 *
 * \brief The type used to identify a window
 *
 * \sa SDL_CreateWindow()
 * \sa SDL_CreateWindowFrom()
 * \sa SDL_DestroyWindow()
 * \sa SDL_GetWindowData()
 * \sa SDL_GetWindowFlags()
 * \sa SDL_GetWindowGrab()
 * \sa SDL_GetWindowPosition()
 * \sa SDL_GetWindowSize()
 * \sa SDL_GetWindowTitle()
 * \sa SDL_HideWindow()
 * \sa SDL_MaximizeWindow()
 * \sa SDL_MinimizeWindow()
 * \sa SDL_RaiseWindow()
 * \sa SDL_RestoreWindow()
 * \sa SDL_SetWindowData()
 * \sa SDL_SetWindowGrab()
 * \sa SDL_SetWindowIcon()
 * \sa SDL_SetWindowPosition()
 * \sa SDL_SetWindowSize()
 * \sa SDL_SetWindowTitle()
 * \sa SDL_ShowWindow()
 */
typedef Uint32 SDL_WindowID;

/**
 * \enum SDL_WindowFlags
 *
 * \brief The flags on a window
 *
 * \sa SDL_GetWindowFlags()
 */
typedef enum
{
    SDL_WINDOW_FULLSCREEN = 0x00000001,         /**< fullscreen window, implies borderless */
    SDL_WINDOW_BORDERLESS = 0x00000002,         /**< no window decoration */
    SDL_WINDOW_SHOWN = 0x00000004,              /**< window is visible */
    SDL_WINDOW_OPENGL = 0x00000008,             /**< window usable with OpenGL context */
    SDL_WINDOW_RESIZABLE = 0x00000010,          /**< window can be resized */
    SDL_WINDOW_MAXIMIZED = 0x00000020,          /**< maximized */
    SDL_WINDOW_MINIMIZED = 0x00000040,          /**< minimized */
    SDL_WINDOW_INPUT_GRABBED = 0x00000080,      /**< window has grabbed input focus */
    SDL_WINDOW_KEYBOARD_FOCUS = 0x00000100,     /**< window has keyboard focus */
    SDL_WINDOW_MOUSE_FOCUS = 0x00000200,        /**< window has mouse focus */
} SDL_WindowFlags;

/**
 * \enum SDL_WindowEventID
 *
 * \brief Event subtype for window events
 */
typedef enum
{
    SDL_WINDOWEVENT_NONE,               /**< Never used */
    SDL_WINDOWEVENT_SHOWN,              /**< Window has been shown */
    SDL_WINDOWEVENT_HIDDEN,             /**< Window has been hidden */
    SDL_WINDOWEVENT_MOVED,              /**< Window has been moved to data1,data2 */
    SDL_WINDOWEVENT_RESIZED,            /**< Window size changed to data1xdata2 */
    SDL_WINDOWEVENT_MINIMIZED,          /**< Window has been minimized */
    SDL_WINDOWEVENT_MAXIMIZED,          /**< Window has been maximized */
    SDL_WINDOWEVENT_RESTORED,           /**< Window has been restored to normal size and position */
    SDL_WINDOWEVENT_ENTER,              /**< The window has gained mouse focus */
    SDL_WINDOWEVENT_LEAVE,              /**< The window has lost mouse focus */
    SDL_WINDOWEVENT_FOCUS_GAINED,       /**< The window has gained keyboard focus */
    SDL_WINDOWEVENT_FOCUS_LOST,         /**< The window has lost keyboard focus */
} SDL_WindowEventID;

/**
 * \enum SDL_RendererFlags
 *
 * \brief Flags used when initializing a render manager.
 */
typedef enum
{
    SDL_Renderer_PresentDiscard = 0x00000001,   /**< Present leaves the contents of the backbuffer undefined */
    SDL_Renderer_PresentCopy = 0x00000002,      /**< Present uses a copy from back buffer to the front buffer */
    SDL_Renderer_PresentFlip2 = 0x00000004,     /**< Present uses a flip, swapping back buffer and front buffer */
    SDL_Renderer_PresentFlip3 = 0x00000008,     /**< Present uses a flip, rotating between two back buffers and a front buffer */
    SDL_Renderer_PresentVSync = 0x00000010,     /**< Present is synchronized with the refresh rate */
    SDL_Renderer_RenderTarget = 0x00000020,     /**< The renderer can create texture render targets */
    SDL_Renderer_Accelerated = 0x00000040,      /**< The renderer uses hardware acceleration */
    SDL_Renderer_Minimal = 0x00000080,          /**< The renderer only supports the read/write pixel and present functions */
} SDL_RendererFlags;

/**
 * \struct SDL_RendererInfo
 *
 * \brief Information on the capabilities of a render manager.
 */
typedef struct SDL_RendererInfo
{
    const char *name;           /**< The name of the renderer */
    Uint32 flags;               /**< Supported SDL_RendererFlags */
    Uint32 blend_modes;         /**< A mask of supported blend modes */
    Uint32 scale_modes;         /**< A mask of supported scale modes */
    Uint32 num_texture_formats; /**< The number of available texture formats */
    Uint32 texture_formats[32]; /**< The available texture formats */
    int max_texture_width;      /**< The maximimum texture width */
    int max_texture_height;     /**< The maximimum texture height */
} SDL_RendererInfo;

/**
 * \enum SDL_TextureAccess
 *
 * \brief The access pattern allowed for a texture
 */
typedef enum
{
    SDL_TextureAccess_Render,   /**< Unlockable video memory, rendering allowed */
    SDL_TextureAccess_Remote,   /**< Unlockable video memory */
    SDL_TextureAccess_Local,    /**< Lockable system memory */
} SDL_TextureAccess;

/**
 * \enum SDL_TextureBlendMode
 *
 * \brief The blend mode used in SDL_RenderCopy()
 */
typedef enum
{
    SDL_TextureBlendMode_None = 0x00000000,     /**< No blending */
    SDL_TextureBlendMode_Mask = 0x00000001,     /**< dst = A ? src : dst (alpha is mask) */
    SDL_TextureBlendMode_Blend = 0x00000002,    /**< dst = (src * A) + (dst * (1-A)) */
    SDL_TextureBlendMode_Add = 0x00000004,      /**< dst = (src * A) + dst */
    SDL_TextureBlendMode_Mod = 0x00000008,      /**< dst = src * dst */
} SDL_TextureBlendMode;

/**
 * \enum SDL_TextureScaleMode
 *
 * \brief The scale mode used in SDL_RenderCopy()
 */
typedef enum
{
    SDL_TextureScaleMode_None = 0x00000000,     /**< No scaling, rectangles must match dimensions */
    SDL_TextureScaleMode_Fast = 0x00000001,     /**< Point sampling or equivalent algorithm */
    SDL_TextureScaleMode_Slow = 0x00000002,     /**< Linear filtering or equivalent algorithm */
    SDL_TextureScaleMode_Best = 0x00000004,     /**< Bicubic filtering or equivalent algorithm */
} SDL_TextureScaleMode;

/**
 * \typedef SDL_TextureID
 *
 * \brief An efficient driver-specific representation of pixel data
 */
typedef Uint32 SDL_TextureID;


/* These are the currently supported flags for the SDL_surface */
/* Used internally (read-only) */
#define SDL_HWSURFACE       0x00000001  /* Surface represents a texture */
#define SDL_PREALLOC        0x00000002  /* Surface uses preallocated memory */
#define SDL_SRCALPHA        0x00000004  /* Blit uses source alpha blending */
#define SDL_SRCCOLORKEY     0x00000008  /* Blit uses a source color key */
#define SDL_RLEACCELOK      0x00000010  /* Private flag */
#define SDL_RLEACCEL        0x00000020  /* Surface is RLE encoded */

/* Evaluates to true if the surface needs to be locked before access */
#define SDL_MUSTLOCK(S)	(((S)->flags & (SDL_HWSURFACE|SDL_RLEACCEL)) != 0)

/* This structure should be treated as read-only, except for 'pixels',
   which, if not NULL, contains the raw pixel data for the surface.
*/
typedef struct SDL_Surface
{
    Uint32 flags;               /* Read-only */
    SDL_PixelFormat *format;    /* Read-only */
    int w, h;                   /* Read-only */
    int pitch;                  /* Read-only */
    void *pixels;               /* Read-write */

    /* information needed for surfaces requiring locks */
    int locked;
    void *lock_data;

    /* clipping information */
    SDL_Rect clip_rect;         /* Read-only */

    /* info for fast blit mapping to other surfaces */
    struct SDL_BlitMap *map;    /* Private */

    /* format version, bumped at every change to invalidate blit maps */
    unsigned int format_version;        /* Private */

    /* Reference count -- used when freeing surface */
    int refcount;               /* Read-mostly */
} SDL_Surface;

/* typedef for private surface blitting functions */
typedef int (*SDL_blit) (struct SDL_Surface * src, SDL_Rect * srcrect,
                         struct SDL_Surface * dst, SDL_Rect * dstrect);


/**
 * \enum SDL_GLattr
 *
 * \brief OpenGL configuration attributes
 */
typedef enum
{
    SDL_GL_RED_SIZE,
    SDL_GL_GREEN_SIZE,
    SDL_GL_BLUE_SIZE,
    SDL_GL_ALPHA_SIZE,
    SDL_GL_BUFFER_SIZE,
    SDL_GL_DOUBLEBUFFER,
    SDL_GL_DEPTH_SIZE,
    SDL_GL_STENCIL_SIZE,
    SDL_GL_ACCUM_RED_SIZE,
    SDL_GL_ACCUM_GREEN_SIZE,
    SDL_GL_ACCUM_BLUE_SIZE,
    SDL_GL_ACCUM_ALPHA_SIZE,
    SDL_GL_STEREO,
    SDL_GL_MULTISAMPLEBUFFERS,
    SDL_GL_MULTISAMPLESAMPLES,
    SDL_GL_ACCELERATED_VISUAL,
    SDL_GL_SWAP_CONTROL
} SDL_GLattr;


/* Function prototypes */

/**
 * \fn int SDL_GetNumVideoDrivers(void)
 *
 * \brief Get the number of video drivers compiled into SDL
 *
 * \sa SDL_GetVideoDriver()
 */
extern DECLSPEC int SDLCALL SDL_GetNumVideoDrivers(void);

/**
 * \fn const char *SDL_GetVideoDriver(int index)
 *
 * \brief Get the name of a built in video driver.
 *
 * \note The video drivers are presented in the order in which they are
 * normally checked during initialization.
 *
 * \sa SDL_GetNumVideoDrivers()
 */
extern DECLSPEC const char *SDLCALL SDL_GetVideoDriver(int index);

/**
 * \fn int SDL_VideoInit(const char *driver_name, Uint32 flags)
 *
 * \brief Initialize the video subsystem, optionally specifying a video driver.
 *
 * \param driver_name Initialize a specific driver by name, or NULL for the default video driver.
 * \param flags FIXME: Still needed?
 *
 * \return 0 on success, -1 on error
 *
 * This function initializes the video subsystem; setting up a connection
 * to the window manager, etc, and determines the available display modes
 * and pixel formats, but does not initialize a window or graphics mode.
 *
 * \sa SDL_VideoQuit()
 */
extern DECLSPEC int SDLCALL SDL_VideoInit(const char *driver_name,
                                          Uint32 flags);

/**
 * \fn void SDL_VideoQuit(void)
 *
 * \brief Shuts down the video subsystem.
 *
 * This function closes all windows, and restores the original video mode.
 *
 * \sa SDL_VideoInit()
 */
extern DECLSPEC void SDLCALL SDL_VideoQuit(void);

/**
 * \fn const char *SDL_GetCurrentVideoDriver(void)
 *
 * \brief Returns the name of the currently initialized video driver.
 *
 * \return The name of the current video driver or NULL if no driver
 *         has been initialized
 *
 * \sa SDL_GetNumVideoDrivers()
 * \sa SDL_GetVideoDriver()
 */
extern DECLSPEC const char *SDLCALL SDL_GetCurrentVideoDriver(void);

/**
 * \fn int SDL_GetNumVideoDisplays(void)
 *
 * \brief Returns the number of available video displays.
 *
 * \sa SDL_SelectVideoDisplay()
 */
extern DECLSPEC int SDLCALL SDL_GetNumVideoDisplays(void);

/**
 * \fn int SDL_SelectVideoDisplay(int index)
 *
 * \brief Set the index of the currently selected display.
 *
 * \return The index of the currently selected display.
 *
 * \note You can query the currently selected display by passing an index of -1.
 *
 * \sa SDL_GetNumVideoDisplays()
 */
extern DECLSPEC int SDLCALL SDL_SelectVideoDisplay(int index);

/**
 * \fn int SDL_GetNumDisplayModes(void)
 *
 * \brief Returns the number of available display modes for the current display.
 *
 * \sa SDL_GetDisplayMode()
 */
extern DECLSPEC int SDLCALL SDL_GetNumDisplayModes(void);

/**
 * \fn const SDL_DisplayMode *SDL_GetDisplayMode(int index)
 *
 * \brief Retrieve information about a specific display mode.
 *
 * \note The display modes are sorted in this priority:
 *       \li bits per pixel -> more colors to fewer colors
 *       \li width -> largest to smallest
 *       \li height -> largest to smallest
 *       \li refresh rate -> highest to lowest
 *
 * \sa SDL_GetNumDisplayModes()
 */
extern DECLSPEC const SDL_DisplayMode *SDLCALL SDL_GetDisplayMode(int index);

/**
 * \fn const SDL_DisplayMode *SDL_GetDesktopDisplayMode(void)
 *
 * \brief Retrieve information about the desktop display mode for the current display.
 */
extern DECLSPEC const SDL_DisplayMode *SDLCALL
SDL_GetDesktopDisplayMode(void);

/**
 * \fn const SDL_DisplayMode *SDL_GetCurrentDisplayMode(void)
 *
 * \brief Retrieve information about the current display mode.
 */
extern DECLSPEC const SDL_DisplayMode *SDLCALL
SDL_GetCurrentDisplayMode(void);

/**
 * \fn SDL_DisplayMode *SDL_GetClosestDisplayMode(const SDL_DisplayMode *mode, SDL_DisplayMode *closest)
 *
 * \brief Get the closest match to the requested display mode.
 *
 * \param mode The desired display mode
 * \param closest A pointer to a display mode to be filled in with the closest match of the available display modes.
 *
 * \return The passed in value 'closest', or NULL if no matching video mode was available.
 *
 * The available display modes are scanned, and 'closest' is filled in with the closest mode matching the requested mode and returned.  The mode format and refresh_rate default to the desktop mode if they are 0.  The modes are scanned with size being first priority, format being second priority, and finally checking the refresh_rate.  If all the available modes are too small, then NULL is returned.
 *
 * \sa SDL_GetNumDisplayModes()
 * \sa SDL_GetDisplayMode()
 */
extern DECLSPEC SDL_DisplayMode *SDLCALL SDL_GetClosestDisplayMode(const
                                                                   SDL_DisplayMode
                                                                   * mode,
                                                                   SDL_DisplayMode
                                                                   * closest);

/**
 * \fn int SDL_SetDisplayMode(const SDL_DisplayMode *mode)
 *
 * \brief Set up the closest available mode on the current display.
 *
 * \param mode The desired display mode
 *
 * \return 0 on success, or -1 if setting the display mode failed.
 */
extern DECLSPEC int SDLCALL SDL_SetDisplayMode(const SDL_DisplayMode * mode);

/**
 * \fn int SDL_SetDisplayPalette(const SDL_Color *colors, int firstcolor, int ncolors)
 *
 * \brief Set the palette entries for indexed display modes.
 *
 * \return 0 on success, or -1 if the display mode isn't palettized or the colors couldn't be set.
 */
extern DECLSPEC int SDLCALL SDL_SetDisplayPalette(const SDL_Color * colors,
                                                  int firstcolor,
                                                  int ncolors);

/**
 * \fn int SDL_GetDisplayPalette(SDL_Color *colors, int firstcolor, int ncolors)
 *
 * \brief Gets the palette entries for indexed display modes.
 *
 * \return 0 on success, or -1 if the display mode isn't palettized
 */
extern DECLSPEC int SDLCALL SDL_GetDisplayPalette(SDL_Color * colors,
                                                  int firstcolor,
                                                  int ncolors);

/**
 * \fn SDL_WindowID SDL_CreateWindow(const char *title, int x, int y, int w, int h, Uint32 flags)
 *
 * \brief Create a window with the specified position, dimensions, and flags.
 *
 * \param title The title of the window
 * \param x The x position of the window
 * \param y The y position of the window
 * \param w The width of the window
 * \param h The height of the window
 * \param flags The flags for the window
 *
 * \return The id of the window created, or zero if window creation failed.
 *
 * \note Setting the position to -1, -1, indicates any position is fine.
 *
 * \sa SDL_DestroyWindow()
 */
extern DECLSPEC SDL_WindowID SDLCALL SDL_CreateWindow(const char *title,
                                                      int x, int y, int w,
                                                      int h, Uint32 flags);

/**
 * \fn SDL_WindowID SDL_CreateWindowFrom(void *data)
 *
 * \brief Create an SDL window struct from an existing native window.
 *
 * \param data A pointer to driver-dependent window creation data
 *
 * \return The id of the window created, or zero if window creation failed.
 *
 * \warning This function is NOT SUPPORTED, use at your own risk!
 *
 * \sa SDL_DestroyWindow()
 */
extern DECLSPEC SDL_WindowID SDLCALL SDL_CreateWindowFrom(const void *data);

/**
 * \fn Uint32 SDL_GetWindowFlags(SDL_WindowID windowID)
 *
 * \brief Get the window flags.
 */
extern DECLSPEC Uint32 SDLCALL SDL_GetWindowFlags(SDL_WindowID windowID);

/**
 * \fn void SDL_SetWindowTitle(SDL_WindowID windowID, const char *title)
 *
 * \brief Set the title of the window, in UTF-8 format.
 *
 * \sa SDL_GetWindowTitle()
 */
extern DECLSPEC void SDLCALL SDL_SetWindowTitle(SDL_WindowID windowID,
                                                const char *title);

/**
 * \fn const char *SDL_GetWindowTitle(SDL_WindowID windowID)
 *
 * \brief Get the title of the window, in UTF-8 format.
 *
 * \sa SDL_SetWindowTitle()
 */
extern DECLSPEC const char *SDLCALL SDL_GetWindowTitle(SDL_WindowID windowID);

/**
 * \fn void SDL_SetWindowIcon(SDL_Surface *icon)
 *
 * \brief Set the icon of the window.
 *
 * \param icon The icon for the window
 *
 * FIXME: The icon needs to be set before the window is first shown.  Should some icon representation be part of the window creation data?
 */
extern DECLSPEC void SDLCALL SDL_SetWindowIcon(SDL_Surface * icon);

/**
 * \fn void SDL_SetWindowData(SDL_WindowID windowID, void *userdata)
 *
 * \brief Associate an arbitrary pointer with the window.
 *
 * \sa SDL_GetWindowData()
 */
extern DECLSPEC void SDLCALL SDL_SetWindowData(SDL_WindowID windowID,
                                               void *userdata);

/**
 * \fn void *SDL_GetWindowData(SDL_WindowID windowID)
 *
 * \brief Retrieve the data pointer associated with the window.
 *
 * \sa SDL_SetWindowData()
 */
extern DECLSPEC void *SDLCALL SDL_GetWindowData(SDL_WindowID windowID);

/**
 * \fn void SDL_SetWindowPosition(SDL_WindowID windowID, int x, int y)
 *
 * \brief Set the position of the window.
 *
 * \sa SDL_GetWindowPosition()
 */
extern DECLSPEC void SDLCALL SDL_SetWindowPosition(SDL_WindowID windowID,
                                                   int x, int y);

/**
 * \fn void SDL_GetWindowPosition(SDL_WindowID windowID, int *x, int *y)
 *
 * \brief Get the position of the window.
 *
 * \sa SDL_SetWindowPosition()
 */
extern DECLSPEC void SDLCALL SDL_GetWindowPosition(SDL_WindowID windowID,
                                                   int *x, int *y);

/**
 * \fn void SDL_SetWindowSize(SDL_WindowID windowID, int w, int w)
 *
 * \brief Set the size of the window's client area.
 *
 * \note You can't change the size of a fullscreen window, it automatically
 * matches the size of the display mode.
 *
 * \sa SDL_GetWindowSize()
 */
extern DECLSPEC void SDLCALL SDL_SetWindowSize(SDL_WindowID windowID, int w,
                                               int h);

/**
 * \fn void SDL_GetWindowSize(SDL_WindowID windowID, int *w, int *w)
 *
 * \brief Get the size of the window's client area.
 *
 * \sa SDL_SetWindowSize()
 */
extern DECLSPEC void SDLCALL SDL_GetWindowSize(SDL_WindowID windowID, int *w,
                                               int *h);

/**
 * \fn void SDL_ShowWindow(SDL_WindowID windowID)
 *
 * \brief Show the window
 *
 * \sa SDL_HideWindow()
 */
extern DECLSPEC void SDLCALL SDL_ShowWindow(SDL_WindowID windowID);

/**
 * \fn void SDL_HideWindow(SDL_WindowID windowID)
 *
 * \brief Hide the window
 *
 * \sa SDL_ShowWindow()
 */
extern DECLSPEC void SDLCALL SDL_HideWindow(SDL_WindowID windowID);

/**
 * \fn void SDL_RaiseWindow(SDL_WindowID windowID)
 *
 * \brief Raise the window so it's above other windows.
 */
extern DECLSPEC void SDLCALL SDL_RaiseWindow(SDL_WindowID windowID);

/**
 * \fn void SDL_MaximizeWindow(SDL_WindowID windowID)
 *
 * \brief Make the window as large as possible.
 *
 * \sa SDL_RestoreWindow()
 */
extern DECLSPEC void SDLCALL SDL_MaximizeWindow(SDL_WindowID windowID);

/**
 * \fn void SDL_MinimizeWindow(SDL_WindowID windowID)
 *
 * \brief Minimize the window to an iconic representation.
 *
 * \sa SDL_RestoreWindow()
 */
extern DECLSPEC void SDLCALL SDL_MinimizeWindow(SDL_WindowID windowID);

/**
 * \fn void SDL_RestoreWindow(SDL_WindowID windowID)
 *
 * \brief Restore the size and position of a minimized or maximized window.
 *
 * \sa SDL_MaximizeWindow()
 * \sa SDL_MinimizeWindow()
 */
extern DECLSPEC void SDLCALL SDL_RestoreWindow(SDL_WindowID windowID);

/**
 * \fn void SDL_SetWindowGrab(SDL_WindowID windowID, int mode)
 *
 * \brief Set the window's input grab mode.
 *
 * \param mode This is 1 to grab input, and 0 to release input.
 *
 * \sa SDL_GrabMode
 * \sa SDL_GetWindowGrab()
 */
extern DECLSPEC void SDLCALL SDL_SetWindowGrab(SDL_WindowID windowID,
                                               int mode);

/**
 * \fn int SDL_GetWindowGrab(SDL_WindowID windowID)
 *
 * \brief Get the window's input grab mode.
 *
 * \return This returns 1 if input is grabbed, and 0 otherwise.
 *
 * \sa SDL_GrabMode
 * \sa SDL_SetWindowGrab()
 */
extern DECLSPEC int SDLCALL SDL_GetWindowGrab(SDL_WindowID windowID);

/**
 * \fn void SDL_DestroyWindow(SDL_WindowID windowID)
 *
 * \brief Destroy a window.
 */
extern DECLSPEC void SDLCALL SDL_DestroyWindow(SDL_WindowID windowID);

/**
 * \fn int SDL_GetNumRenderers(void)
 *
 * \brief Get the number of render managers on the current display.
 *
 * A render manager is a set of code that handles rendering and texture
 * management on a particular display.  Normally there is only one, but
 * some drivers may have several available with different capabilities.
 *
 * \sa SDL_GetRendererInfo()
 * \sa SDL_CreateRenderer()
 */
extern DECLSPEC int SDLCALL SDL_GetNumRenderers(void);

/**
 * \fn SDL_RendererInfo *SDL_GetRendererInfo(int index)
 *
 * \brief Get information about a specific render manager on the current
 *        display.
 *
 * \sa SDL_CreateRenderer()
 */
extern DECLSPEC int SDLCALL SDL_GetRendererInfo(int index,
                                                SDL_RendererInfo * info);

/**
 * \fn int SDL_CreateRenderer(SDL_WindowID window, int index, Uint32 flags)
 *
 * \brief Create and make active a 2D rendering context for a window.
 *
 * \param windowID The window used for rendering.
 * \param index The index of the render manager to initialize, or -1 to initialize the first one supporting the requested flags.
 * \param flags SDL_RendererFlags
 *
 * \return 0 on success, -1 if the flags were not supported, or -2 if
 *         there isn't enough memory to support the requested flags
 *
 * \sa SDL_SelectRenderer()
 * \sa SDL_DestroyRenderer()
 */
extern DECLSPEC int SDLCALL SDL_CreateRenderer(SDL_WindowID windowID,
                                               int index, Uint32 flags);

/**
 * \fn int SDL_SelectRenderer(SDL_WindowID windowID)
 *
 * \brief Select the rendering context for a particular window.
 *
 * \return 0 on success, -1 if the selected window doesn't have a
 *         rendering context.
 */
extern DECLSPEC int SDLCALL SDL_SelectRenderer(SDL_WindowID windowID);

/**
 * \fn SDL_TextureID SDL_CreateTexture(Uint32 format, int access, int w, int h)
 *
 * \brief Create a texture
 *
 * \param format The format of the texture
 * \param access One of the enumerated values in SDL_TextureAccess
 * \param w The width of the texture in pixels
 * \param h The height of the texture in pixels
 *
 * \return The created texture is returned, or 0 if no render manager was active,  the format was unsupported, or the width or height were out of range.
 *
 * \sa SDL_QueryTexture()
 * \sa SDL_DestroyTexture()
 */
extern DECLSPEC SDL_TextureID SDLCALL SDL_CreateTexture(Uint32 format,
                                                        int access, int w,
                                                        int h);

/**
 * \fn SDL_TextureID SDL_CreateTextureFromSurface(Uint32 format, int access, SDL_Surface *surface)
 *
 * \brief Create a texture from an existing surface.
 *
 * \param format The format of the texture, or 0 to pick an appropriate format
 * \param access One of the enumerated values in SDL_TextureAccess
 * \param surface The surface containing pixel data used to fill the texture
 *
 * \return The created texture is returned, or 0 if no render manager was active,  the format was unsupported, or the surface width or height were out of range.
 *
 * \note The surface is not modified or freed by this function.
 *
 * \sa SDL_QueryTexture()
 * \sa SDL_DestroyTexture()
 */
extern DECLSPEC SDL_TextureID SDLCALL SDL_CreateTextureFromSurface(Uint32
                                                                   format,
                                                                   int access,
                                                                   SDL_Surface
                                                                   * surface);

/**
 * \fn int SDL_QueryTexture(SDL_TextureID textureID, Uint32 *format, int *access, int *w, int *h)
 *
 * \brief Query the attributes of a texture
 *
 * \param texture A texture to be queried
 * \param format A pointer filled in with the raw format of the texture.  The actual format may differ, but pixel transfers will use this format.
 * \param access A pointer filled in with the actual access to the texture.
 * \param w A pointer filled in with the width of the texture in pixels
 * \param h A pointer filled in with the height of the texture in pixels
 *
 * \return 0 on success, or -1 if the texture is not valid
 */
extern DECLSPEC int SDLCALL SDL_QueryTexture(SDL_TextureID textureID,
                                             Uint32 * format, int *access,
                                             int *w, int *h);

/**
 * \fn int SDL_QueryTexturePixels(SDL_TextureID textureID, void **pixels, int pitch)
 *
 * \brief Query the pixels of a texture, if the texture does not need to be locked for pixel access.
 *
 * \param texture A texture to be queried, which was created with SDL_TextureAccess_Local
 * \param pixels A pointer filled with a pointer to the pixels for the texture 
 * \param pitch A pointer filled in with the pitch of the pixel data
 *
 * \return 0 on success, or -1 if the texture is not valid, or must be locked for pixel access.
 */
extern DECLSPEC int SDLCALL SDL_QueryTexturePixels(SDL_TextureID textureID,
                                                   void **pixels, int *pitch);

/**
 * \fn int SDL_SetTexturePalette(SDL_TextureID textureID, const SDL_Color * colors, int firstcolor, int ncolors)
 *
 * \brief Update an indexed texture with a color palette
 *
 * \param texture The texture to update
 * \param colors The array of RGB color data
 * \param firstcolor The first index to update
 * \param ncolors The number of palette entries to fill with the color data
 *
 * \return 0 on success, or -1 if the texture is not valid or not an indexed texture
 */
extern DECLSPEC int SDLCALL SDL_SetTexturePalette(SDL_TextureID textureID,
                                                  const SDL_Color * colors,
                                                  int firstcolor,
                                                  int ncolors);

/**
 * \fn int SDL_GetTexturePalette(SDL_TextureID textureID, SDL_Color * colors, int firstcolor, int ncolors)
 *
 * \brief Update an indexed texture with a color palette
 *
 * \param texture The texture to update
 * \param colors The array to fill with RGB color data
 * \param firstcolor The first index to retrieve
 * \param ncolors The number of palette entries to retrieve
 *
 * \return 0 on success, or -1 if the texture is not valid or not an indexed texture
 */
extern DECLSPEC int SDLCALL SDL_GetTexturePalette(SDL_TextureID textureID,
                                                  SDL_Color * colors,
                                                  int firstcolor,
                                                  int ncolors);

/**
 * \fn int SDL_UpdateTexture(SDL_TextureID textureID, const SDL_Rect *rect, const void *pixels, int pitch)
 *
 * \brief Update the given texture rectangle with new pixel data.
 *
 * \param texture The texture to update
 * \param rect A pointer to the rectangle of pixels to update, or NULL to update the entire texture.
 * \param pixels The raw pixel data
 * \param pitch The number of bytes between rows of pixel data
 *
 * \return 0 on success, or -1 if the texture is not valid
 *
 * \note This is a very slow function for textures not created with SDL_TextureAccess_Local.
 */
extern DECLSPEC int SDLCALL SDL_UpdateTexture(SDL_TextureID textureID,
                                              const SDL_Rect * rect,
                                              const void *pixels, int pitch);

/**
 * \fn void SDL_LockTexture(SDL_TextureID textureID, const SDL_Rect *rect, int markDirty, void **pixels, int *pitch)
 *
 * \brief Lock a portion of the texture for pixel access.
 *
 * \param texture The texture to lock for access, which must have been created with SDL_TextureAccess_Local.
 * \param rect A pointer to the rectangle to lock for access. If the rect is NULL, the entire texture will be locked.
 * \param markDirty If this is nonzero, the locked area will be marked dirty when the texture is unlocked.
 * \param pixels This is filled in with a pointer to the locked pixels, appropriately offset by the locked area.
 * \param pitch This is filled in with the pitch of the locked pixels.
 *
 * \return 0 on success, or -1 if the texture is not valid or was created with SDL_TextureAccess_Remote
 *
 * \sa SDL_DirtyTexture()
 * \sa SDL_UnlockTexture()
 */
extern DECLSPEC int SDLCALL SDL_LockTexture(SDL_TextureID textureID,
                                            const SDL_Rect * rect,
                                            int markDirty, void **pixels,
                                            int *pitch);

/**
 * \fn void SDL_UnlockTexture(SDL_TextureID textureID)
 *
 * \brief Unlock a texture, uploading the changes to video memory, if needed.
 *
 * \sa SDL_LockTexture()
 * \sa SDL_DirtyTexture()
 */
extern DECLSPEC void SDLCALL SDL_UnlockTexture(SDL_TextureID textureID);

/**
 * \fn void SDL_DirtyTexture(SDL_TextureID textureID, int numrects, const SDL_Rect * rects)
 *
 * \brief Mark the specified rectangles of the texture as dirty.
 *
 * \note The texture must have been created with SDL_TextureAccess_Local.
 *
 * \sa SDL_LockTexture()
 * \sa SDL_UnlockTexture()
 */
extern DECLSPEC void SDLCALL SDL_DirtyTexture(SDL_TextureID textureID,
                                              int numrects,
                                              const SDL_Rect * rects);

/**
 * \fn void SDL_SelectRenderTexture(SDL_TextureID textureID)
 *
 * \brief Select a texture as the rendering target, or 0 to reselect the current window.
 *
 * \note The texture must have been created with SDL_TextureAccess_Render.
 */
extern DECLSPEC void SDLCALL SDL_SelectRenderTexture(SDL_TextureID textureID);

/**
 * \fn void SDL_RenderFill(const SDL_Rect *rect, Uint32 color)
 *
 * \brief Fill the current rendering target with the specified color.
 *
 * \param rect A pointer to the destination rectangle, or NULL for the entire rendering target.
 * \param color An ARGB color value.
 *
 * \return 0 on success, or -1 if there is no renderer current
 */
extern DECLSPEC int SDLCALL SDL_RenderFill(const SDL_Rect * rect,
                                           Uint32 color);

/**
 * \fn int SDL_RenderCopy(SDL_TextureID textureID, const SDL_Rect *srcrect, const SDL_Rect *dstrect, Uint32 blendMode, Uint32 scaleMode)
 *
 * \brief Copy a portion of the texture to the current rendering target.
 *
 * \param texture The source texture.
 * \param srcrect A pointer to the source rectangle, or NULL for the entire texture.
 * \param dstrect A pointer to the destination rectangle, or NULL for the entire rendering target.
 * \param blendMode SDL_TextureBlendMode to be used if the source texture has an alpha channel.
 * \param scaleMode SDL_TextureScaleMode to be used if the source and destination rectangles don't have the same width and height.
 *
 * \return 0 on success, or -1 if there is no renderer current, or the driver doesn't support the requested operation.
 *
 * \note You can check the video driver info to see what operations are supported.
 */
extern DECLSPEC int SDLCALL SDL_RenderCopy(SDL_TextureID textureID,
                                           const SDL_Rect * srcrect,
                                           const SDL_Rect * dstrect,
                                           int blendMode, int scaleMode);

/**
 * \fn int SDL_RenderReadPixels(const SDL_Rect *rect, void *pixels, int pitch)
 *
 * \brief Read pixels from the current rendering target.
 *
 * \param rect A pointer to the rectangle to read, or NULL for the entire render target
 * \param pixels A pointer to be filled in with the pixel data
 * \param pitch The pitch of the pixels parameter
 *
 * \return 0 on success, or -1 if pixel reading is not supported.
 *
 * \warning This is a very slow operation, and should not be used frequently.
 */
extern DECLSPEC int SDLCALL SDL_RenderReadPixels(const SDL_Rect * rect,
                                                 void *pixels, int pitch);

/**
 * \fn int SDL_RenderWritePixels(const SDL_Rect *rect, const void *pixels, int pitch)
 *
 * \brief Write pixels to the current rendering target.
 *
 * \param rect A pointer to the rectangle to write, or NULL for the entire render target
 * \param pixels A pointer to the pixel data to write
 * \param pitch The pitch of the pixels parameter
 *
 * \return 0 on success, or -1 if pixel writing is not supported.
 *
 * \warning This is a very slow operation, and should not be used frequently.
 */
extern DECLSPEC int SDLCALL SDL_RenderWritePixels(const SDL_Rect * rect,
                                                  const void *pixels,
                                                  int pitch);

/**
 * \fn void SDL_RenderPresent(void)
 *
 * \brief Update the screen with rendering performed.
 */
extern DECLSPEC void SDLCALL SDL_RenderPresent(void);

/**
 * \fn void SDL_DestroyTexture(SDL_TextureID textureID);
 *
 * \brief Destroy the specified texture.
 *
 * \sa SDL_CreateTexture()
 * \sa SDL_CreateTextureFromSurface()
 */
extern DECLSPEC void SDLCALL SDL_DestroyTexture(SDL_TextureID textureID);

/**
 * \fn void SDL_DestroyRenderer(SDL_WindowID windowID);
 *
 * \brief Destroy the rendering context for a window and free associated
 *        textures.
 *
 * \sa SDL_CreateRenderer()
 */
extern DECLSPEC void SDLCALL SDL_DestroyRenderer(SDL_WindowID windowID);

/*
 * Set the gamma correction for each of the color channels.
 * The gamma values range (approximately) between 0.1 and 10.0
 * 
 * If this function isn't supported directly by the hardware, it will
 * be emulated using gamma ramps, if available.  If successful, this
 * function returns 0, otherwise it returns -1.
 */
extern DECLSPEC int SDLCALL SDL_SetGamma(float red, float green, float blue);

/*
 * Set the gamma translation table for the red, green, and blue channels
 * of the video hardware.  Each table is an array of 256 16-bit quantities,
 * representing a mapping between the input and output for that channel.
 * The input is the index into the array, and the output is the 16-bit
 * gamma value at that index, scaled to the output color precision.
 * 
 * You may pass NULL for any of the channels to leave it unchanged.
 * If the call succeeds, it will return 0.  If the display driver or
 * hardware does not support gamma translation, or otherwise fails,
 * this function will return -1.
 */
extern DECLSPEC int SDLCALL SDL_SetGammaRamp(const Uint16 * red,
                                             const Uint16 * green,
                                             const Uint16 * blue);

/*
 * Retrieve the current values of the gamma translation tables.
 * 
 * You must pass in valid pointers to arrays of 256 16-bit quantities.
 * Any of the pointers may be NULL to ignore that channel.
 * If the call succeeds, it will return 0.  If the display driver or
 * hardware does not support gamma translation, or otherwise fails,
 * this function will return -1.
 */
extern DECLSPEC int SDLCALL SDL_GetGammaRamp(Uint16 * red, Uint16 * green,
                                             Uint16 * blue);

/*
 * Sets a portion of the colormap for the given 8-bit surface.  If 'surface'
 * is not a palettized surface, this function does nothing, returning 0.
 * If all of the colors were set as passed to SDL_SetColors(), it will
 * return 1.  If not all the color entries were set exactly as given,
 * it will return 0, and you should look at the surface palette to
 * determine the actual color palette.
 *
 * When 'surface' is the surface associated with the current display, the
 * display colormap will be updated with the requested colors.  If 
 * SDL_HWPALETTE was set in SDL_SetVideoMode() flags, SDL_SetColors()
 * will always return 1, and the palette is guaranteed to be set the way
 * you desire, even if the window colormap has to be warped or run under
 * emulation.
 */
extern DECLSPEC int SDLCALL SDL_SetColors(SDL_Surface * surface,
                                          const SDL_Color * colors,
                                          int firstcolor, int ncolors);

/*
 * Maps an RGB triple to an opaque pixel value for a given pixel format
 */
extern DECLSPEC Uint32 SDLCALL SDL_MapRGB
    (SDL_PixelFormat * format, Uint8 r, Uint8 g, Uint8 b);

/*
 * Maps an RGBA quadruple to a pixel value for a given pixel format
 */
extern DECLSPEC Uint32 SDLCALL SDL_MapRGBA(SDL_PixelFormat * format,
                                           Uint8 r, Uint8 g, Uint8 b,
                                           Uint8 a);

/*
 * Maps a pixel value into the RGB components for a given pixel format
 */
extern DECLSPEC void SDLCALL SDL_GetRGB(Uint32 pixel, SDL_PixelFormat * fmt,
                                        Uint8 * r, Uint8 * g, Uint8 * b);

/*
 * Maps a pixel value into the RGBA components for a given pixel format
 */
extern DECLSPEC void SDLCALL SDL_GetRGBA(Uint32 pixel, SDL_PixelFormat * fmt,
                                         Uint8 * r, Uint8 * g, Uint8 * b,
                                         Uint8 * a);

/*
 * Allocate and free an RGB surface (must be called after SDL_SetVideoMode)
 * If the depth is 4 or 8 bits, an empty palette is allocated for the surface.
 * If the depth is greater than 8 bits, the pixel format is set using the
 * flags '[RGB]mask'.
 * If the function runs out of memory, it will return NULL.
 *
 * The 'flags' tell what kind of surface to create.
 * SDL_SRCCOLORKEY indicates that the surface will be used for colorkey blits.
 * SDL_SRCALPHA means that the surface will be used for alpha blits.
 */
extern DECLSPEC SDL_Surface *SDLCALL SDL_CreateRGBSurface
    (Uint32 flags, int width, int height, int depth,
     Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask);
extern DECLSPEC SDL_Surface *SDLCALL SDL_CreateRGBSurfaceFrom(void *pixels,
                                                              int width,
                                                              int height,
                                                              int depth,
                                                              int pitch,
                                                              Uint32 Rmask,
                                                              Uint32 Gmask,
                                                              Uint32 Bmask,
                                                              Uint32 Amask);
extern DECLSPEC SDL_Surface *SDLCALL
SDL_CreateRGBSurfaceFromTexture(SDL_TextureID textureID);
extern DECLSPEC void SDLCALL SDL_FreeSurface(SDL_Surface * surface);

/*
 * SDL_LockSurface() sets up a surface for directly accessing the pixels.
 * Between calls to SDL_LockSurface()/SDL_UnlockSurface(), you can write
 * to and read from 'surface->pixels', using the pixel format stored in 
 * 'surface->format'.  Once you are done accessing the surface, you should 
 * use SDL_UnlockSurface() to release it.
 *
 * Not all surfaces require locking.  If SDL_MUSTLOCK(surface) evaluates
 * to 0, then you can read and write to the surface at any time, and the
 * pixel format of the surface will not change.
 * 
 * No operating system or library calls should be made between lock/unlock
 * pairs, as critical system locks may be held during this time.
 *
 * SDL_LockSurface() returns 0, or -1 if the surface couldn't be locked.
 */
extern DECLSPEC int SDLCALL SDL_LockSurface(SDL_Surface * surface);
extern DECLSPEC void SDLCALL SDL_UnlockSurface(SDL_Surface * surface);

/*
 * Load a surface from a seekable SDL data source (memory or file.)
 * If 'freesrc' is non-zero, the source will be closed after being read.
 * Returns the new surface, or NULL if there was an error.
 * The new surface should be freed with SDL_FreeSurface().
 */
extern DECLSPEC SDL_Surface *SDLCALL SDL_LoadBMP_RW(SDL_RWops * src,
                                                    int freesrc);

/* Convenience macro -- load a surface from a file */
#define SDL_LoadBMP(file)	SDL_LoadBMP_RW(SDL_RWFromFile(file, "rb"), 1)

/*
 * Save a surface to a seekable SDL data source (memory or file.)
 * If 'freedst' is non-zero, the source will be closed after being written.
 * Returns 0 if successful or -1 if there was an error.
 */
extern DECLSPEC int SDLCALL SDL_SaveBMP_RW
    (SDL_Surface * surface, SDL_RWops * dst, int freedst);

/* Convenience macro -- save a surface to a file */
#define SDL_SaveBMP(surface, file) \
		SDL_SaveBMP_RW(surface, SDL_RWFromFile(file, "wb"), 1)

/*
 * Sets the color key (transparent pixel) in a blittable surface.
 * If 'flag' is SDL_SRCCOLORKEY (optionally OR'd with SDL_RLEACCEL), 
 * 'key' will be the transparent pixel in the source image of a blit.
 * SDL_RLEACCEL requests RLE acceleration for the surface if present,
 * and removes RLE acceleration if absent.
 * If 'flag' is 0, this function clears any current color key.
 * This function returns 0, or -1 if there was an error.
 */
extern DECLSPEC int SDLCALL SDL_SetColorKey
    (SDL_Surface * surface, Uint32 flag, Uint32 key);

/*
 * This function sets the alpha value for the entire surface, as opposed to
 * using the alpha component of each pixel. This value measures the range
 * of transparency of the surface, 0 being completely transparent to 255
 * being completely opaque. An 'alpha' value of 255 causes blits to be
 * opaque, the source pixels copied to the destination (the default). Note
 * that per-surface alpha can be combined with colorkey transparency.
 *
 * If 'flag' is 0, alpha blending is disabled for the surface.
 * If 'flag' is SDL_SRCALPHA, alpha blending is enabled for the surface.
 * OR:ing the flag with SDL_RLEACCEL requests RLE acceleration for the
 * surface; if SDL_RLEACCEL is not specified, the RLE accel will be removed.
 *
 * The 'alpha' parameter is ignored for surfaces that have an alpha channel.
 */
extern DECLSPEC int SDLCALL SDL_SetAlpha(SDL_Surface * surface, Uint32 flag,
                                         Uint8 alpha);

/*
 * Sets the clipping rectangle for the destination surface in a blit.
 *
 * If the clip rectangle is NULL, clipping will be disabled.
 * If the clip rectangle doesn't intersect the surface, the function will
 * return SDL_FALSE and blits will be completely clipped.  Otherwise the
 * function returns SDL_TRUE and blits to the surface will be clipped to
 * the intersection of the surface area and the clipping rectangle.
 *
 * Note that blits are automatically clipped to the edges of the source
 * and destination surfaces.
 */
extern DECLSPEC SDL_bool SDLCALL SDL_SetClipRect(SDL_Surface * surface,
                                                 const SDL_Rect * rect);

/*
 * Gets the clipping rectangle for the destination surface in a blit.
 * 'rect' must be a pointer to a valid rectangle which will be filled
 * with the correct values.
 */
extern DECLSPEC void SDLCALL SDL_GetClipRect(SDL_Surface * surface,
                                             SDL_Rect * rect);

/*
 * Creates a new surface of the specified format, and then copies and maps 
 * the given surface to it so the blit of the converted surface will be as 
 * fast as possible.  If this function fails, it returns NULL.
 *
 * The 'flags' parameter is passed to SDL_CreateRGBSurface() and has those 
 * semantics.  You can also pass SDL_RLEACCEL in the flags parameter and
 * SDL will try to RLE accelerate colorkey and alpha blits in the resulting
 * surface.
 *
 * This function is used internally by SDL_DisplayFormat().
 */
extern DECLSPEC SDL_Surface *SDLCALL SDL_ConvertSurface
    (SDL_Surface * src, SDL_PixelFormat * fmt, Uint32 flags);

/*
 * This function performs a fast fill of the given rectangle with 'color'
 * The given rectangle is clipped to the destination surface clip area
 * and the final fill rectangle is saved in the passed in pointer.
 * If 'dstrect' is NULL, the whole surface will be filled with 'color'
 * The color should be a pixel of the format used by the surface, and 
 * can be generated by the SDL_MapRGB() function.
 * This function returns 0 on success, or -1 on error.
 */
extern DECLSPEC int SDLCALL SDL_FillRect
    (SDL_Surface * dst, SDL_Rect * dstrect, Uint32 color);

/*
 * This performs a fast blit from the source surface to the destination
 * surface.  It assumes that the source and destination rectangles are
 * the same size.  If either 'srcrect' or 'dstrect' are NULL, the entire
 * surface (src or dst) is copied.  The final blit rectangles are saved
 * in 'srcrect' and 'dstrect' after all clipping is performed.
 * If the blit is successful, it returns 0, otherwise it returns -1.
 *
 * The blit function should not be called on a locked surface.
 *
 * The blit semantics for surfaces with and without alpha and colorkey
 * are defined as follows:
 *
 * RGBA->RGB:
 *     SDL_SRCALPHA set:
 * 	alpha-blend (using alpha-channel).
 * 	SDL_SRCCOLORKEY ignored.
 *     SDL_SRCALPHA not set:
 * 	copy RGB.
 * 	if SDL_SRCCOLORKEY set, only copy the pixels matching the
 * 	RGB values of the source colour key, ignoring alpha in the
 * 	comparison.
 * 
 * RGB->RGBA:
 *     SDL_SRCALPHA set:
 * 	alpha-blend (using the source per-surface alpha value);
 * 	set destination alpha to opaque.
 *     SDL_SRCALPHA not set:
 * 	copy RGB, set destination alpha to source per-surface alpha value.
 *     both:
 * 	if SDL_SRCCOLORKEY set, only copy the pixels matching the
 * 	source colour key.
 * 
 * RGBA->RGBA:
 *     SDL_SRCALPHA set:
 * 	alpha-blend (using the source alpha channel) the RGB values;
 * 	leave destination alpha untouched. [Note: is this correct?]
 * 	SDL_SRCCOLORKEY ignored.
 *     SDL_SRCALPHA not set:
 * 	copy all of RGBA to the destination.
 * 	if SDL_SRCCOLORKEY set, only copy the pixels matching the
 * 	RGB values of the source colour key, ignoring alpha in the
 * 	comparison.
 * 
 * RGB->RGB: 
 *     SDL_SRCALPHA set:
 * 	alpha-blend (using the source per-surface alpha value).
 *     SDL_SRCALPHA not set:
 * 	copy RGB.
 *     both:
 * 	if SDL_SRCCOLORKEY set, only copy the pixels matching the
 * 	source colour key.
 *
 * If either of the surfaces were in video memory, and the blit returns -2,
 * the video memory was lost, so it should be reloaded with artwork and 
 * re-blitted:
	while ( SDL_BlitSurface(image, imgrect, screen, dstrect) == -2 ) {
		while ( SDL_LockSurface(image) < 0 )
			Sleep(10);
		-- Write image pixels to image->pixels --
		SDL_UnlockSurface(image);
	}
 * This happens under DirectX 5.0 when the system switches away from your
 * fullscreen application.  The lock will also fail until you have access
 * to the video memory again.
 */
/* You should call SDL_BlitSurface() unless you know exactly how SDL
   blitting works internally and how to use the other blit functions.
*/
#define SDL_BlitSurface SDL_UpperBlit

/* This is the public blit function, SDL_BlitSurface(), and it performs
   rectangle validation and clipping before passing it to SDL_LowerBlit()
*/
extern DECLSPEC int SDLCALL SDL_UpperBlit
    (SDL_Surface * src, SDL_Rect * srcrect,
     SDL_Surface * dst, SDL_Rect * dstrect);
/* This is a semi-private blit function and it performs low-level surface
   blitting only.
*/
extern DECLSPEC int SDLCALL SDL_LowerBlit
    (SDL_Surface * src, SDL_Rect * srcrect,
     SDL_Surface * dst, SDL_Rect * dstrect);

/**
 * \fn int SDL_SoftStretch(SDL_Surface * src, SDL_Rect * srcrect, SDL_Surface * dst, SDL_Rect * dstrect)
 *
 * \brief Perform a fast, low quality, stretch blit between two surfaces of the same pixel format.
 * \note This function uses a static buffer, and is not thread-safe.
 */
extern DECLSPEC int SDLCALL SDL_SoftStretch(SDL_Surface * src,
                                            SDL_Rect * srcrect,
                                            SDL_Surface * dst,
                                            SDL_Rect * dstrect);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* OpenGL support functions.                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 * Dynamically load an OpenGL library, or the default one if path is NULL
 *
 * If you do this, you need to retrieve all of the GL functions used in
 * your program from the dynamic library using SDL_GL_GetProcAddress().
 */
extern DECLSPEC int SDLCALL SDL_GL_LoadLibrary(const char *path);

/*
 * Get the address of a GL function
 */
extern DECLSPEC void *SDLCALL SDL_GL_GetProcAddress(const char *proc);

/*
 * Set an attribute of the OpenGL subsystem before window creation.
 */
extern DECLSPEC int SDLCALL SDL_GL_SetAttribute(SDL_GLattr attr, int value);

/*
 * Get an attribute of the OpenGL subsystem from the windowing
 * interface, such as glX. This is of course different from getting
 * the values from SDL's internal OpenGL subsystem, which only
 * stores the values you request before initialization.
 *
 * Developers should track the values they pass into SDL_GL_SetAttribute
 * themselves if they want to retrieve these values.
 */
extern DECLSPEC int SDLCALL SDL_GL_GetAttribute(SDL_GLattr attr, int *value);

/*
 * Swap the OpenGL buffers, if double-buffering is supported.
 */
extern DECLSPEC void SDLCALL SDL_GL_SwapBuffers(void);

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif
#include "close_code.h"

#endif /* _SDL_video_h */

/* vi: set ts=4 sw=4 expandtab: */
