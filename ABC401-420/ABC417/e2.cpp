#include <atcoder/all>
#include <bits/stdc++.h>

using namespace std;
using namespace atcoder;

using ll = long long;
using mint = modint998244353;

// lazysegtree
// 区間更新
// 区間和

const ll MOD = 998244353;

struct S {
  ll val;
  ll siz;
};

S e() { return S{0LL, 0LL}; }
S op(S a, S b) { return S{(a.val + b.val) % MOD, a.siz + b.siz}; }

using F = ll;
const F ID = MOD;
F id() { return ID; }
S mapping(F f, S x) { return (f == ID ? x : S{f * x.siz, x.siz}); }
F composition(F f, F g) { return (f == ID ? g : f); }

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, M;
  cin >> N >> M;
  vector<int> A(N);
  for (int i = 0; i < N; ++i) {
    cin >> A[i];
  }
  vector<S> V(N);
  for (int i = 0; i < N; ++i) {
    V[i].val = A[i];
    V[i].siz = 1LL;
  }
  lazy_segtree<S, op, e, F, mapping, composition, id> seg(V);

  int L, R;
  for (; M--;) {
    cin >> L >> R;
    L--;
    ll sumLR = seg.prod(L, R).val;
    ll aveLR = (mint(sumLR) * mint(R - L).inv()).val();
    seg.apply(L, R, aveLR);
  }
  for (int i = 0; i < N; ++i) {
    cout << seg.get(i).val << " \n"[i == N - 1];
  }
}