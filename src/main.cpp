
#include <QApplication>

#include <app/main_window.h>

int main( int argc, char* argv[] )
{
	QApplication app( argc, argv );
	app.setApplicationName( "Noises" );

	QFont font = QApplication::font();
	font.setPixelSize( 12 );
	QApplication::setFont( font );

	noises::MainWindow mainWin;
	mainWin.show();

	return app.exec();
}