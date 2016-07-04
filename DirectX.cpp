
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

	if(lpdd->SetCooperativeLevel(hwnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN | DDSCL_FPUSETUP) != DD_OK )
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

	int temp;

	for(index = 0; index < 256; index++ ) 
	{
		
		
			// Swap blue and red components (Bitmap file stores in incorrect order)

			temp = colour_palette[index].peRed;
			colour_palette[index].peRed = colour_palette[index].peBlue;
			colour_palette[index].peBlue = temp;
//			colour_palette[index].peGreen = index;
		
			colour_palette[index].peFlags = PC_NOCOLLAPSE;
				
	}

	if(lpdd->CreatePalette((DDPCAPS_8BIT | DDPCAPS_INITIALIZE | DDPCAPS_ALLOW256 ),colour_palette,&lpddpal,NULL)!=DD_OK)
	{
		DD_Shutdown();
		return 0;
	}

	if( lpddsprimary->SetPalette(lpddpal) != DD_OK ) 
	{	
		DD_Shutdown();
		return 0;
	}

	for(index = 0; index < 256; index++ ) 
	{
		
		
			// Swap blue and red components (Bitmap file stores in incorrect order)

			temp = colour_palette[index].peRed;
			colour_palette[index].peRed = colour_palette[index].peBlue;
			colour_palette[index].peBlue = temp;
		
			colour_palette[index].peFlags = PC_EXPLICIT;
				
	}
	

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





