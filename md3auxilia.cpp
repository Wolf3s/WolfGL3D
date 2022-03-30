//#pragma comment(lib, "jpeg.lib")
//#pragma comment(lib, "libpng.lib")
//#pragma comment(lib, "nv_util.lib")

#include "md3auxilia.h"

#include <GL\gl.h>
#include <gl\glu.h>										// Header File For The GLu32 Library
#include <GL\glaux.h>
#include "nVidia GL\glext.h"
#include "png\array_texture.h"
//#include "png\data_path.h"
#include "png\nv_png.h"
#include <string.h>

//#pragma comment(lib, "libpng.lib")
//using namespace glh;

//    read_png_rgba("rgb_gloss_map.png", decal_img);
//    make_rgba_texture( decal_img, true );

//void make_rgba_texture(const array2<vec4ub> & rgb, bool mipmapped)
//{
//	if(mipmapped)
//		gluBuild2DMipmaps(array_texture_target, GL_RGBA8, rgb.get_width(), rgb.get_height(), GL_RGBA,
//						  GL_UNSIGNED_BYTE, (const void *)rgb.get_pointer());
//	else
//		glTexImage2D(array_texture_target, 0, GL_RGBA, rgb.get_width(), rgb.get_height(), 0,
//					 GL_RGBA, GL_UNSIGNED_BYTE, (const void *)rgb.get_pointer());
//}


//tex_object_2D decaltex;
	// Load the decal texture.
//    decaltex.bind();
//    array2<vec3ub> decal_img;
//    read_png_rgb("rgb_gloss_map.png", decal_img);
//    make_rgb_texture( decal_img, true );
//    decaltex.parameter( GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
//    decaltex.parameter( GL_TEXTURE_MAG_FILTER, GL_LINEAR );

//void make_rgba_texture(const glh::array2<glh::vec3ub> & rgb,
//                     const glh::array2<unsigned char> & a,
//					   bool mipmapped = false);

///////////////////////////////// DECODE JPG \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This decodes the jpeg and fills in the tImageJPG structure
/////
///////////////////////////////// DECODE JPG \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void DecodeJPG(jpeg_decompress_struct* cinfo, tImageJPG *pImageData)
{
	// Read in the header of the jpeg file
	jpeg_read_header(cinfo, TRUE);
	
	// Start to decompress the jpeg file with our compression info
	jpeg_start_decompress(cinfo);

	// Get the image dimensions and row span to read in the pixel data
	pImageData->rowSpan = cinfo->image_width * cinfo->num_components;
	pImageData->sizeX   = cinfo->image_width;
	pImageData->sizeY   = cinfo->image_height;
	
	// Allocate memory for the pixel buffer
	pImageData->data = new unsigned char[pImageData->rowSpan * pImageData->sizeY];
		
	// Here we use the library's state variable cinfo.output_scanline as the
	// loop counter, so that we don't have to keep track ourselves.
	
	// Create an array of row pointers
	unsigned char** rowPtr = new unsigned char*[pImageData->sizeY];
	for (int i = 0; i < pImageData->sizeY; i++)
		rowPtr[i] = &(pImageData->data[i*pImageData->rowSpan]);

	// Now comes the juice of our work, here we extract all the pixel data
	int rowsRead = 0;
	while (cinfo->output_scanline < cinfo->output_height) 
	{
		// Read in the current row of pixels and increase the rowsRead count
		rowsRead += jpeg_read_scanlines(cinfo, &rowPtr[rowsRead], cinfo->output_height - rowsRead);
	}
	
	// Delete the temporary row pointers
	delete [] rowPtr;

	// Finish decompressing the data
	jpeg_finish_decompress(cinfo);
}


///////////////////////////////// LOAD JPG \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This loads the JPG file and returns it's data in a tImageJPG struct
/////
///////////////////////////////// LOAD JPG \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

tImageJPG *LoadJPG(const char *filename)
{
	struct jpeg_decompress_struct cinfo;
	tImageJPG *pImageData = NULL;
	FILE *pFile;
	
	// This is the only function you should care about.  You don't need to
	// really know what all of this does (since you can't cause it's a library!) :)
	// Just know that you need to pass in the jpeg file name, and get a pointer
	// to a tImageJPG structure which contains the width, height and pixel data.
	// Be sure to free the data after you are done with it, just like a bitmap.
	
	// Open a file pointer to the jpeg file and check if it was found and opened 
	if((pFile = fopen(filename, "rb")) == NULL) 
	{
		LPSTR error="";
		sprintf(error, "Unable to load JPG File: %s", filename);
		// Display an error message saying the file was not found, then return NULL
		MessageBox(0, error, "Error", MB_OK);
		return NULL;
	}
	
	// Create an error handler
	jpeg_error_mgr jerr;

	// Have our compression info object point to the error handler address
	cinfo.err = jpeg_std_error(&jerr);
	
	// Initialize the decompression object
	jpeg_create_decompress(&cinfo);
	
	// Specify the data source (Our file pointer)	
	jpeg_stdio_src(&cinfo, pFile);
	
	// Allocate the structure that will hold our eventual jpeg data (must free it!)
	pImageData = (tImageJPG*)malloc(sizeof(tImageJPG));

	// Decode the jpeg file and fill in the image data structure to pass back
	DecodeJPG(&cinfo, pImageData);
	
	// This releases all the stored memory for reading and decoding the jpeg
	jpeg_destroy_decompress(&cinfo);
	
	// Close the file pointer that opened the file
	fclose(pFile);

	// Return the jpeg data (remember, you must free this data after you are done)
	return pImageData;
}














