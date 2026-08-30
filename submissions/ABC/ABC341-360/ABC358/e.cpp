#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;
#define MOD 998244353

ll modinv(ll a,ll p){//aとpは互いに素
    ll x1 = 1;//y1 = 0;
    ll x2 = 0;//y2 = 1;
    ll b = p;
    while(b>0){
        ll q = a/b;
        x1 -= q*x2;
        //y1 -= q*y2;
        a -= q*b;
        swap(x1,x2);
        //swap(y1,y2);
        swap(a,b);
    }
    ll ret = (x1 % p + p) % p;
    return ret;
}

int main()
{
    int K;
    cin >> K;
    vector<int> C(26);
    int max_c = 0;
    rep(i,26){
        cin >> C.at(i);
        max_c = max(max_c,C[i]);
    }

    int size_n = max(K,max_c)+1;
    vector<ll> n(size_n,0);
    n[0] = 1;
    rep(i,size_n-1){
        n[i+1] = n[i] * (i+1) % MOD;
    }

    vector<ll> inv_n(size_n,0);
    rep(i,size_n){
        inv_n[i] = modinv(n[i],MOD);
    }

    auto comb = [&](int i,int j){
        ll ret = n[i];
        ret *= inv_n[i-j];
        ret %= MOD;
        ret *= inv_n[j];
        ret %= MOD;
        return ret;
    };

    vector<vector<ll>> dp(27,vector<ll>(K+1,0));
    dp[0][0] = 1;
    rep(i,26){
        rep(j,K+1){
            for(int k=0;k<=C[i];k++){
                if(j-k>=0){
                    ll a = dp[i][j-k] * comb(j,k);
                    a %= MOD;
                    dp[i+1][j] += a;
                    dp[i+1][j] %= MOD;
                }
            }
            //cout << dp[i][j] << " ";
        }
        //cout << endl;
    }
    ll ans = 0;
    rep(i,K){
        ans += dp[26][i+1];
        ans %= MOD;
    }
    cout << ans << endl;
}