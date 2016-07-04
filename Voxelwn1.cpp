// VOXELWN1.CPP /////////////////////////////////////////////////////////////////
// by Andre' LaMothe
// Windows 95/DirectX version with 320x240 mode x page flipping

// INCLUDES ///////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN  // make sure certain headers are included correctly

#include <windows.h>         // include windows stuff
#include <windowsx.h>        
#include <mmsystem.h>        

#include <iostream.h>        // include C/C++ stuff
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <conio.h>
#include <math.h> 
#include <malloc.h> 
#include <string.h>
#include <io.h>
#include <fcntl.h> 
#include <signal.h>
#include <float.h>

#include <ddraw.h>          // include DirectDraw stuff

// DEFINES ////////////////////////////////////////////////////////////////////

#define WINDOW_CLASS_NAME "WINDOW_CLASS" // this is the name of the window class

// defines for screen parameters
// try 320x200, 320x240, they are way faster
#define SCREEN_WIDTH     320     // the width of the viewing surface
#define SCREEN_HEIGHT    240     // the height of the viewing surface
#define SCREEN_BPP       8       // the bits per pixel
#define SCREEN_COLORS    256     // the maximum number of colors

// defines for fixed point math
#define FIXP_SHIFT       12      // number of decimal places 20.12 	
#define FIXP_MUL         4096    // 2^12, used to convert reals

// defines for angles
#define PIE              ((double)3.14159265) // take a guess

#define ANGLE_360        (SCREEN_WIDTH * 360/60) // fov is 60 deg, so SCREEN_WIDTH * 360/60
#define ANGLE_180        (ANGLE_360/2)
#define ANGLE_120        (ANGLE_360/3)
#define ANGLE_90		 (ANGLE_360/4)
#define ANGLE_60         (ANGLE_360/6)
#define ANGLE_45         (ANGLE_360/8) 
#define ANGLE_30		 (ANGLE_360/12)
#define ANGLE_20         (ANGLE_360/18) 
#define ANGLE_15         (ANGLE_360/24)
#define ANGLE_10         (ANGLE_360/36)  
#define ANGLE_5          (ANGLE_360/72)
#define ANGLE_2          (ANGLE_360/180) 
#define ANGLE_1          (ANGLE_360/360) 
#define ANGLE_0          0
#define ANGLE_HALF_HFOV  ANGLE_30


// defines for height field
#define HFIELD_WIDTH      512	// width of height field data map
#define HFIELD_HEIGHT     512   // height of height field data map  
#define HFIELD_BIT_SHIFT  9     // log base 2 of 512
#define TERRAIN_SCALE_X2  3     // scaling factor for terrain

#define VIEWPLANE_DISTANCE (SCREEN_WIDTH/64)

#define MAX_ALTITUDE     1000   // maximum and minimum altitudes
#define MIN_ALTITUDE     50
#define MAX_SPEED        32     // maximum speed of camera

#define START_X_POS      256    // starting viewpoint position
#define START_Y_POS      256 
#define START_Z_POS      700

#define START_PITCH      80*(SCREEN_HEIGHT/240)    // starting angular heading
#define START_HEADING    ANGLE_90

#define MAX_STEPS        200    // number of steps to cast ray

// TYPES /////////////////////////////////////////////////////////////////////
typedef unsigned char  UCHAR;

// the bitmap file structure /////////////////////////////////////////////////
typedef struct bitmap_file_tag
        {
        BITMAPFILEHEADER bitmapfileheader;  // this contains the bitmapfile header
        BITMAPINFOHEADER bitmapinfoheader;  // this is all the info is
        PALETTEENTRY     palette[256];      // we will store the palette here
        UCHAR            *buffer;           // this is a pointer to the data
        } bitmap_file, *bitmap_file_ptr;

// MACROS /////////////////////////////////////////////////////////////////////

// these query the keyboard in real-time, WIN API stuff
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

// PROTOTYPES ////////////////////////////////////////////////////////////////

int DD_Init(HWND hwnd);
int DD_Shutdown(void);
int DD_Set_Pal_Entry(int index, int red, int green, int blue);
int DD_Load_Pal_RGB(LPPALETTEENTRY colors, int num_colors);
int Load_Bitmap_File(bitmap_file_ptr bitmap, char *filename);
int Unload_Bitmap_File(bitmap_file_ptr bitmap);
void Build_Tables(void);
void Game_Init(void);
void Game_Main(void);
void Game_Shutdown(void);

