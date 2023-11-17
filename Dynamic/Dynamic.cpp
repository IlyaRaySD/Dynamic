// including libraries
#include <iostream>
#include <math.h>
#include <windows.h>
#include "additional_foo.h" // including a header file with additional functions
using namespace std;

// the variable contains console parameters
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

// structure for convenient storage of information about the subject
struct Item {
	string name;
	int weight;
	int price;
};

// function returning the menu item selected by the user
int opt_catch() {
	cout << "\nselect algorithm:\n";
	cout << "1 - Triangle problem\n2 - Turtle problem\n3 - Power of a number\n4 - Biological problem\n5 - Backpack problem\n0 - Quit\n\nEnter the number: ";
	int choose;
	cin >> choose;
	return choose;
}

// function to print the matrix for the triangle problem
void print_mat_triangle(int** mat, int m, int n, int* path) {
	cout << "  ";
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			string space(4 - length_of_num(mat[i][j]), ' '); // variable for calculating neat indentation between matrix elements
			if (j == path[i]) {
				SetConsoleTextAttribute(console, FOREGROUND_RED); // changing console text color to red
				cout << mat[i][j] << space;
				SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED); // changing console text color to white
			}
			else
				cout << mat[i][j] << space;
		}
		cout << endl << "  ";
	}
}

// function called when choosing a triangle problem
void triangle() {
	int n; // number of branches
	cout << "\nNumber of fork 'levels': ";
	cin >> n;
	int** A = new int* [n]; // matrix burden 
	int** D = new int* [n]; // matrix total burden
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

	// filling the matrix with maximum burden
	for (int i = 1; i < n; i++)
		for (int j = 1; j < n + 1; j++)
			D[i][j] = D[i][j] + fmax(D[i - 1][j - 1], D[i - 1][j]);

	int* path_D = finding_path(D, n); // finding a path 
	int* path_A = finding_path(D, n); // for maximum burden
	for (int i = 0; i < n; i++)
		path_A[i]--;
	
	cout << "\nD:\n\n";
	print_mat_triangle(D, n, n + 1, path_D);

	cout << endl;

	cout << "\nA:\n\n";
	print_mat_triangle(A, n, n, path_A);

	cout << "\n\n\nPath (sequence of vertices) marked ";
	SetConsoleTextAttribute(console, FOREGROUND_RED); // changing console text color to red
	cout << "red";
	SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED); // changing console text color to white
	cout << endl;
	for (int i = 0; i < n; i++)
		delete[] A[i];
	delete[] A;
}

// function called when selecting the backpack problem
void backpack() {
	int num_of_items; // number of items
	cout << "\nEnter the number of items: ";
	cin >> num_of_items;

	Item* items = new Item[num_of_items + 1]; // array of items with all their characteristics
	// entering the characteristics of all items
	for (int i = 1; i <= num_of_items; i++) {
		printf("\n\n%d item name (without space!): ", i);
		cin >> items[i].name;
		printf("\n%d item weight: ", i);
		cin >> items[i].weight;
		printf("\n%d item price: ", i);
		cin >> items[i].price;
	}

	bool* b = new bool[num_of_items]; // an array containing whether to put an item in the backpack or not
	for (int i = 0; i < num_of_items; i++) b[i] = 0;

	int capacity; // capacity of backpack
	cout << "\n\nEnter backpack capacity: ";
	cin >> capacity;

	int** A = new int*[num_of_items + 1];  // matrix for dynamic analysis
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

	// output to the console of the dynamic analysis matrix
	cout << '\n';
	for (int i = 0; i <= num_of_items; i++) {
		cout << endl;
		for (int j = 0; j <= capacity; j++)
			cout << A[i][j] << ' ';
		
	}
	cout << " - max price";
	cout << "\n\n";


	int final_weigth = capacity; // final weight of the backpack after filling with items
	int k = num_of_items;
	while (A[k][final_weigth] == A[k][final_weigth - 1]) {
		final_weigth--;
	}

	cout << "Final backpack weigth: " << final_weigth << "\n\n";

	// calculating items that need to be placed for maximum benefit
	while (final_weigth > 0) {
		while (A[k][final_weigth] == A[k - 1][final_weigth])
			k--;
		final_weigth -= items[k].weight;
		b[k-1] = 1;
	}
	cout << "These items need to be put: ";
	for (int i = 0; i < num_of_items; i++) if (b[i]) cout << items[i+1].name << ' ';
	cout << "\n\n";

	for (int i = 0; i < num_of_items + 1; i++)
		delete[] A[i];
	delete[] A;
}

// function called when selected power of a number
void operation() {
	int n; // power
	cout << "\nEnter the power of the number: ";
	cin >> n;

	int* op = solve(n); // optimal number of actions for each degree up to n
	int op_val = op[n];
	cout << "\n\nOptimal value of operations: " << fabs(op_val);

	operations_pow(op, n); // derivation of optimal actions using the example of variable k
	cout << "\n\n";
}

// the function outputs to the console a matrix of intersections of sequence symbols
void print_agct(int** mat, string s1, string s2, int** mat_for_green) {
	cout << "    ";
	for (int i = 0; i < s2.length(); i++) cout << s2[i] << "  ";
	cout << endl;
	for (int i = 0; i < s1.length(); i++) {
		cout << ' ' << s1[i] << "  ";
		for (int j = 0; j < s2.length(); j++) {
			if (mat_for_green[i][j] == 1) {
				SetConsoleTextAttribute(console, FOREGROUND_GREEN); // changing console text color to green
				cout << mat[i][j] << "  ";
				SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED); // changing console text color to white
			}
			else
				cout << mat[i][j] << "  ";
		}
		cout << endl;
	}
}

