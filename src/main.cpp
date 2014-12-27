
#include <QApplication>

#include <QGst/Init>

#include "main_window.h"

int main( int argc, char* argv[] )
{
	QApplication app( argc, argv );
	app.setApplicationName( "Noises" );

	QGst::init( &argc, &argv );

	QFont font = QApplication::font();
	font.setPixelSize( 12 );
	QApplication::setFont( font );

	noises::MainWindow mainWin;
	mainWin.show();

	return app.exec();
}