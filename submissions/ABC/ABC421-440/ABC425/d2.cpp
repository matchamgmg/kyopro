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
    vector<bool> used(H * W, false);
    queue<int> frontier;
    queue<int> whites;
    vector<int> dist(H * W, INF);

    int time = 0;
    rep(i, 0, H)
    {
        rep(j, 0, W)
        {
            if (S[i][j] == '#')
            {
                dist[i * W + j] = time;
                used[i * W + j] = true;
                frontier.emplace(i * W + j);
            }
        }
    }
    vector<int> dx = {0, 1, 0, -1}, dy{1, 0, -1, 0};

    auto check = [&](int x, int y) -> bool
    {
        int cnt = 0;
        rep(d, 0, 4)
        {
            int nx = x + dx[d];
            int ny = y + dy[d];
            if (!grid_check(nx, ny, H, W))
            {
                continue;
            }
            cnt += (S[nx][ny] == '#');
        }
        return cnt == 1;
    };

    while (!frontier.empty())
    {
        ++time;
        while (!frontier.empty() && dist[frontier.front()] < time)
        {
            int p = frontier.front();
            int x = p / W;
            int y = p % W;
            frontier.pop();
            rep(d, 0, 4)
            {
                int nx = x + dx[d];
                int ny = y + dy[d];
                if (!grid_check(nx, ny, H, W))
                {
                    continue;
                }
                if (used[nx * W + ny])
                {
                    continue;
                }
                if (S[nx][ny] == '#')
                {
                    continue;
                }
                used[nx * W + ny] = true;
                dist[nx * W + ny] = time;
                if (check(nx, ny))
                {
                    whites.emplace(nx * W + ny);
                }
            }
        }
        while (!whites.empty())
        {
            int p = whites.front();
            whites.pop();
            S[p / W][p % W] = '#';
            frontier.emplace(p);
        }
    }
    int ans = 0;
    rep(i, 0, H)
    {
        rep(j, 0, W)
        {
            ans += (S[i][j] == '#');
        }
    }
    cout << ans << endl;
    // rep(i, 0, H)
    // {
    //     cout << S[i] << endl;
    // }
}