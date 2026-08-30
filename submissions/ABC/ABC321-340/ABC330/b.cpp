#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)

int main()
{
    int n,l,r;
    cin>>n>>l>>r;
    vector<int> A(n);
    rep(i,n){
        cin>>A.at(i);
    }
    int x;
    rep(i,n){
        if(A.at(i)<l){
            x=l;
        }else if(A.at(i)>r){
            x=r;
        }else{
            x=A.at(i);
        }
        cout<<x<<" ";
    }
}