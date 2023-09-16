#include <iostream>
#include <windows.h>
#include <bitset>
#include <fstream>
#include <chrono>
#include <vector>
#include <algorithm>

using namespace std;

void task1a() {
	unsigned char x = 255;                  //8-разрядное двоичное число 11111111
	unsigned char maska = 1;                //1=00000001 – 8-разрядная маска

	cout << "1a. Изначальное число: " << std::bitset<8>(x) << endl;

	x = x & (~(maska << 4));                //результат x=239

	cout << "Получившееся число: " << std::bitset<8>(x) << endl;
	cout << "Результат вычисления: x=" << (int) x << endl;
}

void task1b() {
	int b;
	unsigned char maska = 1;
	cout << "1б. Введите число: " << endl;
	cin >> b;

	if (b < 0 || b > 255) {
		cout << "Ошибочный ввод" << endl;
		return;
	}

	unsigned char x = (char) b;
	cout << "Изначальное число: " << std::bitset<8>(x) << endl;
	x = x | (maska << 6); // Изменение 7 бита

	cout << "Получившееся число: " << std::bitset<8>(x) << endl;
	cout << "Результат вычисления: x=" << (int) x << endl;
}

void task1c() {
	cout << "1в." << endl;


	unsigned int x = 25;
	const int n = sizeof(int) * 8;
	unsigned maska = (1 << n - 1); // 1 в старшем разряде

	cout << "Начальный вид маски: " << bitset<n>(maska) << endl;
	cout << "Результат: ";

	for (int i = 1; i <= n; ++i) {
		cout << ((x & maska) >> (n - i));
		maska = maska >> 1; // Смещение маски вправо
	}

	// Маска сдвигается каждый раз в цикле на 1 и проходит весь путь от старшего байта к младшему
	// При этом когда в разряде маски значение равно 1 и в разряде числа так же 1 на этом разряде, то она пишется в консоль
	// В итоге мы получаем исходное число в двоичной форме

	cout << endl;
}


void task1() {
	task1a();
	task1b();
	task1c();
}

void task2a() {

	cout << "2a" << endl << "Введите количество чисел для сортировки" << endl;
	int n;
	cin >> n;

	if (n > 8) {
		cout << "Количество чисел выходит за размер разрядной сетки";
	}

	int k;

	unsigned char bits = 0;
	unsigned char maska = 1;

	for (int i = 0; i < n; ++i) {
		cin >> k;

		if (k >= 8) {
			cout << "Неверный ввод, необходимо число < 8" << endl;
			i--;
			continue;
		}
		bits = bits | (maska << (sizeof(bits) * 8 - k - 1));
	}

	maska = 128;

	for (int i = 0; i < sizeof(bits) * 8; ++i) {
		if (bits & maska)
			cout << i << " ";
		maska = maska >> 1;
	}
	cout << endl;
}

void task2b() {

	cout << "2b" << endl << "Введите количество чисел для сортировки до 64" << endl;
	int n;
	cin >> n;

	if (n <= 0 | n > 64) {
		cout << "Количество чисел выходит за размер разрядной сетки";
	}

	int k;

	unsigned long long bits = 0;
	unsigned long long maska = 1;

	for (int i = 0; i < n; ++i) {
		cin >> k;
		if (k > 64) {
			cout << "Неверный ввод, необходимо число <= 64" << endl;
			i--;
			continue;
		}
		bits = bits | (maska << (sizeof(bits) * 8 - k - 1));    // Изменение k-ого бита на 1
	}

	maska = 1ULL << 63;

	for (int i = 0; i < sizeof(bits) * 8; ++i) {
		if (bits & maska)
			cout << i << " ";
		maska = maska >> 1;
	}
	cout << endl;
}

void task2c() {

	cout << "2c" << endl << "Введите количество чисел для сортировки до 64" << endl;
	int n;
	cin >> n;

	if (n <= 0 | n > 64) {
		cout << "Количество чисел выходит за размер разрядной сетки";
	}
	int k;

	// Инициализация массива
	unsigned char bits[8];
	for (int i = 0; i < 8; ++i) {
		bits[i] = 0;
	}

	unsigned char maska = 1;

	for (int i = 0; i < n; ++i) {
		cin >> k;
		if (k > 64) {
			cout << "Неверный ввод, необходимо число <= 64" << endl;
			i--;
			continue;
		}

		char bit = k % 8;
		char iter = k / 8;

		bits[iter] = bits[iter] | (maska << (sizeof(bits[iter]) * 8 - bit - 1));    // Изменение k-ого бита на 1
	}

	for (int i = 0; i < 8; ++i) {
		maska = 128;
		for (int j = 0; j < sizeof(bits[i]) * 8; ++j) {
			if (bits[i] & maska)
				cout << j + i * 8 << " ";
			maska = maska >> 1;
		}
	}

	cout << endl;
}

void task2() {
	task2a();
	task2b();
	task2c();
}

void fillInputFile() {
	std::ofstream fileOut;
	fileOut.open("sorting_file");

	for (int i = 0; i < 10000000; ++i) {
		fileOut << 10000000 - i - 1 << " ";
	}

	fileOut.close();
}

void task3() {

	vector<bool> array(10000000);
	for (int i = 0; i < 10000000; ++i) {
		array[i] = false;
	}

	fillInputFile();

	std::ifstream file;
	file.open("sorting_file");

	std::ofstream fileOut;
	fileOut.open("sorting_file_2");
	int num;
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	while (file >> num) {
		array[num] = true;
	}

	for (int i = 0; i < 10000000; ++i) {
		if (array[i]) {
			fileOut << i << " ";
		}
	}

	file.close();
	fileOut.close();

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Время выполнения = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
			  << "[ms]" << std::endl;
	std::cout << "Размер памяти в мегабайтах " << 10000000 / 1024 / 1024 / 8 << std::endl;
}


void task3another() {

	vector<unsigned long long> array(157000);
	for (int i = 0; i < array.size(); ++i) {
		array[i] = 0ULL;
	}

	fillInputFile();

	std::ifstream file;
	file.open("sorting_file");

	std::ofstream fileOut;
	fileOut.open("sorting_file_2");
	int num;
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	while (file >> num) {
		int bit = num % 64;
		int iter = num / 64;

		array[iter] = array[iter] | (1ULL << (bit - 1));
	}

	unsigned long long maska = 1ULL;

	for (int i = 0; i < array.size(); ++i) {
		maska = 1ULL;
		for (int j = 0; j < sizeof(array[i]) * 8; ++j) {
			if (array[i] & maska)
				fileOut << j + i * sizeof(array[i]) * 8 << " ";
			maska = maska << 1;
		}
	}


	file.close();
	fileOut.close();

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Время выполнения = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
			  << "[ms]" << std::endl;
	std::cout << "Размер памяти в мегабайтах " << sizeof(unsigned long long) * 157000 / 1024 / 1024 << std::endl;
}


int main() {
	SetConsoleOutputCP(CP_UTF8);

	task3();

	int c;
	cout << "Введите номер задания 1-3 или 0 для выхода" << endl;
	while ((cin >> c) && (c != 0)) {
		switch (c) {
			case 1:
				task1();
				break;
			case 2:
				task2();
				break;
			case 3:
				task3();
				break;
		}
	}
	return 0;
}