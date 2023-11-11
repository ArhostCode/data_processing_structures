#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct cell {
	int off;
	int len;
	char nextChar;

	cell(int off, int len, char nextChar) : off(off), len(len), nextChar(nextChar) {}
};

vector<cell> encode(string str) {
	vector<cell> code;
	int firstWindow;
	int secondWindow;
	int maxWindow = 30;
	for (int i = 0; i < str.length(); i++) {
		if (i + 1 <= maxWindow) firstWindow = i;
		else firstWindow = maxWindow;

		if (i + firstWindow < str.length()) secondWindow = firstWindow;
		else secondWindow = str.length() - i;

		string fWindowString = str.substr(i - firstWindow, firstWindow);
		string sWindowString = str.substr(i, secondWindow);

		int off = -1;
		while (true) {
			if (secondWindow <= 0) break;
			string str3 = sWindowString.substr(0, secondWindow);
			off = fWindowString.find(str3);
			if (off != -1) break;
			secondWindow--;
		}

		if (off != -1) {
			cell cd(firstWindow - off, secondWindow, str[i + secondWindow]);
			code.push_back(cd);
			i += secondWindow;
		} else {
			cell cd(0, 0, str[i + secondWindow]);
			code.push_back(cd);
		}
	}
	return code;
}


int main() {

	//string str = "abacdbacc";
	string str = "0010100110010000001";
	cout << str << endl;

	//encoding
	vector<cell> code = encode(str);

	for (int i = 0; i < code.size(); i++) {
		printf("(%d,%d,%c)", code[i].off, code[i].len, code[i].nextChar);
	}
	printf("\n");

	//decoding
	string decoded;
	for (auto & i : code) {
		if (i.len == 0) {
			decoded += i.nextChar;
		} else {
			int len = decoded.length();
			len -= i.off;
			string temp = decoded.substr(len, i.len);
			decoded += temp + i.nextChar;
		}
		cout << decoded << endl;
	}
	cout << decoded;
	return 0;
}