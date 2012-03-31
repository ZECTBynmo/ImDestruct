
#include <QtCore/QCoreApplication>
#include "PNGHandler.h"
#include "qstringlist.h"
#include "ImageDestructor.h"
#include "SVGHandler.h"
#include "qdatetime.h"

int main(int argc, char *argv[])
{
	QCoreApplication application(argc, argv);
	
	// Create and start a timer so we can measure our runtime
	QTime timer;
	timer.start();
	
	// Get our application's arguments
	QStringList strArgs= application.arguments();
	
	// If we weren't passed any extra arguments, just return and don't run the program
	if( strArgs.size() < 3 ) return 0;
	
	// Create our image handlers and destructor
	PNGHandler* pPNGHandler= new PNGHandler();
	SVGHandler* pSVGHandler= new SVGHandler();
	ImageDestructor* pDestructor= new ImageDestructor();
	
	// Open the image
	PNGImage image= pPNGHandler->OpenPNGFile( strArgs[1] );
	
	SVGImage destructedImage= pDestructor->DestructImage( image, QRect( 0,0,image.width,image.height) );
	
	// Write the image to file
	pSVGHandler->WriteSVGFile( destructedImage, strArgs[2] );
	
	printf( "Finished processing in %d milliseconds", timer.elapsed() );

	return application.exec();
}
