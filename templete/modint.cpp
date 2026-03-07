#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;
#define MOD 998244353

ll pow(ll a,ll b,ll mod){
    ll ret = 1;
    while(b>0){
        if(b&1) ret = ret*a % mod;
        a = a*a % mod;
        b = b>>1;
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

struct modint
{
    ll x;
    ll mod;
    modint(ll x) : x(((x%MOD)+MOD)%MOD), mod(MOD) {}

    // 演算子オーバーロード
    modint operator+(modint a)
    {
        ll ret = this->x + a.x;
        ret %= this->mod;
        return modint(ret);
    }
    modint operator-(modint a)
    {
        ll ret = this->x - a.x;
        ret %= this->mod;
        if (ret < 0)
            ret += this->mod;
        return modint(ret);
    }
    modint operator*(modint a)
    {
        ll ret = this->x * a.x;
        ret %= this->mod;
        return modint(ret);
    }
    modint operator/(modint a)
    {
        ll inva = modinv(a.x, this->mod);
        ll ret = this->x * inva % this->mod;
        return modint(ret);
    }
};

ll GCD(ll a,ll b){
    if(b==0){
        return a;
    }
    return GCD(b,a%b);
}

ll nonrecursiveGCD(ll a,ll b){
    while(b>0){
        a %= b;
        swap(a,b);
    }
    return a;
}

ll extGCD(ll a,ll b,ll &x,ll &y){
    if(b==0){
        x = 1;
        y = 0;
        return a;
    }
    /*
    ll s,t;
    ll d = extGCD(b,a%b,s,t);
    x = t;
    y = s - a/b * t;
    */
   ll d = extGCD(b,a%b,y,x);
   y -= a/b * x;
    return d;
}

ll nonrecursiveExtGCD(ll a,ll b,ll &x,ll &y){
    x = 1;
    y = 0;
    ll x2 = 0,y2 = 1;
    while(b>0){
        ll q = a/b;
        x -= q*x2;
        y -= q*y2;
        swap(x,x2);
        swap(y,y2);
        a = a%b;
        swap(a,b);
    }
    return a;
}

int main(){
    ll a = 111,b = 30;
    ll x,y;
    ll d = nonrecursiveExtGCD(a,b,x,y);
    cout << "x = " << x << ", y = " << y << ", gcd = " << d << endl;
    d = nonrecursiveGCD(a,b);
    cout << "gcd(111,30) = " << d << endl;
    cout << "modinv(29,111) = " << modinv(29,111) << endl;
}