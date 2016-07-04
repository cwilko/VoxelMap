
// WinMain.cpp
//
// The Main thread of execution
//
// This calls all the other components of the system in a continuous loop
//


// Include all the other system files

#include "Init.cpp"
#include "InitWindows.cpp"
#include "DirectX.cpp"
// #include "ScreenSave.cpp"		// For Screenshots
#include "Version7.cpp"


// MAIN windows function

int WINAPI WinMain(HINSTANCE hThisInst, HINSTANCE hPrevInst, LPSTR lpszArgs, int nWinMode)
{

	HWND hwnd;
	MSG msg;
	HDC hdc;
	char buffer[20];

	// Un-comment these variables if a frame rate is to be calculated

	// clock_t start, finish;
	// double  duration;


	// Obtain paramaters from command line arguments

	char height_file[40]="";
	char colour_file[40]="";
	int resolution;

	sscanf(lpszArgs, "%s %s %d %d %d %d", 
		colour_file, height_file, &MAP_HEIGHT, &MAP_WIDTH, &MAX_STEPS, &resolution);

	
	// Set screen resolution based on command line argument

	switch(resolution) {
	case _320x240:
		SCREEN_WIDTH	= 320;
		SCREEN_HEIGHT	= 240;
		break;
	case _640x480:
		SCREEN_WIDTH	= 640;
		SCREEN_HEIGHT	= 480;
		break;
	case _800x600:
		SCREEN_WIDTH	= 800;
		SCREEN_HEIGHT	= 600;
		break;
	case _1024x768:
		SCREEN_WIDTH	= 1024;
		SCREEN_HEIGHT	= 768;
		break;
	case _1280x1024:
		SCREEN_WIDTH	= 1280;
		SCREEN_HEIGHT	= 1024;
		break;
	default:
		return 0;
	}



	// Check Map Size argument values

	if (!MAP_HEIGHT || !MAP_WIDTH) return 0;


	// Allocate Memory for Height and Colour Maps

	heights = new int*[MAP_HEIGHT];
	colours = new UCHAR*[MAP_HEIGHT];
	
	for (int index = 0; index < MAP_HEIGHT; index++ ) 
		heights[index] = new int[MAP_WIDTH],
		colours[index] = new UCHAR[MAP_WIDTH];

	
	// Set initial map heights to zero

	for ( int i = 0; i < MAP_HEIGHT; i++) 
		for ( int j = 0; j < MAP_WIDTH; j++) 
			heights[i][j] = 0;

	
	// Load in Colour Map
	
	if(strlen(colour_file)==0) 
		return 0;					
	else
		LoadBitmap(colour_file);

	
	// Load in Height Map
	
	if(strlen(height_file)==0)
		return 0;
	else
		LoadDEM(height_file);


	// Set up initial viewing paramters

	HALF_SCREEN_WIDTH = SCREEN_WIDTH / 2;
	ANGLE_RANGE = (SCREEN_WIDTH * 360 / FOV );
	pitch = -(SCREEN_HEIGHT / 2);
	toRadians = 2.0 * PIE / (double) ANGLE_RANGE;
	

	// Initialise Application Window

	if ( !(hwnd = InitWindows(hThisInst, hPrevInst, nWinMode, hwnd)) ) return 0;



	// Initialise DirectDraw

	if (!DD_Init(hwnd))
	{
		DestroyWindow(hwnd);
		return 0;
	}

	
	// Enter main loop
	// 1. Deal with windows events
	// 2. Do asynchronous processing

	while (1) 
	{

		// Check windows messages

		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if(msg.message == WM_QUIT) { DD_Shutdown(); break; }
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} 
		else 
		{	
		
			// Main Routines


			// Obtain pointer to display buffer

			memset(&ddsd,0,sizeof(ddsd));
			ddsd.dwSize = sizeof(ddsd);
			
			while ( lpddsback->Lock(NULL,&ddsd,DDLOCK_SURFACEMEMORYPTR,NULL) != DD_OK);

			video_buffer = (UCHAR *) ddsd.lpSurface;

			// Clear screen


			memset(video_buffer,250,SCREEN_WIDTH*SCREEN_HEIGHT);


			
			// start = clock();

			// Render current view of landscape

			RenderView();

			// finish = clock();

			
	
			/*
				Un-comment this section if screen shots are wanted
				To save a screenshot press F12

			if (KEY_DOWN(VK_F12)) {
				SaveScreen();
			}

			*/

	
			
			// Release pointer to display memory

			lpddsback->Unlock(NULL);
			

			// Flip back surface to front
			
			while (TRUE)
               {
                   ddrval = lpddsprimary->Flip(NULL, 0);
                   if (ddrval == DD_OK)
					  
					   
                       break;
                   if (ddrval == DDERR_SURFACELOST)
                   {
                       ddrval = lpddsprimary->Restore();
                       if (ddrval != DD_OK)
                           break;
                   }
                   if (ddrval != DDERR_WASSTILLDRAWING)
                       break; 
			} 
			

			// Check to see if user pressed Escape
            
			if (KEY_DOWN(VK_ESCAPE)) {
				
				// Shut down DirectDraw

				DD_Shutdown();		
				
				// Send Quit event to Windows event handler

				PostMessage(main_window_handle,WM_CLOSE,0,0);
			}
		} 

		// Un-comment these for frame rates
		// buffer contains the current frame rate
		// duration = (double) (finish - start);	
		// sprintf(buffer,"%2.4f", (1000.0 / (float)(finish - start)));

	}

	// We should never get here

	return(msg.wParam);

}


// Message Function

LRESULT CALLBACK WindowFunc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	// Identify the event

	switch(message) {
		case WM_ACTIVATE:
            g_bActive = wParam;
            return 0L;
		case WM_DESTROY:

			// Shut down DirectDraw (in case not already done so)
			DD_Shutdown();

			// Terminate System
			PostQuitMessage(0);
			return 0L;
			
	}
	
	// Event not recognised - Return default message procedure

	return DefWindowProc(hwnd,message,wParam,lParam);
	
}

