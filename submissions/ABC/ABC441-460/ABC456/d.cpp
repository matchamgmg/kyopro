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

    string S;
    cin >> S;
    int N = S.size();
    // 最後に選んだ文字種ごとに
    // 文字列の個数を管理
    // 累積和

    vector<vector<mint>> dp(N + 1, vector<mint>(3, 0));
    rep(i, 0, N)
    {
        if (S[i] == 'a')
        {
            dp[i + 1][0] += 1;
            dp[i + 1][0] += dp[i][1] + dp[i][2];
            rep(j, 0, 3)
            {
                dp[i + 1][j] += dp[i][j];
            }
        }
        else if (S[i] == 'b')
        {
            dp[i + 1][1] += 1;
            dp[i + 1][1] += dp[i][2] + dp[i][0];
            rep(j, 0, 3)
            {
                dp[i + 1][j] += dp[i][j];
            }
        }
        else if (S[i] == 'c')
        {
            dp[i + 1][2] += 1;
            dp[i + 1][2] += dp[i][0] + dp[i][1];
            rep(j, 0, 3)
            {
                dp[i + 1][j] += dp[i][j];
            }
        }
    }
    cout << (dp[N][0] + dp[N][1] + dp[N][2]).val() << endl;
}