int Open_Error(char *filename);
int Close_Error(void);
int Write_Error(char *string);

// GLOBALS ////////////////////////////////////////////////////////////////////

LPDIRECTDRAW         lpdd         = NULL;		// dd object
LPDIRECTDRAWSURFACE  lpddsprimary = NULL;		// dd primary surface
LPDIRECTDRAWSURFACE  lpddsback    = NULL;       // dd back buffer surface
LPDIRECTDRAWPALETTE  lpddpal      = NULL;		// a pointer to the created dd palette
PALETTEENTRY         color_palette[256];		// holds the shadow palette entries
DDSURFACEDESC        ddsd;						// a direct draw surface description struct
DDSCAPS              ddscaps;					// a direct draw surface capabilities struct
HRESULT              ddrval;					// result back from dd calls
HWND                 main_window_handle = NULL; // used to store the window handle
char                 *command_line;             // ptr to winmain command line
UCHAR                *video_buffer      = NULL, // pointer to video ram
                     *double_buffer     = NULL, // pointer to double buffer
					 *height_map_ptr   = NULL,  // pointer to height field height data
					 *color_map_ptr    = NULL;  // pointer to height field color data

bitmap_file          height_bmp_file,           // holds the height data
                     color_bmp_file;            // holds the color data

int view_pos_x = START_X_POS,                   // view point x pos
    view_pos_y = START_Y_POS,                   // view point y pos
	view_pos_z = START_Z_POS,                   // view point z pos (altitude)

	view_ang_x = START_PITCH,                   // pitch 
	view_ang_y = START_HEADING,                 // heading, or yaw
	view_ang_z = 0,                             // roll, unused
	
	// this is very important, it is based on the fov and scaling
	// factors, it is the delta slope between two rays piercing
	// the viewplane that a single pixel from each other in the
	// same colum
	dslope = (int)(((double)1/(double)VIEWPLANE_DISTANCE)*FIXP_MUL), 

	cos_look[ANGLE_360],                        // trig lookup tables, really only
	sin_look[ANGLE_360];                        // need onem but 2 is easier

int       error_file_handle = -1;    // this is the file handle for errors
OFSTRUCT  error_file_data;           // this holds the file data for the
char      errbuffer[80];             // holds error strings

// INLINE FUNCTIONS ///////////////////////////////////////////////////////////

inline int COS_LOOK(int theta)
{
// this inline function returns the cosine of the sent angle taking into
// consideration angles > 360 and < than 0

if (theta < 0)
	return(cos_look[theta + ANGLE_360]);
else
if (theta >= ANGLE_360)
	return(cos_look[theta - ANGLE_360]);
else
	return(cos_look[theta]);

} // end COS_LOOK

///////////////////////////////////////////////////////////////////////////////

inline int SIN_LOOK(int theta)
{
// this inline function returns the sine of the sent angle taking into
// consideration angles > 360 and < than 0

if (theta < 0)
	return(sin_look[theta + ANGLE_360]);
else
if (theta >= ANGLE_360)
	return(sin_look[theta - ANGLE_360]);
else
	return(sin_look[theta]);

} // end SIN_LOOK

// DIRECT X FUNCTIONS /////////////////////////////////////////////////////////


// GENERAL FUNCTIONS //////////////////////////////////////////////////////////

void Build_Tables(void)
{
// this function builds all the lookup tables

for (int curr_angle=0; curr_angle < ANGLE_360; curr_angle++)
	{
	double angle_rad = 2*PIE*(double)curr_angle/(double)ANGLE_360;
	
	// compute sin and cos and convert to fixed point
	cos_look[curr_angle] = (int)(cos(angle_rad) * FIXP_MUL);
	sin_look[curr_angle] = (int)(sin(angle_rad) * FIXP_MUL);

	} // end for index

} // end Build_Tables

///////////////////////////////////////////////////////////////////////////////



