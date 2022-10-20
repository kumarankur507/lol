#include<bits/stdc++.h>
using namespace std;

vector<string> opers;
vector<vector<char>> table;
vector<int> status, topo;
vector<vector<int>> adj;

bool dfs(int node, int par){
    status[node]=1;
    for(auto neighbor: adj[node]){
        if(neighbor==par) continue;
        if (status[neighbor] == 1 ) return true;
        else if(status[neighbor] == 0){
            bool hasCycle = dfs(neighbor, node);
            if( hasCycle == true ) return true;
        }
    }
    status[node]=2;
    topo.push_back(node);
    return false;
}

int main(){
    int n;
    cout<<"Enter the number of operators: ";
    cin>>n;
    
    vector<vector<int>> code(n, vector<int> (2));
    vector<int> res(2*n, 0);
    opers.resize(n);
    table.resize(n,vector<char>(n));
    
    int cnt=0;
    for(int i=0; i<n; i++){
        code[i][0]=cnt++;
        code[i][1]=cnt++;
    }

    adj.resize(cnt);
    status.resize(cnt, 0);

    cout<<"Enter the operators: ";
    for(int i=0; i<n; i++)
        cin>>opers[i];
    

    cout<<"Enter precedence table row-wise: (<,>,=,*)\n";
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            cin>>table[i][j];
            if(table[i][j]=='=')
                code[j][1]=code[i][0];
        }
    }

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(table[i][j]=='<')                    //gj -> fi
                adj[code[j][1]].push_back(code[i][0]);
            else if(table[i][j]=='>')               //fi -> gj
                adj[code[i][0]].push_back(code[j][1]);
        }
    }

    for(int i=0; i<cnt; i++){
        if(status[i]!=0) continue;
        bool hasCycle = dfs(i, -1);
        if(hasCycle){
            cout<<"Cycle Present. No precedence table possible.\n";
            return 0;
        }
    }

    //Fill distance array in reverse topological order
    //i.e distance of u should be determined wrt 
    //distance of children v
    for(auto u: topo){
        for(auto v: adj[u]){
            res[u]=max(res[u], res[v]+1);
        }
    }

    cout<<"f g\n";
    for(int i=0; i<n; i++)
        cout<<res[code[i][0]]<<" "<<res[code[i][1]]<<" : "<<opers[i]<<"\n";
}