#include <QApplication>
#include "blackjack.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setOrganizationName("pandafruits");
	app.setApplicationName("blackjack");
	
	Blackjack blackjack;
	blackjack.show();
	
	return app.exec();
}
