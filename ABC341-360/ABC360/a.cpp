#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    string S;
    cin >> S;
    int r;
    int m;
    rep(i,S.size()){
        if(S[i]=='R') r = i;
        if(S[i]=='M') m = i;
    }
    cout << (m-r>0?"Yes":"No") << endl;
}