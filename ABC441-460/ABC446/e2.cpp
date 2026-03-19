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

template <class T>
struct Vec2D
{
    Vec2D(int n) : height(1), width(n), vec(std::vector<T>(n, T())) {}
    Vec2D(int h, int w) : height(h), width(w), vec(std::vector<T>(h * w, T())) {}

    void fill(T x) { std::fill(vec.begin(), vec.end(), x); }

    int get_width() { return width; }
    int get_height() { return height; }
    int get_size() { return height * width; }

    T &operator[](int i) { return vec[i]; }
    const T &operator[](int i) const { return vec[i]; }

    T &operator[](int i, int j) { return vec[i * width + j]; }
    const T &operator[](int i, int j) const { return vec[i * width + j]; }

    std::pair<int, int> trans_index(int i) { return {i / width, i % width}; }
    int trans_index(int x, int y) { return x * width + y; }

    void print()
    {
        int i = 0;
        for (T &x : vec)
        {
            std::cout << x << " \n"[i % width == width - 1];
            ++i;
        }
    }

private:
    int width, height;
    std::vector<T> vec;
};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int M, A, B;
    cin >> M >> A >> B;
    Vec2D<vector<int>> G(M, M);
    Vec2D<int> visited(M, M);
    visited.fill(false);
    queue<int> que;
    rep(s1, 0, M)
    {
        rep(s2, 0, M)
        {
            int s3 = A * s2 + B * s1;
            s3 %= M;
            G[s2, s3].emplace_back(G.trans_index(s1, s2));
            if (s1 == 0 || s2 == 0)
            {
                que.emplace(G.trans_index(s1, s2));
                visited[s1, s2] = true;
            }
        }
    }
    while (!que.empty())
    {
        int u = que.front();
        que.pop();
        for (int v : G[u])
        {
            if (visited[v])
                continue;
            visited[v] = true;
            que.emplace(v);
        }
    }
    int ans = 0;
    rep(i, 0, M)
    {
        rep(j, 0, M)
        {
            ans += (visited[i, j] == false);
        }
    }
    cout << ans << endl;
}