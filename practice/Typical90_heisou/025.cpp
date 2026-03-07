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

// N<10^11 よりf(N)<=9^11
// 重複組合せ 11H10 : (11+9)! / 11! / 9! = 167,960
// 状態 : 各数字の個数
// m = f(m) + B <= N && f(m) == f(m) をカウント

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll N, B;
    cin >> N >> B;
    set<ll> st;
    auto f = [&](ll x) -> ll
    {
        ll res = 1;
        while (x > 0)
        {
            res *= (x % 10);
            x /= 10;
        }
        return res;
    };
    auto f2 = [&](vector<int> &v) -> ll
    {
        ll res = 1;
        rep(i, 0, 10)
        {
            rep(j, 0, v[i])
            {
                res *= i;
            }
        }
        return res;
    };
    int cnt = 0;
    auto DFS = [&](auto f, int depth, int k, vector<int> &v) -> void
    {
        if (k == 10)
        {
            cnt++;
            st.emplace(f2(v));
            return;
        }
        rep(i, depth, 12)
        {
            v[k] = 11 - i;
            f(f, depth + v[k], k + 1, v);
            v[k] = 0;
        }
        return;
    };

    vector<int> v(10, 0);
    DFS(DFS, 0, 0, v);
    ll ans = 0;
    for (ll fm : st)
    {
        ll m = fm + B;
        if (m > N)
            continue;
        if (f(m) != fm)
            continue;
        ans++;
    }
    cout << ans << endl;
}