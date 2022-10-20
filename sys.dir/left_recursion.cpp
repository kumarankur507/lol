#include <bits/stdc++.h>

using namespace std;

void eliminate_left_recursion(vector<vector<string>>& prod, int& n);

int main()
{
	int n;
	cout << "Enter the number of productions:\t";
	cin >> n;
	string temp;
	getline(cin, temp);
	vector<vector<string>> prod(n);

	cout << "Enter productions, one by one (with terminals, non-terminals,etc. separated by space) :\n";
	for (int i = 0; i < n; i++)
	{
		getline(cin, temp);
		stringstream ss;
		ss << temp;
		string word;
		while (ss >> word)
		{
			prod[i].push_back(word);
		}
	}

	int oldN = n;
	eliminate_left_recursion(prod, n);

	if (oldN != n)
	{
		cout << "\nLeft Recursion Found.\nAfter eliminating left recursion, grammar becomes:\n";
		for (int i = 0; i < n; i++)
		{
			for (auto s : prod[i])
			{
				cout << s << " ";
			}
			cout << '\n';
		}
	}
	else
	{
		cout << "\n Left Recursion Not Found! \n";
	}

	return 0;
}

void eliminate_left_recursion(vector<vector<string>>& prod, int& n)
{
	vector<vector<string>> newProd;
	int total = n;
	for (int i = 0; i < n; i++)
	{
		string nt = prod[i][0];
		int j = 2;
		int len = prod[i].size();
		bool rec = false, leftRec = false;
		vector<vector<string>> alpha, beta;
		vector<string> s;
		while (j < len)
		{
			if (prod[i][j] == "|")
			{
				if (rec)
				{
					alpha.push_back(s);
				}
				else
				{
					beta.push_back(s);
				}
				s.clear();
				rec = false;
			}
			if (j == 2 || prod[i][j - 1] == "|")
			{
				if (prod[i][j] == nt)
				{
					leftRec = rec = true;
				}
				else
				{
					s.push_back(prod[i][j]);
					rec = false;
				}
			}
			else if (prod[i][j] != "|")
			{
				s.push_back(prod[i][j]);
			}
			j++;
		}
		if (rec)
		{
			alpha.push_back(s);
		}
		else
		{
			beta.push_back(s);
		}
		s.clear();
		if (!leftRec)
		{
			newProd.push_back(prod[i]);
			continue;
		}
		vector<string> p1, p2; // the 2 new productions which will replace this production
		p1.push_back(prod[i][0]);
		p1.push_back("->");
		for (int l = 0; l < (int)beta.size(); l++)
		{
			for (auto pq : beta[l])
			{
				p1.push_back(pq);
			}
			p1.push_back(prod[i][0] + "'");
			if (l != (int)beta.size() - 1)
				p1.push_back("|");
		}
		p2.push_back(prod[i][0] + "'");
		p2.push_back("->");
		for (int l = 0; l < (int)alpha.size(); l++)
		{
			for (auto pq : alpha[l])
			{
				p2.push_back(pq);
			}
			p2.push_back(prod[i][0] + "'");
			p2.push_back("|");
		}
		p2.push_back("eps");
		total++;
		newProd.push_back(p1);
		newProd.push_back(p2);
	}
	prod = newProd;
	n = total;
}