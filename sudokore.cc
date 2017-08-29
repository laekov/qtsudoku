#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <algorithm>
#include "sudokore.hh"

void Sudokore::solveDFS(int i, int* r, int* l, int* b) {
	if (i == 81) {
		++ this->csol;
		return;
	} else if (this->csol > 1) {
		return;
	}
	if (this->a[i] != 0) {
		this->s[i] = this->a[i];
		solveDFS(i + 1, r, l, b);
	} else {
		int ri(i / 9), li(i % 9), bi(i / 9 / 3 * 3 + i % 9 / 3);
		for (int ti = 0; ti < 9 && this->csol < 2; ++ ti) {
			int v(this->o[ti]);
			if (((r[ri] & (1 << v)) == 0) && ((l[li] & (1 << v)) == 0) && ((b[bi] & (1 << v)) == 0)) {
				r[ri] |= 1 << v;
				l[li] |= 1 << v;
				b[bi] |= 1 << v;
				this->s[i] = v;
				this->solveDFS(i + 1, r, l, b);
				r[ri] &= ~(1 << v);
				l[li] &= ~(1 << v);
				b[bi] &= ~(1 << v);
			}
		}
	}
}

int Sudokore::get(int x) {
	if (x >= 0 && x < 81) {
		return this->a[x];
	} else {
		return -1;
	}
}

void Sudokore::set(int x, int v) {
	if (x >= 0 && x < 81 && v > 0 && v < 10) {
		this->a[x] = v;
	}
}

int Sudokore::solve(bool needReshuffle) {
	int r[9], l[9], b[9];
	this->csol = 0;
	memset(r, 0, sizeof(r));
	memset(l, 0, sizeof(l));
	memset(b, 0, sizeof(b));
	srand(time(0));
	if (needReshuffle) {
		std::random_shuffle(this->o, this->o + 9);
	}
	for (int i = 0; i < 81; ++ i) {
		if (this->a[i]) {
			int ri(i / 9), li(i % 9), bi(i / 9 / 3 * 3 + i % 9 / 3);
			int v(a[i]);
			if (((r[ri] & (1 << v)) == 0) && ((l[li] & (1 << v)) == 0) && ((b[bi] & (1 << v)) == 0)) {
				return 0;
			}
			r[ri] |= 1 << v;
			l[li] |= 1 << v;
			b[bi] |= 1 << v;
		}
	}
	this->solveDFS(0, r, l, b);
	return this->csol;
}

int Sudokore::generate(int diffLv) {
	int od[81];
	for (int i = 0; i < 81; ++ i) {
		od[i] = i;
	}
	std::random_shuffle(od, od + 81);
	memset(this->a, 0, sizeof(this->a));
	this->solve();
	memcpy(this->a, this->s, sizeof(this->s));
	int d(81);
	for (int i = 0; i < 81 && d > diffLv; ++ i) {
		int tmpv(this->a[od[i]]);
		this->a[od[i]] = 0;
		if (this->solve() == 2) {
			this->a[od[i]] = tmpv;
		} else {
			-- d;
		}
	}
	for (int i = 0; i < 81; ++ i) {
		fprintf(stderr, "%d ", s[i]);
		if (i % 9 == 8) {
			fprintf(stderr, "\n");
		}
	}
	return d;
}

bool Sudokore::findConflict(int i) {
	int ri(i / 9), li(i % 9), bi(i / 9 / 3 * 3 + i % 9 / 3);
	if (this->a[i] <= 0 || this->a[i] > 9) {
		return 1;
	}
	for (int j = 0; j < 9; ++ j) {
		if (this->a[ri * 9 + j] == this->a[i] && j != li) {
			return 1;
		}
		if (this->a[j * 9 + li] == this->a[i] && j != ri) {
			return 1;
		}
	}
	int bx(bi / 3 * 3), by(bi % 3 * 3);
	for (int px = 0; px < 3; ++ px) {
		for (int py = 0; py < 3; ++ py) {
			if (bx + px == ri && by + py == li) {
				continue;
			}
			if (this->a[(bx + px)* 9 + by + py] == this->a[i]) {
				return 1;
			}
		}
	}
	return 0;
}

bool Sudokore::isFinished() {
	for (int i = 0; i < 81; ++ i) {
		if (this->findConflict(i)) {
			return 0;
		}
	}
	return 1;
}

