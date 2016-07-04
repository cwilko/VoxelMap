

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <ddraw.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

// DEFINES //////////////////////////

#define WINDOW_CLASS_NAME "WINDOW_CLASS"

#define SCREEN_HEIGHT 480
#define SCREEN_WIDTH 640
#define SCREEN_BPP 8
#define MAX_COLOURS 256

// MACROS ///////////////////////////

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1:0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0:1)

// PROTOTYPES ///////////////////////

int DD_Init(HWND hwnd);
int DD_Shutdown();
int Set_Pal_Entry(int,int,int,int);
void GameMain();

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
BOOL                        g_bActive = FALSE;


char szWinName[] = "MyWin"; 

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


 	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS ;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE ;
//	ddsd.dwBackBufferCount = 1;
  


	if (lpdd->CreateSurface(&ddsd,&lpddsprimary,NULL) != DD_OK)
	{
		DD_Shutdown();
		return 0;
	}


//	ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
//	if (lpddsprimary->GetAttachedSurface(&ddscaps,&lpddsback) != DD_OK)
//	{
//		DD_Shutdown();
//		return 0;
//	}

	memset(colour_palette,0,256*sizeof(PALETTEENTRY));

	for(index = 0; index < 256; index++ ) 
	{
		
		switch (index/64) {
		case 0 :
			colour_palette[index].peRed = index * 4;
			colour_palette[index].peBlue = index * 4;
			colour_palette[index].peGreen = index * 4;
			break;
		case 1 :
			colour_palette[index].peRed = (index%64)*4;
			break;
		case 2:
			colour_palette[index].peBlue = (index%64)*4;
			break;
		case 3:
			colour_palette[index].peGreen = (index%64) * 4; 
			break;
		}
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
			
			lpddsprimary->Lock(NULL,&ddsd,DDLOCK_SURFACEMEMORYPTR,NULL);
			video_buffer = (UCHAR *) ddsd.lpSurface;
			GameMain();
		
			lpddsprimary->Unlock(NULL);

/*			while (TRUE)
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
                       break; }
 */              
				
//			if (KEY_DOWN(VK_ESCAPE)) {
//			DD_Shutdown();
//				PostMessage(main_window_handle,WM_CLOSE,0,0);
//			}
		}
	}

	DD_Shutdown();

	return(msg.wParam);

}

// Message Function

LRESULT CALLBACK WindowFunc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch(message) {
	case WM_ACTIVATEAPP:
            g_bActive = (wParam == WA_ACTIVE) || (wParam == WA_CLICKACTIVE);
            return 0L;
	case WM_DESTROY:
		DD_Shutdown();
		PostQuitMessage(0);
		return 0L;
		break;
	}
	
	return DefWindowProc(hwnd,message,wParam,lParam);
	
}


void GameMain() 
{
	static int px = SCREEN_WIDTH / 2, py = SCREEN_HEIGHT / 2, colour = 0;

	if (KEY_DOWN(VK_F12)) PostMessage(main_window_handle,WM_CLOSE,0,0);
	if (KEY_DOWN(VK_RIGHT)) if(++px>SCREEN_WIDTH-8) px = 8;
	if (KEY_DOWN(VK_LEFT)) if(--px<8) px = SCREEN_WIDTH-8;
	if (KEY_DOWN(VK_UP)) if(--py<8) py = SCREEN_HEIGHT - 8;
	if (KEY_DOWN(VK_DOWN)) if (++py>SCREEN_HEIGHT - 8) py = 8;

	if (KEY_DOWN('C')) {
		if(++colour>=10)
			colour = 0;
		Sleep(100);
	}

	for(int pixels = 0;pixels<32; pixels++)
		video_buffer[(px-4+rand()%8)+(py-4+rand()%8)*SCREEN_WIDTH] = (colour*64)+rand()%64;

//	Sleep(1);

}