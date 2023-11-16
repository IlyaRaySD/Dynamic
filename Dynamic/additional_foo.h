#pragma once

#include <iostream>
#include <math.h>
#include <windows.h>

int clamp(int a) { return (a > 0) ? a : 0; }

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
