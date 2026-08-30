#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(), a.end()
using namespace std;
using ll = long long;
#define MOD 998244353

ll modinv(ll a, ll p)
{              // aとpは互いに素
    ll x1 = 1; // y1 = 0;
    ll x2 = 0; // y2 = 1;
    ll b = p;
    while (b > 0)
    {
        ll q = a / b;
        x1 -= q * x2;
        // y1 -= q*y2;
        a -= q * b;
        swap(x1, x2);
        // swap(y1,y2);
        swap(a, b);
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

int
main()
{
    ll N, K;
    cin >> N >> K;
    modint x = modint(1); // K回目の操作の後1番左にある確率
    rep(i, K)
    {
        x = x * (modint(1) - modint(2) * modint(N - 1) / modint(N) / modint(N)) + (modint(1) - x) * modint(2) / modint(N) / modint(N);
    }
    modint ans = x + (modint(1) - x) * modint(N + 2) / modint(2);
    cout << ans.x << endl;
}