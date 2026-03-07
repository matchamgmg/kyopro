#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int N;
    cin >> N;
    vector<ll> A(N),B(N);
    rep(i,N) cin >> A[i];
    rep(i,N) cin >> B[i];
    sort(ALL(A));
    sort(ALL(B));
    ll ans = 0;
    rep(i,N){
        ans += abs(A[i]-B[i]);
    }
    cout << ans << endl;
}