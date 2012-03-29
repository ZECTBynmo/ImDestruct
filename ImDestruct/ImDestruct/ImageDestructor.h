///////////////////////////////////////////////////////////
//
// ImageDestructor.h: Attempts to convert the majority of an image file to a vector format
//
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "PNGImage.h"
#include "SVGImage.h"

//////////////////////////////////////////////////////////////////////////
//! Attempts to convert the majority of an image file and convert it to a vector format
class ImageDestructor {
public:
	ImageDestructor();	//!< Initialize
	
	//! Break the image into the smallest parts possible
	SVGImage DestructImage( PNGImage& image );
	
private:
	//! Returns whether the given pixels are identical in RGBA
	bool isPixelIdentical( png_byte* pFirstPixel, png_byte* pSecondPixel );
}; // end class ImageDestructor


