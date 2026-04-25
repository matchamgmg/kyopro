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

    struct Node
    {
        int i;
        int p;
        int n;
        Node() : i(-1), p(-1), n(-1) {}
    };

    vector<Node> nodes(N);
    rep(i, 0, N)
    {
        nodes[i].i = i;
    }
    vector<int> decks(N);
    iota(all(decks), 0);

    rep(q, 0, Q)
    {
        int c, p;
        cin >> c >> p;
        c--;
        p--;
        if (nodes[c].p == -1)
        {
            decks[c] = -1;
        }
        else
        {
            nodes[nodes[c].p].n = -1;
        }
        nodes[p].n = c;
        nodes[c].p = p;
    }

    rep(i, 0, N)
    {
        int cnt = 0;
        int j = decks[i];
        while (j != -1)
        {
            j = nodes[j].n;
            cnt++;
        }
        cout << cnt << ' ';
    }
    cout << endl;
}