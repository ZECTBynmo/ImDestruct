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
	
	const uint MIN_DESTRUCT_SIZE= 30, // The smallest size we're willing to try to destruct
			   PIXEL_SIMILARITY_TOLERANCE= 10;
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
	if( rectVertUndrawables.size() > 0 ) {
		for( uint iUndrawable=0; iUndrawable<rectVertUndrawables.size(); ++iUndrawable ) {
			vector<QRect> tempDrawables;
			findHorizontalDividers( image, rectVertUndrawables[iUndrawable], rectHorDrawables, rectHorUndrawables );
			
// 			for( uint iRect=0; iRect<tempDrawables.size(); ++iRect )
// 				rectHorDrawables.push_back( tempDrawables[iRect] );
		}
	} else {
		findHorizontalDividers( image, rectSearchArea, rectHorDrawables, rectHorUndrawables );
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
		drawableArea.rect= QRect( rectVertDrawables[iRect].x()/* + rectSearchArea.x()*/, 
								  rectVertDrawables[iRect].y()/* + rectSearchArea.y()*/, 
								  rectVertDrawables[iRect].width(), 
								  rectVertDrawables[iRect].height() );

		if( rectSearchArea.contains(rectVertDrawables[iRect]) )
			bool bWork= false;

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
	uint uDifferenceSum= 0;
	
	// Test stuff
	uint R1= pFirstPixel[RED],
		 G1= pFirstPixel[GREEN],
		 B1= pFirstPixel[BLUE],
		 A1= pFirstPixel[ALPHA],
		 R2= pSecondPixel[RED],
		 G2= pSecondPixel[GREEN],
		 B2= pSecondPixel[BLUE],
		 A2= pSecondPixel[ALPHA];
	
	// If both pixels are transparent, they're effectively the same
	if( pFirstPixel[ALPHA] == 0 && pSecondPixel[ALPHA] == 0 ) 
		return true;
	
	for( uint iAttribute=0; iAttribute<NUM_PIXEL_ATTRIBUTES; ++iAttribute ) {
		uDifferenceSum+= abs( pFirstPixel[iAttribute] - pSecondPixel[iAttribute] );
	}
	
	if( uDifferenceSum > PIXEL_SIMILARITY_TOLERANCE )
		bIdentical= false;

	return bIdentical;
} // end ImageDestructor::isPixelIdentical()


