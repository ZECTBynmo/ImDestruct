///////////////////////////////////////////////////////////
//
// ImageDestructor.cpp: Attempts to convert the majority of an image file to a vector format
// 
//////////////////////////////////////////////////////////////////////////////
// DevStudio!class ImageDestructor
#include "ImageDestructor.h"
#include "qglobal.h"

namespace {
	enum {
		RED,
		GREEN,
		BLUE,
		ALPHA,
		
		NUM_PIXEL_ATTRIBUTES
	};
	
	const uint MIN_DESTRUCT_SIZE= 30; // The smallest size we're willing to try to destruct
}

//////////////////////////////////////////////////////////////////////////////
/*! Initialize */
ImageDestructor::ImageDestructor() {

} // end ImageDestructor::ImageDestructor()


//////////////////////////////////////////////////////////////////////////////
/*! Break the image into the smallest parts possible */
void ImageDestructor::DestructImage( PNGImage& image, QRect rectSearchArea, SVGImage& destructedImage ) {
	
	vector<QRect> rectVertDrawables,
				  rectVertUndrawables,
				  rectHorDrawables,
				  rectHorUndrawables;
	
	// Divide the image up into drawable and undrawable sections
	findVerticalDividers( image, rectSearchArea, rectVertDrawables, rectVertUndrawables );
	for( uint iUndrawable=0; iUndrawable<rectVertUndrawables.size(); ++iUndrawable ) {
		findHorizontalDividers( image, rectVertUndrawables[iUndrawable], rectHorDrawables, rectHorUndrawables );
	}
	
	// If we didn't find any dividers, we can't go any further
	if( rectVertDrawables.size() == 0 && rectHorDrawables.size() == 0 )
		return;
	
	// Add our drawable rects to the destructed SVG image
	for( uint iRect=0; iRect<rectHorDrawables.size(); ++iRect ) {
		Area drawableArea;
		drawableArea.bFill= true;
		drawableArea.rect= rectHorDrawables[iRect];
		
		destructedImage.areas.push_back( drawableArea );
	}
	
	for( uint iRect=0; iRect<rectVertDrawables.size(); ++iRect ) {
		Area drawableArea;
		drawableArea.bFill= true;
		drawableArea.rect= QRect( rectVertDrawables[iRect].x() + rectSearchArea.x(), 
								  rectVertDrawables[iRect].y() + rectSearchArea.y(), 
								  rectVertDrawables[iRect].width(), 
								  rectVertDrawables[iRect].height() );

		destructedImage.areas.push_back( drawableArea );
	}
	
	// Destruct the undrawable sections further
	for( uint iRect=0; iRect<rectHorUndrawables.size(); ++iRect ) {
		if( rectHorUndrawables[iRect].width() > MIN_DESTRUCT_SIZE || rectHorUndrawables[iRect].height() > MIN_DESTRUCT_SIZE )
			DestructImage( image, rectHorUndrawables[iRect], destructedImage );
	}
	
	return;
} // end ImageDestructor::DestructImage()


//////////////////////////////////////////////////////////////////////////////
/*! Returns whether the given pixels are identical in RGBA */
bool ImageDestructor::isPixelIdentical( png_byte* pFirstPixel, png_byte* pSecondPixel ) {
	bool bIdentical= true;
	
	// If both pixels are transparent, they're effectively the same
	if( pFirstPixel[ALPHA] == 0 && pSecondPixel[ALPHA] == 0 ) 
		return true;
	
	for( uint iAttribute=0; iAttribute<NUM_PIXEL_ATTRIBUTES; ++iAttribute ) {
		if( pFirstPixel[iAttribute] != pSecondPixel[iAttribute] )
			bIdentical= false;
	}

	return bIdentical;
} // end ImageDestructor::isPixelIdentical()


