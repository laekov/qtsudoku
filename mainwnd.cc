#include <QPalette>

#include "sudokore.hh"
#include "mainwnd.hh"
#include "ui_mainwnd.h"
#include "numgrid.hh"

MainWnd::MainWnd(QWidget* parent): QWidget(parent), ui(new Ui::MainWnd) {
	ui->setupUi(this);
	int btnWidth(this->ui->inputArea->width() / 9), btnHeight(this->ui->inputArea->height());
	for (int i = 0; i < 9; ++ i) {
		QPushButton* pb(new QPushButton(this->ui->inputArea));
		pb->setText(QString("%1").arg(i + 1));
		pb->setGeometry(i * btnWidth, 0, btnWidth, btnHeight);
		pb->show();
		this->inputBtns[i] = pb;
	}
	int clothWidth(this->ui->numberArea->width());
	int clothHeight(this->ui->numberArea->height());
	int gridWidth(clothWidth / 9 - 3);
	int gridHeight(clothHeight / 9 - 3);
	for (int i = 0; i < 81; ++ i) {
		int px(i / 9), py(i % 9);
		NumGrid* ng(new NumGrid(i, this->ui->numberArea));
		ng->setGeometry(px * gridWidth + px / 3 * 6, py * gridHeight + py / 3 * 6, gridWidth, gridHeight);
		ng->setNumber(0);
		ng->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
		QObject::connect(ng, SIGNAL(activeChanged(int)), this, SLOT(activeChanged(int)));
		ng->show();
		this->nums[i] = ng;
	}
	QObject::connect(this->ui->buttonReset, SIGNAL(clicked()), this, SLOT(reset()));
}

MainWnd::~MainWnd() {
	delete this->ui;
}

void MainWnd::display() {
	if (this->optStack.empty()) {
		return;
	}
	BoardStatus bs(this->optStack.top());
	int ar(-1), al(-1);
	if (this->activeId != -1) {
		ar = this->activeId / 9;
		al = this->activeId % 9;
	}
	for (int i = 0; i < 81; ++ i) {
		int v(bs[i]), r(i / 9), l(i % 9);
		if (i == this->activeId) {
			v |= 1 << 10;
		} else if (l == al || r == ar) {
			v |= 1 << 11;
		}
		this->nums[i]->setNumber(v);
	}
}

void MainWnd::activeChanged(int num) {
	this->activeId = num;
	this->display();
}

void MainWnd::reset() {
	Sudokore sk;
	sk.generate();
	BoardStatus b;
	for (int i = 0; i < 81; ++ i) {
		int x(1 ^ (1 << sk.get(i)));
		b.push_back(x);
	}
	while (!this->optStack.empty()) {
		this->optStack.pop();
	}
	while (!this->redoStack.empty()) {
		this->redoStack.pop();
	}
	this->activeId = -1;
	this->optStack.push(b);
	this->display();
}

