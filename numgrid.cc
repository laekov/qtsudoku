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
		bgc = QColor(200, 200, 200);
		this->setText(QString("%1").arg(ns[0]));
		ro = 1;
	} else if (totn == 0) {
		this->setText("");
		bgc = QColor(255, 255, 255);
	} else {
		QString t("");
		for (int i = 0; i < totn; ++ i) {
			t += QString("%1 ").arg(ns[i]);
		}
		this->setText(t);
		if (totn == 1) {
			bgc = QColor(200, 255, 200);
		} else {
			bgc = QColor(200, 200, 255);
		}
	}
	if (x & (1 << 10)) {
		bgc = Qt::black, fgc = Qt::white;
	} else if (x & (1 << 11)) {
		fgc = Qt::white, bgc = Qt::darkBlue;
	}
	this->setReadOnly(ro);
	this->setColor(bgc, fgc);
}

bool NumGrid::eventFilter(QObject*, QEvent* evt) {
	if (evt->type() == QEvent::MouseButtonPress) {
		emit activeChanged(this->id);
	}
	return 0;
}

