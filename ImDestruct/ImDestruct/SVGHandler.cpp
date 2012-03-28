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
	QSvgGenerator generator;
	generator.setFileName("C:/tested.svg");
	generator.setSize(QSize(200, 200));
	generator.setViewBox(QRect(0, 0, 200, 200));
	generator.setTitle("SVG Output Test");
	generator.setDescription("A test of SVG output");
		
	QPainter painter;
	painter.begin(&generator);
} // end SVGHandler::SVGHandler()
