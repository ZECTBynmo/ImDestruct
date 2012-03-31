///////////////////////////////////////////////////////////
//
// PNGImage.h: Our PNG image struct
//
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "lpng159/png.h"
#include "qrect.h"

struct PNGImage {
	//! Constructor
	PNGImage() {
		bInitialized= false;
	} 

	bool bInitialized;	//!< Initialized flag, set when we're done loading an image

	int xLoc, yLoc,		//!< x and y locations
		width, height;	//!< Image dimensions

	png_byte colorType,	//!< Our color type
			 bitDepth;	//!< Our bit depth

	png_structp pPNG;	//!< A pointer to the actual PNG data

	png_infop pInfo;	//!< A pointer to the image's info
	int iNumPasses;		//!< Number of passes for image interlacing
	png_bytep* pRows;	//!< A pointer to the current row
	
	// Copy an area of the original image into a new image
	void CopyRectToPtr( QRect areaToCopy, PNGImage& target ) {
		target.pRows = (png_bytep*) malloc(sizeof(png_bytep) * target.height);
		for (target.yLoc=0; target.yLoc<target.height; target.yLoc++)
			target.pRows[target.yLoc] = (png_byte*) malloc(png_get_rowbytes(target.pPNG,target.pInfo));
	}
};