// the function prints the maximum subsequence
string result_agct(int** mat, int** fin, string s1, string s2) {
	string res = "";
	int lon = 0;
	int lonx = 0;
	int lony = 0;
	bool flag = false;
	for (int i = s1.length() - 1; i > 0; i--) {
		for (int j = s2.length() - 1; j > 0; j--) {
			if (mat[i][j] == 1) {
				lon = fin[i][j];
				flag = true;
				lonx = i;
				lony = j;
				break;
			}

		}
		if (flag)
			break;
	}
	res += s1[lonx];
	for (int k = 0; k < lon - 1; k++) {
		if (mat[lonx - 1][lony - 1] == 1) {
			lonx--;
			lony--;
			res += s1[lonx];
			continue;
		}
		else {
			for (int i = lonx; i > lonx - 3 and flag; i--) {
				for (int j = lony; j > lony - 3 and flag; j--) {
					if (i != lonx or j != lony) {
						if (mat[i][j] == 1) {
							lonx = i;
							lony = j;
							flag = false;
						}
					}
				}
			}
		}
		res += s1[lonx];
		flag = true;
	}
	return res;
}

// function called when selected biological problem
void agct() {
	string s1, s2; // first and second subsequence
	cout << "\nEnter a 1st word (consisting of letters 'a', 'g', 'c', 't'): ";
	cin >> s1;
	cout << "\nEnter a 2nd word (consisting of letters 'a', 'g', 'c', 't'): ";
	cin >> s2;

	int** mat = new int*[s1.length()]; // matrix of shaded cells
	int** mat_fin = new int*[s1.length()]; // dynamic analysis matrix
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
	cout << "\nResult subsequence: ";
	for (int i = result_agct(mat, mat_fin, s1, s2).length() - 1; i >= 0; i--)
		cout << result_agct(mat, mat_fin, s1, s2)[i];
	cout << "\n\n";
	for (int i = 0; i < s1.length(); i++)
		delete[] mat[i];
	delete[] mat;
	for (int i = 0; i < s1.length(); i++)
		delete[] mat_fin[i];
	delete[] mat_fin;
} 

// function called when selected turtle problem
void turtle() {
	int m, n; // intersections vertically and horizontally
	cout << "\nEnter the number of horizontal intersections: ";
	cin >> m;
	cout << "\nEnter the number of vertical intersections: ";
	cin >> n;

	int** roads = new int* [n * 2 - 1]; // matrix of all roads between intersections
	for (int i = 0; i < n * 2 - 1; i++) roads[i] = new int[m];

	for (int i = 0; i < n * 2 - 1; i++) for (int j = 0; j < m; j++) roads[i][j] = 0;

	for (int i = 0; i < n * 2 - 1; i++) {
		if (i % 2) {
			cout << '\n';
			for (int j = 0; j < m; j++) {
				printf("Enter vertical roads between intersections (%d, %d) and (%d, %d):", i / 2 + 1, j + 1, i / 2 + 2, j + 1);
				cin >> roads[i][j];
			}
		}
		else {
			cout << '\n';
			for (int j = 0; j < m - 1; j++) {
				printf("Enter gorizontal roads between intersections (%d, %d) and (%d, %d):", i/2 + 1, j + 1, i/2 + 1, j + 2);
				cin >> roads[i][j];
			}
		}
	}
	cout << "\n\n" << "Matrix of roads:\n";
	for (int i = 0; i < n * 2 - 1; i++) {
		for (int j = 0; j < n; j++)
			cout << roads[i][j] << ' ';
		cout << endl;
	}
	cout << "\n\n" << "Matrix of minimum distances from the intersection to the upper right intersection:\n";
	int** inter = min_path(roads, m, n); // matrix of minimum distances from all intersections to the upper right intersection
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++)
			cout << inter[i][j] << ' ';
		cout << endl;
	}



}

int main() {
	string title = "Welcome to working with dynamic programming method!\n\n";
	for (int i = 0; i < title.length(); i++) {
		cout << title[i];
		Sleep(15);
	}
	string question = "\nContinue?(1 - yes, 0 - no)"; // text with a question about continuing the program
link: // mark
	int c; // user selected option
	c = opt_catch();
	if (c == 1) {
		triangle();
		bool ask = 0;
		cout << question << endl;
		cin >> ask;
		if (ask == 1) goto link; // if continuation is selected
		else system("pause"); // end of the program
	}
	if (c == 2) {
		turtle();
		bool ask = 0;
		cout << question << endl;
		cin >> ask;
		if (ask == 1) goto link; // if continuation is selected
		else system("pause"); // end of the program
	}
	if (c == 3) {
		operation();
		bool ask = 0;
		cout << question << endl;
		cin >> ask;
		if (ask == 1) goto link; // if continuation is selected
		else system("pause"); // end of the program
	}
	if (c == 4) {
		agct();
		bool ask = 0;
		cout << question << endl;
		cin >> ask;
		if (ask == 1) goto link; // if continuation is selected
		else system("pause"); // end of the program
	}
	if (c == 5) {
		backpack();
		bool ask = 0;
		cout << question << endl;
		cin >> ask;
		if (ask == 1) goto link; // if continuation is selected
		else system("pause"); // end of the program
	}
	if (c == 0) {
		system("pause"); // end of the program
	}
	if (c > 8 or c < 0) { // if the option is not in the instructions
		cout << endl;
		cout << "Please follow the instructions!";
		cout << endl;
		goto link;
	}
}
// the end of the code


