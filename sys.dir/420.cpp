#include <bits/stdc++.h>
using namespace std;

bool isLL1Grammar = true;
const int ITER = 1000;

vector<vector<string>> generateProds(){
    vector<vector<string>> v(5);
    
    v[0].push_back("E"); v[0].push_back("->");  //E -> TE'
    v[0].push_back("T"); v[0].push_back("E'");

    v[1].push_back("E'"); v[1].push_back("->"); //E' -> +TE' | eps
    v[1].push_back("+");v[1].push_back("T");v[1].push_back("E'");
    v[1].push_back("|"); v[1].push_back("eps");

    v[2].push_back("T"); v[2].push_back("->");  //T -> FT'
    v[2].push_back("F"); v[2].push_back("T'");

    v[3].push_back("T'"); v[3].push_back("->"); //T' -> *FT' | eps
    v[3].push_back("*");v[3].push_back("F");v[3].push_back("T'");
    v[3].push_back("|"); v[3].push_back("eps");

    v[4].push_back("F"); v[4].push_back("->");  //F -> ( E ) | id
    v[4].push_back("(");v[4].push_back("E");v[4].push_back(")");
    v[4].push_back("|"); v[4].push_back("id");

    cout<<"Given grammar:\n";
    for(auto prod: v){
        for(auto str: prod) cout<<str<<" ";
        cout<<"\n";
    }
    return v;
}

bool isTerminal(string s){
    return !(s[0] >= 'A' && s[0] <= 'Z');
}

void calculateFirst(vector<set<string>> &first, map<string, int> index, const vector<vector<string>> &prod, int n){
    vector<bool> first_done(n, false);
    int firstFound = 0;
    int cnt = 0;
    while (firstFound < n){
        cnt++;
        if (cnt == ITER)
            break;
        for (int i = n - 1; i >= 0; i--){
            if (first_done[i])
                continue;
            int len = prod[i].size();
            int j = 2;
            bool flag = true;
            while (j < len){
                if ((j == 2 || prod[i][j - 1] == "|") && isTerminal(prod[i][j])){
                    first[i].insert(prod[i][j]);
                    while (j < len && prod[i][j] != "|") j++;
                    
                    if (j < len && prod[i][j] == "|")
                        j++;
                    
                }
                else if ((j == 2 || prod[i][j - 1] == "|") && !isTerminal(prod[i][j])){
                first:
                    bool epsilon = false;
                    for (auto el : first[index[prod[i][j]]]){
                        if (el != "eps")
                            first[i].insert(el);
                        else
                            epsilon = true;
                    }
                    if (epsilon){
                        if (j + 1 == len || prod[i][j + 1] == "|")
                            first[i].insert("eps");
                        else{
                            if (j + 1 < len && !isTerminal(prod[i][j + 1]) && prod[i][j + 1] != "|"){
                                j++;
                                goto first;
                            }
                            else if (j + 1 < len && isTerminal(prod[i][j + 1])){
                                first[i].insert(prod[i][j + 1]);
                                j++;
                            }
                        }
                    }
                    if (!first_done[index[prod[i][j]]])
                        flag = false;
                    
                    while (j < len && prod[i][j] != "|") j++;
                    
                    if (j < len && prod[i][j] == "|")
                        j++;
                    
                }
                else
                    j++;
            }
            if (flag == true)
                firstFound++;
            first_done[i] = flag;
        }
    }
}

