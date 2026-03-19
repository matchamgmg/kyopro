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
    vector<string> S(H);
    rep(i, 0, H)
    {
        cin >> S[i];
    }
    vector<vector<int>> Warp(26);
    vector<bool> used(26, false);
    rep(i, 0, H)
    {
        rep(j, 0, W)
        {
            if (S[i][j] == '.' || S[i][j] == '#')
            {
                continue;
            }
            else
            {
                Warp[S[i][j] - 'a'].emplace_back(i * W + j);
            }
        }
    }
    vector<int> dx = {0, 1, 0, -1}, dy{1, 0, -1, 0};
    const ll INF = 1LL << 60;
    vector<ll> dist(H * W, INF);
    queue<int> que;
    dist[0] = 0;
    que.emplace(0);
    while (!que.empty())
    {
        int u = que.front();
        que.pop();
        if (!(S[u / W][u % W] == '.' || S[u / W][u % W] == '#'))
        {
            if (!used[S[u / W][u % W] - 'a'])
            {
                used[S[u / W][u % W] - 'a'] = true;
                for (int v : Warp[S[u / W][u % W] - 'a'])
                {
                    if (dist[v] < INF)
                    {
                        continue;
                    }
                    dist[v] = dist[u] + 1;
                    que.emplace(v);
                }
            }
        }
        int x = u / W;
        int y = u % W;
        rep(d, 0, 4)
        {
            int nx = x + dx[d];
            int ny = y + dy[d];
            if (!grid_check(nx, ny, H, W))
                continue;
            if (S[nx][ny] == '#')
                continue;
            if (dist[nx * W + ny] < INF)
                continue;
            dist[nx * W + ny] = dist[u] + 1;
            que.emplace(nx * W + ny);
        }
    }
    cout << (dist[H * W - 1] == INF ? -1 : dist[H * W - 1]) << endl;
}