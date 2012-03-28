///////////////////////////////////////////////////////////
//
// SVGHandler.cpp: Reads and writes SVG files
// 
//////////////////////////////////////////////////////////////////////////////
// DevStudio!class SVGHandler
#include "SVGHandler.h"
#include <QtSvg/QtSvg>
#include "QtGui/qfiledialog.h"

//////////////////////////////////////////////////////////////////////////////
/*! Initialize */
SVGHandler::SVGHandler() {
	QSvgGenerator generator;
	generator.setFileName("C:/tested.svg");
	generator.setSize(QSize(200, 200));
	generator.setViewBox(QRect(0, 0, 200, 200));
	generator.setTitle("SVG Generator Example Drawing");
	generator.setDescription("An SVG drawing created by the SVG Generator "
		"Example provided with Qt.");
} // end SVGHandler::SVGHandler()
