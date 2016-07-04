

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <ddraw.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <stdio.h>



// DEFINES //////////////////////////

#define WINDOW_CLASS_NAME "WINDOW_CLASS"

#define SCREEN_HEIGHT 480
#define SCREEN_WIDTH 640
#define SCREEN_BPP 8
#define MAX_COLOURS 256

// MACROS ///////////////////////////

#define PIE              ((double)3.14159265)
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1:0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0:1)

// PROTOTYPES ///////////////////////

int DD_Init(HWND hwnd);
int DD_Shutdown();
int Set_Pal_Entry(int,int,int,int);




LRESULT CALLBACK WindowFunc(HWND, UINT, WPARAM, LPARAM) ;


// GLOBAL VARIABLES

LPDIRECTDRAW pdd = NULL;
LPDIRECTDRAW4 lpdd = NULL;
LPDIRECTDRAWSURFACE4 lpddsprimary = NULL;
LPDIRECTDRAWSURFACE4 lpddsback = NULL;
LPDIRECTDRAWPALETTE lpddpal = NULL;
PALETTEENTRY colour_palette[256];
DDSURFACEDESC2 ddsd;
DDSCAPS2 ddscaps;
HRESULT ddrval;
HWND main_window_handle = NULL;
UCHAR *video_buffer = NULL;
BOOL g_bActive = FALSE;


char szWinName[] = "MyWin"; 



int heights[512][512];
int colours[512][512];
int x_pos = 256, y_pos = 256, z_pos = 5000;
double z_delta = -360.0 / 10.0, fov = 0.0;
	
	
int flag = 0;
double Z = 0.0, X = 0.0;



void GameMain() {

	double x,y,z,dx,dy,dz,scale,y_angle;
	int row,height,colour;
	unsigned char *pix_ptr;
	


	if (KEY_DOWN(VK_UP)) {
	x_pos+=16;
	if(x_pos>=512) x_pos = 0;

	}

	if (KEY_DOWN(VK_DOWN)) {
	x_pos-=16;
	if(x_pos<=0) x_pos = 512;
	}

	if (KEY_DOWN(VK_LEFT)) {
	fov+=16.0;
	if(fov>=3840) fov = 0.0;
	}

	if (KEY_DOWN(VK_RIGHT)) {
	fov-=16.0;
	if(fov<=0) fov = 3840.0;
	}

	video_buffer += (640 * (480-1)); 

	y_angle = fov + 320.0;
	
	for (int col = 0; col < 640; col++) {
	
		x = x_pos; y = y_pos; z = z_pos;
	
		dx = cos(y_angle*PIE*2.0/3840.0) * 4;  
		dy = sin(y_angle*PIE*2.0/3840.0) * 4 ;


		dz = z_delta;
	
		scale = 0.0;
		row = 0;
	
		pix_ptr = video_buffer;
	
		for (int curr_step = 0; curr_step < 500; curr_step++)
			{
			 	
			
			

			height = heights[((int)x)%512][((int)y)%512];
			colour = colours[((int)x)%512][((int)y)%512];
			
		
			while (height > z) {

				

				*pix_ptr = (250 - colour);

	
				dz = dz + (1.0/10.0);
	
				z = z + scale;
	
				pix_ptr-=640;
	
				if (++row >= 480) {
					curr_step = 500;
					break;
				}
									
				
	
			} 
	
		//	if (Z) flag=1;
			x+=dx;
			if(x<0) x+=512.0;
			y+=dy;
			z+=dz;
			
			scale+=(1.0/10.0);
		
		} 

	
		video_buffer++;		
		y_angle--;
	} // end Render_Terrain


}

// DIRECTX FUNCTIONS

int DD_Init(HWND hwnd)
{

	int index;
	
	if(DirectDrawCreate(NULL,&pdd,NULL)!=DD_OK)
	{
		DD_Shutdown();
		return 0;
	}

	if(pdd->QueryInterface(IID_IDirectDraw4, (LPVOID *) & lpdd ) != DD_OK)
	{
		DD_Shutdown();
		return 0;
	}

	if(lpdd->SetCooperativeLevel(hwnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN ) != DD_OK )
	{
		DD_Shutdown();
		return 0;
	}


	if(lpdd->SetDisplayMode(SCREEN_WIDTH,SCREEN_HEIGHT,SCREEN_BPP,0,0) != DD_OK)
	{
		DD_Shutdown();
		return 0;
	}

	ZeroMemory(&ddsd, sizeof(ddsd));
 	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
	ddsd.dwBackBufferCount = 1;



	if (lpdd->CreateSurface(&ddsd,&lpddsprimary,NULL) != DD_OK)
	{
		DD_Shutdown();
		return 0;
	}


	ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
	if (lpddsprimary->GetAttachedSurface(&ddscaps,&lpddsback) != DD_OK)
	{
		DD_Shutdown();
		return 0;
	}

	memset(colour_palette,0,256*sizeof(PALETTEENTRY));

	for(index = 0; index < 256; index++ ) 
	{
		
		
			colour_palette[index].peRed = 0 ;
			colour_palette[index].peBlue = 0;
			colour_palette[index].peGreen = index;
		
		
		colour_palette[index].peFlags = PC_NOCOLLAPSE;
				
	}

	if(lpdd->CreatePalette((DDPCAPS_8BIT | DDPCAPS_INITIALIZE),colour_palette,&lpddpal,NULL)!=DD_OK)
	{
		DD_Shutdown();
		return 0;
	}

	lpddsprimary->SetPalette(lpddpal);
//	lpddsback->SetPalette(lpddpal);

	

	return 1;

} // end DD_Init

