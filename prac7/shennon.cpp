#include <iostream>
#include "vector"
#include <algorithm>
#include <fstream>
#include "map"
#include "windows.h"

using namespace std;

struct cell {
	char letter;
	string code;
	double probability;

	cell(char letter, const string &code, double probability) : letter(letter), code(code),
																probability(probability) {}
};


void shannonFano(int left, int right, vector<cell *> &cells) {
	if (left == right - 1) {
		return;
	}
	int currentSequenceStart = left;
	int currentSequenceEnd = right - 1;
	int sequenceLeftEnd = 0;
	int sequenceRightStart = 0;
	double leftSum = 0, rightSum = 0, sum = 0;
	for (int i = currentSequenceStart; i <= currentSequenceEnd; i++) {
		sum += cells[i]->probability;
	}
	double minDiff = sum;
	for (int i = currentSequenceStart; i <= currentSequenceEnd; i++) {
		leftSum += cells[i]->probability;
		rightSum = sum - leftSum;
		if (abs(leftSum - rightSum) < minDiff) {
			minDiff = abs(leftSum - rightSum);
		} else {
			sequenceRightStart = i;
			sequenceLeftEnd = i - 1;
			break;
		}
	}
	for (int i = currentSequenceStart; i <= sequenceLeftEnd; i++) {
		cells[i]->code += "0";
	}
	for (int i = sequenceRightStart; i <= currentSequenceEnd; i++) {
		cells[i]->code += "1";
	}
	shannonFano(currentSequenceStart, sequenceLeftEnd + 1, cells);
	shannonFano(sequenceRightStart, currentSequenceEnd + 1, cells);
}

struct comparator {
	bool operator()(cell *lx, cell *rx) const {
		return lx->probability > rx->probability;
	}
};

// Сортировка по вероятностям
void sort(vector<cell *> &cells) {
	sort(cells.begin(), cells.end(), comparator());
}

vector<cell *> createLettersMap(string &str) {
	map<char, double> lettersMap;
	for (char letter: str) {
		lettersMap[letter] = 0;
	}
	for (char letter: str) {
		lettersMap[letter] += 1;
	}
	vector<cell *> cells;
	int i = 0;
	for (const auto &[letter, count]: lettersMap) {
		cells.emplace_back(new cell(letter, "", count / str.length()));
		i++;
	}
	sort(cells);
	return cells;
}

cell *findByCode(string code, vector<cell *> &cells) {
	for (auto &cell: cells) {
		if (cell->code == code)
			return cell;
	}
	return nullptr;
}

cell *findBySymbol(char symbol, vector<cell *> &cells) {
	for (auto cell: cells) {
		if (cell->letter == symbol)
			return cell;
	}
}

string encode(string str, vector<cell *> &cells) {
	string encoded;
	for (char letter: str) {
		encoded += findBySymbol(letter, cells)->code;
	}
	return encoded;
}

string decode(string &str, vector<cell *> &cells) {
	string decoded;
	string currentSequence;
	for (char letter: str) {
		currentSequence += letter;
		cell *crnt = findByCode(currentSequence, cells);
		if (crnt != nullptr) {
			decoded += crnt->letter;
			currentSequence = "";
		}
	}
	return decoded;
}


int main() {
	SetConsoleOutputCP(CP_UTF8);
//	setlocale(LC_ALL, "ru");
	std::ifstream infile("C:\\Users\\e-gul\\CLionProjects\\prac2\\prac7\\text.txt");
	string str;
	getline(infile, str);
//	string str = "Ана-дэус-рики-паки, Дормы-кормыконсту-таки, Энус-дэус-кана-дэусБАЦ!";
	vector<cell *> cells = createLettersMap(str);
	shannonFano(0, cells.size(), cells);
	cout << "Коды и вероятности символов" << endl;
	for (cell *i: cells) {
		cout << i->letter << "        " << i->code << "         " << i->probability << endl;
	}
	string encoded = encode(str, cells);
	cout << "Закодированная строка: " << encoded << endl;
	cout << "Раскодированная строка: " << decode(encoded, cells) << endl;

	return 0;
}