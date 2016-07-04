
#include <math.h>

int heights[512][512];

int x_pos = 256, y_pos = 256, z_pos = 700,
	z_delta = -360 / 10,
	j = 0;
	




void GameMain() {

int x,y,z,dx,dy,dz,scale,row,height,fov;
unsigned char *pix_ptr;

video_buffer += (640 * (480-1)); 

fov = 360;

for (int col = 0; col < 479; col++) {

	x = x_pos; y = y_pos; z = z_pos;

	dx = cos(fov); dy = sin(fov);

	dz = z_delta;

	scale = row = 0;

	pix_ptr = dest_buffer;

	for (int curr_step = 0; curr_step < 200; curr_step++)
		{
		
		x = x + dx; y = y + dy;

		height = heights[x][y];
		
		while (height > z) {

			pix_ptr = color;

			dz = dz + (1/10);

			z = z + scale;

			pix_ptr-=480;

			if (++row >= 640) 									
				break;
								
		

		} 

		
		x+=dx;
		y+=dy;
		z+=dz;
		
		scale+=(1/10);

		} 

	
	video_buffer++;


	} // end for curr_col
fov--;
} // end Render_Terrain


}