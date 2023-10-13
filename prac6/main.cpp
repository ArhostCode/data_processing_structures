#include <iostream>
#include <vector>
#include <climits>
#include <iomanip>
#include "windows.h"

using namespace std;

void printPath(vector<vector<int>> const &path, vector<vector<int>> const &cost, int v, int u) {
	if (path[v][u] == -1 || v >= path.size() || u >= path.size()) {
		cout << "Нет пути" << endl;
		return;
	}
	if (path[v][u] == v) {
		cout << u + 1 << "(стоимость " << cost[path[v][u]][u] << ")" << " ";
		return;
	}
	printPath(path, cost, v, path[v][u]);
	cout << u + 1 << "(стоимость " << cost[path[v][u]][u] << ")" << " ";
}


void floydWarshall(vector<vector<int>> const &adjMatrix, int vertexFromId, int vertexToId) {
	int n = adjMatrix.size();

	if (n == 0) {
		return;
	}

	vector<vector<int>> cost(n, vector<int>(n));
	vector<vector<int>> path(n, vector<int>(n));

	for (int v = 0; v < n; v++) {
		for (int u = 0; u < n; u++) {

			cost[v][u] = adjMatrix[v][u];

			if (v == u) {
				path[v][u] = 0;
			} else if (cost[v][u] != INT_MAX) {
				path[v][u] = v;
			} else {
				path[v][u] = -1;
			}
		}
	}

	for (int k = 0; k < n; k++) {
		for (int v = 0; v < n; v++) {
			for (int u = 0; u < n; u++) {
				if (cost[v][k] != INT_MAX && cost[k][u] != INT_MAX
					&& cost[v][k] + cost[k][u] < cost[v][u]) {
					cost[v][u] = cost[v][k] + cost[k][u];
					path[v][u] = path[k][u];
				}
			}
		}
	}

	cout << "Кратчайший путь: " << vertexFromId + 1 << " ";
	printPath(path, adjMatrix, vertexFromId, vertexToId);
}

int main() {
	SetConsoleOutputCP(CP_UTF8);
	int I = INT_MAX;

	string choice;
	while (choice != "exit") {
		cout
				<< "Выберите задание. given для данном в задании графе, generate - для создания графа"
				<< endl;
		cin >> choice;
		if (choice == "given") {
			vector<vector<int>> adjMatrix =
					{
							{0, 2, 4, 5, I, I},
							{I, 0, I, 4, 3, I},
							{I, I, 0, I, I, 7},
							{I, I, I, 0, 6, 4},
							{I, I, I, I, 0, 2},
							{I, I, I, I, I, 0}
					};

			floydWarshall(adjMatrix, 0, 5);
			cout << endl;
		}
		if (choice == "generate") {
			int n;
			cout << "Введите размер матрицы смежности" << endl;
			cin >> n;
			cout << "Введите матрицу смежности, без пути - 999999" << endl;
			vector<vector<int>> adjMatrix = vector<vector<int>>(n, vector<int>(n));
			for (int i = 0; i < n; ++i) {
				for (int j = 0; j < n; ++j) {
					int c;
					cin >> c;
					if (c == 999999) {
						adjMatrix[i][j] = I;
					} else {
						adjMatrix[i][j] = c;
					}
				}
			}

			int from;
			int to;
			cout << "Введите вершины для которых выполнится поиск" << endl;
			cin >> from;
			cin >> to;

			floydWarshall(adjMatrix, from - 1, to - 1);
			cout << endl;
		}

	}

	return 0;
}