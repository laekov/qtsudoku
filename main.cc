#include <QApplication>
#include <QWidget>
#include <QDialog>
#include <QObject>
#include <QSignalMapper>
#include <QMainWindow>
#include <QtDebug>
#include <vector>

#include "mainwnd.hh"

int main(int argc, char* args[]) {
	QApplication app(argc, args);
	MainWnd* mainWnd(new MainWnd);
	mainWnd->show();
	return app.exec();
}

