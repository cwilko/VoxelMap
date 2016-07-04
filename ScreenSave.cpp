
// ScreenSave.cpp
//
// This file can be included if we wish to take screenshots of the landscapes
//
/*
// To do this :
		First create a directory called anim in the 
	    same directory as the program, then create a bitmap image in this
		directory of the same resolution as the image you want to save.
		Call this image pic0.bmp.
		When F12 is pressed, pic0.bmp is overwritten with the landscape image

		Each time F12 is pressed a new file is created pic1.bmp, pic2.bmp,..etc
		Holding down F12 therefore makes animation frames

		I know this is awkward but it was not meant to be a feature of
		the system, only for creating images for the dissertation
		
*/


int frame = 0;

int SaveScreen() {

	// String buffers for the filenames
	
	char test[] = "anim\\pic";
	char format[] = ".bmp";

	char file[40];
	char file2[40];
	char file3[40];

	// Create the correct file paths and names in the buffers

	sprintf(file, "anim\\pic%d.bmp", frame++);
	sprintf(file3, "Work___Win32_Release\\anim\\pic%d.bmp", frame );
	sprintf(file2, "Work___Win32_Release\\%s", file);

	// Create a pointer to a screen buffer

	UCHAR *screen_buffer = NULL;

	// Assign the pointer to the display

	screen_buffer = (UCHAR *) ddsd.lpSurface;

	// Set the pixel position to the lower left corner

	screen_buffer += (SCREEN_WIDTH * (SCREEN_HEIGHT - 1));


	// Open pic0.bmp for writing

	#define BITMAP_ID        0x4D42				// this is the universal id for a bitmap

	int      file_handle;						// the file handle
	bitmap_file_ptr bitmap = new bitmap_file;	// the bitmap header structure


	OFSTRUCT file_data;							// the file data information

	// open the file if it exists

	if ((file_handle = OpenFile(file,&file_data,OF_READWRITE))==-1)
		return(0);
 
	// Now load the bitmap file header
	
	_lread(file_handle, &bitmap->bitmapfileheader,sizeof(BITMAPFILEHEADER));

	// Test if this is a bitmap file
	if (bitmap->bitmapfileheader.bfType!=BITMAP_ID) {
	
		// close the file
		_lclose(file_handle);
 
		// return error
		return(0);

	} // end if


	// Load the bitmap file header
	
	_lread(file_handle, &bitmap->bitmapinfoheader,sizeof(BITMAPINFOHEADER));


	// Write the palette to the file
	_lwrite(file_handle, (char*) colour_palette,256*sizeof(PALETTEENTRY));
	
	
	// Find the start of the image data
	_lseek(file_handle,-(int)(bitmap->bitmapinfoheader.biSizeImage),SEEK_END);


	// Overwrite the image data in the file

	for (int i=0; i<SCREEN_HEIGHT; i++) {

		_lwrite(file_handle,(char*) screen_buffer,SCREEN_WIDTH * sizeof(UCHAR));
		screen_buffer -= SCREEN_WIDTH;
	
	}


	// Deallocate bitmap header
	delete bitmap;

	// close the file
	_lclose(file_handle);

	// Copy the file to another ( for animation frames )

	CopyFile(file2, file3,TRUE);

	return 1;

}