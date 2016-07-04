
// Init.cpp
//
// Contains all the global variable declarations, include files, macros and defines
//
// 

// INCLUDES /////////////////////////

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <ddraw.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <stdio.h>
#include <io.h>
#include <fstream.h>
#include <time.h>

// DEFINES //////////////////////////

#define WINDOW_CLASS_NAME "WINDOW_CLASS"

#define SCREEN_BPP 8					// Colour depth
#define MAX_COLOURS 256					// Maximum Colours allowed

#define VIEW_DISTANCE 10.0				// Observer distance from viewplane
#define FOV 60							// Field of Vision

#define START_MAP_X 256					// Initial Start positions in map
#define START_MAP_Y 256
#define START_MAP_Z 20000

// MACROS ///////////////////////////

#define PIE              ((double)3.14159265)
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1:0)


// STRUCTURES ///////////////////////


// Holds bitmap file header information

typedef struct bitmap_file_tag {
       
	BITMAPFILEHEADER bitmapfileheader;  
	BITMAPINFOHEADER bitmapinfoheader; 
	
} bitmap_file, *bitmap_file_ptr;


// PROTOTYPES ///////////////////////

HWND InitWindows(HINSTANCE , HINSTANCE ,int ,HWND);		// Initialise Windows
int DD_Init(HWND hwnd);									// Initialise DirectX
int DD_Shutdown();										// Terminate DirectX
void RenderView();										// Render a Screen
int LoadBitmap(char *);									// Load a bitmap file
void LoadDEM(char *);									// Load a DEM file
int SaveScreen();										// Save a screenshot


// Main windows callback function

LRESULT CALLBACK WindowFunc(HWND, UINT, WPARAM, LPARAM) ;


// GLOBAL VARIABLES //////////////////////////////



// DirectX global variables

LPDIRECTDRAW pdd = NULL;
LPDIRECTDRAW4 lpdd = NULL;
LPDIRECTDRAWSURFACE4 lpddsprimary = NULL;
LPDIRECTDRAWSURFACE4 lpddsback = NULL;
LPDIRECTDRAWPALETTE lpddpal = NULL;
DDSURFACEDESC2 ddsd;
DDSCAPS2 ddscaps;
HRESULT ddrval;



int MAP_HEIGHT;								// Height Dimension of Map
int MAP_WIDTH;								// Width Dimension of Map		
int MAX_STEPS;								// Maximum Ray cast length
int SCREEN_HEIGHT;							// Screen resolution height
int SCREEN_WIDTH;							// Screen resolution width

int HALF_SCREEN_WIDTH;						// Half screen resolution width (quicker)
int ANGLE_RANGE;							// Range of values representing 
											// 360 degree rotation

int		**heights;							// Main heights array
UCHAR	**colours;							// Main colours array

PALETTEENTRY colour_palette[256];			// Colour palette array

UCHAR *video_buffer = NULL;					// Pointer to our display memory

int		x_pos = START_MAP_X,
		y_pos = START_MAP_Y, 
		z_pos = START_MAP_Z ;				// Start position of a ray

int pitch;									// Pitch value (see Section 3)

double toRadians;							// Convert a value to radians

double YViewingAngle = 0.0;					// Current viewing direction 
											// between 0 and ANGLE_RANGE

double inc = 1.0 / (double) VIEW_DISTANCE;	// Increment of ray position


// Assign values to screen resolution tags

enum { _320x240, _640x480, _800x600, _1024x768, _1280x1024 };


// Windows variables

HWND main_window_handle = NULL;
BOOL g_bActive = FALSE;
char szWinName[] = "Landscape Generator v1.7"; 

