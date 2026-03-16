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
struct BIT
{
    BIT(int n_) : n(n_), data(n_ + 1) {}

    void add(int p, T x = 1)
    {
        for (++p; p <= n; p += p & -p)
        {
            data[p] += x;
        }
    }

    // [0, p)
    T prefix_sum(int p)
    {
        T res = 0;
        for (; p > 0; p -= p & -p)
        {
            res += data[p];
        }
        return res;
    }

    // [l, r)
    T sum(int l, int r)
    {
        return prefix_sum(r) - prefix_sum(l);
    }

    // binary_search
    // sum[0,k] >= v となる最小のkを返す
    // 存在しない場合はn
    int lower_bound(T v)
    {
        int w = 1, i = 0;
        while (w < n)
        {
            w <<= 1;
        }
        for (++w; w > 0; w >>= 1)
        {
            if (i + w > n)
                continue;
            if (data[i + w] < v)
                v -= data[i += w];
        }
        return i;
    }

    // sum[0,k] > v となる最小のkを返す
    int upper_bound(T v)
    {
        return lower_bound(v + 1);
    }

    // sum[0,k] >= v となる最小のkを返す
    int min_left(T v)
    {
        return lower_bound(v);
    }

    // sum[0, k] <= v となる最大のkを返す
    // 存在しない場合は-1
    int max_right(T v)
    {
        return lower_bound(v + 1) - 1;
    }

    // K番目の要素を取得
    int get_Kth_element(T k)
    {
        return lower_bound(k);
    }

private:
    int n;
    std::vector<T> data;
};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;
    vector<int> A(N);
    rep(i, 0, N) cin >> A[i], A[i]--;
    int Q;
    cin >> Q;
    vector<int> ans(Q);
    vector<pair<ll, int>> Qs;
    rep(qi, 0, Q)
    {
        ll X;
        cin >> X;
        X--;
        if (X < N)
        {
            ans[qi] = A[X];
        }
        else
        {
            Qs.emplace_back(X, qi);
        }
    }
    sort(rall(Qs));

    // 出現頻度が同じものをまとめる
    vector<vector<int>> adds(N + 1);
    {
        vector<int> cnt(M, 0);
        rep(i, 0, N)
        {
            cnt[A[i]]++;
        }
        rep(i, 0, M)
        {
            adds[cnt[i]].push_back(i);
        }
    }

    BIT<int> ft(M);
    int num = 0;
    ll l = N;
    rep(c, 0, N + 1)
    {
        for (int i : adds[c])
        {
            ft.add(i);
            num++;
        }
        while (Qs.size() > 0 && Qs.back().first < l + num)
        {
            auto [X, qi] = Qs.back();
            Qs.pop_back();
            ans[qi] = ft.get_Kth_element(X - l + 1);
        }
        l += num;
    }
    while (Qs.size())
    {
        auto [X, qi] = Qs.back();
        Qs.pop_back();
        ans[qi] = (X - l) % M;
    }
    rep(i, 0, Q)
    {
        cout << ans[i] + 1 << endl;
    }
}