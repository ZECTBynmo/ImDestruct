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
	SVGImage DestructImage( PNGImage& image, QRect rectSearchArea, SVGImage& destructedImage= SVGImage() );
	
private:
	//! Returns whether the given pixels are identical in RGBA
	bool isPixelIdentical( png_byte* pFirstPixel, png_byte* pSecondPixel );
	
	//! Finds a list of horizontal dividers
	void findHorizontalDividers( PNGImage& image, QRect rectSearchArea, vector<QRect>& rectDrawables, vector<QRect>& rectUndrawables );
	//! Finds a list of vertical dividers
	void findVerticalDividers( PNGImage& image, QRect rectSearchArea, vector<QRect>& rectDrawables, vector<QRect>& rectUndrawables );
	
	//! Separates an image up into it's component parts
	//vector<PNGImage>& separateImage( PNGImage& originalImage, SVGImage& svgImage, vector<uint>& uHorizontalDivisors, vector<uint>& uVerticalDivisors );
}; // end class ImageDestructor


