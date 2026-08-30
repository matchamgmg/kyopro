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
    const int INF = 1e9;
    auto e = make_pair(INF, -1);
    vector<vector<pair<int, int>>> dist(H * W, vector<pair<int, int>>(4, e));
    int sx, sy;
    int gx, gy;
    rep(i, 0, H)
    {
        rep(j, 0, W)
        {
            if (S[i][j] == 'S')
            {
                sx = i;
                sy = j;
            }
            if (S[i][j] == 'G')
            {
                gx = i;
                gy = j;
            }
        }
    }
    queue<pair<int, int>> que;
    vector<int> dx = {0, 1, 0, -1}, dy{1, 0, -1, 0};
    rep(k, 0, 4)
    {
        dist[sx * W + sy][k].first = 0;
        que.emplace(sx * W + sy, k);
    }
    while (!que.empty())
    {
        auto p = que.front();
        que.pop();
        int x = p.first / W;
        int y = p.first % W;
        if (S[x][y] == 'o')
        {
            rep(d, 0, 4)
            {
                if (d != p.second)
                {
                    continue;
                }
                int nx = x + dx[d];
                int ny = y + dy[d];
                if (!grid_check(nx, ny, H, W))
                    continue;
                if (S[nx][ny] == '#')
                    continue;
                if (dist[nx * W + ny][d].first != INF)
                    continue;
                dist[nx * W + ny][d] = {dist[p.first][p.second].first + 1, p.second};
                que.emplace(nx * W + ny, d);
            }
        }
        else if (S[x][y] == 'x')
        {
            rep(d, 0, 4)
            {
                if (d == p.second)
                {
                    continue;
                }
                int nx = x + dx[d];
                int ny = y + dy[d];
                if (!grid_check(nx, ny, H, W))
                    continue;
                if (S[nx][ny] == '#')
                    continue;
                if (dist[nx * W + ny][d].first != INF)
                    continue;
                dist[nx * W + ny][d] = {dist[p.first][p.second].first + 1, p.second};
                que.emplace(nx * W + ny, d);
            }
        }
        else
        {
            rep(d, 0, 4)
            {
                int nx = x + dx[d];
                int ny = y + dy[d];
                if (!grid_check(nx, ny, H, W))
                    continue;
                if (S[nx][ny] == '#')
                    continue;
                if (dist[nx * W + ny][d].first != INF)
                    continue;
                dist[nx * W + ny][d] = {dist[p.first][p.second].first + 1, p.second};
                que.emplace(nx * W + ny, d);
            }
        }
    }
    vector<int> ans;
    int k = -1;
    int d = INF;
    rep(i, 0, 4)
    {
        if (dist[gx * W + gy][i].first < d)
        {
            d = dist[gx * W + gy][i].first;
            k = i;
        }
    }
    if (d > 5000000)
    {
        cout << "No" << endl;
        return 0;
    }
    else
    {
        cout << "Yes" << endl;
    }
    while (!(gx == sx && gy == sy))
    {
        int nk = dist[gx * W + gy][k].second;
        ans.emplace_back(k);
        gx -= dx[k];
        gy -= dy[k];
        k = nk;
    }
    reverse(all(ans));
    vector<char> RDLU = {'R', 'D', 'L', 'U'};
    for (int i : ans)
    {
        cout << RDLU[i];
    }
    cout << endl;
    // rep(i, 0, H * W)
    // {
    //     cout << i << ':' << ' ';
    //     rep(j, 0, 4)
    //     {
    //         cout << '(' << dist[i][j].first << ',' << dist[i][j].second << ')' << ' ';
    //     }
    //     cout << endl;
    // }
}