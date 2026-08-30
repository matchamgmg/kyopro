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
void pyes() { cout << "Yes" << endl; }
void pno() { cout << "No" << endl; }
void YN(bool x)
{
    cout << (x ? "Yes" : "No") << endl;
}
template <class T>
void v_cout(const vector<T> &a)
{
    int n = a.size();
    rep(i, 0, n) cout << a[i] << " ";
    cout << endl;
}
template <class T>
void vv_cout(const vector<T> &a)
{
    int n = a.size();
    rep(i, 0, n)
    {
        rep(j, 0, a[i].size()) cout << a[i][j] << " ";
        cout << endl;
    }
}
bool grid_check(int x, int y, int X, int Y)
{
    return (0 <= x && x < X && 0 <= y && y < Y);
}
template <class T>
bool chmax(T &a, T b)
{
    if (a < b)
    {
        a = b;
        return true;
    }
    else
    {
        return false;
    }
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
    {
        return false;
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    vector<ll> X(N + 2, 0);
    int OFFSET = 1;
    rep(i, 0, N)
    {
        cin >> X[i + OFFSET];
    }
    const ll INF = 1e18;
    X[N + 1] = INF;
    set<pair<ll, int>> st;
    st.emplace(0LL, 0);
    st.emplace(INF, N + 1);

    vector<ll> dist(N + 2, INF);

    ll ans = 2 * INF;
    rep(i, 1, N + 1)
    {
        auto itr = st.lower_bound({X[i], 0});
        int n = (*itr).second;
        --itr;
        int p = (*itr).second;
        ll dist_n = dist[n];
        ll dist_x = min(abs(X[i] - X[n]), abs(X[i] - X[p]));
        ll dist_p = dist[p];
        dist[n] = min(dist_n, abs(X[i] - X[n]));
        dist[i] = dist_x;
        dist[p] = min(dist_p, abs(X[i] - X[p]));
        ans += -dist_p - dist_n + min(dist_n, abs(X[i] - X[n])) + dist_x + min(dist_p, abs(X[i] - X[p]));
        st.emplace(X[i], i);
        cout << ans - dist[N + 1] << endl;
    }
}