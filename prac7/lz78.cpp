#include <iostream>
#include <vector>
#include <map>
#include "windows.h"

using namespace std;

vector<pair<int, char>> compress(string data) {
	map<string, int> storage;
	vector<pair<int, char>> compressed_data;
	string current_symbol = "";
	int index = 1;
	int iter = 0;
	for (char character: data) {
		iter++;
		current_symbol += character;
		if (storage.find(current_symbol) == storage.end() || iter == data.size()) {
			storage[current_symbol] = index;
			compressed_data.push_back(make_pair(
					storage[current_symbol.substr(0, current_symbol.length() - 1)], character
			));
			index += 1;
			current_symbol = "";
		}
	}

	return compressed_data;
}

string decompress(vector<pair<int, char>> data) {
	map<int, string> storage;
	string decompressed_data = "";
	int index = 1;
	for (auto character: data) {
		storage[index] = storage[character.first] + character.second;
		decompressed_data += storage[index];
		index += 1;
	}

	return decompressed_data;
}


int main() {
	SetConsoleOutputCP(CP_UTF8);
	string message = "porpoterpoterporter";
	vector<pair<int, char>> compressed_message = compress(message);
	string decompressed_message = decompress(compressed_message);

	cout << "Закодированное сообщение: ";
	for (auto element: compressed_message) {
		cout << "(" << element.first << ", " << element.second << ") ";
	}
	cout << endl;
	cout << "Раскодированное сообщение: " << decompressed_message << endl;
}