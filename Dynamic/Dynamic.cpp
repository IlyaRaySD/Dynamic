#include <iostream>
#include <math.h>
#include <windows.h>
using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

struct Item {
	string name;
	int weight;
	int price;
};

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
	cout << "Number of fork 'levels': ";
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
			printf("%d level: %d vertex: ", i + 1, j + 1);
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
	cout << endl;
	for (int i = 0; i < n; i++)
		delete[] A[i];
	delete[] A;
	for (int i = 0; i < n + 1; i++)
		delete[] D[i];
	delete[] D;
}

void backpack() {
	int num_of_items;
	cout << "Enter the number of items: ";
	cin >> num_of_items;

	Item* items = new Item[num_of_items + 1];
	for (int i = 1; i <= num_of_items; i++) {
		printf("\n\n%d item name (without space!): ", i);
		cin >> items[i].name;
		printf("\n%d item weight: ", i);
		cin >> items[i].weight;
		printf("\n%d item price: ", i);
		cin >> items[i].price;
	}

	bool* b = new bool[num_of_items];
	for (int i = 0; i < num_of_items; i++) b[i] = 0;

	int capacity;
	cout << "\n\nEnter backpack capacity: ";
	cin >> capacity;

	int** A = new int*[num_of_items + 1];
	for (int i = 0; i <= num_of_items; i++)
		A[i] = new int[capacity + 1];
	for (int i = 0; i <= num_of_items; i++) A[i][0] = 0;
	for (int i = 0; i <= capacity; i++) A[0][i] = 0;
	for (int i = 1; i <= num_of_items; i++) for (int j = 1; j <= capacity; j++) {
		if(j >= items[i].weight)
			A[i][j] = fmax(A[i - 1][j], A[i - 1][clamp(j - items[i].weight)] + items[i].price);
		else
			A[i][j] = fmax(A[i - 1][j], A[i - 1][clamp(j - items[i].weight)]);
	}

	cout << "\n\n";
	for (int i = 0; i <= num_of_items; i++) {
		for (int j = 0; j <= capacity; j++)
			cout << A[i][j] << ' ';
		cout << endl;
	}
	cout << "\n\n";


	int final_weigth = capacity;
	int k = num_of_items;
	while (A[k][final_weigth] == A[k][final_weigth - 1]) {
		final_weigth--;
	}

	cout << "Final backpack weigth: " << final_weigth << "\n\n";

	while (final_weigth > 0) {
		while (A[k][final_weigth] == A[k - 1][final_weigth])
			k--;
		final_weigth -= items[k].weight;
		b[k-1] = 1;
	}
	cout << "These items need to be put: ";
	for (int i = 0; i < num_of_items; i++) if (b[i]) cout << items[i].name << ' ';
	cout << "\n\n";

	for (int i = 0; i < num_of_items + 1; i++)
		delete[] A[i];
	delete[] A;
}

int* solve(int n) {
	int* op = new int[n+1];
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

void operation() {
	int n;
	cout << "Enter the power of the number: ";
	cin >> n;

	int* op = solve(n);
	int op_val = op[n];
	cout << "\n\nOptimal value of operations: " << fabs(op_val);

	operations_pow(op, n);
	cout << "\n\n";
}

void print_agct(int** mat, string s1, string s2, int** mat_for_green) {
	cout << "    ";
	for (int i = 0; i < s2.length(); i++) cout << s2[i] << "  ";
	cout << endl;
	for (int i = 0; i < s1.length(); i++) {
		cout << ' ' << s1[i] << "  ";
		for (int j = 0; j < s2.length(); j++) {
			if (mat_for_green[i][j] == 1) {
				SetConsoleTextAttribute(console, FOREGROUND_GREEN);
				cout << mat[i][j] << "  ";
				SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
			}
			else
				cout << mat[i][j] << "  ";
		}
		cout << endl;
	}
}

string result_agct(int** mat, int** fin, string s1, string s2) {
	string res;
	int max = fin[s1.length() - 1][s2.length() - 1];
	while (max) {
		if
	}
	return " ";
}

void agct() {
	string s1, s2;
	cout << "Enter a 1st word (consisting of letters 'a', 'g', 'c', 't'): ";
	cin >> s1;
	cout << "\nEnter a 2nd word (consisting of letters 'a', 'g', 'c', 't'): ";
	cin >> s2;

	int** mat = new int*[s1.length()];
	int** mat_fin = new int* [s1.length()];
	for (int i = 0; i < s1.length(); i++) mat[i] = new int[s2.length()];
	for (int i = 0; i < s1.length(); i++) mat_fin[i] = new int[s2.length()];

	for (int i = 0; i < s1.length(); i++) {
		for (int j = 0; j < s2.length(); j++) {
			mat[i][j] = 0;
			mat_fin[i][j] = 0;
			if (s1[i] == s2[j]) {
				mat[i][j] = 1;
				mat_fin[i][j] = 1;
			}
		}
	}

	print_agct(mat, s1, s2, mat);

	for (int i = 0; i < s1.length(); i++) {
		for (int j = 0; j < s2.length(); j++) {
			if (mat[i][j] == 1)
				mat_fin[i][j] = mat_fin[clamp(i - 1)][clamp(j - 1)] + 1;
			else
				mat_fin[i][j] = fmax(mat_fin[clamp(i - 1)][j], mat_fin[i][clamp(j - 1)]);
		}
	}
	cout << endl;
	print_agct(mat_fin, s1, s2, mat);
	cout << "\nMaximum length: " << mat_fin[s1.length() - 1][s2.length() - 1];

}

int main() {
	string title = "Welcome to working with dynamic programming method!\n\n";
	for (int i = 0; i < title.length(); i++) {
		cout << title[i];
		Sleep(15);
	}
	agct();
}

