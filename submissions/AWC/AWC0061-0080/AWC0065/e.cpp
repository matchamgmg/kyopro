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

const ll INF = 1e18;

using S = pair<ll, ll>;
S op(S a, S b)
{
    return {max(a.first, b.first), min(a.second, b.second)};
};
S e() { return {-INF, INF}; }

ll max_minus_min(S x)
{
    return x.first - x.second;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll N, K, D;
    cin >> N >> K >> D;

    vector<S> H(N);
    rep(i, 0, N)
    {
        ll h;
        cin >> h;
        H[i] = {h, h};
    }

    segtree<S, op, e> seg(H);

    int r = 0;
    int ans = -1;
    rep(l, 0, N)
    {
        if (l >= r)
        {
            ++r;
        }
        while (r <= N && max_minus_min(seg.prod(l, r)) <= D)
        {
            ++r;
        }
        chmax(ans, r - l - 1);
    }
    cout << (ans >= K ? ans : -1) << endl;
}