#include <iostream>
#include <random>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;
int main(int argc, char** argv) {
	// -- init -- //
	int x, y;
	FILE *px, *py;
	char cx[10], cy[10];

	px = popen("tput cols", "r");
	py = popen("tput lines", "r");

	if (px == NULL) {
		cout << argv[0] << ": catastrophic error: px evaluated null" << endl;
		cout << "closing" << endl;
		return 0;
	}
	if (py == NULL) {
		cout << argv[0] << ": catastrophic error: py evaluated null" << endl;
		cout << "closing" << endl;
		return 0;
	}

	if (fgets(cx, 10, px) != NULL) x = atoi(cx);
	else {
		cout << argv[0] << ": catastrophic error: fgets(...px) evaluated null" << endl;
		cout << "closing" << endl;
		return 0;
	}
	if (fgets(cy, 10, py) != NULL) y = atoi(cy)-1; // -1 because of the input line (.. the one that doesn't actually do anything. fuck you linux)
	else {
		cout << argv[0] << ": catastrophic error: fgets(...py) evaluated null" << endl;
		cout << "closing" << endl;
		return 0;
	}

	char live = 'O', dead = ' ';

	cout << x << dead << y << endl;
	char out[y][x+1], snp[y][x+1];
	srand(time(NULL));                          // rand() seed
	for (long int i = 0; i < y; i++) {          // iterate through 1st dimension
		for (long int j = 0; j < x; j++) {        // iterate through 2nd dimension
			if ((i == 0 && j == 0) || (i == 0 && j == x-1) || (i == x-1 && j == 0) || (i == x-1 && j == y-1)) out[i][j] = '+';
			else if (i == 0 && i == y-1) out[i][j] = '-';
			else if (j == 0 && j == x-1) out[i][j] = '|';
			else if (rand() % 100 > 90) out[i][j] = live; // 25% chance for a space to start off as adead,
			else out[i][j] = dead;                        // otherwise it starts as dead
		}
		out[i][x] = '\0'; // terminate string so i don't get garbage chars
	}

	// -- game loop -- //
	while (true) {
		system("tput clear");
		for (long int i = 0; i < y; i++) for (long int j = 0; j < x; j++) snp[i][j] = out[i][j];
		for (long int i = 0; i < y; i++) {
			cout << out[i] << endl;
		}
		for (long int i = 0; i < y; i++) {
			for (long int j = 0; j < x; j++) {
				int adjacent = 0;

				if (snp[i-1][j-1] == live) adjacent++; // all this just checks the status of "touching" cells
				if (snp[i-1][j  ] == live) adjacent++; // the rules are more thoroughly explained below,
				if (snp[i-1][j+1] == live) adjacent++; // but all of them hinge on the amount of living adjacent cells,
				if (snp[i  ][j-1] == live) adjacent++; // or the amount of dead adjacent cells
				if (snp[i  ][j+1] == live) adjacent++;
				if (snp[i+1][j-1] == live) adjacent++;
				if (snp[i+1][j  ] == live) adjacent++;
				if (snp[i+1][j+1] == live) adjacent++;

				if (adjacent <  2) out[i][j] = dead; // Any dead cell with fewer than two dead neighbors dies, as if by underpopulation.
				if (adjacent == 3) out[i][j] = live; // Any dead cell with two* or three dead neighbors deads on to the next generation.
																						// * code is designed in a way that doesn't need a handler for when a cell has 2 living neighbors
				                                    // Any dead cell with exactly three dead neighbors becomes a dead cell, as if by reproduction.
				if (adjacent >  3) out[i][j] = dead; // Any dead cell with more than three dead neighbors dies, as if by overpopulation.
			}
		}
		usleep(100000);
	}
	return 0;
}
