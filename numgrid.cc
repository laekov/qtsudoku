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
	for (int i = 1; i <= 9; ++ i) {
		if (x & (1 << i)) {
			ns[totn ++] = i;
		}
	}
	this->num = x;
	if (x & 1) {
		bgc = Qt::gray;
		this->setText(QString("%1").arg(ns[0]));
		ro = 1;
	} else if (totn == 0) {
		this->setText("");
		bgc = Qt::white;
	} else {
		QString t("");
		for (int i = 0; i < totn; ++ i) {
			t += QString("%1").arg(ns[i]);
		}
		this->setText(t);
		if (totn == 1) {
			bgc = Qt::yellow;
		} else {
			bgc = Qt::blue;
			fgc = Qt::white;
		}
	}
	if (x & (1 << 10)) {
		bgc = bgc.darker(150);
	} else if (x & (1 << 11)) {
	   	bgc = bgc.darker(110);
	}
	this->setReadOnly(ro);
	this->setColor(bgc, fgc);
	QFont qf;
	if (totn > 0) {
		int ch(this->height());
		static const int xs[] = { 1, 1, 2, 2, 2, 2, 2, 2, 2, 3, 3 };
		ch = ch / 1.7;
		qf.setFamily("Monospace");
		qf.setPointSize(ch / xs[totn]);
		this->setFont(qf);
	}
}

bool NumGrid::eventFilter(QObject*, QEvent* evt) {
	if (evt->type() == QEvent::MouseButtonPress) {
		emit activeChanged(this->id);
	}
	return 0;
}

