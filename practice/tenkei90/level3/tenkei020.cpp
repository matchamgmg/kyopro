#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    ll a,b,c;
    cin >> a >> b >> c;
    ll cb = 1;
    rep(i,b){
        cb *= c;
    }
    cout << (a<cb?"Yes":"No") << endl;
}