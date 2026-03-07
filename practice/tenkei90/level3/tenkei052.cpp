#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;
#define MOD 1000000007

int main()
{
    int N;
    cin >> N;
    vector<ll> A(N,0);
    ll x;
    rep(i,N){
        rep(j,6){
            cin >> x;
            A[i] += x;
        }
    }
    ll score = 1;
    rep(i,N){
        score = score * A[i] % MOD;
    }
    cout << score << endl;
}