void calculateFollow(vector<set<string>> &last, vector<set<string>> &first, map<string, int> index, const vector<vector<string>> &prod, int n){
    vector<bool> last_done(n, false);
    last[0].insert("$");

    int lastFound = 0;
    int cnt = 0;
    while (lastFound < n){
        cnt++;
        if (cnt == ITER)
            break;
        int prev = lastFound;
        for (int i = 0; i < n; i++){
            if (last_done[i])
                continue;
            bool flag = true;

            for (int c = 0; c < n; c++){
                int len = prod[c].size();
                int j = 2;
                while (j < len){
                    if (prod[c][j] == prod[i][0]){
                    last:
                        if (j + 1 == len || prod[c][j + 1] == "|" || (first[index[prod[c][j + 1]]].count("eps") && (j + 2 == len || prod[c][j + 2] == "|"))){
                            if (prod[c][j] == prod[c][0])
                                goto last_first;
                            for (auto el : last[index[prod[c][0]]]){
                                if (el != "eps")
                                    last[i].insert(el);
                            }
                            if (!last_done[index[prod[c][0]]])
                                flag = false;
                        }
                    last_first:
                        if ((j + 1 != len) && (prod[c][j + 1] != "|")){
                            if (isTerminal(prod[c][j + 1]))
                                last[i].insert(prod[c][j + 1]);
                            else{
                                bool epsilon = false;
                                for (auto el : first[index[prod[c][j + 1]]]){
                                    if (el != "eps")
                                        last[i].insert(el);
                                    else
                                        epsilon = true;
                                }
                                if (epsilon){
                                    j++;
                                    if (j == len || prod[c][j] == "|")
                                        continue;
                                    goto last;
                                }
                            }
                        }
                    }
                    j++;
                }
            }
            last_done[i] = flag;
            if (flag)
                lastFound++;
        }
        if (lastFound == prev && !last_done[0]){
            last_done[0] = true;
            lastFound++;
        }
    }
}

void build_table(int n, const vector<vector<string>> &prod, vector<set<string>> &first,
                 vector<set<string>> &last,map<string, int> index, map<pair<string, string>, vector<string>> &table){
    for (int i = 0; i < n; i++){
        int len = prod[i].size();
        int j = 2;
        vector<string> temp;
        temp.push_back(prod[i][0]);
        temp.push_back("->");
        set<string> fs;
        while (j < len){
            if (j == 2 || prod[i][j - 1] == "|"){
            epsilon_first:
                if (isTerminal(prod[i][j]))
                    fs.insert(prod[i][j]);
                bool epsilon = false;
                if (!isTerminal(prod[i][j])){
                    for (auto pq : first[index[prod[i][j]]]){
                        if (pq == "eps"){
                            epsilon = true;
                            if (j + 1 == len || prod[i][j + 1] == "|")
                                fs.insert(prod[i][j]);
                        }
                        else
                            fs.insert(pq);
                    }
                }
                if (epsilon){
                    temp.push_back(prod[i][j]);
                    j++;
                    if (j == len || prod[i][j] == "|")
                        continue;
                    goto epsilon_first;
                }
            }
            if (prod[i][j] == "|"){
                for (auto el : fs){
                    if (el == "eps"){
                        for (auto t : last[index[prod[i][0]]]){
                            if (table.count({prod[i][0], t})){
                                isLL1Grammar = false;
                                table[{prod[i][0], t}].push_back(", ");
                                for (auto r : temp)
                                    table[{prod[i][0], t}].push_back(r);
                            }
                            else
                                table[{prod[i][0], t}] = temp;
                        }
                    }
                    else{
                        if (table.count({prod[i][0], el})){
                            isLL1Grammar = false;
                            table[{prod[i][0], el}].push_back(", ");
                            for (auto r : temp)
                                table[{prod[i][0], el}].push_back(r);
                        }
                        else
                            table[{prod[i][0], el}] = temp;
                    }
                }
                temp.clear();
                fs.clear();
                temp.push_back(prod[i][0]);
                temp.push_back("->");
            }
            else
                temp.push_back(prod[i][j]);
            j++;
        }
        for (auto el : fs){
            if (el == "eps"){
                for (auto t : last[index[prod[i][0]]]){
                    if (table.count({prod[i][0], t})){
                        isLL1Grammar = false;
                        table[{prod[i][0], t}].push_back(", ");
                        for (auto r : temp)
                            table[{prod[i][0], t}].push_back(r);
                    }
                    else
                        table[{prod[i][0], t}] = temp;
                }
            }
            else{
                if (table.count({prod[i][0], el})){
                    isLL1Grammar = false;
                    table[{prod[i][0], el}].push_back(", ");
                    for (auto r : temp)
                        table[{prod[i][0], el}].push_back(r);
                }
                else
                    table[{prod[i][0], el}] = temp;
            }
        }
    }
}

