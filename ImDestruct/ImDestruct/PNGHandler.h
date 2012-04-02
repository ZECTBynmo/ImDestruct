///////////////////////////////////////////////////////////
//
// PNGHandler.h: Opens and Saves PNG files
//
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "PNGImage.h"

class QString;

//////////////////////////////////////////////////////////////////////////
//! Opens and Saves PNG files
class PNGHandler {
public:
	PNGHandler();	//!< Initialize
	
	//! Open a PNG file and returns its contents
	PNGImage OpenPNGFile( QString strFileName, bool& bOK );
	
	//! Writes a PNG image to file
	void WritePNGFile( PNGImage image, QString strFileName );
	
}; // end class PNGHandler


