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

    int H, W;
    cin >> H >> W;
    vector<vector<bool>> Choco(H, vector<bool>(W, true));
    int D = H, R = W;
    int Q;
    cin >> Q;
    for (; Q--;)
    {
        int t, x;
        cin >> t >> x;
        if (t == 1)
        {
            int ans = 0;
            int cnt = 0;
            for (int i = D - 1; cnt < x; --i)
            {
                rep(j, 0, W)
                {
                    ans += (Choco[i][j]);
                    Choco[i][j] = false;
                }
                cnt++;
            }
            D -= cnt;
            cout << ans << endl;
        }
        else
        {
            int ans = 0;
            int cnt = 0;
            for (int j = R - 1; cnt < x; --j)
            {
                rep(i, 0, H)
                {
                    ans += (Choco[i][j]);
                    Choco[i][j] = false;
                }
                cnt++;
            }
            R -= cnt;
            cout << ans << endl;
        }
        // rep(i, 0, H)
        // {
        //     rep(j, 0, W)
        //     {
        //         cout << (Choco[i][j] ? '#' : '.');
        //     }
        //     cout << '\n';
        // }
    }
}