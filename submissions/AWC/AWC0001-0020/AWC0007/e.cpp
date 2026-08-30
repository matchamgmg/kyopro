// bitDP
// スタート地点から集合Sの頂点をすべて辿ってゴールへ行くときの最短経路長
// 頂点vを追加するとき，スタート > Sの頂点 > v > ゴール
// Sの最後に訪れる頂点を全て保持
// 状態：(頂点集合S, Sの最後に訪れる頂点) O(M*2^M)
// スコア計算：-len(last, goal) + len(last, v) + len(v, goal)

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <class T>
void chmin(T& a, T b) {
    if (a > b) a = b;
    return;
}

int main() {
    ll N, M;
    cin >> N >> M;
    ll S, T;
    cin >> S >> T;
    vector<ll> P(M);
    for (int i = 0; i < M; ++i) cin >> P[i];
    P.emplace_back(S);
    P.emplace_back(T);
    vector<vector<ll>> edge(M + 2, vector<ll>(M + 2, 0));
    auto calc_dist = [&](int i, int j) -> ll {
        ll xi = (P[i] - 1) / N, yi = (P[i] - 1) % N;
        ll xj = (P[j] - 1) / N, yj = (P[j] - 1) % N;
        return abs(xi - xj) + abs(yi - yj);
    };
    for (int i = 0; i < M + 1; ++i) {
        for (int j = i + 1; j < M + 2; ++j) {
            edge[i][j] = calc_dist(i, j);
        }
    }
    auto get_len = [&](int i, int j) -> ll {
        if (i > j) swap(i, j);
        return edge[i][j];
    };

    const ll INF = 1LL << 60;
    vector<vector<ll>> dp(1LL << M, vector<ll>(M + 1, INF));
    dp[0][M] = get_len(M, M + 1);
    for (int bit = 0; bit < (1LL << M); ++bit) {
        for (int last = 0; last < M + 1; ++last) {
            if (dp[bit][last] == INF) continue;
            ll pcost = dp[bit][last] - get_len(last, M + 1);
            for (int v = 0; v < M; ++v) {
                if (bit >> v & 1) continue;
                int nbit = bit | (1 << v);
                ll ncost = pcost + get_len(last, v) + get_len(v, M + 1);
                chmin(dp[nbit][v], ncost);
            }
        }
    }
    cout << *min_element(dp[(1 << M) - 1].begin(), dp[(1 << M) - 1].end())
         << endl;
}