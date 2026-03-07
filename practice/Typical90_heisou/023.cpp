#include <bits/stdc++.h>

#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
// using mint = modint998244353;
using mint = modint1000000007;
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

// あるマスについて、そのマスにキングを置けるかどうかは
// 直前のW+1マスに対する置き方のみよって決まる
// さらに長さWの01列について、連続する1が発生しないような列は
// O(1.62^W)通り程度しかない

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int H, W;
    cin >> H >> W;
    vector<string> S(H);
    rep(i, 0, H) cin >> S[i];
    vector<vector<int>> state(W);
    vector<int> cnt(W, 0); // 列ごとに番号を管理
    // ある状態に関してキングが置かれいてるマスを管理
    vector<vector<bool>> used(H + 1, vector<bool>(W + 1, false));
    // ある状態から(sx,sy)にキングを置けるかどうか
    vector<map<int, pair<int, bool>>> mp(W);
    vector<vector<int>> next0(W), next1(W); // 次の状態

    //(sx,sy)が八近傍に対して条件を満たしているかを判定
    auto hantei = [&](int sx, int sy) -> bool
    {
        vector<int> dx = {1, 1, 0, -1, -1, -1, 0, 1};
        vector<int> dy = {0, 1, 1, 1, 0, -1, -1, -1};
        rep(d, 0, 8)
        {
            int tx = sx + dx[d];
            int ty = sy + dy[d];
            if (!grid_check(tx, ty, H + 1, W))
                continue;
            if (used[tx][ty])
                return false;
        }
        return true;
    };

    // DFSによりあり得る状態を列挙
    auto DFS = [&](auto f, int pos, int depth, int str) -> void
    {
        int sx = pos / W, sy = pos % W;
        if (depth == W + 1)
        {
            int idx = cnt[sy];
            bool flag = hantei(sx, sy);
            state[sy].emplace_back(str);
            mp[sy][str] = make_pair(idx, flag);
            cnt[sy] += 1;
            return;
        }

        f(f, pos + 1, depth + 1, str);
        if (hantei(sx, sy))
        {
            used[sx][sy] = true;
            f(f, pos + 1, depth + 1, str + (1 << depth));
            used[sx][sy] = false;
        }
        return;
    };

    // 状態を列挙
    rep(i, 0, W) { DFS(DFS, i, 0, 0); }
    // ある状態から次にどの状態に遷移できるかを列挙
    rep(i, 0, W)
    {
        next0[i].resize(cnt[i]);
        next1[i].resize(cnt[i]);
        rep(j, 0, cnt[i])
        {
            int t = state[i][j];
            int t0 = (t >> 1);      // i+1の状態をみる
            int t1 = t0 + (1 << W); // キングを置いた場合
            next0[i][j] = mp[(i + 1) % W][t0].first;
            if (mp[i][t].second)
            {
                next1[i][j] = mp[(i + 1) % W][t1].first;
            }
            else
            {
                next1[i][j] = -1;
            }
        }
    }

    vector<vector<vector<int>>> dp(H + 1, vector<vector<int>>(W + 1, vector<int>(*max_element(all(cnt)), 0)));

    const int MOD = 1e9 + 7;
    dp[0][0][0] = 1;
    rep(i, 0, H)
    {
        rep(j, 0, W)
        {
            int n1 = i, n2 = j + 1;
            if (n2 == W)
            {
                n1 += 1, n2 = 0;
            }
            rep(k, 0, cnt[j])
            {
                if (dp[i][j][k] == 0)
                    continue;
                dp[n1][n2][next0[j][k]] += dp[i][j][k];
                dp[n1][n2][next0[j][k]] %= MOD;
                if (next1[j][k] != -1 && S[i][j] == '.')
                {
                    dp[n1][n2][next1[j][k]] += dp[i][j][k];
                    dp[n1][n2][next1[j][k]] %= MOD;
                }
            }
        }
    }

    int ans = 0;
    rep(i, 0, cnt[0])
    {
        ans += dp[H][0][i];
        ans %= MOD;
    }
    cout << ans << endl;
}