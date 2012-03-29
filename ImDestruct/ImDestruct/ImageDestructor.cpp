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
	
	vector<uint> uVerticalDivisors,
				 uHorizontalDivisors;

	// Loop through every row and check for horizontal divisors
	for( uint yLoc=0; yLoc<image.height; yLoc++ ) {
		bool bHorizontalDivisor= true;
		png_byte* row= image.pRows[yLoc];
		for ( uint xLoc=1; xLoc<image.width; xLoc++) {
			png_byte* pPixel= &( row[xLoc*4] ),
					* pLastPixel= &( row[(xLoc-1)*4] );
					
			if( !isPixelIdentical(pPixel, pLastPixel) )
				bHorizontalDivisor= false;
		} // end for each x
		
		if( bHorizontalDivisor )
			uHorizontalDivisors.push_back( yLoc );
	} // end for each y
	
	// Loop through every column and check for veritcal divisors
	for( uint xLoc= 0; xLoc<image.width; ++xLoc ) {
		bool bVerticalDivisor= true;
		
		for( uint yLoc=1; yLoc<image.height; ++yLoc ) {
			png_byte* pPixel= &( image.pRows[yLoc][xLoc*4] ),
					* pLastPixel= &( image.pRows[yLoc-1][xLoc*4] );
		
			if( !isPixelIdentical(pPixel, pLastPixel) )
				bVerticalDivisor= false;
		} // end for each y
		
		if( bVerticalDivisor )
			uVerticalDivisors.push_back( xLoc );
	} // end for each x
	
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
