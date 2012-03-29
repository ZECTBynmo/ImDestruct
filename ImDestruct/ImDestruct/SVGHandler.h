///////////////////////////////////////////////////////////
//
// SVGHandler.h: Reads and writes SVG files
//
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "SVGImage.h"

class QString; class QSvgGenerator;

//////////////////////////////////////////////////////////////////////////
//! Reads and writes SVG files
class SVGHandler {
public:
	SVGHandler();	//!< Initialize
	
	//! Writes a SVG image to file
	void WriteSVGFile( SVGImage image, QString strFileName );

private:
	QSvgGenerator* m_pGenerator;	//!< SVG generator
	QPainter* m_pPathPainter;		//!< Path painter
	
}; // end class SVGHandler


