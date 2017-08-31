#include <cmath>
#include <QFont>
#include "numgrid.hh"

NumGrid::NumGrid(int id, QWidget* parent): QTextEdit(parent), id(id) {
	this->setReadOnly(1);
	this->installEventFilter(this);
}

void NumGrid::setColor(QColor cbg, QColor cfg) {
	QPalette pte(this->palette());
	pte.setColor(QPalette::Base, cbg);
	pte.setColor(QPalette::Text, cfg);
	this->setPalette(pte);
}

void NumGrid::setNumber(int x) {
	int totn(0), ns[10], ro(0);
	QColor fgc(Qt::black), bgc(Qt::white);
	QString txt("");
	for (int i = 1; i <= 9; ++ i) {
		if (x & (1 << i)) {
			ns[totn ++] = i;
		}
	}
	this->num = x;
	if (x & 1) {
		bgc = Qt::gray;
		ro = 1;
	} else if (totn == 0) {
		bgc = Qt::white;
	} else {
		if (totn == 1) {
			bgc = Qt::yellow;
		} else {
			bgc = Qt::blue;
			fgc = Qt::white;
		}
	}
	for (int i = 0; i < totn; ++ i) {
		txt += QString("%1").arg(ns[i]);
	}
	if (x & (1 << 13)) {
		bgc = Qt::cyan;
	}
	if (x & (1 << 10)) {
		bgc = bgc.darker(150);
	} else if (x & (1 << 11)) {
	   	bgc = bgc.darker(110);
	} else if (x & (1 << 12)) {
		bgc = Qt::red;
		if (x & 1) {
		} else {
			bgc = bgc.lighter(170);
		}
	}
	this->setReadOnly(ro);
	this->setColor(bgc, fgc);
	QFont qf;
	if (totn > 0) {
		int ch(this->height());
		static const int xs[] = { 1, 1, 2, 2, 2, 2, 2, 2, 2, 3, 3 };
		ch = ch / 1.7;
		qf.setFamily("Monospace");
		qf.setPixelSize(ch / xs[txt.length()]);
		this->setFont(qf);
	}
	this->setText(txt);
}

bool NumGrid::eventFilter(QObject*, QEvent* evt) {
	if (evt->type() == QEvent::MouseButtonPress) {
		emit activeChanged(this->id);
	}
	return 0;
}

