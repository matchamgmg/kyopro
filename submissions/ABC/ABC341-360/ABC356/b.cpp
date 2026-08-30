#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int n,m;
    cin >> n >> m;
    vector<int> A(m);
    rep(i,m){
        cin >> A.at(i);
    }
    vector<int> X(m,0);
    vector<bool> check(m,false);
    int a;
    rep(i,n){
        rep(j,m){
            cin >> a;
            X.at(j) += a;
            if(X.at(j)>=A.at(j)){
                check.at(j) = true;
            }
        }
    }
    bool ans = true;
    rep(i,m){
        ans &= check.at(i);
    }
    cout << (ans?"Yes":"No") << endl;
}