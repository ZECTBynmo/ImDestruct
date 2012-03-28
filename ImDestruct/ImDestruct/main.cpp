
#include <QtCore/QCoreApplication>
#include "PNGHandler.h"
#include "qstringlist.h"

int main(int argc, char *argv[])
{
	QCoreApplication application(argc, argv);
	
	// Get our application's arguments
	QStringList strArgs= application.arguments();
	
	// If we weren't passed any extra arguments, just return and don't run the program
	if( strArgs.size() < 3 ) return 0;
	
	// Create our PNG handler
	PNGHandler* pPNGHandler= new PNGHandler();
	
	// Open an the imag
	PNGImage image= pPNGHandler->OpenPNGFile( strArgs[1] );
	
	// Write the image to file
	pPNGHandler->WritePNGFile( image, strArgs[2] );

	return application.exec();
}
