#include <bits/stdc++.h>

#include <atcoder/all>
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
void pyes() { cout << "Yes" << endl; }
void pno() { cout << "No" << endl; }
void YN(bool x) { cout << (x ? "Yes" : "No") << endl; }
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
bool grid_check(int x, int y, int X, int Y) { return (0 <= x && x < X && 0 <= y && y < Y); }
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

namespace Geo2Vec {
const long double eps = 1e-10;

template <class T> struct Line {
  vector<T> start, vec;
  vector<long double> t_limit;

  Line() {}
  Line(vector<T> &start_, vector<T> &vec_) : start(start_), vec(vec_) {
    t_limit.resize(2);
    t_limit[0] = -(1e10);
    t_limit[1] = 1e10;
  }
  Line(vector<T> &start_, vector<T> &vec_, long double t_min, long double t_max) : start(start_), vec(vec_) {
    t_limit.resize(2);
    t_limit[0] = t_min;
    t_limit[1] = t_max;
  }

  vector<long double> GetPoint(long double t) {
    t = max(t, t_limit[0]);
    t = min(t, t_limit[1]);
    vector<long double> res(2);
    res[0] = (long double)start[0] + t * vec[0];
    res[1] = (long double)start[1] + t * vec[1];
    return res;
  }
};

template <class T> vector<T> MakeVec(T x1, T y1, T x2, T y2) {
  vector<T> res(2);
  res[0] = x2 - x1;
  res[1] = y2 - y1;
  return res;
}

template <class T> long double VecSize(vector<T> &A) { return sqrt((long double)(A[0] * A[0] + A[1] * A[1])); }

template <class T> vector<long double> NormVec(vector<T> &A) {
  long double siz = VecSize(A);
  vector<long double> res(2);
  res[0] = A[0] / siz;
  res[1] = A[1] / siz;
  return res;
}

template <class T> vector<T> AddVec(vector<T> &A, vector<T> &B) {
  vector<T> res(2);
  res[0] = A[0] + B[0];
  res[1] = A[1] + B[1];
  return res;
}

template <class T> vector<T> DiffVec(vector<T> &A, vector<T> &B) {
  vector<T> res(2);
  res[0] = A[0] - B[0];
  res[1] = A[1] - B[1];
  return res;
}

template <class T> T Outer(vector<T> &A, vector<T> &B) { return A[0] * B[1] - A[1] * B[0]; }

template <class T> T Inner(vector<T> &A, vector<T> &B) { return A[0] * B[0] + A[1] * B[1]; }

// 垂線の長さ
template <class T> long double NormalLineLength(vector<T> &P, vector<T> &S, vector<T> &G) {
  vector<T> PS = DiffVec(S, P), PG = DiffVec(G, P), SG = DiffVec(G, S);
  return abs(Outer(PS, PG) / VecSize(SG));
}

template <class T> long double NormalLineLength(vector<T> &PS, vector<T> &PG) {
  vector<T> SG = DiffVec(PS, PG);
  return abs(Outer(PS, PG) / VecSize(SG));
}

// 点と線分の最短距離
template <class T> long double Point2LineDistance(vector<T> &P, vector<T> &S, vector<T> &G) {
  vector<T> PS = DiffVec(S, P), PG = DiffVec(G, P), SG = DiffVec(G, S);
  if (Inner(PS, SG) > -eps)
    return VecSize(PS);
  if (Inner(PG, SG) < eps)
    return VecSize(PG);
  return abs(Outer(PS, PG) / VecSize(SG));
}

template <class T> long double Point2LineDistance(vector<T> &PS, vector<T> &PG) {
  vector<T> SG = DiffVec(PG, PS);
  if (Inner(PS, SG) > -eps)
    return VecSize(PS);
  if (Inner(PG, SG) < eps)
    return VecSize(PG);
  return abs(Outer(PS, PG) / VecSize(SG));
}
} // namespace Geo2Vec

void solve() {
  int TSx, TSy, TGx, TGy, ASx, ASy, AGx, AGy;
  cin >> TSx >> TSy >> TGx >> TGy >> ASx >> ASy >> AGx >> AGy;
  int distT = (TSx - TGx) * (TSx - TGx) + (TSy - TGy) * (TSy - TGy);
  int distA = (ASx - AGx) * (ASx - AGx) + (ASy - AGy) * (ASy - AGy);
  if (distA < distT) {
    swap(TSx, ASx);
    swap(TSy, ASy);
    swap(TGx, AGx);
    swap(TGy, AGy);
    swap(distT, distA);
  }

  vector<double> O = {0, 0};
  vector<double> TS = Geo2Vec::MakeVec<double>(0, 0, TSx, TSy);
  vector<double> TG = Geo2Vec::MakeVec<double>(0, 0, TGx, TGy);
  vector<double> AS = Geo2Vec::MakeVec<double>(0, 0, ASx, ASy);
  vector<double> AG = Geo2Vec::MakeVec<double>(0, 0, AGx, AGy);
  vector<double> A = Geo2Vec::DiffVec(AG, AS);
  vector<double> T = Geo2Vec::DiffVec(TG, TS);
  vector<double> AT = Geo2Vec::DiffVec(A, T);
  vector<double> ATS = Geo2Vec::DiffVec(AS, TS);
  vector<double> ATmid(2);
  rep(i, 0, 2) { ATmid[i] = AT[i] / sqrt(distT); }
  vector<double> ATGmid = Geo2Vec::AddVec(ATS, ATmid);
  long double ans = 1e9;
  vector<double> Amid(2);
  rep(i, 0, 2) { Amid[i] = A[i] / sqrt(distT); }
  chmin(ans, Geo2Vec::Point2LineDistance(O, ATS, ATGmid));
  chmin(ans, Geo2Vec::Point2LineDistance(TG, Amid, AG));
  cout << fixed << setprecision(10) << ans << endl;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  while (T--)
    solve();
}