int DD_Shutdown () 
{
	
	
	if(lpdd) 
	{
	
		if (lpddsprimary) 
		{
			lpddsprimary->Release();
			lpddsprimary = NULL;
				
		}
		lpdd->Release();
		lpdd = NULL;

		return 1;
	}
	return 0;

} // end DD_Shutdown


int Set_Palette_Entry(int index, int red, int blue, int green)
{
	PALETTEENTRY colour;

	colour.peRed = (BYTE)red;
	colour.peGreen = (BYTE)green;
	colour.peBlue = (BYTE)blue;
	colour.peFlags = PC_NOCOLLAPSE;

	lpddpal->SetEntries(0,index,1,&colour);

	memcpy(&colour_palette[index], &colour, sizeof(PALETTEENTRY));

	return 1;
}



// MAIN windows function

int WINAPI WinMain(HINSTANCE hThisInst, HINSTANCE hPrevInst, LPSTR lpszArgs, int nWinMode)
{
	HWND hwnd;
	MSG msg;
	WNDCLASS wcl;
	HDC hdc;
	char buffer[] = "hello";

	wcl.hInstance = hThisInst;
	wcl.lpszClassName = WINDOW_CLASS_NAME;
	wcl.lpfnWndProc = WindowFunc;
	wcl.style = CS_HREDRAW | CS_VREDRAW;

	wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	//wcl.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
	wcl.hCursor = LoadCursor(NULL, IDC_ARROW);

	wcl.lpszMenuName = WINDOW_CLASS_NAME;
	wcl.cbClsExtra = 0;
	wcl.cbWndExtra = 0;	

	wcl.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
	

	if(!RegisterClass(&wcl)) return 0;

	if(!(hwnd = CreateWindowEx (
		WS_EX_TOPMOST,
		WINDOW_CLASS_NAME,
		"My First Proper Thing 2",
		WS_POPUP,
		0,
		0,
		GetSystemMetrics(SM_CXSCREEN),
		GetSystemMetrics(SM_CYSCREEN),
		NULL,
		NULL,
		hThisInst,
		NULL
		))) return 0;

	ShowWindow(hwnd, nWinMode);
	UpdateWindow(hwnd);
	SetFocus(hwnd);
	ShowCursor(0);
	main_window_handle = hwnd;

	if (!DD_Init(hwnd))
	{

		DestroyWindow(hwnd);
		return 0;
	}

int i,j,n;
	for ( i = -256; i < 256; i++) {
		n = 0;
		for ( j = -256; j < 256; j++) {

			heights[i+256][j+256] = (2*(i*i) + 3*(j*j) + 9) / 100 + 512;
			colours[i+256][j+256] = (int)((double)((double)heights[i+256][j+256] / 3800.0) * 200.0);
		
		
		}	


	}
/*	for ( i = 256; i < 512; i++) {
		for ( j = 0; j < 512; j++) {
			heights[i][j] = 50;
		}
	}
*/

	while (1) 
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if(msg.message == WM_QUIT) { DD_Shutdown(); break; }
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} 
		else 
		{	
			
			memset(&ddsd,0,sizeof(ddsd));
			ddsd.dwSize = sizeof(ddsd);


		



			
			while ( lpddsback->Lock(NULL,&ddsd,DDLOCK_SURFACEMEMORYPTR,NULL) != DD_OK);

				video_buffer = (UCHAR *) ddsd.lpSurface;

				memset(video_buffer,0,SCREEN_WIDTH*SCREEN_HEIGHT);


				GameMain();
			
				lpddsback->Unlock(NULL);
			
			

		sprintf(buffer,"%f",Z);
	

		if (lpddsback->GetDC(&hdc) == DD_OK) {
		 
			 SetBkColor(hdc, RGB(0, 0, 255));
			SetTextColor(hdc, RGB(255, 255, 0));
       
            TextOut(hdc, 0, 0, buffer, lstrlen(buffer));
           
		
        lpddsback->ReleaseDC(hdc);
		}

		
//Z+=10;
if (Z>3840.0) Z -=3840.0;

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
			
            
			if (KEY_DOWN(VK_ESCAPE)) {
				
			DD_Shutdown();
			
				PostMessage(main_window_handle,WM_CLOSE,0,0);
			}
		} 
	}

	DD_Shutdown();

	return(msg.wParam);

}


// Message Function

LRESULT CALLBACK WindowFunc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch(message) {
	case WM_ACTIVATE:
            g_bActive = wParam;
            return 0L;

	case WM_SETCURSOR:
            // Turn off the cursor since this is a full-screen app
            SetCursor(NULL);
            return TRUE;
	case WM_DESTROY:
		DD_Shutdown();
		PostQuitMessage(0);
		return 0L;
		break;
	}
	
	return DefWindowProc(hwnd,message,wParam,lParam);
	
}

