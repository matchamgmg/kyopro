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

constexpr const int N = 200;
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

bool check_dist(int a, int b)
{
    return max(abs(a / 4 - b / 4), abs(a % 4 - b % 4)) == 1;
}

vector<pair<int, int>> fifty(int block_num, bool st, bool sl, bool gt, bool gl)
{
    vector<pair<int, int>> arr(50 * 50);
    long long max_score = 0;
    if ((st == gt) || (sl == gl))
    {
        if (sl == gl)
        {
            rep(i, 0, 50)
            {
                int ii = (st ? i : 49 - i);
                rep(j, 0, 50)
                {
                    int jj = (sl ? j : 49 - j);
                    if (i & 1)
                    {
                        jj = 49 - jj;
                    }
                    arr[i * 50 + j] = {(block_num / 4) * 50 + ii, (block_num % 4) * 50 + jj};
                }
            }
        }
        else
        {
            rep(j, 0, 50)
            {
                int jj = (sl ? j : 49 - j);
                rep(i, 0, 50)
                {
                    int ii = (st ? i : 49 - i);
                    if (j & 1)
                    {
                        ii = 49 - ii;
                    }
                    arr[j * 50 + i] = {(block_num / 4) * 50 + ii, (block_num % 4) * 50 + jj};
                }
            }
        }
    }
    else
    {
        if (st == sl)
        {
            rep(i, 0, 48)
            {
                int ii = (st ? i : 49 - i);
                rep(j, 0, 50)
                {
                    int jj = (st ? j : 49 - j);
                    if (i & 1)
                    {
                        jj = 49 - jj;
                    }

                    arr[i * 50 + j] = {(block_num / 4) * 50 + ii, (block_num % 4) * 50 + jj};
                }
            }
            rep(j, 0, 50)
            {
                int jj = (st ? j : 49 - j);
                rep(i, 48, 50)
                {
                    int ii = (st ? i : 49 - i);
                    arr[50 * 48 + j * 2 + i - 48] = {(block_num / 4) * 50 + ii, (block_num % 4) * 50 + jj};
                }
            }
        }
        else
        {
            rep(i, 0, 48)
            {
                int ii = (st ? i : 49 - i);
                rep(j, 0, 50)
                {
                    int jj = (st ? 49 - j : j);
                    if (i & 1)
                    {
                        jj = 49 - jj;
                    }

                    arr[i * 50 + j] = {(block_num / 4) * 50 + ii, (block_num % 4) * 50 + jj};
                }
            }
            rep(j, 0, 50)
            {
                int jj = (st ? 49 - j : j);
                rep(i, 48, 50)
                {
                    int ii = (st ? i : 49 - i);
                    arr[50 * 48 + j * 2 + i - 48] = {(block_num / 4) * 50 + ii, (block_num % 4) * 50 + jj};
                }
            }
        }
    }
    return arr;
}

