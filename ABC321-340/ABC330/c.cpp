#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)
using ll = long long;

ll calc(ll D,ll x,ll y){
    ll ans = x*x+y*y-D;
    return (ans>=0?ans:-ans);
}

int main()
{
    ll D;
    cin >> D;
    ll x=0, y=2e6;
    ll ans = (ll)1<<60;
    for(x=0;x<=2e6;x++){
        ans = min(ans,calc(D,x,y));
        while(y>0&&x*x+y*y>D){
            y--;
            ans = min(ans,calc(D,x,y));
            ans = min(ans,calc(D,x,y+1));
        }
    }
    cout << ans << endl;
}