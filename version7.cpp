
// Version7.cpp
//
// so called because it is the seventh version of the program
//
// Contains :
//		The rendering routine to generate an image
//		The bitmap loading routine
//		The DEM file loading routine



// The main rendering routine
// This is where the magic happens
//
// This function is called as many times a second as possible
//
// It generates a single image on the screen corresponding to the viewing parameters
//
// It also checks to see if the user has pressed any keys that may affect the 
// viewing parameters, if so they are updated

void RenderView() {

	double	x,					// Current ray position x co-ordinate
			y,					// Current ray position y co-ordinate
			z,					// Current ray position z co-ordinate
			dx,					// Amount to move the ray in the x direction
			dy,					// Amount to move the ray in the y direction
			m,					// Initial gradient of ray
			scale,				// Height which we increment ray
			y_angle;			// Current slice of voxels

	int		row,				// Current row in screen
			col,				// Current column in screen
			height,				// Height of a voxel
			colour;				// Colour of a voxel

	unsigned char *pix_ptr;		// Pointer to current pixel


	// The Input Control Module
	//
	// Checks various keys on the keyboard to see if pressed
	// 
	// If so, update viewing parameters

	
	// If UP arrow

	if (KEY_DOWN(VK_UP)) {

		// Move forwards

		x_pos+=64 * cos(YViewingAngle * toRadians);
		y_pos+=64 * sin(YViewingAngle * toRadians);

		// Check for map boundary

		if(x_pos>=MAP_HEIGHT) x_pos = MAP_HEIGHT;
		if(x_pos<=0) x_pos = 0;

		if(y_pos>=MAP_WIDTH) y_pos = MAP_WIDTH;
		if(y_pos<=0) y_pos = 0;

	}

	// If DOWN arrow

	if (KEY_DOWN(VK_DOWN)) {

		// Move backwards

		x_pos-=32 * cos(YViewingAngle * toRadians);
		y_pos-=32 * sin(YViewingAngle * toRadians);

		// Check for map boundary

		if(x_pos>=MAP_HEIGHT) x_pos = MAP_HEIGHT;
		if(x_pos<=0) x_pos = 0;

		if(y_pos>=MAP_WIDTH) y_pos = MAP_WIDTH;
		if(y_pos<=0) y_pos = 0;

	}

	// If LEFT arrow

	if (KEY_DOWN(VK_LEFT)) {

		// Turn Counter-clockwise

		YViewingAngle += 32;
		if(YViewingAngle>=ANGLE_RANGE) YViewingAngle = 0;

	}

	// If RIGHT arrow

	if (KEY_DOWN(VK_RIGHT)) {

		// Turn clockwise

		YViewingAngle -= 32;
		if(YViewingAngle<=0) YViewingAngle = ANGLE_RANGE;

	}


	// If PAGE UP key

	if (KEY_DOWN(VK_PRIOR)) {

		// decrease pitch
		// i.e. Look Down

		pitch-=4;
		if (pitch < -1000) pitch = -1000;
	}


	// If PAGE DOWN key

	if (KEY_DOWN(VK_NEXT)) {

		// increase pitch
		// i.e. Look Up

		pitch+=4;
		if (pitch > 200) pitch = 200;
	}

	// If F1 key

	if (KEY_DOWN(VK_F1)) {

		// Move Downwards

		z_pos-=1000;
		if (z_pos < 1000) z_pos = 1000;
	}

	// If F2 key

	if (KEY_DOWN(VK_F2)) {
		
		// Move Upwards

		z_pos+=1000;
		if (z_pos > 80000) z_pos = 80000;
	}


	// Initialise a pointer to the lower left of the screen

	video_buffer += (SCREEN_WIDTH * (SCREEN_HEIGHT - 1)); 

	// Set y_angle to current position plus half screen width
	// This is to sweep across through every column

	y_angle = (double) YViewingAngle + HALF_SCREEN_WIDTH;
	

	// Main algorithm
	// See section 3


	// Loop through every column in screen

	for (col = 0; col < SCREEN_WIDTH; col++) {

		// Initialise ray position to viewpoint

		x = x_pos; y = y_pos; z = z_pos;			
	
		// Calculate dx and dy (amount to move ray in x/y directions)

		dx = cos(y_angle * toRadians) ;  
		dy = sin(y_angle * toRadians) ;

		// Calculate initial slope of ray

		m = pitch * inc;
	
		// Reset row and height increment
		
		scale = 0.0;
		row = 0;
	
		// Reset current pixel to lower-left of screen

		pix_ptr = video_buffer;
	

		// Cast ray to Maximum ray cast length

		for (int i = 0; i < MAX_STEPS; i++)
			{
			 	
			x+=dx;				// Update ray position
			y+=dy;				
			z+=m;

			// Check if ray has gone outside map
			// If so we can leave the loop!

			if (x >= MAP_WIDTH || x<0 || y >= MAP_HEIGHT || y<0) {
				
				height = 0;
				colour = 0;
				i = MAX_STEPS;

			}
			else {				
				
				// Otherwise get height and colour of current voxel
			
				height = heights[(int)x][(int)y];
				colour = colours[(int)x][(int)y];
			}

			// Update ray height increment

			scale+=inc;
			
		
			// While the voxel is higher than the ray, move up it

			while (height > z) {				


				*pix_ptr = colour;						// Set pixel colour

				pix_ptr -= SCREEN_WIDTH;				// Move to next pixel

				m += inc;								// Change slope of ray
	
				z += scale;								// Move ray up voxel

				
				// If we have reached the top of the screen - exit loop
	
				if (++row >= SCREEN_HEIGHT) {
					i = MAX_STEPS;
					break;
				}
										
				
	
			}
	

		} // Time to move to next column

	
		// Move pointer to next column

		video_buffer++;		

		// Move to next landscape slice

		y_angle--;

	} // All screen processing finished


} // End of RenderView routine




