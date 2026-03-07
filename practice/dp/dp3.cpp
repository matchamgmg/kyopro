#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)

//部分和問題
//部分和数え上げ

int main()
{
    int n,A;
    cin>>n>>A;
    vector<int> a(n);
    rep(i,n){
        cin>>a.at(i);
    }
    //vector<vector<bool>> dp(n+1,vector<bool>(A+1,false));
    vector<vector<int>> dp(n+1,vector<int>(A+1,0));
    //dp.at(0).at(0) = true;
    dp.at(0).at(0) = 1;
    rep(i,n){
        rep(j,A+1){
            //dp.at(i+1).at(j) |= dp.at(i).at(j);
            (dp.at(i+1).at(j) += dp.at(i).at(j))%1'000'000'009; //1'000'000'009で割った余りで出力
            if(j-a.at(i)>=0){
                //dp.at(i+1).at(j) |= dp.at(i).at(j-a.at(i));
                (dp.at(i+1).at(j) += dp.at(i).at(j-a.at(i)))%1'000'000'009;
            }
        }
    }
    //cout << (dp.at(n).at(A)?"Yes":"No") << endl;
    cout << dp.at(n).at(A) << endl;
}