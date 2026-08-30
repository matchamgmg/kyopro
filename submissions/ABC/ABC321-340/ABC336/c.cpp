#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)
using ll = long long;

int main()
{
    ll n;
    cin>>n;
    vector<int> b5;
    auto base5 = [&](ll x){
        if(x==0){
            b5.push_back(0);
            return;
        }
        while(x>0){
            b5.push_back(x%5);
            x = x/5;
        }
        reverse(b5.begin(),b5.end());
    };
    base5(n-1);
    rep(i,b5.size()){
        cout << b5.at(i)*2;
    }
    cout << endl;
}