Answer bitDP_approach()
{
    Answer res;
    vector<vector<long long>> Block(4, vector<long long>(4, 0));
    rep(i, 0, N)
    {
        rep(j, 0, N)
        {
            Block[i / 50][j / 50] += A[i][j];
        }
    }
    const long long INF = 1LL << 60;
    vector<vector<pair<long long, pair<int, int>>>> dp(16, vector<pair<long long, pair<int, int>>>(1 << 16, {-INF, {-1, -1}}));
    rep(i, 0, 4)
    {
        rep(j, 0, 4)
        {
            int k = i * 4 + j;
            dp[k][1 << k].first = Block[i][j];
        }
    }
    long long max_dp = 0;
    int x = 0, y = 0;
    rep(bit, 0, 1 << 16)
    {
        rep(i, 0, 16)
        {
            if (dp[i][bit].first == -INF)
                continue;
            long long k = popcount<unsigned int>(bit);
            int xi = i / 4;
            int yi = i % 4;
            rep(j, 0, 16)
            {
                if (bit >> j & 1)
                    continue;
                int xj = j / 4;
                int yj = j % 4;
                int d = max(abs(xi - xj), abs(yi - yj));
                if (d != 1)
                    continue;
                int nbit = bit | (1 << j);
                if (chmax(dp[j][nbit].first, dp[i][bit].first + (k + 1) * Block[xj][yj]))
                {
                    dp[j][nbit].second.first = i;
                    dp[j][nbit].second.second = bit;
                }
                if (k == 15 && dp[j][nbit].first > max_dp)
                {
                    x = xj;
                    y = yj;
                    max_dp = dp[j][nbit].first;
                }
            }
        }
    }
    vector<int> Block_array(16);
    Block_array[15] = x * 4 + y;
    int bit = (1 << 16) - 1;
    rrep(i, 0, 15)
    {
        Block_array[i] = dp[Block_array[i + 1]][bit].second.first;
        bit = dp[Block_array[i + 1]][bit].second.second;
    }
    rep(i, 0, 16)
    {
        cout << Block_array[i] / 4 << ' ' << Block_array[i] % 4 << '\n';
    }
    vector<vector<int>> stslgtgl(16, vector<int>(4, -1));
    rep(i, 0, 15)
    {
        int xi = Block_array[i] / 4;
        int yi = Block_array[i] % 4;
        int xj = Block_array[i + 1] / 4;
        int yj = Block_array[i + 1] % 4;
        if (xi == xj)
        {
            if (yi < yj)
            {
                stslgtgl[i][3] = 0;
                stslgtgl[i][2] = 0;
                stslgtgl[i + 1][1] = 1;
                stslgtgl[i + 1][0] = 1;
            }
            else
            {
                stslgtgl[i][3] = 1;
                stslgtgl[i][2] = 1;
                stslgtgl[i + 1][1] = 0;
                stslgtgl[i + 1][0] = 0;
            }
        }
        else if (yi == yj)
        {
            if (xi < xj)
            {
                stslgtgl[i][2] = 0;
                stslgtgl[i][3] = 0;
                stslgtgl[i + 1][0] = 1;
                stslgtgl[i + 1][1] = 1;
            }
            else
            {
                stslgtgl[i][2] = 1;
                stslgtgl[i][3] = 1;
                stslgtgl[i + 1][0] = 0;
                stslgtgl[i + 1][1] = 0;
            }
        }
        else
        {
            if (xi < xj)
            {
                if (yi < yj)
                {
                    stslgtgl[i][2] = 0;
                    stslgtgl[i][3] = 0;
                    stslgtgl[i + 1][0] = 1;
                    stslgtgl[i + 1][1] = 1;
                }
                else
                {
                    stslgtgl[i][2] = 0;
                    stslgtgl[i][3] = 1;
                    stslgtgl[i + 1][0] = 1;
                    stslgtgl[i + 1][1] = 0;
                }
            }
            else
            {
                if (yi < yj)
                {
                    stslgtgl[i][2] = 1;
                    stslgtgl[i][3] = 0;
                    stslgtgl[i + 1][0] = 0;
                    stslgtgl[i + 1][1] = 1;
                }
                else
                {
                    stslgtgl[i][2] = 1;
                    stslgtgl[i][3] = 1;
                    stslgtgl[i + 1][0] = 0;
                    stslgtgl[i + 1][1] = 0;
                }
            }
        }
    }
    stslgtgl[0][0] = stslgtgl[0][1] = stslgtgl[15][2] = stslgtgl[15][3] = 0;
    rep(i, 0, 16)
    {
        int idx = Block_array[i];
        vector<pair<int, int>> arr = fifty(idx, stslgtgl[idx][0], stslgtgl[idx][1], stslgtgl[idx][2], stslgtgl[idx][3]);
        rep(a, 0, 50)
        {
            rep(b, 0, 50)
            {
                res.ans[i * 2500 + a * 50 + b] = arr[a * 50 + b];
            }
        }
    }
    return res;
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
    Answer ans = bitDP_approach();

    ans.print_ans();
}