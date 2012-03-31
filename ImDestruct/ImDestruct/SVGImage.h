///////////////////////////////////////////////////////////
//
// SVGImage.h: Our SVG image struct
//
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include <vector>
#include "qnamespace.h"
#include "qrect.h"
#include "qpainter.h"

class QString;

using namespace std; using namespace Qt;

// A path is composed of vectors of x and y coordinates and color information
struct Path {
	vector<float> fX, fY;
	
	bool bFill;
	BrushStyle style;
	
	QPolygon GetPolygon() {
		QPolygon poly;
		for( uint iPos=0; iPos<fX.size(); ++iPos ) {
			poly.append( QPoint(fX[iPos], fY[iPos]) );
		}
		return poly;
	} // end GetPolygon()
};

// An area is a rectangle and color information
struct Area {
	Area() {
		style= SolidPattern;
	}

	QRect rect;
	bool bFill;
	
	BrushStyle style;
};

// An SVG image can be composed of paths, images, and various other info
struct SVGImage {
	vector<Path> paths;		//!< Our paths
	vector<Area> areas;		//!< Our rectangle areas
	
	QString strDescription,	//!< Our title and description strings
			strTitle;
	
	void PaintSVG( QPainter& pPainter ) {
		// Draw our areas
		for( uint iArea=0; iArea<areas.size(); ++iArea ) {
			pPainter.setPen( areas[iArea].style );
		
			if( areas[iArea].bFill )
				pPainter.fillRect( areas[iArea].rect, areas[iArea].style );
			else
				pPainter.drawRect( areas[iArea].rect );
		} // end for each area
		
		// Draw our paths
		for( uint iPath=0; iPath<paths.size(); ++iPath ) {
			pPainter.setPen( paths[iPath].style );
			
			if( paths[iPath].bFill ) {
				pPainter.drawPolygon( paths[iPath].GetPolygon() );	
			} else {
				pPainter.drawPolyline( paths[iPath].GetPolygon() );
			}
		} // end for each path
		
	} // end PaintSVG()
};

