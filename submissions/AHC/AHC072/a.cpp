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

const int INF = 1e9;
array<char, 4> UDLR = {'U', 'D', 'L', 'R'};
array<int, 4> dx = {-1, 1, 0, 0}, dy = {0, 0, -1, 1};

struct Problem {
  int N;
  int K;
  vector<string> C;
  vector<vector<deque<int>>> Slimes; // 0 ~ 11: color(slime)
  vector<vector<int>> Nests;         // 0 ~ 11: color(nest), -1: floor, -2: wall
  Problem() {
    input();
  }
  void input() {
    cin >> N >> K;
    C.resize(N);
    for (string &s : C) {
      cin >> s;
    }
  }
};

struct Legal_action {
  int x;
  int y;
  int k;
  int d;
  int l;
  friend ostream &operator<<(ostream &os, const Legal_action &ac) {
    cout << ac.x << ' ' << ac.y << ' ' << ac.k << ' ' << UDLR[ac.d] << ' ' << ac.l;
    return os;
  }
};

struct Answer {
  vector<Legal_action> actions;
  void print() {
    for (Legal_action &action : actions) {
      cout << action << '\n';
    }
  }
};

struct Slime {
  int color;
  int x;
  int y;
  int rest;
  bool operator<(const Slime &rhs) const {
    return rest < rhs.rest;
  }
};

struct Solver {
  Problem P;
  mt19937 mt = mt19937{0};

  void initialize_field(vector<vector<deque<int>>> &Slimes, vector<vector<int>> &Nests) {
    for (int i = 0; i < P.N; ++i) {
      for (int j = 0; j < P.N; ++j) {
        char &c = P.C[i][j];
        if (c == '.') {
          Nests[i][j] = -1;
        } else if (c == '#') {
          Nests[i][j] = -2;
        } else if (c >= 'a') {
          Slimes[i][j].push_back(c - 'a');
          Nests[i][j] = -1;
        } else {
          Nests[i][j] = (c - 'A');
        }
      }
    }
  }
  void calc_all_color_distance(vector<vector<vector<int>>> &Dist, const vector<vector<deque<int>>> &Slimes,
                               const vector<vector<int>> &Nests) {
    vector<pair<int, int>> nest(P.K);
    for (int i = 0; i < P.N; ++i) {
      for (int j = 0; j < P.N; ++j) {
        if (Nests[i][j] >= 0) {
          nest[Nests[i][j]] = {i, j};
        }
      }
    }
    queue<pair<int, int>> que;
    for (int i = 0; i < P.K; ++i) {
      auto p = nest[i];
      Dist[i][p.first][p.second] = 0;
      que.push(p);
      while (!que.empty()) {
        p = que.front();
        int pd = Dist[i][p.first][p.second];
        que.pop();
        for (int d = 0; d < 4; ++d) {
          int nx = p.first + dx[d];
          int ny = p.second + dy[d];
          if (!(0 <= nx && nx < P.N && 0 <= ny && ny < P.N)) {
            continue;
          }
          if (Nests[nx][ny] == -2) {
            continue;
          }
          if (Dist[i][nx][ny] != INF) {
            continue;
          }
          Dist[i][nx][ny] = pd + 1;
          que.emplace(nx, ny);
        }
      }
    }
  }

  Legal_action get_single_greedy_action(const Slime &slime, const vector<vector<deque<int>>> &Slimes,
                                        const vector<vector<int>> &Nests, const vector<vector<vector<int>>> &Dist) {
    Legal_action action;
    action.x = slime.x;
    action.y = slime.y;
    int max_l = Slimes[slime.x][slime.y].size();
    action.k = max_l - 1;
    vector<pair<int, int>> best_action_candidates;
    int best_rest = INF;
    for (int d = 0; d < 4; ++d) {
      for (int l = 1; l <= max_l; ++l) {
        int nx = slime.x + dx[d] * l;
        int ny = slime.y + dy[d] * l;
        if (!(0 <= nx && nx < P.N && 0 <= ny && ny < P.N)) {
          continue;
        }
        if (Nests[nx][ny] == -2) {
          break;
        }
        int rest = Dist[slime.color][nx][ny];
        if (rest < best_rest) {
          best_action_candidates.clear();
          best_action_candidates.emplace_back(d, l);
          best_rest = rest;
        } else if (rest == best_rest) {
          best_action_candidates.emplace_back(d, l);
        }
      }
    }
    int n = best_action_candidates.size();
    int r = mt() % n;
    action.d = best_action_candidates[r].first;
    action.l = best_action_candidates[r].second;
    return action;
  }

  void run_single_greedy_action(Slime &slime, const Legal_action &action, vector<vector<deque<int>>> &Slimes,
                                const vector<vector<vector<int>>> &Dist, const vector<vector<int>> &Nests) {
    Slimes[slime.x][slime.y].pop_back();
    int px = slime.x;
    int py = slime.y;
    slime.x += dx[action.d] * action.l;
    slime.y += dy[action.d] * action.l;
    slime.rest = Dist[slime.color][slime.x][slime.y];
    Slimes[slime.x][slime.y].push_back(slime.color);
    while (!Slimes[px][py].empty() && Slimes[px][py].back() == Nests[px][py]) {
      Slimes[px][py].pop_back();
    }
    while (!Slimes[slime.x][slime.y].empty() && Slimes[slime.x][slime.y].back() == Nests[slime.x][slime.y]) {
      Slimes[slime.x][slime.y].pop_back();
    }
  }

  Answer solve_single_greedy() {
    Answer ans;
    vector<vector<deque<int>>> Slimes(P.N, vector<deque<int>>(P.N));
    vector<vector<bool>> Top_is_back(P.N, vector<bool>(P.N, false));
    vector<vector<int>> Nests(P.N, vector<int>(P.N));
    initialize_field(Slimes, Nests);
    vector<vector<vector<int>>> Dist(P.K, vector<vector<int>>(P.N, vector<int>(P.N, INF)));
    calc_all_color_distance(Dist, Slimes, Nests);
    // 距離の遠いものから順番に動かす
    // 最後まで運ぶ
    priority_queue<Slime> slime_que;
    [&]() {
      for (int x = 0; x < P.N; ++x) {
        for (int y = 0; y < P.N; ++y) {
          for (int col : Slimes[x][y]) {
            slime_que.emplace(col, x, y, Dist[col][x][y]);
          }
        }
      }
    }();
    while (!slime_que.empty()) {
      auto s = slime_que.top();
      slime_que.pop();
      while (s.rest > 0) {
        Legal_action action = get_single_greedy_action(s, Slimes, Nests, Dist);
        ans.actions.push_back(action);
        run_single_greedy_action(s, action, Slimes, Dist, Nests);
      }
    }
    return ans;
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  Solver sol;
  Answer ans = sol.solve_single_greedy();
  ans.print();
}