void print_table(int n, const vector<vector<string>> &prod, set<string> symbols,
                 map<pair<string, string>, vector<string>> table){
    cout << "  ";
    for (auto sym : symbols){
        if (sym == "eps")
            continue;
        cout << setw(8) << sym;
        cout << " ";
    }
    cout << endl;

    for (int i = 0; i < n; i++){
        cout << setw(3) << prod[i][0] << " ";
        for (auto sym : symbols){
            if (sym == "eps") continue;
            if (!table.count({prod[i][0], sym})){
                cout << setw(8) << "- ";
                cout << " ";
                continue;
            }
            string str;
            for (auto c : table[{prod[i][0], sym}])
                str += c;
            cout << setw(8) << str;
            cout << " ";
        }
        cout << '\n';
    }
}

bool isStringValid(int n,const vector<vector<string>>& prod, map<pair<string,string>,vector<string> > &table,vector<string> &sen)
{	
	int len=sen.size();		
	stack<string> st;
	st.push("$");
	st.push(prod[0][0]);
	
	bool doable=true;
    int i=0;
	string output;
	while(i<len&&!st.empty()){
		string input;
		for(int k=i; k<len; k++)
			input+=sen[k];

		string s=st.top();		
		if(s==sen[i]&&s=="$")
			break;
		else if(s==sen[i]){
			output.clear();
			i++;
			st.pop();
			continue;
		}
		else if(isTerminal(s)){
			doable=false;
			break;
		}
		else {
			if(!table.count({s,sen[i]})){
				doable=false;
				break;				
			}
			vector<string> p=table[{s,sen[i]}];
			int psz=p.size();
			st.pop();
			for(int k=psz-1; k>=2; k--){
				if(p[k]!="eps")
					st.push(p[k]);
			}

			output.clear();
			for(auto str: p)
				output+=str;
			
		}
	}

    return doable;
}


int main(){
    vector<vector<string>> prod = generateProds();
    int n = prod.size();

    vector<set<string>> first(n), last(n);
    vector<bool> last_done(n, false);
    map<string, int> index;
    map<pair<string, string>, vector<string>> table;

    for (int i = 0; i < n; i++)
        index[prod[i][0]] = i;

    calculateFirst(first, index, prod, n);

    cout << "\nFirst Sets: \n";

    for (int i = 0; i < n; i++){
        cout << prod[i][0] << " : ";
        for (auto el : first[i])
            cout << el << ' ';
        cout << '\n';
    }

    calculateFollow(last, first, index, prod, n);

    cout << "\nFollow Sets: \n";

    for (int i = 0; i < n; i++){
        cout << prod[i][0] << " : ";
        for (auto el : last[i])
            cout << el << ' ';
        cout << '\n';
    }

    build_table(n, prod, first, last, index, table);

    set<string> symbols;
    for (int i = 0; i < n; i++){
        for (auto a : first[i])
            symbols.insert(a);
        for (auto l : last[i])
            symbols.insert(l);
    }

    cout << "\nParsing Table \n\n";

    print_table(n, prod, symbols, table);

    if (!isLL1Grammar){
        cout << "\n Grammar is not LL(1)\n";
        return 0;
    }

    int q;
    cout<<"Enter number of sentences to parse:";
    cin>>q;
    string temp, word;
    getline(cin, temp);
    
    while(q--){
        cout<<"Enter sentence:\n";
        getline(cin, temp);
        vector<string> sentence;
        stringstream ss(temp);
        while(ss>>word){
            sentence.push_back(word);
        }
        sentence.push_back("$");
        bool res = isStringValid(n, prod, table, sentence);
        cout<<(res?"Successful\n":"Unsuccessful: Found error\n");
    }
    return 0;
}