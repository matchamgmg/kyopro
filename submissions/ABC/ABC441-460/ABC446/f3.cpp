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

//
// k以下の頂点からすべての辺を調べる
// k以下の頂点に到達可能ならば，そこから伸びる辺を調べる
// k以下の頂点のうち、到達可能な頂点の集合S
// Sから一手以内で到達可能な頂点の集合T
// Sの要素数 == k : Yes
//      T \ S
// Sの要素数 != k : No
// T\S の最小値を管理

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;
    vector<vector<int>> G(N);
    rep(i, 0, M)
    {
        int U, V;
        cin >> U >> V;
        U--;
        V--;
        G[U].emplace_back(V);
    }
    vector<bool> visited(N, false);
    pq_g<int> S, T_minus_S;
    T_minus_S.emplace(0);
    visited[0] = true;
    rep(i, 0, N)
    {
        while (!T_minus_S.empty() && T_minus_S.top() <= i)
        {
            int u = T_minus_S.top();
            T_minus_S.pop();
            S.emplace(u);
            for (int v : G[u])
            {
                if (visited[v])
                {
                    continue;
                }
                T_minus_S.emplace(v);
                visited[v] = true;
            }
        }
        if (S.size() == i + 1)
        {
            cout << T_minus_S.size() << endl;
        }
        else
        {
            cout << -1 << endl;
        }
    }
}