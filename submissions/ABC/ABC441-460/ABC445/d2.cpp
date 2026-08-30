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

    int H, W, N;
    cin >> H >> W >> N;

    vector<int> h(N), w(N);
    vector<bool> used(N, false);
    vector<pair<int, int>> ans(N);
    pq<pair<int, int>> h_que, w_que;
    rep(i, 0, N)
    {
        cin >> h[i] >> w[i];
        h_que.emplace(h[i], i);
        w_que.emplace(w[i], i);
    }
    int x = 0, y = 0;
    int cnt = 0;
    // H*W overflow
    while (cnt < N)
    {
        while (!h_que.empty() && h_que.top().first == H)
        {
            auto p = h_que.top();
            h_que.pop();
            if (used[p.second])
            {
                continue;
            }
            used[p.second] = true;
            cnt++;
            ans[p.second] = {x, y};
            y += w[p.second];
            W -= w[p.second];
        }
        while (!w_que.empty() && w_que.top().first == W)
        {
            auto p = w_que.top();
            w_que.pop();
            if (used[p.second])
            {
                continue;
            }
            used[p.second] = true;
            cnt++;
            ans[p.second] = {x, y};
            x += h[p.second];
            H -= h[p.second];
        }
    }
    rep(i, 0, N)
    {
        cout << ans[i].first + 1 << ' ' << ans[i].second + 1 << endl;
    }
}