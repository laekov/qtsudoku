#ifndef NUMGRID_HH
#define NUMGRID_HH
#include <QTextEdit>
#include <QWidget>
#include <QColor>
#include <QPalette>
#include <QEvent>
class NumGrid: public QTextEdit {
	Q_OBJECT
	private:
		int id, num;
	public:
		explicit NumGrid(int, QWidget* = 0);
		void setColor(QColor, QColor);
		void setNumber(int);
		friend class MainWnd;
		bool eventFilter(QObject*, QEvent*);
	signals:
		void activeChanged(int);
};
#endif