void Render_Terrain(int vp_x,     
					int vp_y, 
					int vp_z, 
					int vp_ang_x, 
					int vp_ang_y, 
					int vp_ang_z, 
					UCHAR *dest_buffer)
{
// this function renders the terrain at the given position and orientation

int xr,                 // used to compute the point the ray intersects the
    yr,                 // the height data
    curr_column,        // current screen column being processed
    curr_step,          // current step ray is at
    raycast_ang,        // current angle of ray being cast
	dx,dy,dz,           // general deltas for ray to move from pt to pt
	curr_voxel_scale,   // current scaling factor to draw each voxel line
	column_height,      // height of the column intersected and being rendered
	curr_row,           // number of rows processed in current column
	x_ray,y_ray,z_ray,  // the position of the tip of the ray
	map_addr;           // temp var used to hold the addr of data bytes
		
UCHAR color,            // color of pixel being rendered
      *dest_column_ptr; // address screen pixel being rendered 
	  
// convert needed vars to fixed point
vp_x = (vp_x << FIXP_SHIFT);
vp_y = (vp_y << FIXP_SHIFT);
vp_z = (vp_z << FIXP_SHIFT);

// push down destination buffer to bottom of screen
dest_buffer += (SCREEN_WIDTH * (SCREEN_HEIGHT-1)); 

// compute starting angle, at current angle plus half field of view
raycast_ang = vp_ang_y + ANGLE_HALF_HFOV;

// cast a ray for each column of the screen
for (curr_column=0; curr_column < SCREEN_WIDTH-1; curr_column++)
	{
	// seed starting point for cast
	x_ray = vp_x;
	y_ray = vp_y;
	z_ray = vp_z;

	// compute deltas to project ray at, note the spherical cancelation factor
	dx = COS_LOOK(raycast_ang) << 1;
	dy = SIN_LOOK(raycast_ang) << 1;
	
	// dz is a bit complex, remember dz is the slope of the ray we are casting
	// therefore, we need to take into consideration the down angle, or
	// x axis angle, the more we are looking down the larger the intial dz
	// must be

	dz = dslope * (vp_ang_x - SCREEN_HEIGHT);
	
	// reset current voxel scale 
	curr_voxel_scale = 0;

	// reset row
	curr_row = 0;

	// get starting address of bottom of current video column 
	dest_column_ptr = dest_buffer;

	// enter into casting loop
	for (curr_step = 0; curr_step < MAX_STEPS; curr_step++)
		{
		// compute pixel in height map to process
		// note that the ray is converted back to an int
		// and it is clipped to to stay positive and in range
		xr = (x_ray  >> FIXP_SHIFT);
		yr = (y_ray  >> FIXP_SHIFT);

		xr = (xr & (HFIELD_WIDTH-1));
		yr = (yr & (HFIELD_HEIGHT-1));

		map_addr = (xr + (yr << HFIELD_BIT_SHIFT));

		// get current height in height map, note the conversion to fixed point
		// and the added multiplication factor used to scale the mountains
		column_height = (height_map_ptr[map_addr] << (FIXP_SHIFT+TERRAIN_SCALE_X2));
		
		// test if column height is greater than current voxel height for current step
		// from intial projection point
		if (column_height > z_ray)
			{
			// we know that we have intersected a voxel column, therefore we must
			// render it until we have drawn enough pixels on the display such that
			// thier projection would be correct for the height of this voxel column
			// or until we have reached the top of the screen

			// get the color for the voxel
			color = color_map_ptr[map_addr];

			// draw vertical column voxel
			while(1)
				{
				// draw a pixel
				*dest_column_ptr = color;

				// now we need to push the ray upward on z axis, so increment the slope
				dz+=dslope;

				// now translate the current z position of the ray by the current voxel
				// scale per unit
				z_ray+=curr_voxel_scale;

				// move up one video line
				dest_column_ptr-=SCREEN_WIDTH;

				// test if we are done with column
				if (++curr_row >= SCREEN_HEIGHT) 
					{
					// force exit of outer steping loop
					// chezzy, but better than GOTO!
					curr_step = MAX_STEPS;
					break;
								
					} // end if

				// test if we can break out of the loop
				if (z_ray > column_height) break;

				} // end while

			} // end if

		// update the position of the ray
		x_ray+=dx;
		y_ray+=dy;
		z_ray+=dz;

		// update the current voxel scale, remember each step out means the scale increases
		// by the delta scale
		curr_voxel_scale+=dslope;

		} // end for curr_step

	// advance video pointer to bottom of next column
	dest_buffer++;

	// advance to next angle
	raycast_ang--;	

	} // end for curr_col

} // end Render_Terrain



// HERE ARE OUR GAME CONSOLE FUNCTIONS ///////////////////////////////////////////////////////