///////////////////////////////// CREATE TEXTURE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This creates a texture in OpenGL that we can texture map
/////
///////////////////////////////// CREATE TEXTURE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

extern "C" void CreateTexture(UINT &texture, LPSTR strFileName)//modfied bu Idot
{

	if(!strFileName)									// Return from the function if no file name was passed in
		return;
	
	/*if (strncmp(".jpg", strFileName, 4) == 0){
         //return true;
		
		}*/

/*	if (strncmpi(".png", strFileName, 4) == 0)){
         //return true;

      }*/
	int len=strlen(strFileName);
	if (((strFileName[len-4]=='.')&&(strFileName[len-3]=='p')&&
		(strFileName[len-2]=='n')&&(strFileName[len-1]=='g'))||
		((strFileName[len-4]=='.')&&(strFileName[len-3]=='P')&&
		(strFileName[len-2]=='N')&&(strFileName[len-1]=='G'))){//try PNG
		//MessageBox(0, strFileName, "Try PNG", MB_OK);
		glh::array2<glh::vec4ub> tex_img;
		// Generate a texture with the associative texture ID stored in the array
		glGenTextures(1, &texture);

		// Bind the texture to the texture arrays index and init the texture
		glBindTexture(GL_TEXTURE_2D, texture);

		read_png_rgba(strFileName, tex_img);
		make_rgba_texture( tex_img, true );

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		return;
		}

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *	
	
	tImageJPG *pImage = LoadJPG(strFileName);			// Load the image and store the data

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *
	

	if(pImage == NULL)									// If we can't load the file, quit!
		exit(0);
	// Generate a texture with the associative texture ID stored in the array
	glGenTextures(1, &texture);

	// Bind the texture to the texture arrays index and init the texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// Build Mipmaps (builds different versions of the picture for distances - looks better)
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pImage->sizeX, pImage->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pImage->data);

	// Lastly, we need to tell OpenGL the quality of our texture map.  GL_LINEAR_MIPMAP_LINEAR
	// is the smoothest.  GL_LINEAR_MIPMAP_NEAREST is faster than GL_LINEAR_MIPMAP_LINEAR, 
	// but looks blochy and pixilated.  Good for slower computers though. 
	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);	


	// Now we need to free the image data that we loaded since OpenGL stored it as a texture
	if (pImage){// If we loaded the image
		if (pImage->data){// If there is texture data	
		free(pImage->data);// Free the texture data, we don't need it anymore
		}free(pImage);}// Free the image structure

	/*
	AUX_RGBImageRec *pBitmap = NULL;
	
	if(!strFileName)									// Return from the function if no file name was passed in
		return;

	pBitmap = auxDIBImageLoad(strFileName);				// Load the bitmap and store the data
	
	if(pBitmap == NULL)									// If we can't load the file, quit!
		exit(0);

	// Generate a texture with the associated texture variable
	glGenTextures(1, &texture);

	// This sets the alignment requirements for the start of each pixel row in memory.
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

	// Bind the texture to the texture variable passed in
	glBindTexture(GL_TEXTURE_2D, texture);

	// Build Mipmaps (builds different versions of the picture for distances - looks better)
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->sizeX, pBitmap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap->data);

	// Lastly, we need to tell OpenGL the quality of our texture map.  GL_LINEAR_MIPMAP_LINEAR
	// is the smoothest.  GL_LINEAR_MIPMAP_NEAREST is faster than GL_LINEAR_MIPMAP_LINEAR, 
	// but looks blochy and pixilated.  Good for slower computers though.  
		
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

	// Now we need to free the bitmap data that we loaded since openGL stored it as a texture

	if (pBitmap)										// If we loaded the bitmap
	{
		if (pBitmap->data)								// If there is texture data
		{
			free(pBitmap->data);						// Free the texture data, we don't need it anymore
		}

		free(pBitmap);									// Free the bitmap structure
	}*/
}


