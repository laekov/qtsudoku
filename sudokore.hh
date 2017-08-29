#ifndef SUDOKORE_HH
#define SUDOKORE_HH
#include <cstring>

class Sudokore {
	private:
		int a[81], csol, s[81], o[9];
		void solveDFS(int, int*, int*, int*);
	public:
		Sudokore() {
			memset(a, 0, sizeof(a));
			for (int i = 0; i < 9; ++ i) {
				o[i] = i + 1;
			}
		}
		int get(int);
		void set(int, int);
		int solve(bool = true);
		int generate(int = 50);
		bool findConflict(int);
		bool isFinished();
};
#endif
