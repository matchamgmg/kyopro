#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)

//ナップサック問題

int main()
{
    int n,restriction;
    cin>>n>>restriction;
    vector<int> val(n),wei(n);
    rep(i,n){
        cin >> val.at(i) >> wei.at(i);
    }
    vector<vector<int>> dp(n+1,vector<int>(restriction+1,0));
    rep(i,n){
        rep(j,restriction+1){
            if(j-wei.at(i)>=0){
                dp.at(i+1).at(j) = max(dp.at(i).at(j),dp.at(i).at(j-wei.at(i))+val.at(i));
            }else{
                dp.at(i+1).at(j) = dp.at(i).at(j);
            }
        }
    }
    cout << dp.back().back() << endl;
}