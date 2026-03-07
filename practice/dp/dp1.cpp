#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)

//最大和問題

int main()
{
    int n;
    cin >> n;
    vector<int> A(n);
    rep(i,n){
        cin >> A.at(i);
    }
    vector<int> dp(n+1,0);
    rep(i,n){
        dp.at(i+1) = max(dp.at(i),dp.at(i)+A.at(i));
    }
    cout << dp.back() << endl;
}