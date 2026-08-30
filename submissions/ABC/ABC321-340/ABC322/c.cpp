#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)

int main()
{
    int n,m;
    cin>>n>>m;
    vector<int> A(m);
    rep(i,m){
        cin>>A.at(i);
        A.at(i)--;
    }
    rep(i,n){
        cout << *lower_bound(A.begin(),A.end(),i) - i << " ";
    }
}