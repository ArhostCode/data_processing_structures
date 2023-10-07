#include <iostream>
#include <vector>
#include <windows.h>
#include <algorithm>
#include <map>
#include <sstream>

using namespace std;

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

bool check(char c) {
	return isalpha(c) || isdigit(c);
}

void replaceFirstAndLast(string &text) {

	if (!check(text[0]) || !check(text[text.size() - 1])) {
		cout
				<< "Неверно поданный текст. Текст начинается и заканчивается словом и имеет минимум 2, разделённых знаками препинания"
				<< endl;
		return;
	}
	string firstWord;
	string lastWord;

	int indexFirst = 0;
	int indexLast = 0;
	for (int i = 0; i < text.size(); ++i) {
		if (isdigit(text[i]) || isalpha(text[i])) {
			firstWord += text[i];
		} else {
			indexFirst = i;
			break;
		}
	}

	for (int i = text.size() - 1; i >= 0; --i) {
		if (isdigit(text[i]) || isalpha(text[i])) {
			lastWord += text[i];
		} else {
			indexLast = i;
			break;
		}
	}
	std::reverse(lastWord.begin(), lastWord.end());

	cout << lastWord + text.substr(indexFirst, indexLast - indexFirst + 1) + firstWord << endl;
}


map<string, int> search(vector<string> pat, string txt, int q) {
	map<string, int> result;

	for (auto pattern: pat) {
		int patternSize = pattern.size();
		int stringSize = txt.size();
		int i, j;
		int patternHashValue = 0;
		int textHashValue = 0;
		int h = 1;
		int d = 256;
		for (i = 0; i < patternSize - 1; i++)
			h = (h * d) % q;
		if (patternSize > stringSize)
			continue;

		for (i = 0; i < patternSize; i++) {
			patternHashValue = (d * patternHashValue + pattern[i]) % q;
			textHashValue = (d * textHashValue + txt[i]) % q;
		}

		for (i = 0; i < stringSize; i++) {
			if (patternHashValue == textHashValue) {
				for (j = 0; j < patternSize; j++) {
					if (txt[i + j] != pattern[j]) {
						break;
					}
				}
				if (j == patternSize)
					result[pattern] += 1;
			}
			if (i < stringSize - patternSize) {
				textHashValue = (d * (textHashValue - txt[i] * h) + txt[i + patternSize]) % q;
				if (textHashValue < 0)
					textHashValue = (textHashValue + q);
			}
		}
	}
	return result;
}

int simpleSearch(string pattern, string text) {
	for (int i = 0; i < text.size(); ++i) {
		for (int j = 0; j < pattern.size(); ++j) {
			if (i + j >= text.size())
				return -1;
			if (text[i + j] != pattern[j])
				break;
			if (j == pattern.size() - 1)
				return i;
		}
	}
	return -1;
}

int main() {
	SetConsoleOutputCP(CP_UTF8);
	string choice;
	while (choice != "exit") {
		cout
				<< "Выберите задание. exchange для обмена 1 и последнего, rabin - подсчёт подстрок в строке, simple - для простого поиска подстроки в строке"
				<< endl;
		getline(cin, choice);
		if (choice == "exchange") {
			string text;
			getline(cin, text);
			replaceFirstAndLast(text);
		}
		if (choice == "rabin") {
			string text;
			cout << "Введите токены через пробел" << endl;
			getline(cin, text);
			vector<string> s;
			split(text, ' ', s);
			cout << "Введите текст" << endl;
			getline(cin, text);
			map<string, int> result = search(s, text, 65713);
			for (auto r: result) {
				cout << r.first << " " << r.second << endl;
			}
		}
		if (choice == "simple") {
			string pattern;
			string text;
			cout << "Введите токен" << endl;
			getline(cin, pattern);
			cout << "Введите текст" << endl;
			getline(cin, text);
			cout << "Подстрока находится по адресу: " << simpleSearch(pattern, text) << endl;
		}
	}
}