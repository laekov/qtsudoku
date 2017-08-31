#include <QPalette>
#include <QMessageBox>
#include <cstdlib>
#include <ctime>

#include "sudokore.hh"
#include "mainwnd.hh"
#include "ui_mainwnd.h"
#include "numgrid.hh"

MainWnd::MainWnd(QWidget* parent): QWidget(parent), ui(new Ui::MainWnd) {
	ui->setupUi(this);
	int btnWidth(this->ui->inputArea->width() / 11), btnHeight(this->ui->inputArea->height());
	for (int i = 0; i < 9; ++ i) {
		QPushButton* pb(new QPushButton(this->ui->inputArea));
		pb->setText(QString("%1").arg(i + 1));
		pb->setWindowTitle(QString("%1").arg(i + 1));
		pb->setGeometry(i * btnWidth, 0, btnWidth, btnHeight);
		pb->show();
		this->inputBtns[i] = pb;
		QObject::connect(pb, SIGNAL(clicked()), this, SLOT(numberChanged()));
	} { 
		QPushButton* pb(new QPushButton(this->ui->inputArea));
		pb->setText("M");
		pb->setWindowTitle("Mark");
		pb->setWhatsThis("Set mark");
		pb->setGeometry(9 * btnWidth, 0, btnWidth, btnHeight);
		pb->setStyleSheet("background-color: cyan");
		pb->show();
		QObject::connect(pb, SIGNAL(clicked()), this, SLOT(setMark()));
	} {
		QPushButton* pb(new QPushButton(this->ui->inputArea));
		pb->setText("C");
		pb->setWindowTitle("Clear");
		pb->setWhatsThis("Clear this grid");
		pb->setGeometry(10 * btnWidth, 0, btnWidth, btnHeight);
		pb->show();
		QObject::connect(pb, SIGNAL(clicked()), this, SLOT(clearNumbers()));
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
	QObject::connect(this->ui->buttonUndo, SIGNAL(clicked()), this, SLOT(undo()));
	QObject::connect(this->ui->buttonRedo, SIGNAL(clicked()), this, SLOT(redo()));
}

MainWnd::~MainWnd() {
	delete this->ui;
}

void MainWnd::display() {
	if (this->optStack.empty()) {
		return;
	}
	BoardStatus bs(this->optStack.top());
	int ar(-1), al(-1), hs(0);
	if (this->activeId != -1) {
		ar = this->activeId / 9;
		al = this->activeId % 9;
		hs = bs[this->activeId] & 1;
		if (hs) {
			hs = bs[this->activeId] ^ 1;
		}
	}
	for (int i = 0; i < 81; ++ i) {
		int v(bs[i]), r(i / 9), l(i % 9);
		if (hs) {
			if (bs[i] & hs) {
				v |= 1 << 12;
			}
		} else {
			if (i == this->activeId) {
				v |= 1 << 10;
			} else if (l == al || r == ar) {
				v |= 1 << 11;
			}
		}
		this->nums[i]->setNumber(v);
	}
}

void MainWnd::activeChanged(int num) {
	if (num == this->activeId) {
		this->activeId = -1;
	} else {
		this->activeId = num;
	}
	this->display();
}

void MainWnd::reset() {
	Sudokore sk;
	QString diffLv(this->ui->diffLvEdit->text());
	int difflv; 
	bool valid;
	difflv = diffLv.toInt(&valid);
	if (!valid || difflv < 10 || difflv > 80) {
		difflv = 50;
	}
	sk.generate(difflv);
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

void MainWnd::numberChanged() {
	if (this->optStack.empty() || this->activeId == -1) {
		return;
	}
	BoardStatus bs(this->optStack.top());
	QString titleText(((QPushButton*)this->sender())->windowTitle());
	int chgNum(titleText.toInt());
	bs[this->activeId] ^= 1 << chgNum;
	this->optStack.push(bs);
	for (; !this->redoStack.empty(); this->redoStack.pop());
	this->display();
	if (this->won()) {
		QMessageBox* mb(new QMessageBox(this));
		mb->setWindowTitle("HINT");
		mb->setText("YOU WIN");
		mb->exec();
	}
}

bool MainWnd::won() {
	if (this->optStack.empty()) {
		return 0;
	}
	BoardStatus st(this->optStack.top());
	Sudokore sdk;
	for (int i = 0; i < 81; ++ i) {
		int x(0);
		for (int j = 1; j <= 9; ++ j) {
			if (st[i] & (1 << j)) {
				if (!x) {
					x = j;
				} else {
					return 0;
				}
			}
		}
		sdk.set(i, x);
	}
	return sdk.isFinished();
}

void MainWnd::undo() {
	if (this->optStack.empty()) {
		return;
	}
	BoardStatus bs(this->optStack.top());
	this->optStack.pop();
	if (this->optStack.empty()) {
		this->optStack.push(bs);
	} else {
		this->redoStack.push(bs);
	}
	this->display();
}

void MainWnd::redo() {
	if (this->redoStack.empty()) {
		return;
	}
	BoardStatus bs(this->redoStack.top());
	this->optStack.push(bs);
	this->redoStack.pop();
	this->display();
}

void MainWnd::setMark() {
	if (this->optStack.empty()) {
		return;
	}
	BoardStatus bs(this->optStack.top());
	if (this->activeId == -1) {
		return;
	}
	if (bs[this->activeId] & 1) {
		return;
	}
	bs[this->activeId] ^= (1 << 13);
	this->optStack.push(bs);
	this->display();
}

void MainWnd::clearNumbers() {
	if (this->optStack.empty()) {
		return;
	}
	BoardStatus bs(this->optStack.top());
	if (this->activeId == -1) {
		return;
	}
	if (bs[this->activeId] & 1) {
		return;
	}
	bs[this->activeId] &= 0;
	this->optStack.push(bs);
	this->display();
}

