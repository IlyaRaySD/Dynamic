//this header file stores all the additional functions
#pragma once

#include <iostream>
#include <math.h>
#include <windows.h>

// function limits values ​​between 0 and >0
int clamp(int a) { return (a > 0) ? a : 0; }

// function returns number digit
int length_of_num(int num) {
	int l = 0;
	do {
		l++;
		num /= 10;
	} while (num);
	return l;
}

// function for finding the maximum path in a triangle
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

// function for calculating the optimal number of operations when raising to a power
int* solve(int n) {
	int* op = new int[n + 1];
	op[1] = 0, op[0] = 0;
	int i, j;
	for (i = 2; i <= n; i++) {
		op[i] = fabs(op[i - 1]) + 1;
		for (j = 2; j <= i - 1; j++) {
			op[i] = fmin(op[i], fabs(op[j]) + fabs(op[i - j]) + 1);
			if (i % j == 0)
				op[i] = fmin(fabs(op[i]), fabs(op[i / j]) + j - 1) * -1;
		}
	}
	return op;
}

// the function displays optimal actions using the variable k as an example
void operations_pow(int* oper, int n) {
	for (int i = 1; i <= n; i++) {
		if (oper[i] < 0) {
			int j = 2;
			while (i % j != 0)
				j++;
			printf("\n  (k^%d)^%d  (power %d)", j, i / j, i);
		}
		else
			printf("\n  k^%d*k  (power %d)", i - 1, i);
	}
}

// function returns a matrix of minimum distances from all intersections to the upper right intersection
int** min_path(int** roads, int m, int n) {
	int** inter = new int* [m];
	for (int i = 0; i < m; i++) inter[i] = new int[n];
	for (int i = 0; i < m; i++) for (int j = 0; j < n; j++) inter[i][j] = 0;

	for (int i = 1; i < m; i++)
		inter[i][n - 1] = inter[i - 1][n - 1] + roads[(i - 1) * 2 + 1][n - 1];
	for (int i = n - 2; i >= 0; i--)
		inter[0][i] = inter[0][i + 1] + roads[0][i];

	for (int i = 1; i < m; i++) {
		for (int j = n - 2; j >= 0; j--) {
			inter[i][j] = fmin(inter[i - 1][j] + roads[(i - 1) * 2 + 1][j], inter[i][j + 1] + roads[i * 2][j]);
		}
	}

	return inter;
}
