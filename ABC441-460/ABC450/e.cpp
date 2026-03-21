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

// S_k に含まれるXの個数

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string X, Y;
    cin >> X >> Y;

    ll x = X.size(), y = Y.size();
    ll max_len = 1e18;

    auto Mul = [](const vector<vector<ll>> &a, const vector<vector<ll>> &b) -> vector<vector<ll>> &
    {
        vector<vector<ll>> res = {{0, 0}, {0, 0}};
        rep(i, 0, 2)
        {
            rep(j, 0, 2)
            {
                rep(k, 0, 2)
                {
                    res[i][j] += a[i][k] * b[k][j];
                }
            }
        }
        return res;
    };

    vector<vector<ll>> base = {{1, 1}, {1, 0}},
                       I = {{1, 0}, {0, 1}};
    vector<vector<vector<ll>>> doubling;
    doubling.emplace_back(I);
    doubling.emplace_back(base);
    while (doubling.back()[0][0] * y + doubling.back()[0][1] * x <= max_len)
    {
        doubling.emplace_back(Mul(doubling.back(), doubling.back()));
    }
    int Q;
    cin >> Q;
    int siz = doubling.size();

    map<ll, pair<ll, ll>> mp;

    auto f = [&](auto f, ll Z, char C) -> ll
    {
        if (mp.contains(Z))
        {
            ll k = mp[Z].first;
            while (k > 0)
            {
            }
        }
        ll k = siz - 1;
        ll kk = 0;
        vector<vector<ll>> arr = I;
        while (k >= 0)
        {
            vector<vector<ll>> temp = Mul(arr, doubling[k]);
            if (temp[0][0] * y + temp[0][1] * x < Z)
            {
                arr = temp;
                kk += 1LL << k;
            }
            k--;
        }
    };

    rep(q, 0, Q)
    {
        ll L, R;
        char C;
        cin >> L >> R >> C;
        L--;
        ll ans = f(R, C) - f(L, C);
        cout << ans << '\n';
    }
}