#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)

//個数無制限部分和問題
//個数制限付き部分和問題 x

int main()
{
    int n,A;
    cin>>n>>A;
    vector<int> a(n);
    rep(i,n){
        cin>>a.at(i);
    }
    vector<vector<int>> dp(n+1,vector<int>(A+1,0));
    dp.at(0).at(0) = 1;
    rep(i,n){
        rep(j,A+1){
            dp.at(i+1).at(j) = dp.at(i).at(j);
            if(j-a.at(i)>=0){
                dp.at(i+1).at(j) += dp.at(i+1).at(j-a.at(i));
            }
        }
    }
    cout << dp.at(n).at(A) << endl;
}