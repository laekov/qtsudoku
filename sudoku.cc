#include <QtDebug>
#include <QTextEdit>
#include <QTextEdit>
#include <QPalette>
#include <QColor>

#include "sudoku.hh"

void Sudoku::generate(int diffLv) {
	((Sudokore*)this)->generate(diffLv);
	for (int i = 0; i < 81; ++ i) {
		int tmp;
		if ((tmp = this->get(i))) {
			this->edits[i]->setPlainText(QString("%1").arg(tmp));
			this->edits[i]->setReadOnly(1);
			QPalette pte(this->edits[i]->palette());
			pte.setColor(QPalette::Base, QColor(233, 233, 0));
			this->edits[i]->setPalette(pte);
		}
	}
}

void Sudoku::onNumberChange(int id) {
	QTextEdit* sdr(qobject_cast<QTextEdit*>(this->sender()));
	bool errHdl;
	int chgId(sdr->windowTitle().toInt(&errHdl));
	if (!errHdl) {
		return;
	}
	int chgNum(sdr->toPlainText().toInt(&errHdl));
	if (!errHdl || chgNum <= 0 || chgNum > 9) {
		QPalette pte(sdr->palette());
		pte.setColor(QPalette::Base, QColor(233, 99, 99));
		sdr->setPalette(pte);
		return;
	}
	QPalette pte(sdr->palette());
	pte.setColor(QPalette::Base, QColor(255, 255, 255));
	sdr->setPalette(pte);
	this->set(chgId, chgNum);
	if (this->findConflict(chgId)) {
		QPalette pte(sdr->palette());
		pte.setColor(QPalette::Base, QColor(233, 99, 99));
		sdr->setPalette(pte);
	}
	if (this->isFinished()) {
		puts("You win!!!");
	}
}

