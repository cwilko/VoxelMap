

int heights[MAP_HEIGHT][MAP_WIDTH];
UCHAR colours[MAP_HEIGHT][MAP_WIDTH];

int x_pos = START_MAP_X, y_pos = START_MAP_Y, z_pos = 2000;
double z_delta = -360.0 / 10.0, fov = 0.0;
	
	
int flag = 0, poo = 5;
double Z = 0.0, X = 0.0, trash = 0.0;



void GameMain() {

	double x,y,z,dx,dy,dz,scale,y_angle;
	int row,height,colour;
	unsigned char *pix_ptr;
	


	if (KEY_DOWN(VK_UP)) {
	x_pos+=16;
	if(x_pos>=5000) x_pos = 5000;

	}

	if (KEY_DOWN(VK_DOWN)) {
	x_pos-=16;
	if(x_pos<=0) x_pos = 0;
	}

	if (KEY_DOWN(VK_LEFT)) {
	fov+=64;
	if(fov>=3840) fov = 0.0;
	}

	if (KEY_DOWN(VK_RIGHT)) {
	fov-=64;
	if(fov<=0) fov = 3840.0;
	}

	video_buffer += (640 * (480-1)); 

	y_angle = fov + 320.0;
	
	for (int col = 0; col < 640; col++) {
	
		x = x_pos; y = y_pos; z = z_pos;
	
		dx = cos(y_angle*PIE*2.0/3840.0) * 2;  
		dy = sin(y_angle*PIE*2.0/3840.0) * 2 ;


		dz = z_delta;
	
		scale = 0.0;
		row = 0;
	
		pix_ptr = video_buffer;
	
		for (int curr_step = 0; curr_step < 400; curr_step++)
			{
			 	
			
			colour = colours[((int)x)%5000][((int)y)%5000];

			if (x>5000 || x<0 || y>5000 || y<0) height = 0, colour = 0;
			else if ((height = heights[((int)x)%5000][((int)y)%5000] - poo) < 0) height = 0;

			
		
			
		
			while (height > z) {

				

				*pix_ptr = colour;

	
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
			if(x<0) x+=5000.0;
			y+=dy;
			z+=dz;
			
			scale+=(1.0/10.0);
		
		} 

	
		video_buffer++;		
		y_angle--;
	} // end Render_Terrain


}




int LoadBitmap(bitmap_file_ptr bitmap, char *filename)
{
// this function opens a bitmap file and loads the data into bitmap
// this function will only work with non-compressed 8 bit palettized images
// it uses file handles instead of streams just for a change, no reason

#define BITMAP_ID        0x4D42       // this is the universal id for a bitmap

int      file_handle,                 // the file handle
         index;                       // looping index

OFSTRUCT file_data;                   // the file data information

// open the file if it exists
if ((file_handle = OpenFile(filename,&file_data,OF_READ))==-1)
   return(0);
 
// now load the bitmap file header
_lread(file_handle, &bitmap->bitmapfileheader,sizeof(BITMAPFILEHEADER));

// test if this is a bitmap file
if (bitmap->bitmapfileheader.bfType!=BITMAP_ID)
   {
   // close the file
   _lclose(file_handle);
 
   // return error
   return(0);

   } // end if

// we know this is a bitmap, so read in all the sections

// load the bitmap file header
_lread(file_handle, &bitmap->bitmapinfoheader,sizeof(BITMAPINFOHEADER));

// now the palette
_lread(file_handle, &bitmap->palette,256*sizeof(PALETTEENTRY));


// finally the image data itself
_lseek(file_handle,-(int)(bitmap->bitmapinfoheader.biSizeImage),SEEK_END);



// now read it in
for (int i=0; i<5000; i++) {


		_lread(file_handle,colours[i],5000 * sizeof(UCHAR));
	
}


// close the file
_lclose(file_handle);

// return success
return(1);

} // end Load_Bitmap_File



void LoadDEM() {

	int profile, points, templow = 6000, maxElevations = 0;
	int width, height;
	double scale_x, scale_y;
	char ch = 0;
	
	ifstream fin("lassen.txt");

	fin >> width >> height;

	scale_x = (double) MAP_WIDTH / (double) width;
 	scale_y = (double) MAP_HEIGHT / (double) height;

	while (!fin.eof()) {

		fin >> poo;

		fin >> profile >> points;

		fin >> poo; 

		fin >> trash >> trash >> trash >> trash >> trash;

		
		
		for(int i = (profile * scale_y); i < ((profile + 1) * scale_y); i++) {
			for (int index = 0; index < points; index++) {
				fin >> poo;
				for(int j = (index * scale_x); j < ((index + 1) * scale_x); j++) {
					if(poo < templow) templow = poo;
					if(poo > maxElevations) maxElevations = poo;
					heights[i][j] = poo;
				}
			}
		}
	} 

	fin.close();
	poo =  templow;

}


