///////////////////////////////////////////////////////////
//
// SVGHandler.cpp: Reads and writes SVG files
// 
//////////////////////////////////////////////////////////////////////////////
// DevStudio!class SVGHandler
#include "SVGHandler.h"
#include <QtSvg/QtSvg>
#include "QtGui/qfiledialog.h"
#include "QtGui/qpainter.h"

//////////////////////////////////////////////////////////////////////////////
/*! Initialize */
SVGHandler::SVGHandler() {
	// Create our SVG generator
	m_pGenerator= new QSvgGenerator();
	
	// Create our path painter
	m_pPathPainter= new QPainter();
} // end SVGHandler::SVGHandler()


//////////////////////////////////////////////////////////////////////////////
/*! Writes a SVG image to file */
void SVGHandler::WriteSVGFile( SVGImage image, QString strFileName ) {
	m_pGenerator->setFileName( strFileName );
	m_pGenerator->setTitle( "please work" );
	
	m_pPathPainter->begin( m_pGenerator );
	image.PaintSVG( *m_pPathPainter );
	m_pPathPainter->end();
} // end SVGHandler::WriteSVGFile()
