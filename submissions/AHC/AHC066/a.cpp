#include <bits/stdc++.h>
using namespace std;
#define rep(i, s, n) for (int i = s; i < (int)n; i++)
#define rrep(i, s, n) for (int i = (int)n - 1; i >= s; i--)
#define all(a) a.begin(), a.end()
#define rall(a) a.rbegin(), a.rend()

class TimeKeeper {
private:
  std::chrono::high_resolution_clock::time_point start_time_;
  int64_t time_threshold_;

public:
  TimeKeeper() {};

  TimeKeeper(const int64_t &time_threshold)
      : start_time_(std::chrono::high_resolution_clock::now()), time_threshold_(time_threshold) {}

  bool isTimeOver() const {
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;
    auto diff = std::chrono::high_resolution_clock::now() - this->start_time_;

    return duration_cast<milliseconds>(diff).count() >= time_threshold_;
  }

  auto get_time() const {
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;
    auto diff = std::chrono::high_resolution_clock::now() - this->start_time_;
    return duration_cast<milliseconds>(diff).count();
  }
};

using P = pair<int, int>;
constexpr const int INF = 1e9;

// 入力用クラス
struct Problem {
  int N, M, T;
  vector<string> v, h;
  vector<P> ball, basket;
  Problem() { input(); }

  void input() {
    cin >> N >> M >> T;
    v.resize(N);
    h.resize(N - 1);
    rep(i, 0, N) { cin >> v[i]; }
    rep(i, 0, N - 1) { cin >> h[i]; }
    ball.resize(M);
    basket.resize(M);
    rep(k, 0, M) { cin >> ball[k].first >> ball[k].second >> basket[k].first >> basket[k].second; }
  }
};

// 出力用クラス
struct Answer {
  string a;
  void print() { cout << a << endl; }
  void fprint() {
    string filename = "output.txt";
    ofstream ofs(filename);
    ofs << a << endl;
    ofs.close();
  }
};

struct Solver {
  Problem prob;
  int S;
  Solver(Problem &problem) : prob(problem), S(problem.N * problem.N * 4) {}
  vector<vector<int>> dist; // 頂点: (i, j, dir)
  vector<vector<int>> prev_state;
  vector<vector<char>> prev_operatoion;

  // main関数から呼ぶ
  void solve() { /* solve1();*/ }

  // 幹線木を設計して，そこを経由する方針
  void solve1() {}
};

int main() {
  Problem prob;
  Solver sol(prob);
  sol.solve();
}