#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;
#define MOD 998244353

ll POW(ll a,ll b){
  ll ret = 1;
  while(b>0){
    if(b&1){
      ret = ret * a % MOD;
    }
    a = a * a % MOD;
    b >>= 1;
  }
  return ret;
}

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

int main(){
  ll N;
  cin >> N;
  ll n = N;
  ll r = 1;
  while(n>0){
    r = r * 10 % MOD;
    n /= 10;
  }
  ll s = (POW(r,N) - 1) % MOD;
  if(s<0) s += MOD;
  ll t = (r - 1) % MOD;
  if(t<0) t += MOD;
  ll ans = N % MOD * s % MOD * modinv(t,MOD) % MOD;
  cout << ans << endl;
}