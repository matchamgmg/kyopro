#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)
using ll = long long;

int main()
{
    ll a,m,l,r;
    cin>>a>>m>>l>>r;
    ll k1,k2,ans;
    if(l-a>0){
        k1 = (r-a)/m+1;
        k2 = (l-a+m-1)/m;
    }else if(r-a<0){
        k1 = (a-l)/m+1;
        k2 = (a-r+m-1)/m;
    }else{
        k1 = (r-a)/m+1;
        k2 = (l-a)/m;
    }
    ans = k1-k2;
    cout << ans << endl;
}