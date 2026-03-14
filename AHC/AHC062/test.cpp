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

constexpr const int N = 3;
array<array<long long, N>, N> A;

struct Answer
{
    array<pair<int, int>, N * N> ans;
    long long score;
    void calc_score()
    {
        score = 0;
        rep(i, 0, N * N)
        {
            score += i * A[ans[i].first][ans[i].second];
        }
    }
    void print_ans()
    {
        for (auto &p : ans)
        {
            cout << p.first << ' ' << p.second << '\n';
        }
    }
    bool check_ans()
    {
        bool res = true;
        rep(i, 1, N * N)
        {
            res &= (max(abs(ans[i].first - ans[i - 1].first), abs(ans[i].second - ans[i - 1].second)) == 1);
        }
        return res;
    }
    Answer() {}
};

Answer bitDP_approach(array<array<long long, N>, N> &A)
{
    Answer res;
    array<array<long long, 3>, 3> Block;
    for (auto &b : Block)
    {
        b.fill(0);
    }
    rep(i, 0, N)
    {
        rep(j, 0, N)
        {
            Block[i][j] += A[i][j];
        }
    }
    array<array<long long, 1 << 9>, 9> dp;
    const long long INF = 1LL << 60;
    for (auto &d : dp)
    {
        d.fill(-INF);
    }
    rep(i, 0, 3)
    {
        rep(j, 0, 3)
        {
            int k = i * 3 + j;
            dp[k][1 << k] = Block[i][j];
        }
    }
    long long max_dp = 0;
    int x = 0, y = 0;
    rep(bit, 0, 1 << 9)
    {
        rep(i, 0, 9)
        {
            if (dp[i][bit] == -INF)
                continue;
            long long k = popcount<unsigned int>(bit);
            int xi = i / 3;
            int yi = i % 3;
            rep(j, 0, 9)
            {
                if (bit >> j & 1)
                    continue;
                int xj = j / 3;
                int yj = j % 3;
                int d = max(abs(xi - xj), abs(yi - yj));
                if (d != 1)
                    continue;
                int nbit = bit | (1 << j);
                chmax(dp[j][nbit], dp[i][bit] + (k + 1) * Block[xj][yj]);
                if (k == 8 && dp[j][nbit] > max_dp)
                {
                    x = xj;
                    y = yj;
                    max_dp = dp[j][nbit];
                }
            }
        }
    }
    vector<int> Block_array(9);
    Block_array[8] = x * 3 + y;
    int bi = (1 << 9) - 1;
    rrep(i, 0, 8)
    {
        for (int j = 0; j < 9; ++j)
        {
            cout << (bi - (1 << j)) << endl;
            cout << dp[j][(bi - (1 << j))] + (i + 1) * Block[j / 4][j % 4] << ' ' << dp[Block_array[i + 1]][bi] << endl;
            if ((bi >> j) % 2)
            {
                if (dp[j][bi ^ (1 << j)] + (i + 1) * Block[x][y] == dp[x * 3 + y][bi])
                {
                    x = j / 3;
                    y = j % 3;
                    bi = bi ^ (1 << j);
                    Block_array[i] = x * 3 + y;
                }
            }
        }
    }
    rep(i, 0, 9)
    {
        cout << Block_array[i] / 3 << ' ' << Block_array[i] % 3 << '\n';
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    rep(i, 0, N)
    {
        rep(j, 0, N)
        {
            cin >> A[i][j];
        }
    }
    bitDP_approach(A);
}