//////////////////////////////////////////////////////////////////////////////
/*! Finds a list of horizontal dividers */
void ImageDestructor::findHorizontalDividers( PNGImage& image, QRect rectSearchArea, vector<QRect>& rectDrawables, vector<QRect>& rectUndrawables ) {
	vector<uint> uHorizontalDivisors;
	
	uint uNumSequentialDividers= 0;
	
	uint xStart= rectSearchArea.x(),
		 yStart= rectSearchArea.y(),
		 xEnd= rectSearchArea.right(),
		 yEnd= rectSearchArea.bottom();

	// Loop through every row and check for horizontal divisors
	for( uint yLoc=yStart; yLoc<yEnd; yLoc++ ) {
		bool bHorizontalDivisor= true;
		png_byte* row= image.pRows[yLoc];
		for ( uint xLoc=xStart+1; xLoc<xEnd; xLoc++) {
			png_byte* pPixel= &( row[xLoc*4] ),
				* pLastPixel= &( row[(xLoc-1)*4] );

			if( !isPixelIdentical(pPixel, pLastPixel) )
				bHorizontalDivisor= false;
		} // end for each x

		if( bHorizontalDivisor ) {			
			if( uHorizontalDivisors.size() > 0 && uHorizontalDivisors[uHorizontalDivisors.size()-1] == yLoc - 1 ) {
				uNumSequentialDividers++;
			}			
			
			uHorizontalDivisors.push_back( yLoc );
		} else {
			if( uNumSequentialDividers > 0 ) {
				rectDrawables.push_back( QRect(xStart, yLoc-uNumSequentialDividers, rectSearchArea.width(), uNumSequentialDividers) );
			}
			uNumSequentialDividers= 0;
		}
	} // end for each y
	
	// If we got here with some sequential dividers left, that means we got all the way to the bottom with the sequential chunk
	if( uNumSequentialDividers > 0 ) {
		rectDrawables.push_back( QRect(xStart, rectSearchArea.height()-uNumSequentialDividers, rectSearchArea.width(), uNumSequentialDividers) );
	}
	
	// At this point we should have found all the drawable rectangles
	// Get out if there's only one drawable rect and it takes up the whole screen, or there's nothing drawable
	if( rectDrawables.size() == 1 && rectDrawables[0] == rectSearchArea )
		return;
	else if( rectDrawables.size() == 0 ) {
		rectUndrawables.push_back( rectSearchArea );
		return;
	}

	// Add the remaining parts of the image to the undrawable rects
	for( uint iRect=0; iRect<=rectDrawables.size(); ++iRect ) {		
		QRect undrawableRect;

		if( iRect == rectDrawables.size() ) {
			undrawableRect= QRect( 0, rectDrawables[iRect-1].bottom(), rectSearchArea.right(), rectSearchArea.height()-rectDrawables[iRect-1].bottom() );
		} else if( iRect == 0 ) {
			undrawableRect= QRect( 0, 0, rectSearchArea.right(), rectDrawables[iRect].y() ); 
		} else {
			undrawableRect= QRect( 0, rectDrawables[iRect-1].bottom(), rectSearchArea.right(), rectDrawables[iRect].y()-rectDrawables[iRect-1].bottom() ); 
		}

		if( undrawableRect.height() > 0 && undrawableRect.isValid() )
			rectUndrawables.push_back( undrawableRect );
	} // end for each drawable rect
} // end ImageDestructor::findHorizontalDividers()


//////////////////////////////////////////////////////////////////////////////
/*! Finds a list of vertical dividers */
void ImageDestructor::findVerticalDividers( PNGImage& image, QRect rectSearchArea, vector<QRect>& rectDrawables, vector<QRect>& rectUndrawables ) {
	vector<uint> uVerticalDivisors;
	
	uint uNumSequentialDividers= 0;
	
	uint xStart= rectSearchArea.x(),
		 yStart= rectSearchArea.y(),
		 xEnd= rectSearchArea.right(),
		 yEnd= rectSearchArea.bottom();
	
	// Loop through every column and check for veritcal divisors
	for( uint xLoc=xStart; xLoc<xEnd; ++xLoc ) {
		bool bVerticalDivisor= true;

		for( uint yLoc=yStart+1; yLoc<yEnd; ++yLoc ) {
			png_byte* pPixel= &( image.pRows[yLoc][xLoc*4] ),
				* pLastPixel= &( image.pRows[yLoc-1][xLoc*4] );

			if( !isPixelIdentical(pPixel, pLastPixel) )
				bVerticalDivisor= false;
		} // end for each y

		if( bVerticalDivisor ) {			
			if( uVerticalDivisors.size() > 0 && uVerticalDivisors[uVerticalDivisors.size()-1] == xLoc - 1 ) {
				uNumSequentialDividers++;
			}			

			uVerticalDivisors.push_back( xLoc );
		} else {
			if( uNumSequentialDividers > 0 ) {
				QRect drawableRect= QRect(xLoc-uNumSequentialDividers, 0, uNumSequentialDividers, rectSearchArea.height());
				rectDrawables.push_back( drawableRect );				
			}
			uNumSequentialDividers= 0;
		}
	} // end for each x
	
	// If we got here with some sequential dividers left, that means we got all the way to the bottom with the sequential chunk
	if( uNumSequentialDividers > 0 ) {
		rectDrawables.push_back( QRect(xStart, rectSearchArea.width()-uNumSequentialDividers, uNumSequentialDividers, rectSearchArea.height()) );
	}
	
	// At this point we should have found all the drawable rectangles when looking vertically
	// Get out if there's only one drawable rect and it takes up the whole screen, or there's nothing drawable
	if( rectDrawables.size() == 1 && rectDrawables[0] == rectSearchArea )
		return;
	else if( rectDrawables.size() == 0 ) {
		rectUndrawables.push_back( rectSearchArea );
		return;
	}
	
	// Add the remaining parts of the image to the undrawable rects
	for( uint iRect=0; iRect<=rectDrawables.size(); ++iRect ) {		
		QRect undrawableRect;
		
		if( iRect == rectDrawables.size() ) {
			undrawableRect= QRect( rectDrawables[iRect-1].right(), 0, rectSearchArea.right()-rectDrawables[iRect-1].right(), rectSearchArea.height() );
		} else if( iRect == 0 ) {
			undrawableRect= QRect( 0, 0, rectDrawables[iRect].x(), rectSearchArea.y() ); 
		} else {
			undrawableRect= QRect( rectDrawables[iRect-1].right(), 0, rectDrawables[iRect].x()-rectDrawables[iRect-1].right(), rectSearchArea.height() ); 
		}
		
		if( undrawableRect.width() > 0 && undrawableRect.isValid() )
			rectUndrawables.push_back( undrawableRect );
	} // end for each drawable rect
} // end ImageDestructor::findVerticalDividers()