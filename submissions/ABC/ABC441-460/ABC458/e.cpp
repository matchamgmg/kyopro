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

    int T = 1e6 + 2;
    vector<mint> kaijou(3 * T + 1, 0);
    kaijou[0] = 1;
    rep(i, 0, 3 * T)
    {
        kaijou[i + 1] = kaijou[i] * (i + 1);
    }

    vector<int> X(3);
    rep(i, 0, 3)
    {
        cin >> X[i];
    }
    mint ans = 0;
    rep(k, 2, X[1] + 2)
    {
        if (k % 2 == 0)
        {
            int m = k / 2;
            if (m - 1 < 0 || X[0] - m < 0 || X[0] - 1 < 0)
            {
                continue;
            }
            if (m - 1 < 0 || X[2] - m < 0 || X[2] - 1 < 0)
            {
                continue;
            }
            if (X[1] - k + 1 < 0 || X[0] + X[1] + X[2] - k + 1 < 0)
            {
                continue;
            }
            mint p1 = kaijou[X[0] - 1] * kaijou[X[0] - m].inv() * kaijou[m - 1].inv();
            mint p3 = kaijou[X[2] - 1] * kaijou[X[2] - m].inv() * kaijou[m - 1].inv();
            mint p2 = kaijou[X[0] + X[1] + X[2] - k + 1] * kaijou[X[1] - k + 1].inv() * kaijou[X[0] + X[2]].inv();
            ans += 2 * p1 * p3 * p2;
        }
        else
        {
            int m = k / 2;
            if (m - 1 >= 0 && X[0] - m >= 0 && X[0] - 1 >= 0 && X[2] - m - 1 >= 0 && X[2] - 1 >= 0 && X[1] - k + 1 >= 0 && X[0] + X[1] + X[2] - k + 1 >= 0)
            {
                mint p1 = kaijou[X[0] - 1] * kaijou[X[0] - m].inv() * kaijou[m - 1].inv();
                mint p3 = kaijou[X[2] - 1] * kaijou[X[2] - m - 1].inv() * kaijou[m].inv();
                mint p2 = kaijou[X[0] + X[1] + X[2] - k + 1] * kaijou[X[1] - k + 1].inv() * kaijou[X[0] + X[2]].inv();
                ans += p1 * p3 * p2;
            }
            if (m - 1 >= 0 && X[2] - m >= 0 && X[2] - 1 >= 0 && X[0] - m - 1 >= 0 && X[0] - 1 >= 0 && X[1] - k + 1 >= 0 && X[0] + X[1] + X[2] - k + 1 >= 0)
            {
                mint p1 = kaijou[X[0] - 1] * kaijou[X[0] - m - 1].inv() * kaijou[m].inv();
                mint p3 = kaijou[X[2] - 1] * kaijou[X[2] - m].inv() * kaijou[m - 1].inv();
                mint p2 = kaijou[X[0] + X[1] + X[2] - k + 1] * kaijou[X[1] - k + 1].inv() * kaijou[X[0] + X[2]].inv();
                ans += p1 * p3 * p2;
            }
        }
    }
    cout << ans.val() << endl;
}