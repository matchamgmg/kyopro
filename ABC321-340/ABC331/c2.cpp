#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)
using ll = long long;
#define NUM 1000001

int main()
{
    int n;
    cin>> n;
    vector<vector<int>> A(NUM,vector<int>());
    int a;
    rep(i,n){
        cin>>a;
        A.at(a-1).push_back(i);
    }
    ll ans = 0;
    vector<ll> Ans(n);
    
    rep(i,NUM){
        int j = NUM-1-i;
        rep(k,A.at(j).size()){
            Ans.at(A.at(j).at(k)) = ans; 
        }
        ans += (j+1)*(ll)A.at(j).size();
    }
    rep(i,n){
        cout << Ans.at(i) << " ";
    }
}