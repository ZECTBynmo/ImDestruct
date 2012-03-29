///////////////////////////////////////////////////////////
//
// ImageDestructor.cpp: Attempts to convert the majority of an image file to a vector format
// 
//////////////////////////////////////////////////////////////////////////////
// DevStudio!class ImageDestructor
#include "ImageDestructor.h"
#include "qglobal.h"

namespace {
	enum {
		RED,
		GREEN,
		BLUE,
		ALPHA,
		
		NUM_PIXEL_ATTRIBUTES
	};
}

//////////////////////////////////////////////////////////////////////////////
/*! Initialize */
ImageDestructor::ImageDestructor() {

} // end ImageDestructor::ImageDestructor()


//////////////////////////////////////////////////////////////////////////////
/*! Break the image into the smallest parts possible */
SVGImage ImageDestructor::DestructImage( PNGImage& image ) {
	SVGImage destructedImage;

	// Loop through every pixel
	for( uint yLoc=0; yLoc<image.height; yLoc++ ) {
		png_byte* row= image.pRow[yLoc];
		for ( uint xLoc=0; xLoc<image.width; xLoc++) {
			png_byte* pPixel= &(row[xLoc*4]);
			
			
		} // end for each x
	} // end for each y
	
	return destructedImage;
} // end ImageDestructor::DestructImage()


//////////////////////////////////////////////////////////////////////////////
/*! Returns whether the given pixels are identical in RGBA */
bool ImageDestructor::isPixelIdentical( png_byte* pFirstPixel, png_byte* pSecondPixel ) {
	bool bIdentical= true;
	
	for( uint iAttribute=0; iAttribute<NUM_PIXEL_ATTRIBUTES; ++iAttribute ) {
		if( pFirstPixel[iAttribute] != pSecondPixel[iAttribute] )
			bIdentical= false;
	}

	return bIdentical;
} // end ImageDestructor::isPixelIdentical()
