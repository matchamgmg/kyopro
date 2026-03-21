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

struct UnionFind
{
    UnionFind(int n_) : n(n_), group_num(n_) { par.assign(n_, -1); }
    int root(int x)
    {
        if (par[x] < 0)
            return x;
        return par[x] = root(par[x]);
    }
    int size(int x) { return -par[root(x)]; }
    bool same(int x, int y) { return root(x) == root(y); }
    bool merge(int x, int y)
    {
        x = root(x);
        y = root(y);
        if (x == y)
            return false;
        if (size(x) > size(y))
            swap(x, y);
        par[y] += par[x];
        par[x] = y;
        group_num--;
        return true;
    }
    int groups() { return group_num; }

private:
    int n;
    int group_num;
    vector<int> par;
};

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
    UnionFind uf(H * W);
    vector<int> dx = {0, 1, 0, -1}, dy{1, 0, -1, 0};
    rep(x, 0, H)
    {
        rep(y, 0, W)
        {
            rep(d, 0, 4)
            {
                int nx = x + dx[d];
                int ny = y + dy[d];
                if (!grid_check(nx, ny, H, W))
                    continue;
                if (S[x][y] == '.' && S[nx][ny] == '.')
                {
                    uf.merge(x * W + y, nx * W + ny);
                }
            }
        }
    }
    int groups = uf.groups();
    int black = 0;
    rep(i, 0, H)
    {
        rep(j, 0, W)
        {
            black += (S[i][j] == '#');
        }
    }
    set<int> surround;
    rep(i, 0, H)
    {
        rep(j, 0, W)
        {
            if (S[i][j] == '.' && (i == 0 || i == H - 1 || j == 0 || j == W - 1))
            {
                surround.emplace(uf.root(i * W + j));
            }
        }
    }
    int ans = groups - black - surround.size();
    cout << ans << endl;
}