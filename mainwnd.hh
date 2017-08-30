#ifndef MAINWND_HH
#define MAINWND_HH
#include <QWidget> 
#include <QLabel>
#include <QPushButton>
#include <vector>
#include <stack>

typedef std::vector<int> BoardStatus;

namespace Ui {
	class MainWnd;
};

class MainWnd: public QWidget {
	Q_OBJECT
	public:
		explicit MainWnd(QWidget* = 0);
		~MainWnd();
	private:
		Ui::MainWnd *ui;
		QPushButton* inputBtns[9];
		class NumGrid *nums[81];
		std::stack<BoardStatus> optStack, redoStack;
		int activeId;
	public slots:
		void reset();
		void display();
		void activeChanged(int);
};
#endif
