#include<bits/stdc++.h>
using namespace std;

class Node{
    public:
        Node *a;
        Node *b;
        bool isAccept;
        Node() {
            a = b = NULL;
            isAccept = false;
        }
};

Node *root;

void developDFA(){
    Node *q0 = new Node(), *q1 = new Node();
    Node *q2 = new Node(), *q3 = new Node();

    q0->a = q2;
    q0->b = q1;

    q1->a = NULL;
    q1->b = q1;

    q2->a = q3;
    q2->b = q1;

    q3->a = q3;
    q3->b = q1;

    q1->isAccept = true;
    q2->isAccept = true;

    root = q0;
}

int main(){
    developDFA();
    
    while(true){
        string str;
        cout<<"Enter string to be parsed: ";
        getline(cin, str);

        Node *curr = root;
        int idx = 0;
        while(idx < str.size()){
            
            if(curr == NULL)
                break;
            
            if(str[idx] != 'a' && str[idx] != 'b'){
                //Foreign character
                curr = NULL;
                break;
            }

            if(str[idx] == 'a')
                curr = curr->a;
            else
                curr = curr->b;

            idx++;
        }

        if(curr == NULL || !curr->isAccept){
            cout<<"Not accepted\n\n";
        }else{
            cout<<"Accepted\n\n";
        }
    }
}