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

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll N, L, R, T;
    cin >> N >> L >> R >> T;
    struct Okashi
    {
        ll n, p, s;
        Okashi() {};
        Okashi(ll n_, ll p_, ll s_) : n(n_), p(p_), s(s_) {}
        bool operator<(const Okashi &rhs) const
        {
            return p == rhs.p ? s == rhs.s ? n < rhs.n : s > rhs.s : p < rhs.p;
        }
    };
    vector<Okashi> O(N);
    rep(i, 0, N)
    {
        ll P, S;
        cin >> P >> S;
        O[i] = Okashi(i, P, S);
    }
    sort(all(O));
    int i = 0;
    while (i < N)
    {
        if (L <= O[i].p && O[i].p <= R && O[i].s >= T)
        {
            cout << O[i].n + 1 << endl;
            return 0;
        }
        ++i;
    }
    cout << -1 << endl;
}