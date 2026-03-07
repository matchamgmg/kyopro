#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)
using ll = long long;

int main()
{
    ll b;
    cin >> b;
    ll x;
    for(ll a=1;a<16;a++){
        x=1;
        for(int j=0;j<a;j++){
            x*=a;
        }
        if(x==b){
            cout << a << endl;
            return 0;
        }
    }
    cout << -1 << endl;
}