void Game_Main(void)
{
// main control function like C/C++ MAIN()

// note the use of statics, since this function will be entered and exited
static int autopilot_on    = 1,   // state of autopilot
           autopilot_timer = 50, // counts time til heading change
		   autopilot_turn  = 0,   // heading direction
		   speed           = 8;   // current speed of view camera

// the word "auto" encoded as bits, 16 left if you want to add!
static unsigned int autobmp[] = {0x65770000,0x75250000,0x57270000}; 

// record starting time of frame
DWORD frame_start_time = GetTickCount();

// is user exiting?
if (KEY_DOWN(VK_ESCAPE))
	{
	PostMessage(main_window_handle,WM_DESTROY,0,0); 
	return;
	} // end if

// acquire dd pointer to back buffer ram, note it is always linear
memset(&ddsd,0,sizeof(ddsd));
ddsd.dwSize = sizeof(ddsd);
while (lpddsback->Lock(NULL,&ddsd,DDLOCK_SURFACEMEMORYPTR,NULL)!=DD_OK);
double_buffer = (UCHAR *)ddsd.lpSurface;

// first clear double buffer
memset(double_buffer,0, SCREEN_WIDTH*SCREEN_HEIGHT);

// change heading, note these are the only controls that
// disengage autopilot
if (KEY_DOWN(VK_LEFT)) 
	{
	// rotate CC
	view_ang_y+=ANGLE_5; 
	autopilot_on    = 0; 
	autopilot_timer = 0;
	} // end if
else
if (KEY_DOWN(VK_RIGHT))	
	{ 
	// rotate CW
	view_ang_y-=ANGLE_5;
	autopilot_on    = 0;  
	autopilot_timer = 0;
	} // end if

// change altitude
if (KEY_DOWN('A')) 
	view_pos_z+=8;
else
if (KEY_DOWN('Z')) 
	view_pos_z-=8;

// change speed
if (KEY_DOWN(VK_UP)) 
	speed+=2;
else
if (KEY_DOWN(VK_DOWN)) 
	speed-=2;

// change pitch 
if (KEY_DOWN(VK_PRIOR)) 
	view_ang_x+=2;
else
if (KEY_DOWN(VK_NEXT)) 
	view_ang_x-=2;

// stop the camera totally
if (KEY_DOWN(VK_SPACE)) 
	{
	autopilot_on    = 0;
	autopilot_timer = 0;
	speed           = 0;
	} // end if


// move viewpoint
view_pos_x += ((speed*COS_LOOK(view_ang_y)) >> FIXP_SHIFT);
view_pos_y += ((speed*SIN_LOOK(view_ang_y)) >> FIXP_SHIFT);

// keep viewpoint in playfield
if (view_pos_x >=HFIELD_WIDTH) 
	view_pos_x = 0;
else
if (view_pos_x < 0)	
	view_pos_x = HFIELD_WIDTH-1;

if (view_pos_y >=HFIELD_HEIGHT)	
	view_pos_y = 0;
else
if (view_pos_y < 0)	
	view_pos_y = HFIELD_HEIGHT-1;

// test heading
if (view_ang_y >= ANGLE_360) 
	view_ang_y-=ANGLE_360;
else
if (view_ang_y <  ANGLE_0) 
	view_ang_y+=ANGLE_360;

// test speed
if (speed > MAX_SPEED) 
	speed = MAX_SPEED;
else
if (speed < -MAX_SPEED) 
	speed = -MAX_SPEED;

// test altitude
if ((view_pos_z+=8) > MAX_ALTITUDE) 
	view_pos_z = MAX_ALTITUDE;
else
if ((view_pos_z-=8) < MIN_ALTITUDE) 
	view_pos_z = MIN_ALTITUDE;

// draw the next frame of terrain to double buffer
Render_Terrain(view_pos_x, 
			   view_pos_y, 
			   view_pos_z, 
			   view_ang_x, 
			   view_ang_y,
			   view_ang_z,
			   double_buffer);

// release dd pointer to back buffer
lpddsback->Unlock(ddsd.lpSurface);

// flip back buffer to primary buffer
lpddsprimary->Flip(NULL,DDFLIP_WAIT);

// lock to 30 fps, this is totally inaccurate!
while((GetTickCount() - frame_start_time) <= 30);

} // end Game_Main
