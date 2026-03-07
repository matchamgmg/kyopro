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
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
#define YN(a) \
    if (a)    \
    {         \
        pyes; \
    }         \
    else      \
    {         \
        pno;  \
    }
#define v_cout(a)                         \
    {                                     \
        int n = a.size();                 \
        rep(i, 0, n) cout << a[i] << " "; \
        cout << endl;                     \
    }
#define vv_cout(a)                                         \
    {                                                      \
        int n = a.size();                                  \
        rep(i, 0, n)                                       \
        {                                                  \
            rep(j, 0, a[i].size()) cout << a[i][j] << " "; \
            cout << endl;                                  \
        }                                                  \
        cout << endl;                                      \
    }
#define grid_check(x, y, X, Y) (0 <= x && x < X && 0 <= y && y < Y)
template <class T>
bool chmax(T &a, T b)
{
    if (a < b)
    {
        a = b;
        return true;
    }
    else
        return false;
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
        return false;
}

class TimeKeeper
{
private:
    std::chrono::high_resolution_clock::time_point start_time_;
    int64_t time_threshold_;

public:
    TimeKeeper() {};

    TimeKeeper(const int64_t &time_threshold)
        : start_time_(std::chrono::high_resolution_clock::now()),
          time_threshold_(time_threshold)
    {
    }

    bool isTimeOver() const
    {
        using std::chrono::duration_cast;
        using std::chrono::milliseconds;
        auto diff = std::chrono::high_resolution_clock::now() - this->start_time_;

        return duration_cast<milliseconds>(diff).count() >= time_threshold_;
    }

    auto get_time() const
    {
        using std::chrono::duration_cast;
        using std::chrono::milliseconds;
        auto diff = std::chrono::high_resolution_clock::now() - this->start_time_;
        return duration_cast<milliseconds>(diff).count();
    }
};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int Q;
    cin >> Q;
    vector<ll> X(Q), Y(Q);
    rep(i, 0, Q) cin >> X[i] >> Y[i];
    vector<int> qidx(Q);
    iota(all(qidx), 0);

    ll N = 2e7 + 1;

    int maxN = 1;
    while (maxN < N)
    {
        maxN <<= 1;
    }

    vector<long long> Hilbert_val(Q);
    auto Hilbert_order = [&](int x, int y) -> long long
    {
        long long rx, ry, d;
        d = 0;
        for (long long s = maxN >> 1; s > 0; s >>= 1)
        {
            rx = (x & s) > 0, ry = (y & s) > 0;
            d += s * s * ((rx * 3) ^ ry);
            if (ry > 0)
                continue;
            if (rx > 0)
            {
                x = maxN - 1 - x;
                y = maxN - 1 - y;
            }
            swap(x, y);
        }
        return d;
    };

    for (int i = 0; i < Q; ++i)
        Hilbert_val[i] = Hilbert_order(X[i], Y[i]);
    sort(all(qidx),
         [&](int i, int j)
         { return Hilbert_val[i] < Hilbert_val[j]; });

    int start = 0;
    while (qidx[start] != 0)
        start++;
    auto calc_dist = [&](int i, int j) -> ll
    {
        return abs(X[i] - X[j]) + abs(Y[i] - Y[j]);
    };
    ll time = 0;
    rep(i, 0, Q)
    {
        time += calc_dist(qidx[(i + start) % Q], qidx[(i + 1 + start) % Q]);
    }
    if (time <= 1e10)
    {
        rep(i, 0, Q)
        {
            cout << qidx[(i + start) % Q] + 1 << endl;
        }
    }
    else
    {
        TimeKeeper tk(1500);
        mt19937 mt(0);
        const ll INF = 1LL << 60;
        auto f = [&](int e, int pos) -> void
        {
            while (e != pos)
            {
                swap(qidx[e], qidx[(e + 1) % Q]);
                e = (e + 1) % Q;
            }
        };
        while (!tk.isTimeOver() && time > 1e10)
        {
            int p = mt() % (Q - 1) + 1;
            ll ptime = calc_dist(qidx[p], qidx[p - 1]) + calc_dist(qidx[p], qidx[(p + 1) % Q]);
            ll ntime = INF;
            int q = -1;
            rep(i, 0, Q)
            {
                if (i == p)
                    continue;
                if ((i + 1) % Q == p)
                    continue;
                ll t = -ptime - calc_dist(qidx[i], qidx[(i + 1) % Q]) + calc_dist(qidx[i], qidx[p]) + calc_dist(qidx[p], qidx[(i + 1) % Q]);
                if (t < ntime)
                {
                    ntime = t;
                    q = i;
                }
            }
            if (ptime > ntime)
            {
                f(p, q);
            }
        }
        start = 0;
        while (qidx[start] != 0)
            start++;
        rep(i, 0, Q)
        {
            cout << qidx[(i + start) % Q] + 1 << endl;
        }
    }
}