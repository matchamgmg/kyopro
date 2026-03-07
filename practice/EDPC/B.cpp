#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;
#define INF 1LL<<60

void chmin(ll &a,ll b){
    if(a>b) a = b;
}

int main()
{
    int N,K;
    cin >> N >> K;
    vector<ll> H(N);
    rep(i,N){
        cin >> H[i];
    }
    vector<ll> dp(N,INF);
    dp[0] = 0;
    rep(i,N-1){
        rep(j,K){
            if(i+j+1<N){
                chmin(dp[i+j+1],dp[i]+abs(H[i+j+1]-H[i]));
            }
        }
    }
    cout << dp[N-1] << endl;
}