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

    int N, Q;
    cin >> N >> Q;
    vector<string> S(Q);
    rep(i, 0, Q)
    {
        cin >> S[i];
    }
    const int INF = 1 << 29;
    vector<int> dist(2e6, INF);
    queue<int> que;

    auto DFS = [&](auto f, int d, int s, int k) -> void
    {
        if (d == N)
        {
            dist[s] = 0;
            que.emplace(s);
            return;
        }
        for (int cnt = 0; cnt < 3; ++cnt)
        {
            if (cnt + k >= 3)
            {
                continue;
            }
            s = s * 3 + cnt + k;
            f(f, d + 1, s, k + cnt);
            s /= 3;
        }
    };
    int str = 0;
    DFS(DFS, 0, str, 0);
    while (!que.empty())
    {
        int str = que.front();
        int rev_str = 0;
        int s = str;
        int c = 0;
        while (c < N)
        {
            rev_str = rev_str * 3 + s % 3;
            s /= 3;
            ++c;
        }
        que.pop();
        int d = dist[str];
        int under = 0;
        int top = rev_str;
        int three = 1, three2 = 1;
        rep(i, 0, N)
        {
            three *= 3;
        }
        rep(i, 0, N - 1)
        {

            int nstr = top % three * three2 + str % three2;
            three /= 3;
            three2 *= 3;
            if (dist[nstr] <= d + 1)
            {
                continue;
            }
            chmin(dist[nstr], d + 1);
            que.emplace(nstr);
        }
    }
    rep(i, 0, Q)
    {
        int str = 0;
        rep(j, 0, N)
        {
            str = str * 3 + S[i][j] - 'A';
        }
        int d = dist[str];
        cout << (d == INF ? -1 : d) << endl;
    }
}