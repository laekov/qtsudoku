#include <QApplication>
#include <QWidget>
#include <QDialog>
#include <QObject>
#include <QSignalMapper>
#include <QMainWindow>
#include <QtDebug>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "mainwnd.hh"

int main(int argc, char* args[]) {
	srand(time(0));
	QApplication app(argc, args);
	MainWnd* mainWnd(new MainWnd);
	mainWnd->show();
	return app.exec();
}

