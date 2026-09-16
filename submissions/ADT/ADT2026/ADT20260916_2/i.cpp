/**
 * https://github.com/matchamgmg/kyopro/tree/main
 */

#include <atcoder/all>
#include <bits/stdc++.h>

using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
// using mint = modint1000000007;
template <class T> using pq = priority_queue<T, vector<T>>;               // 大きい順
template <class T> using pq_g = priority_queue<T, vector<T>, greater<T>>; // 小さい順

#define rep(i, s, n) for (int i = (s); i < (int)(n); i++)
#define rrep(i, s, n) for (int i = (n - 1); i >= (int)(s); i--)
#define all(a) a.begin(), a.end()
#define rall(a) a.rbegin(), a.rend()
void pyes() {
  cout << "Yes" << endl;
}
void pno() {
  cout << "No" << endl;
}
void YN(bool x) {
  cout << (x ? "Yes" : "No") << endl;
}
template <class T> void v_cout(const vector<T> &a) {
  int n = a.size();
  rep(i, 0, n) cout << a[i] << " ";
  cout << endl;
}
template <class T> void vv_cout(const vector<T> &a) {
  int n = a.size();
  rep(i, 0, n) {
    rep(j, 0, a[i].size()) cout << a[i][j] << " ";
    cout << endl;
  }
}
bool grid_check(int x, int y, int X, int Y) {
  return (0 <= x && x < X && 0 <= y && y < Y);
}
template <class T> bool chmax(T &a, T b) {
  if (a < b) {
    a = b;
    return true;
  } else {
    return false;
  }
}
template <class T> bool chmin(T &a, T b) {
  if (a > b) {
    a = b;
    return true;
  } else {
    return false;
  }
}

struct S {
  ll sum_A;
  ll max_sumA_B;
};

const ll INF = 1e18;

S op(const S &a, const S &b) {
  ll sum = a.sum_A + b.sum_A;
  ll max_AB = max(a.max_sumA_B, a.sum_A + b.max_sumA_B);
  return S{sum, max_AB};
}

S e() {
  return S{0, -INF};
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, Q;
  cin >> N >> Q;
  vector<ll> A(N), B(N);
  rep(i, 0, N) {
    cin >> A[i];
  }
  rep(i, 0, N) {
    cin >> B[i];
  }
  struct Query {
    int t;
    int i;
    ll x;
  };
  vector<Query> query(Q);
  rep(q, 0, Q) {
    cin >> query[q].t >> query[q].i >> query[q].x;
    query[q].i--;
  }
  vector<pair<ll, pair<pair<int, int>, int>>> BB;
  rep(i, 0, N) {
    BB.push_back({B[i], {{i, i}, -1}});
  }
  int cnt = 0;
  rep(q, 0, Q) {
    if (query[q].t == 2) {
      BB.push_back({query[q].x, {{query[q].i, N + cnt++}, -1}});
    }
  }
  vector<int> B_next(N, -1);
  rrep(i, 0, BB.size()) {
    BB[i].second.second = B_next[BB[i].second.first.first];
    B_next[BB[i].second.first.first] = BB[i].second.first.second;
  }
  sort(rall(BB));

  segtree<S, op, e> seg(BB.size());
  vector<int> current_idx(N);
  rep(i, 0, BB.size()) {
    if (BB[i].second.first.second < N) {
      seg.set(i, S{A[BB[i].second.first.first], A[BB[i].second.first.first] + BB[i].first});
      current_idx[BB[i].second.first.first] = BB[i].second.first.second;
    }
  }
  rep(i, 0, BB.size()) {
    cout << BB[i].first << ' ' << BB[i].second.first.first << ' ' << BB[i].second.first.second << ' '
         << BB[i].second.second << endl;
  }

  cout << seg.all_prod().max_sumA_B << endl;

  rep(q, 0, Q) {
    int p = current_idx[query[q].i];
    if (query[q].t == 1) {
      seg.set(p, S{query[q].x, query[q].x + BB[p].first});
      A[BB[p].second.first.first] = query[q].x;
    } else {
      seg.set(p, e());
      int np = BB[p].second.second;
      seg.set(np, S{A[BB[np].second.first.first], A[BB[np].second.first.first] + BB[np].first});
    }
    cout << seg.all_prod().max_sumA_B << endl;
  }
}