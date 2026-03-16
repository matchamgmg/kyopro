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

// レピュニット数を行列累乗で求める

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll mod_p = 10007;
    ll K, M;
    cin >> K >> M;
    ll mod_pm = mod_p * M;
    ll N = 0;

    vector<pair<ll, ll>> query(K);
    rep(k, 0, K)
    {
        cin >> query[k].first >> query[k].second;
    }

    auto calc_repunit = [mod_pm](ll x) -> pair<ll, ll>
    {
        vector<vector<ll>> A = {{10, 1}, {0, 1}};
        vector<vector<ll>> I = {{1, 0}, {0, 1}};
        ll TEN2 = 1;
        ll TEN3 = 10;
        auto mul = [mod_pm](const vector<vector<ll>> &a, const vector<vector<ll>> &b) -> vector<vector<ll>>
        {
            vector<vector<ll>> res = {{0, 0}, {0, 0}};
            rep(i, 0, 2)
            {
                rep(j, 0, 2)
                {
                    rep(k, 0, 2)
                    {
                        res[i][j] += a[i][k] * b[k][j] % mod_pm;
                        res[i][j] %= mod_pm;
                    }
                }
            }
            return res;
        };
        auto arr_mod = [mod_pm](vector<vector<ll>> &v) -> void
        {
            rep(i, 0, 2)
            {
                rep(j, 0, 2)
                {
                    v[i][j] %= mod_pm;
                }
            }
        };

        while (x > 0)
        {
            if (x & 1)
            {
                I = mul(I, A);
                arr_mod(I);
                TEN2 *= TEN3;
                TEN2 %= mod_pm;
            }
            A = mul(A, A);
            arr_mod(A);
            TEN3 *= TEN3;
            TEN3 %= mod_pm;
            x >>= 1;
        }
        return {I[0][1], TEN2};
    };

    ll TEN = 1;
    rrep(k, 0, K)
    {
        ll C = query[k].first, L = query[k].second;

        ll N_i = (C * TEN) % mod_pm;
        ll l = L;
        auto [repunit, TEN2] = calc_repunit(L);
        N_i *= repunit;
        N_i %= mod_pm;
        N += N_i;
        N %= mod_pm;
        TEN *= TEN2;
        TEN %= mod_pm;
    }
    cout << N / M << endl;
}