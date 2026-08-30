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

    int N, M;
    cin >> N >> M;
    vector<int> P(N);
    vector<ll> V(N);
    rep(i, 0, N)
    {
        cin >> P[i] >> V[i];
    }

    const ll INF = 1LL << 60;
    vector<vector<ll>> dp_l(N + 1, vector<ll>(M + 1, -INF)), dp_r(N + 1, vector<ll>(M + 1, -INF));

    rep(i, 0, M + 1)
    {
        dp_l[0][i] = 0;
    }
    rep(i, 0, N)
    {
        rep(j, 0, M + 1)
        {
            chmax(dp_l[i + 1][j], dp_l[i][j]);
            if (j - P[i] >= 0)
            {
                chmax(dp_l[i + 1][j], dp_l[i][j - P[i]] + V[i]);
            }
        }
    }

    ll max_value = *max_element(all(dp_l[N]));

    rep(i, 0, M + 1)
    {
        dp_r[N][i] = 0;
    }
    rrep(i, 0, N)
    {
        rep(j, 0, M + 1)
        {
            chmax(dp_r[i][j], dp_r[i + 1][j]);
            if (j - P[i] >= 0)
            {
                chmax(dp_r[i][j], dp_r[i + 1][j - P[i]] + V[i]);
            }
        }
    }

    // 必ず選ぶ:
    // どちらでもよい:
    // 決して選ばない:

    vector<char> ans(N);
    rep(i, 0, N)
    {
        bool a = false;
        bool c = false;
        rep(j, 0, M + 1)
        {
            if (dp_l[i + 1][j] + dp_r[i + 1][M - j] == max_value)
            {
                a |= (j - P[i] >= 0 && dp_l[i][j - P[i]] + V[i] == dp_l[i + 1][j]);
                c |= (dp_l[i][j] == dp_l[i + 1][j]);
            }
        }
        if (a && c)
        {
            ans[i] = 'B';
        }
        else
        {
            ans[i] = (a ? 'A' : 'C');
        }
    }
    for (char s : ans)
    {
        cout << s;
    }
    cout << endl;
}