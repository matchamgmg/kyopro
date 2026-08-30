#include <bits/stdc++.h>

#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
// using mint = modint1000000007;
template <class T>
using pq = priority_queue<T, vector<T>>; // 大きい順
template <class T>
using pq_g = priority_queue<T, vector<T>, greater<T>>; // 小さい順

#define rep(i, s, n) for (int i = s; i < (int)n; i++)
#define rrep(i, s, n) for (int i = (int)n - 1; i >= s; i--)
#define all(a) a.begin(), a.end()
#define rall(a) a.rbegin(), a.rend()
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
#define YN(a) \
    if (a)    \
    {         \
        pyes; \
    }         \
    else      \
    {         \
        pno;  \
    }
#define v_cout(a)                         \
    {                                     \
        int n = a.size();                 \
        rep(i, 0, n) cout << a[i] << " "; \
        cout << endl;                     \
    }
#define vv_cout(a)                                         \
    {                                                      \
        int n = a.size();                                  \
        rep(i, 0, n)                                       \
        {                                                  \
            rep(j, 0, a[i].size()) cout << a[i][j] << " "; \
            cout << endl;                                  \
        }                                                  \
        cout << endl;                                      \
    }
#define grid_check(x, y, X, Y) (0 <= x && x < X && 0 <= y && y < Y)
template <class T>
bool chmax(T &a, T b)
{
    if (a < b)
    {
        a = b;
        return true;
    }
    else
        return false;
}
template <class T>
bool chmin(T &a, T b)
{
    if (a > b)
    {
        a = b;
        return true;
    }
    else
        return false;
}

// 各レピュニット数をpMで割った余りを、ダブリングにより求める
// 2^k 桁のレピュニット数

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll mod_p = 10007;
    ll K, M;
    cin >> K >> M;
    ll mod_pm = mod_p * M;
    ll N = 0;

    ll l_max = 1e9;
    vector<ll> repunit_mod_pm(1, 1);
    ll d = 1;
    ll TEN = 10;
    while (d <= l_max)
    {
        ll r = repunit_mod_pm.back();
        repunit_mod_pm.emplace_back((r * TEN % mod_pm + r) % mod_pm);
        TEN *= TEN;
        TEN %= mod_pm;
        d <<= 1;
    }

    auto mod_pow = [mod_pm](ll a, ll n) -> ll
    {
        ll res = 1;
        while (n > 0)
        {
            if (n & 1)
            {
                res *= a;
                res %= mod_pm;
            }
            a *= a;
            a %= mod_pm;
            n >>= 1;
        }
        return res;
    };

    TEN = 1;
    vector<pair<ll, ll>> query(K);
    rep(k, 0, K)
    {
        cin >> query[k].first >> query[k].second;
    }
    rrep(k, 0, K)
    {
        ll C = query[k].first, L = query[k].second;

        ll N_i = (C * TEN) % mod_pm;
        ll l = L;
        ll TEN2 = 1;
        ll TEN3 = 10;
        ll repunit = 0;
        int i = 0;
        while (l > 0)
        {
            if (l & 1)
            {
                repunit += (repunit_mod_pm[i] * TEN2) % mod_pm;
                repunit %= mod_pm;
                TEN2 *= TEN3;
                TEN2 %= mod_pm;
            }
            TEN3 *= TEN3;
            TEN3 %= mod_pm;
            ++i;
            l >>= 1;
        }
        N_i *= repunit;
        N_i %= mod_pm;
        N += N_i;
        N %= mod_pm;
        TEN *= TEN2;
        TEN %= mod_pm;
    }
    cout << N / M << endl;
}