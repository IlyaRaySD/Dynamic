#include <iostream>
#include <math.h>
#include <string>
#include <windows.h>
using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

int length_of_num(int num) {
	int l = 0;
	do {
		l++;
		num /= 10;
	} while (num);
	return l;
}

int* finding_path(int** mat, int n) {
	int max = mat[n - 1][0];
	int max_index;
	int* red_indexes = new int[n];
	for (int i = 1; i <= n; i++) {
		if (max < mat[n - 1][i]) {
			max = mat[n - 1][i];
			max_index = i;
		}
	}
	red_indexes[n - 1] = max_index;
	for (int i = n - 2; i >= 0; i--) {
		(mat[i][red_indexes[i + 1]] > mat[i][red_indexes[i + 1] - 1]) ? red_indexes[i] = red_indexes[i + 1] : red_indexes[i] = red_indexes[i + 1] - 1;
	}
	return red_indexes;
}

void print_mat_triangle(int** mat, int m, int n, int* path) {
	cout << "  ";
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			string space(4 - length_of_num(mat[i][j]), ' ');
			if (j == path[i]) {
				SetConsoleTextAttribute(console, FOREGROUND_RED);
				cout << mat[i][j] << space;
				SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
			}
			else
				cout << mat[i][j] << space;
		}
		cout << endl << "  ";
	}
}

void triangle() {
	int n;
	cout << "number of fork 'levels': ";
	cin >> n;
	int** A = new int* [n];
	int** D = new int* [n];
	for (int i = 0; i < n; i++) {
		A[i] = new int[n];
		D[i] = new int[n + 1];
	}
	for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) A[i][j] = 0;
	for (int i = 0; i < n; i++) {
		printf("\n%d level of triangle(%d numbers):\n\n", i + 1, i + 1);
		for (int j = 0; j <= i; j++) {
			printf("%d level: %d verticle: ", i + 1, j + 1);
			cin >> A[i][j];
			cout << '\r';
		}
	}

	for (int i = 0; i < n; i++) {
		D[i][0] = 0;
		for (int j = 1; j <= n; j++)
			D[i][j] = A[i][j - 1];
	}

	for (int i = 1; i < n; i++)
		for (int j = 1; j < n + 1; j++)
			D[i][j] = D[i][j] + fmax(D[i - 1][j - 1], D[i - 1][j]);

	int* path_D = finding_path(D, n);
	int* path_A = finding_path(D, n);
	for (int i = 0; i < n; i++)
		path_A[i]--;
	

	cout << "\nD:\n\n";
	print_mat_triangle(D, n, n + 1, path_D);

	cout << endl;

	cout << "\nA:\n\n";
	print_mat_triangle(A, n, n, path_A);
	cout << "\n\n\nPath (sequence of vertices) marked ";
	SetConsoleTextAttribute(console, FOREGROUND_RED);
	cout << "red";
	SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}

int main() {
	triangle();
}

