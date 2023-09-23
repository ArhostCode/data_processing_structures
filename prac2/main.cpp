#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <bits/stdc++.h>
#include "set"

using namespace std;

struct City {
	int code;
	char name[10];
};


struct CityReference {
public:
	int code;
	int offset;


	CityReference(int code, int offset) {
		this->code = code;
		this->offset = offset;
	}
};

char randomGen() {
	string charset = "abcdefghijklmnopqrstuvwxyz";
	return charset[rand() % charset.length()];
}

void generateFile(int count) {
	srand(time(NULL));
	vector<City> cities;
	for (int i = 0; i < count; ++i) {
		City city;
		city.code = rand() % 1000000;
		for (int j = 0; j < 10; ++j) {
			city.name[j] = randomGen();
		}
		cities.push_back(city);
	}

	ofstream wf("cities.dat", ios::out | ios::binary);

	if (!wf) {
		cout << "Cannot open file!" << endl;
		return;
	}
	for (int i = 0; i < cities.size(); i++)
		wf.write((char *) &cities[i], sizeof(City));
	wf.close();
}

void linearSearch(int code) {
	ifstream rf("cities.dat", ios::in | ios::binary);

	if (!rf) {
		cout << "Cannot open file!" << endl;
		return;
	}

	City *city = new City();

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	while (rf.read((char *) city, sizeof(City))) {
		if (city->code == code)
			break;
	}

	if (city->code == code) {
		cout << "Город найден " << city->code << " " << city->name << endl;
	} else {
		cout << "Город не найден" << endl;
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()
			  << "[microseconds]" << std::endl;

	rf.close();
}

void readFile() {
	vector<City *> cities;

	ifstream rf("cities.dat", ios::out | ios::binary);
	if (!rf) {
		cout << "Cannot open file!" << endl;
	}

	City *city = new City();

	while (rf.read((char *) city, sizeof(City))) {
		cities.push_back(city);
		city = new City();
	}

	for (int i = 0; i < cities.size(); ++i) {
		cout << cities[i]->code << " " << cities[i]->name << endl;
	}

	rf.close();
}

void generateFibonacciNumbers(std::vector<int> &fibonacciNumbers, int n) {
	int a = 0;
	int b = 1;
	while (b <= n) {
		fibonacciNumbers.push_back(b);
		int temp = a;
		a = b;
		b += temp;
	}
}

int min(int a, int b) {
	return a < b ? a : b;
}

int fibonacciSearch(std::vector<CityReference *> &arr, int x) {
	std::vector<int> fibonacciNumbers;
	generateFibonacciNumbers(fibonacciNumbers, arr.size());

	int offset = -1;
	int k = fibonacciNumbers.size() - 1;

	while (k > 0) {
		int i = min(offset + fibonacciNumbers[k], arr.size() - 1);

		if (arr[i]->code < x) {
			k -= 2;
			offset = i;
		} else if (arr[i]->code > x) {
			k -= 1;
		} else {
			return arr[i]->offset;
		}
	}

	return -1;
}

bool compareRefs(const CityReference *comp1, const CityReference *comp2) {
	return comp1->code < comp2->code;
}

void fibonacciSearchFile(int code) {
	ifstream rf("cities.dat", ios::in | ios::binary);

	if (!rf) {
		cout << "Cannot open file!" << endl;
		return;
	}

	vector<CityReference *> refs;

	City *city = new City();
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	int index = 0;
	while (rf.read((char *) city, sizeof(City))) {
		refs.emplace_back(new CityReference(city->code, index));
		index++;
	}
	rf.close();

	std::sort(refs.begin(), refs.end(), compareRefs);


	int offset = fibonacciSearch(refs, code);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()
			  << "[microseconds]" << std::endl;
	if (offset != -1) {
		FILE *fp = fopen("cities.dat", "rb");
		fseek(fp, sizeof(City) * offset, SEEK_SET);
		fread(city, sizeof(City), 1, fp);
		cout << "Город найден " << city->code << " " << city->name << endl;
	} else {
		cout << "Город не найден" << endl;
	}

}

int main() {
	SetConsoleOutputCP(CP_UTF8);

	int query = 0;
	while (query != -1) {
		cout << "Введите действие - 1 - генерация файла, 2 - чтение файла, 3 - линейный поиск, 4 - фибоначчи поиск"
			 << endl;
		cin >> query;
		switch (query) {
			case -1:
				return 0;
			case 1:
				int cnt;
				cout << "Введите количество записей:" << endl;
				cin >> cnt;
				generateFile(cnt);
				break;
			case 2:
				readFile();
				break;
			case 3:
				int s;
				cout << "Введите число для поиска записи:" << endl;
				cin >> s;
				linearSearch(s);
				break;
			case 4:
				int sf;
				cout << "Введите число для поиска записи:" << endl;
				cin >> sf;
				fibonacciSearchFile(sf);
				break;
			default:
				cout << "Неверное действие" << endl;
		}
	}

//	generateFile(100);
//	linearSearch(6000);
//	readFile();
//	cout << sizeof(City2) << endl;
//	fibonacciSearchFile(6443);

	return 0;
}