#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;
#define m_inf -(1LL<<60)

int main()
{
    ll N;
    cin >> N;
    vector<ll> A(N);
    rep(i,N){
        cin >> A[i];
    }
    vector<ll> dp_odd(N+1,m_inf),dp_even(N+1,m_inf);
    //k匹目まで出会ったあとの最大経験値
    dp_even[0] = 0;
    rep(i,N){
        dp_odd[i+1] = max(dp_even[i]+A[i],dp_odd[i]);
        dp_even[i+1] = max(dp_even[i],dp_odd[i]+2*A[i]);
    }
    cout << max(dp_even[N],dp_odd[N]) << endl;
}