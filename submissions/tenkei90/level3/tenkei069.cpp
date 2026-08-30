#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;
#define MOD 1'000'000'007

int main()
{
    ll N,K;
    cin >> N >> K;
    if(N==1){
        cout << K << endl;
        return 0;
    }
    ll ans = K * (K-1) % MOD;
    ll n = N-2;
    ll a = 1;
    ll b = K-2;
    while(n>0){
        if(n&1){
            a = a * b % MOD;
        }
        b = b * b % MOD;
        n >>= 1;
    }
    ans = ans * a % MOD;
    cout << ans << endl;
}