///////////////////////////////////////////////////////////
//
// PNGImage.h: Our PNG image struct
//
//////////////////////////////////////////////////////////////////////////////
#pragma once

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
	png_bytep* pRow;	//!< A pointer to the current row
};

