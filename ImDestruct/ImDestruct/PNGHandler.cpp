///////////////////////////////////////////////////////////
//
// PNGHandler.cpp: Opens and Saves PNG files
// 
//////////////////////////////////////////////////////////////////////////////
// DevStudio!class PNGHandler
#include "PNGHandler.h"
#include "lpng159/png.h"
#include "qstring.h"
#include "PNGImage.h"

namespace {

}

//////////////////////////////////////////////////////////////////////////////
/*! Initialize */
PNGHandler::PNGHandler() {
	
} // end PNGHandler::PNGHandler()


//////////////////////////////////////////////////////////////////////////////
/*! Open a PNG file and returns its contents */
PNGImage PNGHandler::OpenPNGFile( QString strFileName ) {
	PNGImage image,			// Image to be returned if successful
			 emptyImage;	// Uninitialized image returned if reading fails

	char header[8];			// 8 is the maximum size that can be checked

	// open file and test for it being a png
	FILE* pFile= fopen( strFileName.toLatin1(), "rb" );
	if (!pFile)
		return emptyImage;
	fread(header, 1, 8, pFile);
// 	if (png_sig_cmp(header, 0, 8))
// 		return emptyImage;


	// initialize stuff
	image.pPNG = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!image.pPNG)
		return emptyImage;

	image.pInfo = png_create_info_struct(image.pPNG);
	if (!image.pInfo)
		return emptyImage;

	if (setjmp(png_jmpbuf(image.pPNG)))
		return emptyImage;

	png_init_io(image.pPNG, pFile);
	png_set_sig_bytes(image.pPNG, 8);

	png_read_info(image.pPNG, image.pInfo);

	image.width = png_get_image_width(image.pPNG, image.pInfo);
	image.height = png_get_image_height(image.pPNG, image.pInfo);
	image.colorType = png_get_color_type(image.pPNG, image.pInfo);
	image.bitDepth = png_get_bit_depth(image.pPNG, image.pInfo);

	image.iNumPasses = png_set_interlace_handling(image.pPNG);
	png_read_update_info(image.pPNG, image.pInfo);


	/* read file */
	if (setjmp(png_jmpbuf(image.pPNG)))
		return emptyImage;

	image.pRows = (png_bytep*) malloc(sizeof(png_bytep) * image.height);
	for (image.yLoc=0; image.yLoc<image.height; image.yLoc++)
		image.pRows[image.yLoc] = (png_byte*) malloc(png_get_rowbytes(image.pPNG,image.pInfo));

	png_read_image(image.pPNG, image.pRows);

	fclose(pFile);

	return image;
} // end PNGHandler::OpenPNGFile()


//////////////////////////////////////////////////////////////////////////////
/*! Writes a PNG image to file */
void PNGHandler::WritePNGFile( PNGImage image, QString strFileName ) {
	/* create file */
	FILE* pFile = fopen(strFileName.toLatin1(), "wb");
	if (!pFile)
		return;


	/* initialize stuff */
	image.pPNG = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!image.pPNG)
		return;

	image.pInfo = png_create_info_struct(image.pPNG);
	if (!image.pInfo)
		return;

	if (setjmp(png_jmpbuf(image.pPNG)))
		return;

	png_init_io(image.pPNG, pFile);


	/* write header */
	if (setjmp(png_jmpbuf(image.pPNG)))
		return;

	png_set_IHDR(image.pPNG, image.pInfo, image.width, image.height,
		image.bitDepth, image.colorType, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_write_info(image.pPNG, image.pInfo);


	/* write bytes */
	if (setjmp(png_jmpbuf(image.pPNG)))
		return;

	png_write_image(image.pPNG, image.pRows);


	/* end write */
	if (setjmp(png_jmpbuf(image.pPNG)))
		return;

	png_write_end(image.pPNG, NULL);

	/* cleanup heap allocation */
	for (image.yLoc=0; image.yLoc<image.height; image.yLoc++)
		free(image.pRows[image.yLoc]);
	free(image.pRows);

	fclose(pFile);
} // end PNGHandler::WritePNGFile()
