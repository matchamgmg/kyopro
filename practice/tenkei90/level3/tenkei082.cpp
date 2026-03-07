#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;
#define MOD 1'000'000'007LL

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

ll pow(ll a,ll b,ll mod){
    ll ret = 1;
    while(b>0){
        if(b&1) ret = ret*a % mod;
        a = a*a % mod;
        b = b>>1;
    }
    return ret;
}

int main()
{
    ll L,R;
    cin >> L >> R;
    //1~Rまでから1~L-1までを除く
    //1~9...sum_i=1^9(i) = (1+9)*9/2 = 45 n=9
    //10~99...sum_i=10^99(2i) = (10+99)*90/2 = 4905 n=90 *2
    //100~999...sum_i=100^999(3i) = (100+999)*900/2 = 494550 n=900 *3
    //前処理
    vector<ll> a(19);//a[k]...1~10^k-1までの答え
    a[0] = 0;
    rep(i,a.size()){
        if(i>0){
            a[i] = a[i-1] + (pow(10,i-1,MOD) + pow(10,i,MOD) - 1LL) % MOD
            * ((pow(10,i,MOD) - pow(10,i-1,MOD) + MOD) % MOD) % MOD * modinv(2,MOD) % MOD
            * i % MOD;
        }
    }
    //L,Rの桁数を求める
    L--;
    ll l=L,r=R;
    ll dl=0,dr=0;
    while(l>0){
        l/=10;
        dl++;
    }
    while(r>0){
        r/=10;
        dr++;
    }
    //cout << L << " " << dl << endl;
    //cout << R << " " << dr << endl;
    auto ansN = [&](ll x,ll d){
        if(x<=0) return 0LL;
        ll ret = a[d-1] + (pow(10,d-1,MOD) + x) % MOD
        * ((x - pow(10,d-1,MOD) + 1LL + MOD) % MOD) % MOD * modinv(2,MOD) % MOD
        * d % MOD;
        return ret;
    };
    ll ansl = ansN(L,dl);
    ll ansr = ansN(R,dr);
    ll ans = (ansr - ansl + MOD) % MOD;
    //cout << ansr << endl;
    //cout << ansl << endl;
    cout << ans << endl;
}