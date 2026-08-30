#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)
using ll = long long;

int main()
{
    int n;
    cin >> n;
    vector<ll> A,B;
    int a;
    rep(i,n){
        cin>>a;
        A.push_back(a);
        B.push_back(a);
    }
    sort(A.begin(),A.end());
    ll ans;
    vector<ll> sum(n+1,0);
    rep(i,n){
        sum.at(i+1) = sum.at(i) + A.at(i);
    }
    rep(i,n){
        auto itr = upper_bound(A.begin(),A.end(),B.at(i));
        ans = sum.at(n) - sum.at(distance(A.begin(),itr));
        cout << ans << " ";
    }
}