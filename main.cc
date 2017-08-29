#include <QApplication>
#include <QWidget>
#include <QDialog>
#include <QObject>
#include <QSignalMapper>
#include <QMainWindow>
#include <vector>

#include "sudoku.hh"

int main(int argc, char* args[]) {
	QApplication app(argc, args);
	QMainWindow* dialog(new QMainWindow());
	dialog->setWindowTitle("orz");
	dialog->setMaximumHeight(textSize * 9);
	dialog->setMinimumHeight(textSize * 9);
	dialog->setMaximumWidth(textSize * 9);
	dialog->setMinimumWidth(textSize * 9);
	dialog->show();
	Sudoku *sudokuWindow(new Sudoku(dialog));
	return app.exec();
}

