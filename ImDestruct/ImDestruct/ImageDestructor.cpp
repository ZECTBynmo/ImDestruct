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
}

//////////////////////////////////////////////////////////////////////////////
/*! Initialize */
ImageDestructor::ImageDestructor() {

} // end ImageDestructor::ImageDestructor()


//////////////////////////////////////////////////////////////////////////////
/*! Break the image into the smallest parts possible */
SVGImage ImageDestructor::DestructImage( PNGImage& image ) {
	SVGImage destructedImage;
	
	vector<QRect> rectDrawables,
				  rectUndrawables;
	
	// Find our horizontal and vertical dividers
	findHorizontalDividers( image, QRect(0, 0, image.width, image.height), rectDrawables, rectUndrawables );	
	findVerticalDividers( image, QRect(0, 0, image.width, image.height), rectDrawables, rectUndrawables );
	
	
	
	return destructedImage;
} // end ImageDestructor::DestructImage()


//////////////////////////////////////////////////////////////////////////////
/*! Returns whether the given pixels are identical in RGBA */
bool ImageDestructor::isPixelIdentical( png_byte* pFirstPixel, png_byte* pSecondPixel ) {
	bool bIdentical= true;
	
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

	// Loop through every row and check for horizontal divisors
	for( uint yLoc=0; yLoc<image.height; yLoc++ ) {
		bool bHorizontalDivisor= true;
		png_byte* row= image.pRows[yLoc];
		for ( uint xLoc=1; xLoc<image.width; xLoc++) {
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
				rectDrawables.push_back( QRect(0, yLoc-uNumSequentialDividers, rectSearchArea.width(), uNumSequentialDividers) );
			}
			uNumSequentialDividers= 0;
		}
	} // end for each y

} // end ImageDestructor::findHorizontalDividers()


//////////////////////////////////////////////////////////////////////////////
/*! Finds a list of vertical dividers */
void ImageDestructor::findVerticalDividers( PNGImage& image, QRect rectSearchArea, vector<QRect>& rectDrawables, vector<QRect>& rectUndrawables ) {
	vector<uint> uVerticalDivisors;
	
	uint uNumSequentialDividers= 0;
	
	// Loop through every column and check for veritcal divisors
	for( uint xLoc= 0; xLoc<image.width; ++xLoc ) {
		bool bVerticalDivisor= true;

		for( uint yLoc=1; yLoc<image.height; ++yLoc ) {
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
	
	// At this point we should have found all the drawable rectangles when looking vertically
	// Add the rest of the image to the undrawables
	for( uint iRect=0; iRect<rectDrawables.size(); ++iRect ) {
		uint xUndrawableStart= 0;
		
		//if( rectUndrawables.size() == 0 && rectDrawables[iRect].x() != 0 )
			//xUndrawableStart
		
	
		if( rectDrawables[iRect].x() == 0 ) {
			continue;
		} else {
			if( rectDrawables.size() == iRect + 1 ) {
				QRect undrawableRect= QRect( 0, 0, rectDrawables[iRect].x(), rectSearchArea.height() );
				rectUndrawables.push_back( undrawableRect );
			} else {
				QRect undrawableRect= QRect( 0, 0, rectDrawables[iRect+1].x() - rectDrawables[iRect].x(), rectSearchArea.height() );
				rectUndrawables.push_back( undrawableRect );
			}	// end if this isn't our last rect
		} // end if this rect doesn't start at 0
	} // end for each drawable rect
} // end ImageDestructor::findVerticalDividers()