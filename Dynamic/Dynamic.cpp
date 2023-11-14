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
}

int main() {
	string title = "Welcome to working with dynamic programming method!\n\n";
	for (int i = 0; i < title.length(); i++) {
		cout << title[i];
		Sleep(15);
	}
	backpack();
}

