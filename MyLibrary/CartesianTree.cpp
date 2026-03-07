#include<bits/stdc++.h>
using namespace std;


// min root
// if use max root, change 2 points (if, while)
template<Class T>
struct CartesianTree {
    int root;
    vector<int> parent, left, right;

    CartesianTree() : root(0) {}
    CartesianTree(const vector<T>& v) :
    root(0), parent(v.size(),-1), left(v.size(),-1), right(v.size(),-1) {
        vector<int> st(v.size(), 0);    // index
        int top = 0; // stack size
        for(int i = 1; i<v.size(); ++i){
            if(v[st[top]] > v[i]){  // pop
                while(top >= 1 && v[st[top-1]] > v[i]) --top;
                left[i] = st[top];  // last popped
                parent[left[i]] = i;
                if(top == 0) {
                    root = i;
                }else{
                    parent[i] = st[top-1];
                    right[parent[i]] = i;
                }
                st[top] = i;
            }else{  // only push
                parent[i] = st[top];
                right[parent[i]] = i;
                st[++top] = i;
            }
        }
    }
};


int main(){

}