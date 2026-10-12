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
  ll val;
  ll siz;
};

struct F {
  bool on;
};

S op(const S &a, const S &b) {
  return S{a.val + b.val, a.siz + b.siz};
}

S e() {
  return S{0, 0};
}

S mapping(const F &f, const S &x) {
  S res;
  res.siz = x.siz;
  if (f.on) {
    res.val = x.siz - x.val;
  } else {
    res.val = x.val;
  }
  return res;
}

F composition(const F &f, const F &g) {
  F res;
  res.on = g.on ^ f.on;
  return res;
}

F id() {
  return F{false};
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int Q;
  cin >> Q;
  vector<ll> T(Q), L(Q), R(Q);
  rep(i, 0, Q) {
    cin >> T[i] >> L[i] >> R[i];
    L[i]--;
  }
  vector<ll> temp = L;
  rep(q, 0, Q) {
    temp.push_back(R[q]);
  }
  temp.push_back(0);
  temp.push_back(1e18);
  sort(all(temp));
  temp.erase(unique(all(temp)), temp.end());
  int N = temp.size();
  vector<int> dict(2 * Q);
  rep(i, 0, Q) {
    dict[i] = lower_bound(all(temp), L[i]) - temp.begin();
  }
  rep(i, 0, Q) {
    dict[i + Q] = lower_bound(all(temp), R[i]) - temp.begin();
  }

  lazy_segtree<S, op, e, F, mapping, composition, id> seg(N);
  rep(i, 0, N - 1) {
    seg.set(i, S{0, temp[i + 1] - temp[i]});
  }
  seg.set(N - 1, S{0, (ll)(1e18) - temp[N - 1]});
  rep(q, 0, Q) {
    ll t, l, r;
    t = T[q];
    l = dict[q];
    r = dict[q + Q];
    if (t == 1) {
      seg.apply(l, r, F{true});
    } else {
      cout << seg.prod(l, r).val << endl;
    }
  }
}