// The LoadBitmap routine ///////////////////////////////////////////
//
// Loads the given filename into the colour map array
//


int LoadBitmap(char *filename)
{

	// This function will only work with non-compressed 8 bit palettized images
	// It uses file handles instead of streams

	#define BITMAP_ID        0x4D42				// this is the universal id for a bitmap

	int      file_handle;						// the file handle
	bitmap_file_ptr bitmap = new bitmap_file;	// the bitmap header structure


	OFSTRUCT file_data;							// the file data information

	
	// Open the file for reading if it exists

	if ((file_handle = OpenFile(filename,&file_data,OF_READ))==-1)
		return(0);
 
	
	// Now load the bitmap file header
	
	_lread(file_handle, &bitmap->bitmapfileheader,sizeof(BITMAPFILEHEADER));

	
	// Test if this is a bitmap file
	
	if (bitmap->bitmapfileheader.bfType!=BITMAP_ID) {
	
		// If not - close the file
		_lclose(file_handle);
 
		// return error
		return(0);

	} 

	// We know this is a bitmap, so read in all the sections
	
	// Load the bitmap file header
	
	_lread(file_handle, &bitmap->bitmapinfoheader,sizeof(BITMAPINFOHEADER));

	// Now the palette

	_lread(file_handle, colour_palette,256*sizeof(PALETTEENTRY));


	// Get the size of the image

	_lseek(file_handle,-(int)(bitmap->bitmapinfoheader.biSizeImage),SEEK_END);



	// Now read it in

	for (int i=0; i<MAP_HEIGHT; i++) {

		_lread(file_handle,colours[i],MAP_WIDTH * sizeof(UCHAR));
	
	}


	// Deallocate bitmap header
	delete bitmap;

	// Close the file
	_lclose(file_handle);

	// Return success
	return(1);

} // end LoadBitmap()



// LoadDEM() ////////////////////////////////////////////////////
//
// The DEM file loading routine
//
// Loads the given filename into the height map array
//


void LoadDEM(char* filename) {

		
	int value;							// A height value
	double trash;						// Trash variables in the file

	int profile,						// Current profile number
		numPoints,						// Number of samples on this profile
		templow = 10000,				// Lowest height so far
		temphigh = 0;					// Highest height so far

	int width, height;					// Width/Height of map

	double	scale_x,					// Scale factor to scale DEM in x direction
			scale_y;					// Scale factor to scale DEM in y direction
	
	//char ch = 0;
	
	// Open file

	ifstream fin(filename);

	// Get width and height of DEM

	fin >> width >> height;

	// Create temporary array of DEM values

	int *tempHeights = new int[width * height];

	// Initialise the values to zero

	for (int h = 0; h < width * height; h++) tempHeights[h] = 0;


	// Caculate scale factors based on size of bitmap file

	scale_x = (double) MAP_WIDTH / (double) width;
    scale_y = (double) MAP_HEIGHT / (double) height;


	// While not at end of file

	while (!fin.eof()) {

		fin >> value;							// Trash

		fin >> profile >> numPoints;			// Get profile number
												// Get number of samples

		fin >> value;								// Trash

		// Floating point trash

		fin >> trash >> trash >> trash >> trash >> trash;	

		
		// The DEM data

		for (int point = 0; point < numPoints; point++) {
			
				fin >> value;							// Get a height
				
				if(value < templow) templow = value;		// Is it the lowest?
				if(value > temphigh) temphigh = value;		// Is it the highest?

				// Put it in temporary array
				tempHeights[(profile - 1) * width + point] = value;
		}	
			
	} // End of file

	
	// Close file

	fin.close();


	// Scale data into height map
	//
	// VERY COMPLICATED
	// 
	// see section 3 on interpolation
	// .. basically..
	
	int i,j;

	// Place value of temporary array into height map at positions
	// depending on the scale factor.
	//
	// Interpolate horizontally between these values
	//
	// 
	// remembering to :
	//		Set all heights relative to lowest value
	//		Scale heights up as well

	
	
	for (i = 0; i < height ; i++ ) {
		j = 0;
		while ( tempHeights[i*width+j] != 0 && j < width ) {

			for(int index = 0; index < scale_x; index++) {
				
				heights[(int) (i * scale_y)][(int) (j*scale_x + index)] = 
					((tempHeights[i*width+j] + index * ((double) (tempHeights[i*width+j+1] - tempHeights[i*width+j]) / scale_x ))-(templow-1))*4 *(int)scale_x;
			
			}
		j++;
		}
	}

	int last = 0, count = 0, x = 0;
	double add,sofar;


	// then...
	// Interpolate vertically between these lines created

	for ( j = 0; j < MAP_WIDTH ; j++ ) {
		add = sofar = 0.0;
		last = 0;
		i = 0;
		x = 0;
	
		while ( x < MAP_HEIGHT - 1) {
			last = heights[i][j];
			x = i + 1;
			while( heights[x][j] == 0 && x < MAP_HEIGHT - 1) x++;
			if ( x < MAP_HEIGHT - 1) {
				sofar = 0.0;
				add = (double) (heights[x][j] - last) / (double) (x-i);
				while (i < x ) {
					sofar += add;
					heights[i++][j] = last + sofar;				
				}
			}
		}
	}
		
	
	// Deallocate temporary array

	delete [] tempHeights;
	

} // End of DEM loader


