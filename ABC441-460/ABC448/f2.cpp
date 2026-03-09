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

void Sort_by_snake_order(int N, int Q, std::vector<int> &index_vec, std::vector<int> &L, std::vector<int> &R)
{
    int divide = std::max(1, int(N / std::sqrt(Q)));
    auto Comp = [&L, &R, divide](int i, int j) -> bool
    {
        if (L[i] / divide != L[j] / divide)
        {
            return L[i] < L[j];
        }
        if ((L[i] / divide) & 1)
        {
            return R[i] > R[j];
        }
        return R[i] < R[j];
    };
    std::sort(index_vec.begin(), index_vec.end(), Comp);
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    vector<int> X(N), Y(N);
    rep(i, 0, N) cin >> X[i] >> Y[i];
    vector<int> idx(N);
    rep(i, 0, N) idx[i] = i;
    int W = 2e7;
    Sort_by_snake_order(W, N, idx, X, Y);
    int start = 0;
    while (idx[start] != 0)
        start++;
    rep(i, 0, N)
    {
        cout << idx[(start + i) % N] + 1 << ' ';
    }
    cout << endl;
}