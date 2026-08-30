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

void solve()
{
    int N, M, K;
    cin >> N >> M >> K;
    string S;
    cin >> S;
    vector<vector<int>> G(N);
    rep(i, 0, M)
    {
        int U, V;
        cin >> U >> V;
        U--;
        V--;
        G[U].emplace_back(V);
    }

    // dp[i][j] : i回目の操作の直前，頂点jにいるとき，その手番の人が必勝or必敗
    vector<vector<bool>> dp(2 * K + 1, vector<bool>(N, false));
    rep(i, 0, N)
    {
        dp[2 * K][i] = (S[i] == 'A');
    }
    rrep(i, 0, 2 * K)
    {
        rep(j, 0, N)
        {
            bool win = false;
            for (int k : G[j])
            {
                win = win | (dp[i + 1][k] == false);
            }
            dp[i][j] = win;
        }
    }
    cout << (dp[0][0] ? "Alice" : "Bob") << endl;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    for (; T--;)
        solve();
}