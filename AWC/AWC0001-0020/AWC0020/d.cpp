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
    int N, Len;
    cin >> N >> Len;
    set<int> st;
    vector<int> L(N), R(N);
    rep(i, 0, N)
    {
        int X, r;
        cin >> X >> r;
        st.emplace(X - r);
        st.emplace(X + r + 1);
        L[i] = X - r;
        R[i] = X + r + 1;
    }
    map<int, int> mp;
    int siz = 0;
    for (int s : st)
    {
        mp[s] = siz++;
    }
    vector<int> S(siz, 0);
    rep(i, 0, N)
    {
        S[mp[L[i]]]++;
        S[mp[R[i]]]--;
    }
    rep(i, 1, siz)
    {
        S[i] += S[i - 1];
    }
    auto l = mp.lower_bound(0);
    if (l != mp.begin())
    {
        l--;
    }
    else
    {
        pno;
        return 0;
    }
    auto r = mp.upper_bound(Len);
    if (r == mp.end())
    {
        pno;
        return 0;
    }
    bool ok = true;
    while (l != r)
    {
        ok &= (S[(*l).second] > 0);
        l++;
    }
    YN(ok);
}