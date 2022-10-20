#include <bits/stdc++.h>
using namespace std;

const int ITERS = 1000;

int n;
vector<vector<string>> productions;
map<string, int> code;
vector<set<string>> first, last;
map<pair<string, string>, vector<string> > table;

// Utility function to return whether s is terminal or not
bool isTerminal(string s) {
	return !(s.size()>0 && s[0] >= 'A' && s[0] <= 'Z');
}

void calculateFirst() {
	vector<bool> first_done(n, false);
	int firstFound = 0;
	int iters_done = 0;
	while (firstFound < n){
		iters_done++;
		if (iters_done == ITERS)
			break;
		
		for (int i = n - 1; i >= 0; i--) {
			if (first_done[i])
				continue;
			
			int len = productions[i].size();
			int j = 2;
			bool flag = true;
			while (j < len) {
				if ((j == 2 || productions[i][j - 1] == "|") && isTerminal(productions[i][j])) {
					first[i].insert(productions[i][j]);
					while (j < len && productions[i][j] != "|"){
						j++;
					}
					if (j < len && productions[i][j] == "|"){
						j++;
					}
				} else if ((j == 2 || productions[i][j - 1] == "|") && !isTerminal(productions[i][j])) {
					first:
					bool epsilon = false;
					for (auto el : first[code[productions[i][j]]]) {
						if (el != "eps")
							first[i].insert(el);
						else
							epsilon = true;
					}
					if (epsilon) {
						if (j + 1 == len || productions[i][j + 1] == "|"){
							first[i].insert("eps");
						}else {
							if (j + 1 < len && !isTerminal(productions[i][j + 1]) && productions[i][j + 1] != "|") {
								j++;
								goto first;
							} else if (j + 1 < len && isTerminal(productions[i][j + 1])) {
								first[i].insert(productions[i][j + 1]);
								j++;
							}
						}
					}
					if (!first_done[code[productions[i][j]]]) {
						flag = false;
					}
					while (j < len && productions[i][j] != "|") {
						j++;
					}
					if (j < len && productions[i][j] == "|"){
						j++;
					}
				} else{
					j++;
				}
			}

			if (flag == true)
				firstFound++;
			first_done[i] = flag;
		}
	}
}

void calculateFollow() {
	vector<bool> last_done(n, false);
	last[0].insert("$");

	int lastFound = 0;
	int iters_done = 0;
	while (lastFound < n) {
		iters_done++;
		if (iters_done == ITERS) break;
		int prev = lastFound;
		for (int i = 0; i < n; i++) {
			if (last_done[i])continue;
			bool flag = true;
			for (int c = 0; c < n; c++) {
				int len = productions[c].size();
				int j = 2;
				while (j < len) {
					if (productions[c][j] == productions[i][0]) {
last:
						if (j + 1 == len || productions[c][j + 1] == "|" || (first[code[productions[c][j + 1]]].count("eps") && (j + 2 == len || productions[c][j + 2] == "|"))) {
							if (productions[c][j] == productions[c][0]) goto last_first;
							for (auto el : last[code[productions[c][0]]]) if (el != "eps") last[i].insert(el);
							if (!last_done[code[productions[c][0]]]) flag = false;
						}
last_first:
						if ((j + 1 != len) && (productions[c][j + 1] != "|")) {
							if (isTerminal(productions[c][j + 1])) last[i].insert(productions[c][j + 1]);
							else {
								bool epsilon = false;
								for (auto el : first[code[productions[c][j + 1]]]) {
									if (el != "eps") last[i].insert(el);
									else epsilon = true;
								}
								if (epsilon) {
									j++;
									if (j == len || productions[c][j] == "|") continue;
									goto last;
								}
							}
						}
					}
					j++;
				}
			}
			last_done[i] = flag;
			if (flag) lastFound++;
		}
		if (lastFound == prev && !last_done[0]) {
			last_done[0] = true;
			lastFound++;
		}
	}
}


int main() {

	cout << "Enter the number of productions:\t";
	cin >> n;
	string temp;
	getline(cin, temp);
	productions.resize(n);
	first.resize(n);
	last.resize(n);

	cout << "Enter productions, one by one (with terminals, non-terminals,etc. separated by space) :\n";
	for (int i = 0; i < n; i++) {
		getline(cin, temp);
		stringstream ss;
		ss << temp;
		string word;
		while (ss >> word)
			productions[i].push_back(word);
	}

	for (int i = 0; i < n; i++)
		code[productions[i][0]] = i;

	calculateFirst();

	cout << "\nFIRST: \n";

	for (int i = 0; i < n; i++) {
		cout << productions[i][0] << " : ";
		for (string element : first[i])
			cout << element << " ";
		cout << endl;
	}

	calculateFollow();

	cout << "\nFOLLOW: \n";

	for (int i = 0; i < n; i++) {
		cout << productions[i][0] << " : ";
		for (string element : last[i])
			cout << element << " ";
		cout << endl;
	}

	return 0;
}