//////////////////////////////////////////////////////////////////////////////
/*! Finds a list of horizontal dividers */
void ImageDestructor::findHorizontalDividers( PNGImage& image, QRect rectSearchArea, vector<QRect>& rectDrawables, vector<QRect>& rectUndrawables ) {
	vector<uint> uHorizontalDivisors;
	
	uint uNumSequentialDividers= 0,
		 uNumStartDrawables= rectDrawables.size();
	
	uint xStart= rectSearchArea.x(),
		 yStart= rectSearchArea.y(),
		 xEnd= rectSearchArea.right(),
		 yEnd= rectSearchArea.bottom();

	// Loop through every row and check for horizontal divisors
	for( uint yLoc=yStart; yLoc<yEnd; yLoc++ ) {
		bool bHorizontalDivisor= true;
		for ( uint xLoc=xStart+1; xLoc<xEnd; xLoc++) {
			png_byte* pPixel= &( image.pRows[yLoc][xLoc*4] ),
					* pLastPixel= &( image.pRows[yLoc][(xLoc-1)*4] );

			if( !isPixelIdentical(pPixel, pLastPixel) ) {
				bHorizontalDivisor= false;
				break;
			}
		} // end for each x

		if( bHorizontalDivisor ) {			
			if( uHorizontalDivisors.size() > 0 && uHorizontalDivisors[uHorizontalDivisors.size()-1] == yLoc - 1 ) {
				uNumSequentialDividers++;
			}			
			uHorizontalDivisors.push_back( yLoc );
		} else {
			if( uNumSequentialDividers > 0 ) {
				QRect testrect= QRect(xStart, yLoc-uNumSequentialDividers, rectSearchArea.width(), uNumSequentialDividers);
				if( rectDrawables.size() > uNumStartDrawables && testrect.top()-rectDrawables[rectDrawables.size()-1].bottom() < 0 ) 
					bool btest= false;
					
				if( testrect.height() < 3  )
					bool btest= false;
					
				rectDrawables.push_back( testrect );
			}
			uNumSequentialDividers= 0;
		}
	} // end for each y
	
	// If we got here with some sequential dividers left, that means we got all the way to the bottom with the sequential chunk
	if( uNumSequentialDividers > 0 ) {	
		QRect testrect= QRect(xStart, yEnd-uNumSequentialDividers, rectSearchArea.width(), uNumSequentialDividers);
		if( rectDrawables.size() > uNumStartDrawables && testrect.top()-rectDrawables[rectDrawables.size()-1].bottom() < 0 ) 
			bool btest= 0;
			
		if( testrect.height() < 2 )
			bool btest= false;
		rectDrawables.push_back( testrect );
	}
	
	// At this point we should have found all the drawable rectangles
	// Get out if there's only one drawable rect and it takes up the whole screen, or there's nothing drawable
	if( rectDrawables.size() == uNumStartDrawables+1 && rectDrawables[uNumStartDrawables] == rectSearchArea )
		return;
	else if( rectDrawables.size() == uNumStartDrawables ) {
		rectUndrawables.push_back( rectSearchArea );
		return;
	}

	// Add the remaining parts of the image to the undrawable rects
	for( uint iRect=uNumStartDrawables; iRect<=rectDrawables.size(); ++iRect ) {		
		QRect undrawableRect;

		if( iRect == rectDrawables.size() ) {
			QRect testRect= QRect( 0, 
								   rectDrawables[iRect-1].bottom(), 
								   rectSearchArea.right(), 
								   rectSearchArea.bottom()-rectDrawables[iRect-1].bottom() );

			int testheight= rectSearchArea.height()-rectDrawables[iRect-1].bottom();

			if( !testRect.isValid() )
				bool bTest= false;
		
			undrawableRect= testRect;
								   
		} else if( iRect == uNumStartDrawables ) {
		
			undrawableRect= QRect( 0, 
								   0, 
								   rectSearchArea.right()+1, 
								   rectDrawables[iRect].y() ); 
		} else {
		
			undrawableRect= QRect( 0, 
								   rectDrawables[iRect-1].bottom(), 
								   rectSearchArea.right(), 
								   rectDrawables[iRect].top()-rectDrawables[iRect-1].bottom() ); 
		}

		if( undrawableRect.height() > 0 && undrawableRect.isValid() )
			rectUndrawables.push_back( undrawableRect );
		else
			bool bTest= false;
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

			if( !isPixelIdentical(pPixel, pLastPixel) ) {
				bVerticalDivisor= false;
				break;
			}
		} // end for each y

		if( bVerticalDivisor ) {			
			if( uVerticalDivisors.size() > 0 && uVerticalDivisors[uVerticalDivisors.size()-1] == xLoc - 1 ) {
				uNumSequentialDividers++;
			}			

			uVerticalDivisors.push_back( xLoc );
		} else {
			if( uNumSequentialDividers > 0 ) {
				QRect drawableRect= QRect(xLoc-uNumSequentialDividers, yStart, uNumSequentialDividers, rectSearchArea.height());
				rectDrawables.push_back( drawableRect );				
			}
			uNumSequentialDividers= 0;
		}
	} // end for each x
	
	// If we got here with some sequential dividers left, that means we got all the way to the bottom with the sequential chunk
	if( uNumSequentialDividers > 0 ) {	
		rectDrawables.push_back( QRect(rectSearchArea.width()-uNumSequentialDividers, yStart, uNumSequentialDividers, rectSearchArea.height()) );
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
			undrawableRect= QRect( rectDrawables[iRect-1].right(), 
								   0, rectSearchArea.right()-rectDrawables[iRect-1].right(), 
								   rectSearchArea.height() );
		} else if( iRect == 0 ) {
			undrawableRect= QRect( 0, 
								   0, 
								   rectDrawables[iRect].x()+1,
								   rectSearchArea.height() ); 
		} else {		
			undrawableRect= QRect( rectDrawables[iRect-1].right(), 
								   0, 
								   rectDrawables[iRect].left() - rectDrawables[iRect-1].right(), 
								   rectSearchArea.height() ); 
		}
		
		if( undrawableRect.width() > 0 && undrawableRect.isValid() )
			rectUndrawables.push_back( undrawableRect );
		else
			bool bTest= false;
	} // end for each drawable rect
} // end ImageDestructor::findVerticalDividers()