#ifndef SUDOKU_H
#define SUDOKU_H
#include <QObject>
#include <vector>
#include <QTextEdit>
#include "sudokore.hh"

const int textSize = 74;

class Sudoku: public QObject, Sudokore {
	Q_OBJECT
	private:
		QTextEdit* edits[81];
	public:
		Sudoku(QWidget* parent) {
			for (int i = 0; i < 81; ++ i) {
				QTextEdit* ec(new QTextEdit(parent));
				ec->setGeometry(i % 9 * textSize, i / 9 * textSize, textSize, textSize);
				ec->setWindowOpacity(200);
				ec->setWindowTitle(QString("%1").arg(i));
				ec->setTabChangesFocus(1);
				ec->show();
				QObject::connect(ec, SIGNAL(textChanged()), this, SLOT(onNumberChange()));
				this->edits[i] = ec;
			}
			this->generate();
		}
	public slots:
		void onNumberChange(int = -1);
		void generate(int = 40);
};
#endif
