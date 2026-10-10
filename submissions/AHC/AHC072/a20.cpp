#include <bits/stdc++.h>
using namespace std;

constexpr int MAX_ACTIONS = 100000;
constexpr int TIME_LIMIT_MS = 1900;

// fill8 / flushX 用の局所探索。
// X = 7, 6, 5, 4 をすべて試し、完全解のうち最小手数を採用する。
constexpr int LOCAL_ROUTE_HORIZON = 24;
constexpr int LOCAL_SEARCH_DEPTH = 5;
constexpr int LOCAL_BEAM_WIDTH = 64;
constexpr int LOCAL_INSERT_BEAM_WIDTH = 32;
constexpr int LOCAL_SETUP_CANDIDATE_LIMIT = 24;

// 旧: flush局所探索内の挿入近傍。コード互換のため定数は残すが無効化する。
constexpr int INSERT_MAX_STEPS = 3;
constexpr int INSERT_COLOR_CANDIDATE_LIMIT = 4;

// 新: color-fixed 本体の操作列に、別色の通常操作を数手だけ挿入する短いbeam。
// active色の処理ロジック自体は変えず、各反復で
//   activeを進める / helper色を1手だけ前倒しする
// のどちらを採るかをreceding-horizonで決める。
constexpr int INTERLEAVE_SEARCH_DEPTH = 6;
constexpr int INTERLEAVE_BEAM_WIDTH = 12;
constexpr int INTERLEAVE_HELPER_COLOR_LIMIT = 3;
constexpr int INTERLEAVE_MAX_HELPER_STREAK = 3;
// 完全finish rolloutはbeam中では行わず、cheap評価で残った候補だけに行う。
constexpr int INTERLEAVE_FINAL_ROLLOUT_CANDIDATES = 6;

// color-fixed の borrowed-helper 近傍。
// flush packet が他色の純粋towerへ着地したあと、helperを数手だけ同行させ、
// helperがtopに来たタイミングで自身のtreeへ切り離す。
// attach は既存の Flush move そのものなので追加コストなし。
constexpr int BORROW_MAX_SHARED_STEPS = 3;
constexpr int BORROW_DETACH_CANDIDATE_LIMIT = 8;
constexpr int BORROW_FUTURE_CREDIT_CAP = 3;

// 再利用可能なジャンプ台の将来価値。
// 現在のflushだけでなく、後続flushが同じ台を使える場合に
// 局所探索で少し余分なsetupコストを許容する。
constexpr int REUSE_CREDIT_PER_LEVEL = 1;

// color-mixed phase:
// 個数の少ない色だけを同時に扱い、最大4匹のtop segmentを
// 2色までのbundleとして輸送する。通常時は短いbeam searchで
// 合流準備から共通輸送までを見る。詰まった場合は一時的に
// 単色top blockを巣方向へ送るrecoveryへ切り替える。
constexpr int SMALL_COLOR_MAX_SLIMES = 7;
constexpr int MIXED_MAX_PACKET = 4;
constexpr int MIXED_MAX_COLORS = 2;
constexpr int MIXED_MAX_STEPS = 4000;

// small-color mixed transport 本体の短いbeam search。
// 1手貪欲では見えない
//   合流準備 -> bundle形成 -> 共通輸送
// を数手まとめて評価する。
constexpr int MIXED_SEARCH_DEPTH = 5;
constexpr int MIXED_SEARCH_BEAM_WIDTH = 64;

// mixed beam では、合流のために一時的に巣距離が1だけ悪化する手も許す。
constexpr int MIXED_MAX_BACKTRACK = 1;

// packet pair の meeting 候補は、両packetからこの距離以内のセルを調べる。
// beam は毎回張り直すので、遠いpairも近づけば後続探索で候補になる。
constexpr int MIXED_MEETING_RADIUS = 8;

// 1状態から全合法手を展開すると、+1 detour を許したことで枝数が急増する。
// meeting target への接近を局所scoreへ入れた上で上位だけをbeamへ渡す。
constexpr int MIXED_MOVE_CANDIDATE_LIMIT = 96;

constexpr int MIXED_RECOVERY_DEPTH = 5;
constexpr int MIXED_RECOVERY_BEAM_WIDTH = 32;

constexpr array<int, 4> DX = {-1, 1, 0, 0};
constexpr array<int, 4> DY = {0, 0, -1, 1};
constexpr array<char, 4> DIR_CHAR = {'U', 'D', 'L', 'R'};

struct Problem {
  int N, K;
  vector<string> C;

  Problem() {
    cin >> N >> K;
    C.resize(N);
    for (auto &row : C)
      cin >> row;
  }
};

struct Action {
  int x;
  int y;
  int k;
  int d;
  int l;

  friend ostream &operator<<(ostream &os, const Action &a) {
    return os << a.x << ' ' << a.y << ' ' << a.k << ' ' << DIR_CHAR[a.d] << ' ' << a.l;
  }
};

struct Tree {
  // adj[v] の bit d が立っていれば、d 方向に木の辺がある。
  vector<uint8_t> adj;

  // 巣を根とした情報。
  vector<int> parent;
  vector<int> depth;
  vector<int> child_count;
  vector<char> used;

  int edge_count = 0;
};

struct SimulationResult {
  bool success = false;
  vector<Action> actions;
  int inserted_actions = 0;
};

struct MeetingPlan {
  bool valid = false;
  int meeting = -1;
  int shared_length = 0;
  int extra_detour = 0;
  int split_cost = 0;
  int estimated_saving = INT_MIN;
  int approach_distance = INT_MAX;
  int readiness = INT_MIN;
};

// 盤面は最大20x20、各towerは最大8段なので固定長で保持する。
// vector<vector<int>> と違い、盤面コピー時にセルごとのheap allocationが発生しない。
constexpr int MAX_CELLS = 20 * 20;

struct Tower {
  array<uint8_t, 8> slime{};
  uint8_t height = 0;

  bool empty() const {
    return height == 0;
  }
  int size() const {
    return (int)height;
  }
  int back() const {
    return (int)slime[height - 1];
  }
  int operator[](int i) const {
    return (int)slime[i];
  }

  void push_back(int color) {
    assert(height < 8);
    slime[height++] = (uint8_t)color;
  }

  void pop_back() {
    assert(height > 0);
    --height;
  }
};

using Board = array<Tower, MAX_CELLS>;
using ColorCounts = array<int, 12>;
static_assert(is_trivially_copyable_v<Board>);

struct Solver {
  Problem P;
  int V;

  vector<int> nest;
  vector<int> nest_color;
  vector<vector<int>> terminals;
  vector<Tree> trees;

  // dist_to_nest[color][v]: v から color の巣までの最短距離。
  vector<vector<int>> dist_to_nest;

  // floor_dist[u][v]: 壁を避けた盤面上の全点対最短距離。
  // flush経路上判定とジャンプ台再利用価値の推定に使う。
  vector<vector<int>> floor_dist;

  // mixed_common_len[(a*K+b)][v]:
  // v から a,b の両方について最短路方向へ同時に進める最大連続長。
  // mixed packet の共有輸送区間の長さを近似する。
  vector<vector<int>> mixed_common_len;

  // meeting探索を軽くするため、各セルから一定半径内のセルだけ列挙しておく。
  vector<vector<int>> mixed_near_cells;

  // best meeting は盤面のtower高さではなく、色とpacket位置だけで評価する。
  // beam中に同じpairが何度も現れるのでキャッシュする。
  mutable unordered_map<uint64_t, MeetingPlan> mixed_meeting_plan_cache;

  // shortest_path_demand[color][v]:
  // 初期terminalのうち、v を通る最短路を持つものの個数。
  // flush size ごとの再利用価値は、この需要を packet size で割って評価する。
  vector<vector<int>> shortest_path_demand;

  // flushの最短路領域は盤面状態に依存しないため、
  // (active color, flush source) ごとに一度だけ構築して再利用する。
  vector<vector<vector<int>>> flush_route_cache;
  vector<vector<array<uint8_t, MAX_CELLS>>> flush_route_mask_cache;

  // 現在試している fill8 / flushX の X。
  // solve() から 7, 6, 5, 4 の順に切り替える。
  int strategy_flush_size = 4;

  // borrowed-helper / 旧flush内insertは停止。
  // 代わりに solve_one_color() の上位で短いinterleave beamを使う。
  bool borrow_neighbor_enabled = false;
  bool insert_neighbor_enabled = false;
  bool interleave_neighbor_enabled = true;
  mutable int current_inserted_actions = 0;

  struct MixedMove {
    int source = -1;
    int destination = -1;
    int moved_size = 0;
    long long score = LLONG_MIN;
    int distance_gain = 0;
    int homed = 0;
  };

  struct MixedPhaseResult {
    bool success = false;
    Board board{};
    vector<Action> actions;
    int normal_moves = 0;
    int recovery_moves = 0;
    int recovery_calls = 0;
  };

  struct MixedSearchState {
    Board board{};
    array<MixedMove, MIXED_SEARCH_DEPTH> moves{};
    array<uint64_t, MIXED_SEARCH_DEPTH + 1> path_hash{};
    uint8_t move_count = 0;
    int remaining = INT_MAX;
    int distance_sum = INT_MAX;
    // 以降を各色別々に運ぶと仮定した推定操作数と、
    // 既存mixed bundle / 将来meetingで節約できる推定操作数。
    int independent_ops = INT_MAX;
    int bundle_value = 0;
    int meeting_value = 0;
    int meeting_readiness = 0;
    int estimated_remaining_ops = INT_MAX;
    long long score = LLONG_MIN;
    uint64_t hash = 0;
  };

  struct RecoveryState {
    Board board{};
    array<MixedMove, MIXED_RECOVERY_DEPTH> moves{};
    uint8_t move_count = 0;
    int remaining = INT_MAX;
    int distance_sum = INT_MAX;
    long long score = LLONG_MIN;
    uint64_t hash = 0;
  };

  struct FinishCostCacheEntry {
    Board board{};
    int color = -1;
    int flush_size = -1;
    int value = INT_MAX;
  };

  // 同一盤面・同一色・同一flush sizeのfinish rolloutをtrial間でも再利用する。
  // hash衝突時はBoard本体を比較するため、評価情報は失わない。
  mutable unordered_map<uint64_t, vector<FinishCostCacheEntry>> finish_cost_cache;
  mutable size_t finish_cost_cache_entries = 0;

  // mixed prefix後はsmall colorが全て消えているため、mixed側は
  // 「残った色の相対順 + flush size」だけで決定する。
  mutable unordered_map<uint64_t, SimulationResult> mixed_suffix_cache;

  mt19937 mt{0};
  chrono::steady_clock::time_point start_time;

  Solver() : V(P.N * P.N), start_time(chrono::steady_clock::now()) {
    nest.assign(P.K, -1);
    nest_color.assign(V, -1);
    terminals.resize(P.K);

    initialize_problem();
    build_distances();
    build_mixed_pair_cache();
    build_flush_route_cache();
    build_all_trees();
  }

  // =========================================================
  // Grid
  // =========================================================

  int cell_id(int x, int y) const {
    return x * P.N + y;
  }

  pair<int, int> cell_pos(int v) const {
    return {v / P.N, v % P.N};
  }

  bool inside(int x, int y) const {
    return 0 <= x && x < P.N && 0 <= y && y < P.N;
  }

  bool passable(int x, int y) const {
    return inside(x, y) && P.C[x][y] != '#';
  }

  int next_cell(int v, int d) const {
    auto [x, y] = cell_pos(v);
    int nx = x + DX[d];
    int ny = y + DY[d];
    if (!passable(nx, ny))
      return -1;
    return cell_id(nx, ny);
  }

  int direction_between(int u, int v) const {
    auto [ux, uy] = cell_pos(u);
    auto [vx, vy] = cell_pos(v);

    for (int d = 0; d < 4; ++d) {
      if (ux + DX[d] == vx && uy + DY[d] == vy)
        return d;
    }
    return -1;
  }

  bool time_over() const {
    auto now = chrono::steady_clock::now();
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - start_time).count();
    return elapsed >= TIME_LIMIT_MS;
  }

  // =========================================================
  // Input / initial information
  // =========================================================

  void initialize_problem() {
    for (int x = 0; x < P.N; ++x) {
      for (int y = 0; y < P.N; ++y) {
        int v = cell_id(x, y);
        char c = P.C[x][y];

        if ('A' <= c && c <= 'L') {
          int color = c - 'A';
          nest[color] = v;
          nest_color[v] = color;
        } else if ('a' <= c && c <= 'l') {
          int color = c - 'a';
          terminals[color].push_back(v);
        }
      }
    }
  }

  // =========================================================
  // Shortest path distances
  // =========================================================

  void build_distances() {
    dist_to_nest.assign(P.K, vector<int>(V, INT_MAX));

    for (int color = 0; color < P.K; ++color) {
      queue<int> que;
      int root = nest[color];
      dist_to_nest[color][root] = 0;
      que.push(root);

      while (!que.empty()) {
        int v = que.front();
        que.pop();

        for (int d = 0; d < 4; ++d) {
          int nv = next_cell(v, d);
          if (nv == -1)
            continue;
          if (dist_to_nest[color][nv] != INT_MAX)
            continue;

          dist_to_nest[color][nv] = dist_to_nest[color][v] + 1;
          que.push(nv);
        }
      }
    }

    floor_dist.assign(V, vector<int>(V, INT_MAX));

    for (int source = 0; source < V; ++source) {
      auto [sx, sy] = cell_pos(source);
      if (!passable(sx, sy))
        continue;

      queue<int> que;
      floor_dist[source][source] = 0;
      que.push(source);

      while (!que.empty()) {
        int v = que.front();
        que.pop();

        for (int d = 0; d < 4; ++d) {
          int nv = next_cell(v, d);
          if (nv == -1)
            continue;
          if (floor_dist[source][nv] != INT_MAX)
            continue;

          floor_dist[source][nv] = floor_dist[source][v] + 1;
          que.push(nv);
        }
      }
    }

    // 各セルが、その色の何匹の初期terminalから巣への最短路に
    // 含まれ得るかを数える。
    // flush size は戦略ごとに変わるので、ここでは需要そのものだけ保存する。
    shortest_path_demand.assign(P.K, vector<int>(V, 0));

    for (int color = 0; color < P.K; ++color) {
      for (int v = 0; v < V; ++v) {
        if (dist_to_nest[color][v] == INT_MAX)
          continue;

        int demand = 0;
        for (int u : terminals[color]) {
          if (floor_dist[u][v] == INT_MAX)
            continue;
          if (floor_dist[u][v] + dist_to_nest[color][v] == dist_to_nest[color][u]) {
            ++demand;
          }
        }

        shortest_path_demand[color][v] = demand;
      }
    }
  }

  void build_mixed_pair_cache() {
    mixed_common_len.assign(P.K * P.K, vector<int>(V, 0));

    // 共通最短路は、1歩進むたび両色の巣距離が1ずつ減るDAGになる。
    // 距離和の小さいセルからDPすれば、各セルからの最長共有区間が求まる。
    for (int a = 0; a < P.K; ++a) {
      for (int b = a + 1; b < P.K; ++b) {
        vector<int> order;
        order.reserve(V);
        for (int v = 0; v < V; ++v) {
          auto [x, y] = cell_pos(v);
          if (!passable(x, y))
            continue;
          if (dist_to_nest[a][v] == INT_MAX || dist_to_nest[b][v] == INT_MAX) {
            continue;
          }
          order.push_back(v);
        }

        sort(order.begin(), order.end(), [&](int lhs, int rhs) {
          int dl = dist_to_nest[a][lhs] + dist_to_nest[b][lhs];
          int dr = dist_to_nest[a][rhs] + dist_to_nest[b][rhs];
          if (dl != dr)
            return dl < dr;
          return lhs < rhs;
        });

        vector<int> dp(V, 0);
        for (int v : order) {
          int best = 0;
          for (int d = 0; d < 4; ++d) {
            int nv = next_cell(v, d);
            if (nv == -1)
              continue;
            if (!is_shortest_step(a, v, nv))
              continue;
            if (!is_shortest_step(b, v, nv))
              continue;
            best = max(best, 1 + dp[nv]);
          }
          dp[v] = best;
        }

        mixed_common_len[a * P.K + b] = dp;
        mixed_common_len[b * P.K + a] = dp;
      }
    }

    mixed_near_cells.assign(V, {});
    for (int u = 0; u < V; ++u) {
      auto [x, y] = cell_pos(u);
      if (!passable(x, y))
        continue;
      for (int v = 0; v < V; ++v) {
        if (floor_dist[u][v] <= MIXED_MEETING_RADIUS) {
          mixed_near_cells[u].push_back(v);
        }
      }
    }

    mixed_meeting_plan_cache.reserve(1 << 16);
  }

  bool is_shortest_step(int color, int from, int to) const {
    if (from < 0 || to < 0)
      return false;
    if (dist_to_nest[color][from] == INT_MAX)
      return false;
    return dist_to_nest[color][to] + 1 == dist_to_nest[color][from];
  }

  /**
   * source -> v -> nest が最短路になるか。
   */
  bool on_some_shortest_path(int color, int source, int v) const {
    if (source < 0 || v < 0)
      return false;
    if (floor_dist[source][v] == INT_MAX)
      return false;
    if (dist_to_nest[color][source] == INT_MAX || dist_to_nest[color][v] == INT_MAX) {
      return false;
    }

    return floor_dist[source][v] + dist_to_nest[color][v] == dist_to_nest[color][source];
  }

  // =========================================================
  // Greedy Steiner tree
  // =========================================================

  void add_tree_edge(Tree &tree, int u, int v) const {
    int d = direction_between(u, v);
    assert(d != -1);

    uint8_t bu = uint8_t(1u << d);
    uint8_t bv = uint8_t(1u << (d ^ 1));

    if (!(tree.adj[u] & bu))
      ++tree.edge_count;

    tree.adj[u] |= bu;
    tree.adj[v] |= bv;
  }

  bool has_tree_edge(const Tree &tree, int v, int d) const {
    return (tree.adj[v] & uint8_t(1u << d)) != 0;
  }

  /**
   * 巣だけから始め、現在の木全体を multi-source BFS の始点として、
   * 木に最も近い未接続スライムを1体ずつ接続する。
   *
   * これを各色について固定木として一度だけ構築する。
   */
  Tree build_greedy_tree(int color) const {
    Tree tree;
    tree.adj.assign(V, 0);

    vector<char> in_tree(V, false);
    vector<char> is_terminal(V, false);

    for (int v : terminals[color])
      is_terminal[v] = true;

    const int root = nest[color];
    in_tree[root] = true;

    while (true) {
      bool finished = true;
      for (int v : terminals[color]) {
        if (!in_tree[v]) {
          finished = false;
          break;
        }
      }
      if (finished)
        break;

      queue<int> que;
      vector<int> prev(V, -2);

      // 現在の木全体を始点にする。
      for (int v = 0; v < V; ++v) {
        if (!in_tree[v])
          continue;
        prev[v] = -1;
        que.push(v);
      }

      int target = -1;

      while (!que.empty()) {
        int v = que.front();
        que.pop();

        if (is_terminal[v] && !in_tree[v]) {
          target = v;
          break;
        }

        for (int d = 0; d < 4; ++d) {
          int nv = next_cell(v, d);
          if (nv == -1)
            continue;
          if (prev[nv] != -2)
            continue;

          prev[nv] = v;
          que.push(nv);
        }
      }

      assert(target != -1);

      int cur = target;
      while (prev[cur] != -1) {
        int p = prev[cur];
        add_tree_edge(tree, cur, p);
        in_tree[cur] = true;
        in_tree[p] = true;
        cur = p;
      }
    }

    return tree;
  }

  void build_root_info(int color, Tree &tree) const {
    tree.parent.assign(V, -2);
    tree.depth.assign(V, -1);
    tree.child_count.assign(V, 0);
    tree.used.assign(V, false);

    int root = nest[color];

    queue<int> que;
    tree.parent[root] = -1;
    tree.depth[root] = 0;
    tree.used[root] = true;
    que.push(root);

    while (!que.empty()) {
      int v = que.front();
      que.pop();

      for (int d = 0; d < 4; ++d) {
        if (!has_tree_edge(tree, v, d))
          continue;

        int nv = next_cell(v, d);
        assert(nv != -1);

        if (tree.parent[nv] != -2)
          continue;

        tree.parent[nv] = v;
        tree.depth[nv] = tree.depth[v] + 1;
        tree.used[nv] = true;
        ++tree.child_count[v];
        que.push(nv);
      }
    }
  }

  void build_all_trees() {
    trees.resize(P.K);

    for (int color = 0; color < P.K; ++color) {
      trees[color] = build_greedy_tree(color);
      build_root_info(color, trees[color]);
    }
  }

  // =========================================================
  // Simulation state helpers
  // =========================================================

  Board initial_board() const {
    Board board{};

    for (int color = 0; color < P.K; ++color) {
      for (int v : terminals[color]) {
        board[v].push_back(color);
      }
    }

    return board;
  }

  int top_color(const Board &board, int v) const {
    if (board[v].empty())
      return -1;
    return board[v].back();
  }

  int top_block_size(const Board &board, int v) const {
    if (board[v].empty())
      return 0;

    int color = board[v].back();
    int cnt = 0;

    for (int i = (int)board[v].size() - 1; i >= 0 && board[v][i] == color; --i) {
      ++cnt;
    }

    return cnt;
  }

  ColorCounts count_colors(const Board &board) const {
    ColorCounts counts{};
    for (int v = 0; v < V; ++v) {
      const auto &tower = board[v];
      for (int i = 0; i < tower.size(); ++i) {
        ++counts[tower[i]];
      }
    }
    return counts;
  }

  ColorCounts initial_color_counts() const {
    ColorCounts counts{};
    for (int color = 0; color < P.K; ++color) {
      counts[color] = (int)terminals[color].size();
    }
    return counts;
  }

  int count_color(const Board &board, int color) const {
    int result = 0;
    for (int v = 0; v < V; ++v) {
      const auto &tower = board[v];
      for (int i = 0; i < tower.size(); ++i) {
        result += (tower[i] == color);
      }
    }
    return result;
  }

  int count_all(const Board &board) const {
    int result = 0;
    for (int v = 0; v < V; ++v)
      result += board[v].size();
    return result;
  }

  int count_all(const ColorCounts &counts) const {
    int result = 0;
    for (int color = 0; color < P.K; ++color)
      result += counts[color];
    return result;
  }

  // 巣でtopから帰巣した匹数を返す。countsを渡した場合は差分更新する。
  int home_cell(Board &board, int v, ColorCounts *counts = nullptr) const {
    int color = nest_color[v];
    if (color == -1)
      return 0;

    int homed = 0;
    while (!board[v].empty() && board[v].back() == color) {
      board[v].pop_back();
      ++homed;
    }
    if (counts != nullptr) {
      (*counts)[color] -= homed;
      assert((*counts)[color] >= 0);
    }
    return homed;
  }

  // =========================================================
  // Actions
  // =========================================================

  bool execute_action(Board &board, vector<Action> &actions, int source, int moved_size, int destination,
                      ColorCounts *counts = nullptr, uint64_t *board_hash = nullptr) const {
    if ((int)actions.size() >= MAX_ACTIONS)
      return false;
    if (board[source].empty())
      return false;
    if (!(1 <= moved_size && moved_size <= (int)board[source].size()))
      return false;
    if ((int)board[destination].size() + moved_size > 8)
      return false;

    auto [sx, sy] = cell_pos(source);
    auto [tx, ty] = cell_pos(destination);

    int d = -1;
    int l = -1;

    if (sx == tx) {
      if (sy < ty) {
        d = 3;
        l = ty - sy;
      } else if (ty < sy) {
        d = 2;
        l = sy - ty;
      }
    } else if (sy == ty) {
      if (sx < tx) {
        d = 1;
        l = tx - sx;
      } else if (tx < sx) {
        d = 0;
        l = sx - tx;
      }
    }

    if (d == -1 || l <= 0)
      return false;

    int h = board[source].size();
    int k = h - moved_size;

    if (!(0 <= k && k < h))
      return false;
    if (l > k + 1)
      return false;

    int v = source;
    for (int step = 0; step < l; ++step) {
      v = next_cell(v, d);
      if (v == -1)
        return false;
    }
    if (v != destination)
      return false;

    Tower old_source, old_destination;
    if (board_hash != nullptr) {
      old_source = board[source];
      old_destination = board[destination];
    }

    actions.push_back({sx, sy, k, d, l});

    for (int i = 0; i < moved_size; ++i) {
      int color = board[source].back();
      board[source].pop_back();
      board[destination].push_back(color);
    }

    home_cell(board, source, counts);
    home_cell(board, destination, counts);

    if (board_hash != nullptr) {
      *board_hash = update_board_hash(*board_hash, source, old_source, board[source], destination, old_destination,
                                      board[destination]);
    }

    return true;
  }

  // =========================================================
  // Small-color mixed transport
  // =========================================================

  bool is_small_color(int color) const {
    return 0 <= color && color < P.K && !terminals[color].empty() &&
           (int)terminals[color].size() <= SMALL_COLOR_MAX_SLIMES;
  }

  int count_small_slimes(const Board &board) const {
    int result = 0;
    for (int v = 0; v < V; ++v) {
      for (int i = 0; i < board[v].size(); ++i) {
        result += is_small_color(board[v][i]);
      }
    }
    return result;
  }

  int small_distance_sum(const Board &board) const {
    int result = 0;
    for (int v = 0; v < V; ++v) {
      for (int i = 0; i < board[v].size(); ++i) {
        int color = board[v][i];
        if (!is_small_color(color))
          continue;
        if (dist_to_nest[color][v] == INT_MAX)
          return INT_MAX / 4;
        result += dist_to_nest[color][v];
      }
    }
    return result;
  }

  int tower_small_count(const Tower &tower) const {
    int result = 0;
    for (int i = 0; i < tower.size(); ++i) {
      result += is_small_color(tower[i]);
    }
    return result;
  }

  int tower_small_distance(const Tower &tower, int v) const {
    int result = 0;
    for (int i = 0; i < tower.size(); ++i) {
      int color = tower[i];
      if (!is_small_color(color))
        continue;
      result += dist_to_nest[color][v];
    }
    return result;
  }

  int top_small_suffix_size(const Tower &tower) const {
    int result = 0;
    for (int i = tower.size() - 1; i >= 0; --i) {
      if (!is_small_color(tower[i]))
        break;
      ++result;
    }
    return result;
  }

  uint16_t top_small_color_mask(const Tower &tower) const {
    uint16_t mask = 0;
    for (int i = tower.size() - 1; i >= 0; --i) {
      int color = tower[i];
      if (!is_small_color(color))
        break;
      mask |= uint16_t(1u << color);
    }
    return mask;
  }

  int top_small_transitions(const Tower &tower) const {
    int suffix = top_small_suffix_size(tower);
    if (suffix <= 1)
      return 0;

    int first = tower.size() - suffix;
    int result = 0;
    for (int i = first + 1; i < tower.size(); ++i) {
      result += tower[i] != tower[i - 1];
    }
    return result;
  }

  void home_tower(Tower &tower, int v) const {
    int color = nest_color[v];
    if (color == -1)
      return;
    while (!tower.empty() && tower.back() == color)
      tower.pop_back();
  }

  static uint64_t splitmix64(uint64_t x) {
    x += 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    return x ^ (x >> 31);
  }

  uint64_t tower_code(const Tower &tower) const {
    uint64_t code = tower.size();
    for (int i = 0; i < tower.size(); ++i) {
      code |= (uint64_t)(tower[i] + 1) << (4 + 4 * i);
    }
    return code;
  }

  uint64_t cell_hash(int v, const Tower &tower) const {
    if (tower.empty())
      return 0;
    uint64_t x = ((uint64_t)(v + 1) << 40) ^ tower_code(tower);
    return splitmix64(x);
  }

  uint64_t update_board_hash(uint64_t hash, int source, const Tower &old_source, const Tower &new_source,
                             int destination, const Tower &old_destination, const Tower &new_destination) const {
    hash ^= cell_hash(source, old_source);
    hash ^= cell_hash(destination, old_destination);
    hash ^= cell_hash(source, new_source);
    hash ^= cell_hash(destination, new_destination);
    return hash;
  }

  bool board_equal(const Board &a, const Board &b) const {
    return memcmp(a.data(), b.data(), sizeof(Tower) * V) == 0;
  }

  void apply_move_unchecked(Board &board, int source, int moved_size, int destination, ColorCounts *counts = nullptr,
                            uint64_t *board_hash = nullptr) const {
    Tower old_source, old_destination;
    if (board_hash != nullptr) {
      old_source = board[source];
      old_destination = board[destination];
    }

    for (int i = 0; i < moved_size; ++i) {
      int color = board[source].back();
      board[source].pop_back();
      board[destination].push_back(color);
    }
    home_cell(board, source, counts);
    home_cell(board, destination, counts);

    if (board_hash != nullptr) {
      *board_hash = update_board_hash(*board_hash, source, old_source, board[source], destination, old_destination,
                                      board[destination]);
    }
  }

  // XOR型のセル独立hash。1操作で変化するsource/destinationだけ差分更新できる。
  uint64_t hash_board(const Board &board) const {
    uint64_t h = 0;
    for (int v = 0; v < V; ++v) {
      h ^= cell_hash(v, board[v]);
    }
    return h;
  }

  int common_shortest_direction_count(uint16_t color_mask, int v) const {
    if (color_mask == 0)
      return 0;
    int result = 0;

    for (int d = 0; d < 4; ++d) {
      int nv = next_cell(v, d);
      if (nv == -1)
        continue;

      bool ok = true;
      for (int color = 0; color < P.K; ++color) {
        if (!(color_mask & uint16_t(1u << color)))
          continue;
        if (!is_shortest_step(color, v, nv)) {
          ok = false;
          break;
        }
      }
      result += ok;
    }

    return result;
  }

  int popcount_colors(uint16_t mask) const {
    return __builtin_popcount((unsigned)mask);
  }

  uint16_t top_packet_color_mask(const Tower &tower, int packet_size) const {
    uint16_t mask = 0;
    packet_size = min(packet_size, tower.size());
    for (int i = 0; i < packet_size; ++i) {
      int color = tower[tower.size() - 1 - i];
      if (!is_small_color(color))
        return 0;
      mask |= uint16_t(1u << color);
    }
    return mask;
  }

  int top_packet_transitions(const Tower &tower, int packet_size) const {
    packet_size = min(packet_size, tower.size());
    if (packet_size <= 1)
      return 0;
    int first = tower.size() - packet_size;
    int result = 0;
    for (int i = first + 1; i < tower.size(); ++i) {
      result += tower[i] != tower[i - 1];
    }
    return result;
  }

  int common_shortest_length(int a, int b, int v) const {
    if (a == b || a < 0 || b < 0 || v < 0)
      return 0;
    return mixed_common_len[a * P.K + b][v];
  }

  uint64_t meeting_plan_key(int color1, int pos1, int color2, int pos2) const {
    if (color1 > color2) {
      swap(color1, color2);
      swap(pos1, pos2);
    }
    // color < 16, position < 512 なので固定bitで十分。
    return (uint64_t)color1 | ((uint64_t)color2 << 4) | ((uint64_t)pos1 << 8) | ((uint64_t)pos2 << 17);
  }

  /**
   * pure packet 2個について最良meetingセルを求める。
   *
   * 別々に運ぶ推定コスト:
   *   dist(c1,p1) + dist(c2,p2)
   * meeting m で合流し、L手だけ共通最短路を運ぶ推定コスト:
   *   d(p1,m) + d(p2,m) + L
   *   + (dist(c1,m)-L) + (dist(c2,m)-L) + split_cost
   *
   * 差を整理すると
   *   saving = L - extra_detour1 - extra_detour2 - split_cost
   * となる。これを「予想削減操作数」として直接使う。
   */
  MeetingPlan best_meeting_plan(int color1, int pos1, int color2, int pos2) const {
    if (color1 == color2)
      return {};

    uint64_t key = meeting_plan_key(color1, pos1, color2, pos2);
    if (auto it = mixed_meeting_plan_cache.find(key); it != mixed_meeting_plan_cache.end()) {
      return it->second;
    }

    int a = color1, b = color2;
    int u = pos1, v = pos2;
    if (a > b) {
      swap(a, b);
      swap(u, v);
    }

    MeetingPlan best;

    if (floor_dist[u][v] == INT_MAX || floor_dist[u][v] > 2 * MIXED_MEETING_RADIUS) {
      mixed_meeting_plan_cache.emplace(key, best);
      return best;
    }

    for (int m : mixed_near_cells[u]) {
      int d1 = floor_dist[u][m];
      int d2 = floor_dist[v][m];
      if (d2 == INT_MAX || d2 > MIXED_MEETING_RADIUS)
        continue;
      if (dist_to_nest[a][m] == INT_MAX || dist_to_nest[b][m] == INT_MAX) {
        continue;
      }

      int shared = common_shortest_length(a, b, m);
      if (shared <= 0)
        continue;

      int extra1 = d1 + dist_to_nest[a][m] - dist_to_nest[a][u];
      int extra2 = d2 + dist_to_nest[b][m] - dist_to_nest[b][v];
      if (extra1 < 0 || extra2 < 0)
        continue;

      // 共有区間が近い方の巣まで到達できれば、巣で自然分離できる可能性が高い。
      int split_cost = (shared == min(dist_to_nest[a][m], dist_to_nest[b][m])) ? 0 : 1;

      int saving = shared - extra1 - extra2 - split_cost;
      int approach = d1 + d2;
      int readiness = 12 * shared - 2 * approach - 6 * (extra1 + extra2) - 4 * split_cost;

      bool better =
          !best.valid || saving > best.estimated_saving ||
          (saving == best.estimated_saving && readiness > best.readiness) ||
          (saving == best.estimated_saving && readiness == best.readiness && approach < best.approach_distance) ||
          (saving == best.estimated_saving && readiness == best.readiness && approach == best.approach_distance &&
           m < best.meeting);
      if (!better)
        continue;

      best.valid = true;
      best.meeting = m;
      best.shared_length = shared;
      best.extra_detour = extra1 + extra2;
      best.split_cost = split_cost;
      best.estimated_saving = saving;
      best.approach_distance = approach;
      best.readiness = readiness;
    }

    mixed_meeting_plan_cache.emplace(key, best);
    return best;
  }

  struct PurePacket {
    int v = -1;
    int color = -1;
    int size = 0;
  };

  vector<PurePacket> visible_pure_packets(const Board &board) const {
    vector<PurePacket> packets;
    packets.reserve(64);

    for (int v = 0; v < V; ++v) {
      if (board[v].empty())
        continue;
      int color = top_color(board, v);
      if (!is_small_color(color))
        continue;

      // すでに2色bundleになっているtowerは meeting 候補ではなく
      // existing bundle として別評価する。
      if (popcount_colors(top_small_color_mask(board[v])) != 1)
        continue;

      int block = top_block_size(board, v);
      int packet = min(MIXED_MAX_PACKET, block);
      if (packet <= 0)
        continue;
      packets.push_back({v, color, packet});
    }

    return packets;
  }

  struct MeetingEstimate {
    int saving = 0;
    int readiness = 0;
  };

  MeetingEstimate estimate_future_meeting(const Board &board, array<int, MAX_CELLS> *target_meeting = nullptr) const {
    if (target_meeting)
      target_meeting->fill(-1);

    auto packets = visible_pure_packets(board);

    struct Candidate {
      int i;
      int j;
      MeetingPlan plan;
    };
    vector<Candidate> candidates;
    candidates.reserve(packets.size() * 2);

    // sourceごとのtargetは、最もsavingが大きく、同値なら近いmeetingを使う。
    array<int, MAX_CELLS> target_quality{};
    target_quality.fill(INT_MIN);

    for (int i = 0; i < (int)packets.size(); ++i) {
      for (int j = i + 1; j < (int)packets.size(); ++j) {
        const auto &a = packets[i];
        const auto &b = packets[j];
        if (a.color == b.color)
          continue;
        if (a.size + b.size > MIXED_MAX_PACKET)
          continue;
        if (floor_dist[a.v][b.v] == INT_MAX || floor_dist[a.v][b.v] > 2 * MIXED_MEETING_RADIUS) {
          continue;
        }

        MeetingPlan plan = best_meeting_plan(a.color, a.v, b.color, b.v);
        if (!plan.valid || plan.estimated_saving <= 0)
          continue;

        candidates.push_back({i, j, plan});

        if (target_meeting) {
          int qa = 100 * plan.estimated_saving + plan.readiness - floor_dist[a.v][plan.meeting];
          int qb = 100 * plan.estimated_saving + plan.readiness - floor_dist[b.v][plan.meeting];
          if (qa > target_quality[a.v]) {
            target_quality[a.v] = qa;
            (*target_meeting)[a.v] = plan.meeting;
          }
          if (qb > target_quality[b.v]) {
            target_quality[b.v] = qb;
            (*target_meeting)[b.v] = plan.meeting;
          }
        }
      }
    }

    sort(candidates.begin(), candidates.end(), [](const Candidate &lhs, const Candidate &rhs) {
      if (lhs.plan.estimated_saving != rhs.plan.estimated_saving) {
        return lhs.plan.estimated_saving > rhs.plan.estimated_saving;
      }
      if (lhs.plan.readiness != rhs.plan.readiness) {
        return lhs.plan.readiness > rhs.plan.readiness;
      }
      return lhs.plan.approach_distance < rhs.plan.approach_distance;
    });

    // 同じpacketを複数の将来bundleへ二重計上しないよう、
    // 高価値pairからgreedy matchingする。
    vector<char> used(packets.size(), false);
    MeetingEstimate result;
    for (const auto &candidate : candidates) {
      if (used[candidate.i] || used[candidate.j])
        continue;
      used[candidate.i] = used[candidate.j] = true;
      result.saving += candidate.plan.estimated_saving;
      result.readiness += candidate.plan.readiness;
    }
    return result;
  }

  /**
   * small color を今後すべて「色blockごとに別々に」運ぶと仮定した推定操作数。
   * 同色の連続blockは最大4匹まで1packetとして同時に運べるため、
   * slime数ではなくpacket数 × 巣距離で数える。
   */
  int independent_small_ops_cell(const Tower &tower, int v) const {
    int result = 0;
    int i = 0;
    while (i < tower.size()) {
      int color = tower[i];
      if (!is_small_color(color)) {
        ++i;
        continue;
      }
      int j = i + 1;
      while (j < tower.size() && tower[j] == color)
        ++j;
      int block_size = j - i;
      int packet_count = (block_size + MIXED_MAX_PACKET - 1) / MIXED_MAX_PACKET;
      if (dist_to_nest[color][v] != INT_MAX) {
        result += packet_count * dist_to_nest[color][v];
      }
      i = j;
    }
    return result;
  }

  int estimate_independent_small_ops(const Board &board) const {
    int result = 0;
    for (int v = 0; v < V; ++v) {
      result += independent_small_ops_cell(board[v], v);
    }
    return result;
  }

  int existing_bundle_saving_cell(const Tower &tower, int v) const {
    int suffix = top_small_suffix_size(tower);
    if (suffix <= 1)
      return 0;

    int packet = min(MIXED_MAX_PACKET, suffix);
    uint16_t mask = top_packet_color_mask(tower, packet);
    if (popcount_colors(mask) != 2)
      return 0;

    int colors[2] = {-1, -1};
    int ptr = 0;
    for (int color = 0; color < P.K; ++color) {
      if (mask & uint16_t(1u << color))
        colors[ptr++] = color;
    }

    int shared = common_shortest_length(colors[0], colors[1], v);
    if (shared <= 0)
      return 0;

    int split_cost = (shared == min(dist_to_nest[colors[0]][v], dist_to_nest[colors[1]][v])) ? 0 : 1;
    int transitions = top_packet_transitions(tower, packet);
    int local = shared - split_cost - max(0, transitions - 1);
    return max(0, local);
  }

  int estimate_existing_bundle_saving(const Board &board) const {
    int saving = 0;
    for (int v = 0; v < V; ++v) {
      saving += existing_bundle_saving_cell(board[v], v);
    }
    return saving;
  }

  void update_mixed_search_features(MixedSearchState &state) const {
    state.independent_ops = estimate_independent_small_ops(state.board);
    state.bundle_value = estimate_existing_bundle_saving(state.board);
    auto meeting = estimate_future_meeting(state.board);
    state.meeting_value = meeting.saving;
    state.meeting_readiness = meeting.readiness;

    state.estimated_remaining_ops = max(0, state.independent_ops - state.bundle_value - state.meeting_value);
  }

  void update_mixed_search_features_after_move(MixedSearchState &child, const MixedSearchState &parent, int source,
                                               const Tower &old_source, int destination,
                                               const Tower &old_destination) const {
    child.independent_ops = parent.independent_ops - independent_small_ops_cell(old_source, source) -
                            independent_small_ops_cell(old_destination, destination) +
                            independent_small_ops_cell(child.board[source], source) +
                            independent_small_ops_cell(child.board[destination], destination);

    child.bundle_value = parent.bundle_value - existing_bundle_saving_cell(old_source, source) -
                         existing_bundle_saving_cell(old_destination, destination) +
                         existing_bundle_saving_cell(child.board[source], source) +
                         existing_bundle_saving_cell(child.board[destination], destination);

    auto meeting = estimate_future_meeting(child.board);
    child.meeting_value = meeting.saving;
    child.meeting_readiness = meeting.readiness;
    child.estimated_remaining_ops = max(0, child.independent_ops - child.bundle_value - child.meeting_value);
  }

  /**
   * mixed move の局所評価。
   *
   * 通常beamでは各色の巣距離を1だけ悪化させる一時detourを許す。
   * これにより「一度横へ寄ってmeeting -> その後bundleで共有輸送」が探索可能になる。
   */
  optional<MixedMove> evaluate_mixed_move(const Board &board, int source, int moved_size, int destination,
                                          int length) const {
    if (source < 0 || destination < 0 || source == destination)
      return nullopt;
    if (board[source].empty())
      return nullopt;
    if (!(1 <= moved_size && moved_size <= board[source].size()))
      return nullopt;
    if (board[destination].size() + moved_size > 8)
      return nullopt;

    uint16_t moved_mask = 0;
    int backtrack = 0;
    for (int i = 0; i < moved_size; ++i) {
      int color = board[source][board[source].size() - 1 - i];
      if (!is_small_color(color))
        return nullopt;
      moved_mask |= uint16_t(1u << color);

      int delta = dist_to_nest[color][destination] - dist_to_nest[color][source];
      if (delta > MIXED_MAX_BACKTRACK)
        return nullopt;
      backtrack += max(0, delta);
    }
    if (popcount_colors(moved_mask) > MIXED_MAX_COLORS)
      return nullopt;

    int before_count = tower_small_count(board[source]) + tower_small_count(board[destination]);
    int before_dist =
        tower_small_distance(board[source], source) + tower_small_distance(board[destination], destination);
    int destination_small_before = top_small_suffix_size(board[destination]);

    Tower source_after = board[source];
    Tower destination_after = board[destination];

    for (int i = 0; i < moved_size; ++i) {
      int color = source_after.back();
      source_after.pop_back();
      destination_after.push_back(color);
    }
    home_tower(source_after, source);
    home_tower(destination_after, destination);

    int after_count = tower_small_count(source_after) + tower_small_count(destination_after);
    int after_dist = tower_small_distance(source_after, source) + tower_small_distance(destination_after, destination);

    int homed = before_count - after_count;
    int distance_gain = before_dist - after_dist;

    int top_packet = min(MIXED_MAX_PACKET, top_small_suffix_size(destination_after));
    uint16_t result_mask = top_packet_color_mask(destination_after, top_packet);
    int result_colors = popcount_colors(result_mask);
    int transitions = top_packet_transitions(destination_after, top_packet);

    if (result_colors > MIXED_MAX_COLORS)
      return nullopt;
    if (transitions > 2)
      return nullopt;

    int common_dirs = common_shortest_direction_count(result_mask, destination);
    int jump_power = 0;
    if (top_packet > 0) {
      jump_power = destination_after.size() - top_packet + 1;
    }

    int merge_bonus = 0;
    if (destination_small_before > 0 && result_colors >= 1) {
      merge_bonus = min(destination_small_before, moved_size);
    }

    int split_bonus = 0;
    if (homed > 0 && top_small_suffix_size(destination_after) > 0) {
      split_bonus = 1;
    }

    int clean_bundle_bonus = 0;
    if (result_colors == 2) {
      clean_bundle_bonus = (transitions == 1 ? 2 : 0);
    }

    // これは候補の事前順位付けだけに使う。
    // beam本体の評価は「予想残り操作数」で行うため、距離gainを以前より弱くする。
    long long score = 0;
    score += 4000LL * homed;
    score += 60LL * distance_gain;
    score += 20LL * moved_size;
    score += 45LL * common_dirs * max(1, top_packet);
    score += 20LL * jump_power;
    score += 90LL * merge_bonus;
    score += 70LL * split_bonus;
    score += 70LL * clean_bundle_bonus;
    score -= 90LL * backtrack;

    if (result_colors == 2 && common_dirs == 0)
      score -= 80;
    score += 2LL * length;

    return MixedMove{
        source, destination, moved_size, score, distance_gain, homed,
    };
  }

  vector<MixedMove> generate_mixed_moves(const Board &board,
                                         const array<int, MAX_CELLS> *target_meeting = nullptr) const {
    vector<MixedMove> result;

    for (int source = 0; source < V; ++source) {
      int suffix = top_small_suffix_size(board[source]);
      if (suffix <= 0)
        continue;

      int max_packet = min(MIXED_MAX_PACKET, suffix);
      for (int moved_size = 1; moved_size <= max_packet; ++moved_size) {
        uint16_t moved_mask = 0;
        bool valid_packet = true;
        for (int i = 0; i < moved_size; ++i) {
          int color = board[source][board[source].size() - 1 - i];
          if (!is_small_color(color)) {
            valid_packet = false;
            break;
          }
          moved_mask |= uint16_t(1u << color);
        }
        if (!valid_packet || popcount_colors(moved_mask) > MIXED_MAX_COLORS) {
          continue;
        }

        int k = board[source].size() - moved_size;
        int max_jump = k + 1;

        for (int d = 0; d < 4; ++d) {
          int v = source;
          for (int l = 1; l <= max_jump; ++l) {
            int nv = next_cell(v, d);
            if (nv == -1)
              break;
            v = nv;

            if (board[v].size() + moved_size > 8)
              continue;

            auto candidate = evaluate_mixed_move(board, source, moved_size, v, l);
            if (!candidate.has_value())
              continue;

            // pairごとのbest meetingへ近づく手は、たとえ巣距離が+1でも
            // candidate prefilterで落ちにくくする。
            if (target_meeting && (*target_meeting)[source] != -1) {
              int target = (*target_meeting)[source];
              int before = floor_dist[source][target];
              int after = floor_dist[v][target];
              if (before != INT_MAX && after != INT_MAX) {
                candidate->score += 180LL * (before - after);
              }
            }

            result.push_back(*candidate);
          }
        }
      }
    }

    sort(result.begin(), result.end(), [](const MixedMove &a, const MixedMove &b) {
      if (a.score != b.score)
        return a.score > b.score;
      if (a.homed != b.homed)
        return a.homed > b.homed;
      if (a.distance_gain != b.distance_gain) {
        return a.distance_gain > b.distance_gain;
      }
      if (a.moved_size != b.moved_size)
        return a.moved_size > b.moved_size;
      if (a.source != b.source)
        return a.source < b.source;
      return a.destination < b.destination;
    });

    if ((int)result.size() > MIXED_MOVE_CANDIDATE_LIMIT) {
      result.resize(MIXED_MOVE_CANDIDATE_LIMIT);
    }
    return result;
  }

  long long mixed_search_score(const MixedSearchState &state) const {
    // 最優先は「ここまで使った手数 + 予想残り手数」。
    // 1操作の差を、下のtie-break heuristicより十分大きくする。
    long long predicted_total = (long long)state.move_count + state.estimated_remaining_ops;
    long long score = -10000LL * predicted_total;

    // 同じ予想操作数なら、meetingが実現に近い状態を残す。
    score += 20LL * state.meeting_readiness;
    score += 40LL * state.bundle_value;

    // さらに同程度なら、実際に帰巣が進み、総距離も短い状態を優先。
    score -= 8LL * state.remaining;
    score -= state.distance_sum;
    return score;
  }

  /**
   * small-color mixed transport のbeam search。
   *
   * 1. 巣距離+1までの一時detourを許す。
   * 2. pure packet pairごとにbest meeting cellを求め、そのmeetingへの接近を誘導する。
   * 3. 状態評価は距離総和ではなく、
   *      実手数 + (別々に運ぶ推定手数 - bundle/meetingの予想節約手数)
   *    を主指標にする。
   */
  vector<MixedMove> search_mixed_beam(const Board &start_board, const unordered_set<uint64_t> &global_visited) const {
    MixedSearchState initial;
    initial.board = start_board;
    initial.remaining = count_small_slimes(start_board);
    initial.distance_sum = small_distance_sum(start_board);
    initial.hash = hash_board(start_board);
    initial.path_hash[0] = initial.hash;
    update_mixed_search_features(initial);
    initial.score = mixed_search_score(initial);

    vector<MixedSearchState> beam{initial};
    optional<MixedSearchState> best;

    for (int depth = 0; depth < MIXED_SEARCH_DEPTH; ++depth) {
      vector<MixedSearchState> next;
      unordered_set<uint64_t> seen;
      seen.reserve(MIXED_SEARCH_BEAM_WIDTH * MIXED_MOVE_CANDIDATE_LIMIT);

      for (const auto &state : beam) {
        array<int, MAX_CELLS> meeting_target;
        estimate_future_meeting(state.board, &meeting_target);
        auto moves = generate_mixed_moves(state.board, &meeting_target);

        // 親盤面は1回だけcopyし、各候補ではsource/destinationだけapply/undoする。
        Board scratch = state.board;

        for (const auto &move : moves) {
          Tower old_source = scratch[move.source];
          Tower old_destination = scratch[move.destination];
          uint64_t child_hash = state.hash;
          apply_move_unchecked(scratch, move.source, move.moved_size, move.destination, nullptr, &child_hash);

          bool reject = global_visited.contains(child_hash);
          if (!reject) {
            for (int i = 0; i <= state.move_count; ++i) {
              if (state.path_hash[i] == child_hash) {
                reject = true;
                break;
              }
            }
          }
          if (!reject && !seen.insert(child_hash).second)
            reject = true;

          if (!reject) {
            int independent_ops = state.independent_ops - independent_small_ops_cell(old_source, move.source) -
                                  independent_small_ops_cell(old_destination, move.destination) +
                                  independent_small_ops_cell(scratch[move.source], move.source) +
                                  independent_small_ops_cell(scratch[move.destination], move.destination);

            int bundle_value = state.bundle_value - existing_bundle_saving_cell(old_source, move.source) -
                               existing_bundle_saving_cell(old_destination, move.destination) +
                               existing_bundle_saving_cell(scratch[move.source], move.source) +
                               existing_bundle_saving_cell(scratch[move.destination], move.destination);

            auto meeting = estimate_future_meeting(scratch);

            MixedSearchState child;
            // ここで初めてBoardを保存する。重複候補にはBoard copyしない。
            child.board = scratch;
            child.moves = state.moves;
            child.path_hash = state.path_hash;
            child.move_count = state.move_count;
            child.moves[child.move_count] = move;
            ++child.move_count;
            child.remaining = state.remaining - move.homed;
            child.distance_sum = state.distance_sum - move.distance_gain;
            child.independent_ops = independent_ops;
            child.bundle_value = bundle_value;
            child.meeting_value = meeting.saving;
            child.meeting_readiness = meeting.readiness;
            child.estimated_remaining_ops = max(0, child.independent_ops - child.bundle_value - child.meeting_value);
            child.hash = child_hash;
            child.path_hash[child.move_count] = child_hash;
            child.score = mixed_search_score(child);

            if (!best.has_value() || child.score > best->score ||
                (child.score == best->score && child.estimated_remaining_ops < best->estimated_remaining_ops) ||
                (child.score == best->score && child.estimated_remaining_ops == best->estimated_remaining_ops &&
                 child.remaining < best->remaining) ||
                (child.score == best->score && child.estimated_remaining_ops == best->estimated_remaining_ops &&
                 child.remaining == best->remaining && child.distance_sum < best->distance_sum)) {
              best = child;
            }

            if (child.remaining == 0) {
              vector<MixedMove> result;
              result.reserve(child.move_count);
              for (int i = 0; i < child.move_count; ++i) {
                result.push_back(child.moves[i]);
              }
              return result;
            }

            next.push_back(std::move(child));
          }

          scratch[move.source] = old_source;
          scratch[move.destination] = old_destination;
        }
      }

      if (next.empty())
        break;

      sort(next.begin(), next.end(), [](const MixedSearchState &a, const MixedSearchState &b) {
        if (a.score != b.score)
          return a.score > b.score;
        if (a.estimated_remaining_ops != b.estimated_remaining_ops) {
          return a.estimated_remaining_ops < b.estimated_remaining_ops;
        }
        if (a.meeting_value != b.meeting_value) {
          return a.meeting_value > b.meeting_value;
        }
        if (a.meeting_readiness != b.meeting_readiness) {
          return a.meeting_readiness > b.meeting_readiness;
        }
        if (a.remaining != b.remaining)
          return a.remaining < b.remaining;
        if (a.distance_sum != b.distance_sum) {
          return a.distance_sum < b.distance_sum;
        }
        return a.hash < b.hash;
      });

      if ((int)next.size() > MIXED_SEARCH_BEAM_WIDTH) {
        next.resize(MIXED_SEARCH_BEAM_WIDTH);
      }
      beam = std::move(next);
    }

    if (!best.has_value())
      return {};

    vector<MixedMove> result;
    result.reserve(best->move_count);
    for (int i = 0; i < best->move_count; ++i) {
      result.push_back(best->moves[i]);
    }
    return result;
  }

  vector<MixedMove> generate_recovery_moves(const Board &board) const {
    vector<MixedMove> result;

    for (int source = 0; source < V; ++source) {
      if (board[source].empty())
        continue;
      int color = top_color(board, source);
      if (!is_small_color(color))
        continue;

      int block_size = top_block_size(board, source);
      int max_packet = min(MIXED_MAX_PACKET, block_size);

      for (int moved_size = 1; moved_size <= max_packet; ++moved_size) {
        int k = board[source].size() - moved_size;
        int max_jump = k + 1;

        for (int d = 0; d < 4; ++d) {
          int v = source;
          for (int l = 1; l <= max_jump; ++l) {
            int nv = next_cell(v, d);
            if (nv == -1)
              break;
            if (!is_shortest_step(color, v, nv))
              break;
            v = nv;

            if (board[v].size() + moved_size > 8)
              continue;

            Tower source_after = board[source];
            Tower destination_after = board[v];
            int before_count = tower_small_count(source_after) + tower_small_count(destination_after);
            int before_dist = tower_small_distance(source_after, source) + tower_small_distance(destination_after, v);

            for (int i = 0; i < moved_size; ++i) {
              int c = source_after.back();
              source_after.pop_back();
              destination_after.push_back(c);
            }
            home_tower(source_after, source);
            home_tower(destination_after, v);

            // recoveryでも3色以上の複雑なsmall towerは作らない。
            if (popcount_colors(top_small_color_mask(destination_after)) > MIXED_MAX_COLORS) {
              continue;
            }
            if (top_small_transitions(destination_after) > 2)
              continue;

            int after_count = tower_small_count(source_after) + tower_small_count(destination_after);
            int after_dist = tower_small_distance(source_after, source) + tower_small_distance(destination_after, v);
            int homed = before_count - after_count;
            int gain = before_dist - after_dist;

            long long score = 15000LL * homed + 300LL * gain + 3LL * l;
            result.push_back({source, v, moved_size, score, gain, homed});
          }
        }
      }
    }

    sort(result.begin(), result.end(), [](const MixedMove &a, const MixedMove &b) {
      if (a.score != b.score)
        return a.score > b.score;
      if (a.homed != b.homed)
        return a.homed > b.homed;
      if (a.distance_gain != b.distance_gain) {
        return a.distance_gain > b.distance_gain;
      }
      if (a.moved_size != b.moved_size)
        return a.moved_size > b.moved_size;
      if (a.source != b.source)
        return a.source < b.source;
      return a.destination < b.destination;
    });

    return result;
  }

  vector<MixedMove> search_mixed_recovery(const Board &start_board,
                                          const unordered_set<uint64_t> &global_visited) const {
    const int initial_remaining = count_small_slimes(start_board);
    const int initial_distance = small_distance_sum(start_board);

    RecoveryState initial;
    initial.board = start_board;
    initial.remaining = initial_remaining;
    initial.distance_sum = initial_distance;
    initial.score = 0;
    initial.hash = hash_board(start_board);

    vector<RecoveryState> beam{initial};
    optional<RecoveryState> best_progress;

    for (int depth = 0; depth < MIXED_RECOVERY_DEPTH; ++depth) {
      vector<RecoveryState> next;
      unordered_set<uint64_t> seen;
      seen.reserve(MIXED_RECOVERY_BEAM_WIDTH * 32);

      for (const auto &state : beam) {
        auto moves = generate_recovery_moves(state.board);
        Board scratch = state.board;

        for (const auto &move : moves) {
          Tower old_source = scratch[move.source];
          Tower old_destination = scratch[move.destination];
          int before_count = tower_small_count(old_source) + tower_small_count(old_destination);
          int before_dist =
              tower_small_distance(old_source, move.source) + tower_small_distance(old_destination, move.destination);

          uint64_t child_hash = state.hash;
          apply_move_unchecked(scratch, move.source, move.moved_size, move.destination, nullptr, &child_hash);

          bool reject = !seen.insert(child_hash).second || global_visited.contains(child_hash);

          if (!reject) {
            int after_count = tower_small_count(scratch[move.source]) + tower_small_count(scratch[move.destination]);
            int after_dist = tower_small_distance(scratch[move.source], move.source) +
                             tower_small_distance(scratch[move.destination], move.destination);

            RecoveryState child;
            child.board = scratch;
            child.moves = state.moves;
            child.move_count = state.move_count;
            child.moves[child.move_count++] = move;
            child.remaining = state.remaining - (before_count - after_count);
            child.distance_sum = state.distance_sum - (before_dist - after_dist);
            child.hash = child_hash;
            child.score = 100000LL * (initial_remaining - child.remaining) +
                          1000LL * (initial_distance - child.distance_sum) - 10LL * child.move_count;

            bool recovered = child.remaining < initial_remaining;
            if (!recovered) {
              auto normal = generate_mixed_moves(child.board);
              recovered = !normal.empty();
            }

            if (recovered) {
              vector<MixedMove> result;
              result.reserve(child.move_count);
              for (int i = 0; i < child.move_count; ++i) {
                result.push_back(child.moves[i]);
              }
              return result;
            }

            if (child.distance_sum < initial_distance &&
                (!best_progress.has_value() || child.score > best_progress->score)) {
              best_progress = child;
            }

            next.push_back(std::move(child));
          }

          scratch[move.source] = old_source;
          scratch[move.destination] = old_destination;
        }
      }

      if (next.empty())
        break;

      sort(next.begin(), next.end(), [](const RecoveryState &a, const RecoveryState &b) {
        if (a.score != b.score)
          return a.score > b.score;
        if (a.remaining != b.remaining)
          return a.remaining < b.remaining;
        if (a.distance_sum != b.distance_sum) {
          return a.distance_sum < b.distance_sum;
        }
        return a.hash < b.hash;
      });
      if ((int)next.size() > MIXED_RECOVERY_BEAM_WIDTH) {
        next.resize(MIXED_RECOVERY_BEAM_WIDTH);
      }
      beam = std::move(next);
    }

    if (best_progress.has_value()) {
      vector<MixedMove> result;
      result.reserve(best_progress->move_count);
      for (int i = 0; i < best_progress->move_count; ++i) {
        result.push_back(best_progress->moves[i]);
      }
      return result;
    }

    return {};
  }

  MixedPhaseResult build_small_mixed_prefix() const {
    MixedPhaseResult result;
    result.board = initial_board();
    result.actions.reserve(512);

    int small_color_count = 0;
    for (int color = 0; color < P.K; ++color) {
      small_color_count += is_small_color(color);
    }
    if (small_color_count == 0) {
      result.success = true;
      return result;
    }

    int remaining_small = count_small_slimes(result.board);
    uint64_t current_hash = hash_board(result.board);

    unordered_set<uint64_t> visited;
    visited.reserve(8192);
    visited.insert(current_hash);

    for (int step = 0; step < MIXED_MAX_STEPS; ++step) {
      if (remaining_small == 0) {
        result.success = true;
        return result;
      }

      auto plan = search_mixed_beam(result.board, visited);
      if (!plan.empty()) {
        for (const auto &move : plan) {
          if (!execute_action(result.board, result.actions, move.source, move.moved_size, move.destination, nullptr,
                              &current_hash)) {
            return result;
          }
          ++result.normal_moves;
          remaining_small -= move.homed;
          visited.insert(current_hash);

          if (remaining_small == 0) {
            result.success = true;
            return result;
          }
        }
        continue;
      }

      ++result.recovery_calls;
      auto recovery = search_mixed_recovery(result.board, visited);
      if (recovery.empty())
        return result;

      for (const auto &recovery_move : recovery) {
        if (!execute_action(result.board, result.actions, recovery_move.source, recovery_move.moved_size,
                            recovery_move.destination, nullptr, &current_hash)) {
          return result;
        }
        ++result.recovery_moves;
        remaining_small -= recovery_move.homed;
        visited.insert(current_hash);
      }
    }

    return result;
  }

  /**
   * source から根方向の「次の集約点」を返す。
   *
   * ・分岐点
   * ・巣
   *
   * のうち最初に現れるもの。
   */
  int next_merge_point(int color, int source) const {
    const auto &tree = trees[color];
    int v = tree.parent[source];

    while (v != -1) {
      if (v == nest[color])
        return v;
      if (tree.child_count[v] >= 2)
        return v;
      v = tree.parent[v];
    }

    return -1;
  }

  /**
   * full blockを動かす場合に合法な範囲で、
   * 固定木上を根方向へできるだけ長くジャンプする。
   *
   * ただし、
   * ・木が曲がる
   * ・次の分岐点/巣に着く
   * ・同色towerを見つける
   * のいずれかで止める。
   *
   * 容量不足なら -1 を返し、1辺移動側の処理に任せる。
   */
  int find_jump_destination(const Board &board, int color, int source, int moved_size) const {
    const auto &tree = trees[color];

    int first_parent = tree.parent[source];
    if (first_parent < 0)
      return -1;

    int direction = direction_between(source, first_parent);
    if (direction == -1)
      return -1;

    int h = board[source].size();
    int k = h - moved_size;
    int max_jump = k + 1;

    int merge_point = next_merge_point(color, source);

    vector<int> candidates;
    int v = source;

    for (int l = 1; l <= max_jump; ++l) {
      int p = tree.parent[v];
      if (p < 0)
        break;

      if (direction_between(v, p) != direction)
        break;

      candidates.push_back(p);
      v = p;

      if (!board[v].empty() && top_color(board, v) == color)
        break;
      if (v == merge_point)
        break;
    }

    for (int i = (int)candidates.size() - 1; i >= 0; --i) {
      int destination = candidates[i];
      if ((int)board[destination].size() + moved_size <= 8) {
        return destination;
      }
    }

    return -1;
  }

  /**
   * flush packet 専用のジャンプ先探索。
   *
   * 通常輸送はGreedy Steiner treeに従うが、flush中だけは木を外れ、
   * 巣への最短距離が1マスごとに必ず減少する直線だけを使う。
   * したがって選ばれたジャンプ全体も必ず最短路上にある。
   *
   * packet_sizeは論理的に固定し、同色towerへ着地しても増やさない。
   */
  int find_flush_jump_destination(const Board &board, int color, int source, int packet_size) const {
    if (dist_to_nest[color][source] == INT_MAX)
      return -1;

    int h = board[source].size();
    int k = h - packet_size;
    int max_jump = k + 1;

    int best = -1;
    int best_length = -1;
    int best_base_height = -1;

    for (int d = 0; d < 4; ++d) {
      int v = source;

      for (int l = 1; l <= max_jump; ++l) {
        int nv = next_cell(v, d);
        if (nv == -1)
          break;

        // 途中の各1マスが最短路方向でなければ、この方向は終了。
        if (!is_shortest_step(color, v, nv))
          break;

        v = nv;

        if ((int)board[v].size() + packet_size > 8)
          continue;

        int base_height = board[v].size();

        // まず長いジャンプを優先。
        // 同じ長さなら、次のジャンプ力を得やすい高い台を優先。
        if (l > best_length || (l == best_length && base_height > best_base_height) ||
            (l == best_length && base_height == best_base_height && v < best)) {
          best = v;
          best_length = l;
          best_base_height = base_height;
        }
      }
    }

    return best;
  }

  vector<int> shortest_next_cells(const Board &board, int color, int source, int packet_size) const {
    vector<int> result;

    for (int d = 0; d < 4; ++d) {
      int v = next_cell(source, d);
      if (v == -1)
        continue;
      if (!is_shortest_step(color, source, v))
        continue;
      result.push_back(v);
    }

    sort(result.begin(), result.end(), [&](int a, int b) {
      bool fit_a = (int)board[a].size() + packet_size <= 8;
      bool fit_b = (int)board[b].size() + packet_size <= 8;
      if (fit_a != fit_b)
        return fit_a > fit_b;

      int ha = board[a].size();
      int hb = board[b].size();
      if (ha != hb)
        return ha > hb;
      return a < b;
    });

    return result;
  }

  /**
   * packet_size 匹を論理的にマージせず、そのまま巣まで送る。
   *
   * flush の本体では packet_size=4 を使う。
   * 同色towerへ着地しても、次の操作で動かすのは上の packet_size 匹だけ。
   *
   * 前方の同色towerが容量不足なら、そのtowerの上4匹を先に flush して
   * 空きを作ってから元のpacketを再開する。
   */
  bool flush_packet_to_nest_core(Board &board, vector<Action> &actions, int color, int source, int packet_size,
                                 ColorCounts *counts = nullptr) const {
    if (packet_size <= 0)
      return true;
    if ((int)actions.size() >= MAX_ACTIONS)
      return false;

    if (source == nest[color]) {
      home_cell(board, source, counts);
      return true;
    }

    if (board[source].empty())
      return false;
    if (top_color(board, source) != color)
      return false;
    if (top_block_size(board, source) < packet_size)
      return false;
    if (dist_to_nest[color][source] == INT_MAX)
      return false;

    int destination = find_flush_jump_destination(board, color, source, packet_size);

    if (destination != -1) {
      if (!execute_action(board, actions, source, packet_size, destination, counts)) {
        return false;
      }

      if (destination == nest[color])
        return true;

      return flush_packet_to_nest_core(board, actions, color, destination, packet_size, counts);
    }

    // 直線ジャンプができない場合も、Steiner treeには戻らず、
    // 最短距離が1減る隣接マスのどれかへ進む。
    vector<int> nexts = shortest_next_cells(board, color, source, packet_size);

    // そのままpacket全体を受けられる最短路隣接点を優先。
    for (int destination2 : nexts) {
      int capacity = 8 - (int)board[destination2].size();
      if (capacity < packet_size)
        continue;

      if (!execute_action(board, actions, source, packet_size, destination2, counts)) {
        continue;
      }

      if (destination2 == nest[color])
        return true;

      return flush_packet_to_nest_core(board, actions, color, destination2, packet_size, counts);
    }

    // すべて容量不足なら、最短路方向にある同色towerを先にflushして
    // 空きを作れる経路を試す。
    for (int destination2 : nexts) {
      if (board[destination2].empty())
        continue;
      if (top_color(board, destination2) != color)
        continue;

      int front_block = top_block_size(board, destination2);
      if (front_block <= 0)
        continue;

      int front_flush = min(strategy_flush_size, front_block);
      Board board_backup = board;
      size_t action_size_backup = actions.size();
      optional<ColorCounts> counts_backup;
      if (counts != nullptr)
        counts_backup = *counts;

      if (!flush_packet_to_nest_core(board, actions, color, destination2, front_flush, counts)) {
        board = std::move(board_backup);
        actions.resize(action_size_backup);
        if (counts_backup.has_value())
          *counts = *counts_backup;
        continue;
      }

      return flush_packet_to_nest_core(board, actions, color, source, packet_size, counts);
    }

    return false;
  }

  /**
   * evaluate_flush_cost() 専用の action 適用。
   *
   * 実際の Action 列は不要なので vector<Action> を生成せず、
   * 実行手数だけを action_count に加算する。
   * 盤面更新・合法性判定・帰巣処理は execute_action() と同じ。
   */
  bool execute_action_count_only(Board &board, int &action_count, int source, int moved_size, int destination,
                                 ColorCounts *counts = nullptr, uint64_t *board_hash = nullptr) const {
    if (action_count >= MAX_ACTIONS)
      return false;
    if (board[source].empty())
      return false;
    if (!(1 <= moved_size && moved_size <= (int)board[source].size()))
      return false;
    if ((int)board[destination].size() + moved_size > 8)
      return false;

    auto [sx, sy] = cell_pos(source);
    auto [tx, ty] = cell_pos(destination);

    int d = -1;
    int l = -1;

    if (sx == tx) {
      if (sy < ty) {
        d = 3;
        l = ty - sy;
      } else if (ty < sy) {
        d = 2;
        l = sy - ty;
      }
    } else if (sy == ty) {
      if (sx < tx) {
        d = 1;
        l = tx - sx;
      } else if (tx < sx) {
        d = 0;
        l = sx - tx;
      }
    }

    if (d == -1 || l <= 0)
      return false;

    int h = board[source].size();
    int k = h - moved_size;

    if (!(0 <= k && k < h))
      return false;
    if (l > k + 1)
      return false;

    int v = source;
    for (int step = 0; step < l; ++step) {
      v = next_cell(v, d);
      if (v == -1)
        return false;
    }
    if (v != destination)
      return false;

    Tower old_source, old_destination;
    if (board_hash != nullptr) {
      old_source = board[source];
      old_destination = board[destination];
    }

    ++action_count;

    for (int i = 0; i < moved_size; ++i) {
      int color = board[source].back();
      board[source].pop_back();
      board[destination].push_back(color);
    }

    home_cell(board, source, counts);
    home_cell(board, destination, counts);

    if (board_hash != nullptr) {
      *board_hash = update_board_hash(*board_hash, source, old_source, board[source], destination, old_destination,
                                      board[destination]);
    }

    return true;
  }

  bool flush_packet_to_nest_count_only(Board &board, int &action_count, int color, int source, int packet_size,
                                       ColorCounts *counts = nullptr, uint64_t *board_hash = nullptr) const {
    if (packet_size <= 0)
      return true;
    if (action_count >= MAX_ACTIONS)
      return false;

    if (source == nest[color]) {
      Tower old_source;
      if (board_hash != nullptr)
        old_source = board[source];
      home_cell(board, source, counts);
      if (board_hash != nullptr) {
        *board_hash ^= cell_hash(source, old_source);
        *board_hash ^= cell_hash(source, board[source]);
      }
      return true;
    }

    if (board[source].empty())
      return false;
    if (top_color(board, source) != color)
      return false;
    if (top_block_size(board, source) < packet_size)
      return false;
    if (dist_to_nest[color][source] == INT_MAX)
      return false;

    int destination = find_flush_jump_destination(board, color, source, packet_size);

    if (destination != -1) {
      if (!execute_action_count_only(board, action_count, source, packet_size, destination, counts, board_hash)) {
        return false;
      }

      if (destination == nest[color])
        return true;

      return flush_packet_to_nest_count_only(board, action_count, color, destination, packet_size, counts, board_hash);
    }

    vector<int> nexts = shortest_next_cells(board, color, source, packet_size);

    for (int destination2 : nexts) {
      int capacity = 8 - (int)board[destination2].size();
      if (capacity < packet_size)
        continue;

      if (!execute_action_count_only(board, action_count, source, packet_size, destination2, counts, board_hash)) {
        continue;
      }

      if (destination2 == nest[color])
        return true;

      return flush_packet_to_nest_count_only(board, action_count, color, destination2, packet_size, counts, board_hash);
    }

    for (int destination2 : nexts) {
      if (board[destination2].empty())
        continue;
      if (top_color(board, destination2) != color)
        continue;

      int front_block = top_block_size(board, destination2);
      if (front_block <= 0)
        continue;

      int front_flush = min(strategy_flush_size, front_block);
      Board board_backup = board;
      int action_count_backup = action_count;
      optional<ColorCounts> counts_backup;
      if (counts != nullptr)
        counts_backup = *counts;
      uint64_t hash_backup = board_hash != nullptr ? *board_hash : 0;

      if (!flush_packet_to_nest_count_only(board, action_count, color, destination2, front_flush, counts, board_hash)) {
        board = board_backup;
        action_count = action_count_backup;
        if (counts_backup.has_value())
          *counts = *counts_backup;
        if (board_hash != nullptr)
          *board_hash = hash_backup;
        continue;
      }

      return flush_packet_to_nest_count_only(board, action_count, color, source, packet_size, counts, board_hash);
    }

    return false;
  }

  enum class LocalMoveType : uint8_t {
    Flush,
    Setup,
    Insert,
    BorrowCarry,
    BorrowDetach,
  };

  struct LocalMove {
    LocalMoveType type = LocalMoveType::Setup;
    int source = -1;
    int destination = -1;
    int moved_size = 0;
    int color = -1;
  };

  struct LocalState {
    Board board{};

    // 局所探索の深さは固定なので、操作列も固定長で持つ。
    // vector の heap allocation / copy を避ける。
    array<LocalMove, LOCAL_SEARCH_DEPTH> moves{};
    uint8_t move_count = 0;

    // board hashはsource/destination差分だけで更新する。
    uint64_t board_hash = 0;
    // board + 局所メタ情報の状態hash。
    uint64_t hash = 0;

    // 論理的なflushの現在位置。
    int flush_pos = -1;
    bool flush_finished = false;

    // ここから最短路ベースの決定的flushを行った場合の残り手数。
    int finish_cost = INT_MAX;

    // move_count + finish_cost。
    int total_cost = INT_MAX;

    // 現在盤面に残っているactive colorの匹数。
    // reuse_creditのfuture_use_capを差分更新するために保持する。
    int active_remaining = 0;

    // 後続flushで再利用できるplatformの将来価値。
    int reuse_credit = 0;

    // total_cost - (初期盤面から増えたreuse_credit)。
    // 局所探索ではこの値も比較に使う。
    int effective_cost = INT_MAX;

    // beamの多様性維持用。大きいほど「将来platformになりそう」。
    int potential = 0;

    // 他色挿入近傍の状態。
    // 局所探索の先頭でのみ開始でき、開始後は同じ色を最大3手進める。
    // Flush/Setupへ戻った時点で挿入phaseを閉じる。
    bool insert_phase_open = true;
    int insert_color = -1;
    int insert_steps = 0;
    int insert_credit = 0;

    // borrowed-helper 近傍の途中状態。
    // helper_origin は attach 直後（最初のcarry前）の位置。
    // shared_steps が奇数なら helper がtop、偶数なら active がtop。
    int borrow_helper_color = -1;
    int borrow_helper_size = 0;
    int borrow_origin = -1;
    int borrow_shared_steps = 0;

    // helperを切り離したことで将来のその色の輸送が何手程度短くなるかのcredit。
    int borrow_credit = 0;
  };

  /**
   * flush が今後通り得る「最短路領域」を列挙する。
   *
   * Steiner tree上の1本の経路ではなく、sourceから巣までの
   * いずれかの最短路に含まれ、かつsourceからHORIZON以内のセルを全て含む。
   */
  void build_flush_route_cache() {
    flush_route_cache.assign(P.K, vector<vector<int>>(V));
    flush_route_mask_cache.assign(P.K, vector<array<uint8_t, MAX_CELLS>>(V));

    for (int color = 0; color < P.K; ++color) {
      for (int source = 0; source < V; ++source) {
        auto &route = flush_route_cache[color][source];
        auto &mask = flush_route_mask_cache[color][source];
        mask.fill(0);

        if (dist_to_nest[color][source] == INT_MAX)
          continue;

        for (int v = 0; v < V; ++v) {
          if (v == source)
            continue;
          if (floor_dist[source][v] == INT_MAX)
            continue;
          if (floor_dist[source][v] > LOCAL_ROUTE_HORIZON)
            continue;
          if (!on_some_shortest_path(color, source, v))
            continue;

          route.push_back(v);
          mask[v] = 1;
        }

        sort(route.begin(), route.end(), [&](int a, int b) {
          int da = floor_dist[source][a];
          int db = floor_dist[source][b];
          if (da != db)
            return da < db;
          if (dist_to_nest[color][a] != dist_to_nest[color][b]) {
            return dist_to_nest[color][a] > dist_to_nest[color][b];
          }
          return a < b;
        });
      }
    }
  }

  const vector<int> &flush_route_cells(int color, int source) const {
    return flush_route_cache[color][source];
  }

  const array<uint8_t, MAX_CELLS> &flush_route_mask(int color, int source) const {
    return flush_route_mask_cache[color][source];
  }

  /**
   * setup対象色のGreedy Steiner treeで v からparent方向へ進んだとき、
   * flushの最短路領域へ初めて到達するまでの辺数。
   */
  int steps_to_route(int color, int v, const array<uint8_t, MAX_CELLS> &on_route, int max_steps) const {
    if (!trees[color].used[v])
      return -1;

    int cur = v;
    for (int step = 0; step <= max_steps; ++step) {
      if (on_route[cur])
        return step;

      int p = trees[color].parent[cur];
      if (p < 0)
        break;
      cur = p;
    }

    return -1;
  }

  /**
   * 現在の盤面で、platformが後続flushにも使われる見込みを概算する。
   *
   * platformが、残っているactive colorスライムの位置から巣への
   * いずれかの最短路上にあるなら、そのスライム数を需要として数える。
   * 4匹につきflushを1回とみなし、現在のflush1回分を除いた回数を
   * 「再利用回数」とする。
   */
  int estimated_extra_platform_uses(const Board &board, int active_color, int platform) const {
    (void)board;
    if (platform < 0 || platform >= V)
      return 0;

    int demand = shortest_path_demand[active_color][platform];
    return max(0, demand / strategy_flush_size - 1);
  }

  int count_color_in_tower(const Tower &tower, int color) const {
    int result = 0;
    for (int i = 0; i < tower.size(); ++i) {
      result += (tower[i] == color);
    }
    return result;
  }

  int future_platform_use_cap(int active_remaining) const {
    return max(0, active_remaining / strategy_flush_size - 1);
  }

  /**
   * 1セルが reusable_platform_credit に寄与する値。
   *
   * future_use_cap が変わらない遷移では source / destination の2セルだけ
   * 再計算すれば、盤面全走査なしでcreditを厳密に差分更新できる。
   */
  int reusable_platform_cell_credit(const Board &board, int active_color, int v, int future_use_cap) const {
    if (future_use_cap <= 0)
      return 0;
    if (v < 0 || v >= V)
      return 0;
    if (board[v].empty())
      return 0;

    int color = top_color(board, v);
    if (color < 0 || color == active_color)
      return 0;
    if (top_block_size(board, v) != (int)board[v].size())
      return 0;

    int h = board[v].size();
    int platform_max_height = 8 - strategy_flush_size;
    if (!(1 <= h && h <= platform_max_height))
      return 0;

    int extra_uses = min(estimated_extra_platform_uses(board, active_color, v), future_use_cap);
    return extra_uses * h * REUSE_CREDIT_PER_LEVEL;
  }

  /**
   * 盤面上に残る再利用可能platformの将来価値。
   *
   * active_remaining は呼び出し側で保持し、count_color() の盤面全走査を
   * 毎回行わない。future_use_cap が変わった場合の再構築時だけ全セルを見る。
   */
  int reusable_platform_credit(const Board &board, int active_color, int active_remaining) const {
    int future_use_cap = future_platform_use_cap(active_remaining);
    if (future_use_cap == 0)
      return 0;

    int credit = 0;
    for (int v = 0; v < V; ++v) {
      credit += reusable_platform_cell_credit(board, active_color, v, future_use_cap);
    }
    return credit;
  }

  /**
   * flush自身が次に行える操作を全て列挙する。
   * packet_size=4は論理的に固定し、同色towerへ着地しても
   * packetを大きくしない。
   */
  vector<LocalMove> generate_flush_moves(const Board &board, int color, int flush_pos) const {
    vector<LocalMove> result;

    if (flush_pos == nest[color])
      return result;
    if (board[flush_pos].empty())
      return result;
    if (top_color(board, flush_pos) != color)
      return result;
    if (top_block_size(board, flush_pos) < strategy_flush_size)
      return result;
    if (dist_to_nest[color][flush_pos] == INT_MAX)
      return result;

    int h = board[flush_pos].size();
    int k = h - strategy_flush_size;
    int max_jump = k + 1;

    struct Candidate {
      LocalMove move;
      int length = 0;
      int base_height = 0;
      int reuse = 0;
    };
    vector<Candidate> candidates;

    for (int d = 0; d < 4; ++d) {
      int v = flush_pos;

      for (int l = 1; l <= max_jump; ++l) {
        int nv = next_cell(v, d);
        if (nv == -1)
          break;
        if (!is_shortest_step(color, v, nv))
          break;

        v = nv;

        if ((int)board[v].size() + strategy_flush_size > 8)
          continue;

        int reuse = 0;
        if (!board[v].empty() && top_color(board, v) != color && top_block_size(board, v) == (int)board[v].size() &&
            (int)board[v].size() <= 8 - strategy_flush_size) {
          reuse = estimated_extra_platform_uses(board, color, v);
        }

        candidates.push_back({
            {LocalMoveType::Flush, flush_pos, v, strategy_flush_size},
            l,
            (int)board[v].size(),
            reuse,
        });
      }
    }

    // 直接の進行量を最優先し、同程度なら再利用価値のあるplatform着地を優先。
    sort(candidates.begin(), candidates.end(), [](const Candidate &a, const Candidate &b) {
      if (a.length != b.length)
        return a.length > b.length;
      if (a.reuse != b.reuse)
        return a.reuse > b.reuse;
      if (a.base_height != b.base_height)
        return a.base_height > b.base_height;
      return a.move.destination < b.move.destination;
    });

    result.reserve(candidates.size());
    for (const auto &candidate : candidates) {
      result.push_back(candidate.move);
    }

    return result;
  }

  /**
   * 他色をplatform候補へ運ぶ1手を列挙する。
   *
   * 以前は「flush最短路領域に隣接したスライムを1手で直接載せる」だけだった。
   * 今回は最大5手程度先読みするため、他色自身のGreedy Steiner treeを
   * parent方向へ1辺ずつ進める中間操作も探索対象にする。
   *
   * 条件:
   * ・active colorは動かさない
   * ・移動元は純粋な同色tower
   * ・移動先も空、または同じ色の純粋tower
   * ・setup中のtower高さは4以下
   * ・その色のparent列をたどると、残り探索手数以内にflush最短路領域へ到達する
   *
   * これにより、例えば経路から4マス離れた他色スライムも
   * 4手使ってplatform位置まで運べる。
   */
  vector<LocalMove> generate_setup_moves(const Board &board, int active_color, int flush_pos,
                                         int remaining_depth) const {
    vector<LocalMove> result;
    if (remaining_depth <= 0)
      return result;

    const auto &on_route = flush_route_mask(active_color, flush_pos);

    struct CandidateMove {
      LocalMove move;
      int route_dist = INT_MAX;
      int reaches_route = 0;
      int moved_size = 0;
    };
    vector<CandidateMove> candidates;

    for (int source = 0; source < V; ++source) {
      if (source == flush_pos)
        continue;
      if (board[source].empty())
        continue;

      int color = top_color(board, source);
      if (color < 0 || color == active_color)
        continue;

      // setupで扱うtowerは純粋towerだけ。
      if (top_block_size(board, source) != (int)board[source].size()) {
        continue;
      }

      if (!trees[color].used[source])
        continue;

      int destination = trees[color].parent[source];
      if (destination < 0)
        continue;
      if (destination == flush_pos)
        continue;

      // 自身の巣へ入れると消えてplatform材料にならない。
      if (destination == nest[color])
        continue;

      // sourceからrouteへ向かうparent列が、残り手数内に存在するものだけ。
      int source_dist = steps_to_route(color, source, on_route, remaining_depth);
      if (source_dist < 0)
        continue;

      int destination_dist = steps_to_route(color, destination, on_route, remaining_depth);

      // route上のtowerをroute外へ持ち出さない。
      // route外なら、必ず1歩routeへ近づく操作だけを許す。
      if (source_dist == 0) {
        if (destination_dist != 0)
          continue;
      } else {
        if (destination_dist != source_dist - 1)
          continue;
      }

      int destination_color = top_color(board, destination);

      // active colorを他色で覆わない。
      if (destination_color == active_color)
        continue;

      // 空マス、または同色の純粋towerにだけ積む。
      if (!board[destination].empty()) {
        if (destination_color != color)
          continue;
        if (top_block_size(board, destination) != (int)board[destination].size()) {
          continue;
        }
      }

      // flush packetを載せたとき高さ8以下になるよう、
      // setup towerは高さ (8 - flush_size) 以下に抑える。
      int capacity = (8 - strategy_flush_size) - (int)board[destination].size();
      if (capacity <= 0)
        continue;

      int moved_size = min((int)board[source].size(), capacity);
      if (moved_size <= 0)
        continue;

      CandidateMove candidate;
      candidate.move = {
          LocalMoveType::Setup,
          source,
          destination,
          moved_size,
      };
      candidate.route_dist = destination_dist;
      candidate.reaches_route = (destination_dist == 0);
      candidate.moved_size = moved_size;
      candidates.push_back(candidate);
    }

    // 分岐数を抑える。
    // まずrouteへ実際に到達する手、次にrouteまで近い手を優先。
    sort(candidates.begin(), candidates.end(), [](const CandidateMove &a, const CandidateMove &b) {
      if (a.reaches_route != b.reaches_route) {
        return a.reaches_route > b.reaches_route;
      }
      if (a.route_dist != b.route_dist) {
        return a.route_dist < b.route_dist;
      }
      if (a.moved_size != b.moved_size) {
        return a.moved_size > b.moved_size;
      }
      if (a.move.destination != b.move.destination) {
        return a.move.destination < b.move.destination;
      }
      return a.move.source < b.move.source;
    });

    if ((int)candidates.size() > LOCAL_SETUP_CANDIDATE_LIMIT) {
      candidates.resize(LOCAL_SETUP_CANDIDATE_LIMIT);
    }

    result.reserve(candidates.size());
    for (const auto &candidate : candidates) {
      result.push_back(candidate.move);
    }
    return result;
  }

  // helper色のcolor-fixedを「物理的な1操作」だけ前倒しするための候補を作る。
  // 複雑なplatform探索までは呼ばず、現在の通常輸送規則に沿う1手だけを返す。
  optional<LocalMove> generate_one_insert_action(const Board &board, int active_color, int helper_color) const {
    if (helper_color < 0 || helper_color >= P.K)
      return nullopt;
    if (helper_color == active_color)
      return nullopt;

    int source = deepest_active_cell(board, helper_color);
    if (source < 0 || source == nest[helper_color])
      return nullopt;
    if (board[source].empty())
      return nullopt;
    if (top_color(board, source) != helper_color)
      return nullopt;

    // 挿入ではmixed towerを触らない。helperのpure towerだけを進める。
    if (top_block_size(board, source) != (int)board[source].size()) {
      return nullopt;
    }

    auto destination_is_safe = [&](int destination, int moved_size) {
      if (destination < 0 || destination >= V)
        return false;
      if ((int)board[destination].size() + moved_size > 8)
        return false;
      if (destination == nest[helper_color])
        return true;
      if (board[destination].empty())
        return true;
      // active色や第三色を覆わず、pureな同色towerへのmergeだけ許す。
      if (top_color(board, destination) != helper_color)
        return false;
      return top_block_size(board, destination) == (int)board[destination].size();
    };

    auto make_move = [&](int src, int dst, int moved_size) -> optional<LocalMove> {
      if (src < 0 || dst < 0 || moved_size <= 0)
        return nullopt;
      if (!destination_is_safe(dst, moved_size))
        return nullopt;
      return LocalMove{
          LocalMoveType::Insert, src, dst, moved_size, helper_color,
      };
    };

    int block_size = top_block_size(board, source);
    if (block_size <= 0)
      return nullopt;

    // 高さ8なら、platform探索なしの決定的flushを1手だけ前倒しする。
    if ((int)board[source].size() == 8 && block_size >= strategy_flush_size) {
      int destination = find_flush_jump_destination(board, helper_color, source, strategy_flush_size);
      if (destination != -1) {
        if (auto move = make_move(source, destination, strategy_flush_size)) {
          return move;
        }
      }

      auto nexts = shortest_next_cells(board, helper_color, source, strategy_flush_size);
      for (int dst : nexts) {
        if (auto move = make_move(source, dst, strategy_flush_size)) {
          return move;
        }
      }
      return nullopt;
    }

    // 通常のSteiner tree上のfull-block jump。
    int jump_destination = find_jump_destination(board, helper_color, source, block_size);
    if (jump_destination != -1) {
      if (auto move = make_move(source, jump_destination, block_size)) {
        return move;
      }
    }

    int parent = trees[helper_color].parent[source];
    if (parent < 0)
      return nullopt;

    int capacity = 8 - (int)board[parent].size();

    // 前方同色towerが詰まりかけている場合は、通常版と同じく
    // 必要分だけ詰めて高さ8を作る操作を1手として返す。
    if (!board[parent].empty() && top_color(board, parent) == helper_color &&
        top_block_size(board, parent) == (int)board[parent].size() && capacity < block_size) {
      if (capacity > 0) {
        return make_move(source, parent, capacity);
      }

      // parentが既に満杯なら、そのtowerのflushを1手だけ進める。
      int front_block = top_block_size(board, parent);
      int front_flush = min(strategy_flush_size, front_block);
      if (front_flush <= 0)
        return nullopt;

      int destination = find_flush_jump_destination(board, helper_color, parent, front_flush);
      if (destination != -1) {
        if (auto move = make_move(parent, destination, front_flush)) {
          return move;
        }
      }
      auto nexts = shortest_next_cells(board, helper_color, parent, front_flush);
      for (int dst : nexts) {
        if (auto move = make_move(parent, dst, front_flush)) {
          return move;
        }
      }
      return nullopt;
    }

    if (capacity <= 0)
      return nullopt;
    int moved_size = min(block_size, capacity);
    return make_move(source, parent, moved_size);
  }

  // 現在のactive flushの直前に挿入する他色1手を列挙する。
  // いったん色を選んだら、同じhelper色だけを最大INSERT_MAX_STEPS手続ける。
  vector<LocalMove> generate_insert_moves(const Board &board, int active_color, int flush_pos, int locked_color,
                                          int insert_steps) const {
    vector<LocalMove> result;
    if (!insert_neighbor_enabled)
      return result;
    if (insert_steps >= INSERT_MAX_STEPS)
      return result;

    const auto &on_route = flush_route_mask(active_color, flush_pos);

    struct Candidate {
      LocalMove move;
      int score = INT_MIN;
    };
    vector<Candidate> candidates;

    for (int color = 0; color < P.K; ++color) {
      if (color == active_color)
        continue;
      if (locked_color >= 0 && color != locked_color)
        continue;

      auto move = generate_one_insert_action(board, active_color, color);
      if (!move.has_value())
        continue;

      Board child = board;
      vector<Action> dummy;
      if (!execute_action(child, dummy, move->source, move->moved_size, move->destination)) {
        continue;
      }

      // active色を隠す副作用がないことを念のため確認する。
      if (count_color_in_tower(child[move->destination], active_color) > 0 &&
          top_color(child, move->destination) != active_color) {
        continue;
      }

      int score = 0;

      // activeのflush最短路上にpure helper towerを作れる操作を強く優先。
      if (on_route[move->destination])
        score += 1000;

      if (!child[move->destination].empty() && top_color(child, move->destination) == color &&
          top_block_size(child, move->destination) == (int)child[move->destination].size()) {
        score += 80 * (int)child[move->destination].size();
      }

      // helper自身のtreeを進めつつ、active経路へ近づくものを優先。
      int route_dist = steps_to_route(color, move->destination, on_route, INSERT_MAX_STEPS);
      if (route_dist >= 0) {
        score += 200 * (INSERT_MAX_STEPS - route_dist + 1);
      }

      // 同点なら一度に大きいblockを進める方を優先。
      score += 10 * move->moved_size;
      candidates.push_back({*move, score});
    }

    sort(candidates.begin(), candidates.end(), [](const Candidate &a, const Candidate &b) {
      if (a.score != b.score)
        return a.score > b.score;
      if (a.move.moved_size != b.move.moved_size) {
        return a.move.moved_size > b.move.moved_size;
      }
      if (a.move.color != b.move.color)
        return a.move.color < b.move.color;
      if (a.move.destination != b.move.destination) {
        return a.move.destination < b.move.destination;
      }
      return a.move.source < b.move.source;
    });

    if ((int)candidates.size() > INSERT_COLOR_CANDIDATE_LIMIT) {
      candidates.resize(INSERT_COLOR_CANDIDATE_LIMIT);
    }
    for (const auto &candidate : candidates) {
      result.push_back(candidate.move);
    }
    return result;
  }

  // flush_pos のtowerが [helper...][active packet...] の2blockだけから成るかを調べる。
  // 既存Flushが純粋な他色towerへ着地した直後がこの形になる。
  bool detect_borrow_attach_state(const Board &board, int active_color, int flush_pos, int &helper_color,
                                  int &helper_size) const {
    helper_color = -1;
    helper_size = 0;
    if (flush_pos < 0 || flush_pos >= V)
      return false;
    const Tower &tower = board[flush_pos];
    if (tower.size() <= strategy_flush_size)
      return false;
    if (top_color(board, flush_pos) != active_color)
      return false;
    if (top_block_size(board, flush_pos) != strategy_flush_size)
      return false;

    int lower = tower.size() - strategy_flush_size;
    int c = tower[0];
    if (c == active_color)
      return false;
    for (int i = 0; i < lower; ++i) {
      if (tower[i] != c)
        return false;
    }
    for (int i = lower; i < tower.size(); ++i) {
      if (tower[i] != active_color)
        return false;
    }

    if (lower <= 0 || lower > 8 - strategy_flush_size)
      return false;
    helper_color = c;
    helper_size = lower;
    return true;
  }

  // borrowed segment が現在towerの全体を占めていることを確認する。
  // Carry先を空マスに限定しているため、borrow開始後はこの形を維持できる。
  bool validate_borrow_segment(const LocalState &state, int active_color) const {
    if (state.borrow_helper_color < 0)
      return false;
    if (state.flush_pos < 0 || state.flush_pos >= V)
      return false;
    const Tower &tower = state.board[state.flush_pos];
    int expected = strategy_flush_size + state.borrow_helper_size;
    if (tower.size() != expected)
      return false;

    bool helper_top = (state.borrow_shared_steps & 1);
    if (!helper_top) {
      for (int i = 0; i < state.borrow_helper_size; ++i) {
        if (tower[i] != state.borrow_helper_color)
          return false;
      }
      for (int i = state.borrow_helper_size; i < expected; ++i) {
        if (tower[i] != active_color)
          return false;
      }
    } else {
      for (int i = 0; i < strategy_flush_size; ++i) {
        if (tower[i] != active_color)
          return false;
      }
      for (int i = strategy_flush_size; i < expected; ++i) {
        if (tower[i] != state.borrow_helper_color)
          return false;
      }
    }
    return true;
  }

  // borrowed-helper 中に許す操作を列挙する。
  // ・Carry: activeの最短路方向へ mixed segment 全体を1マス運ぶ。
  // ・Detach: helperがtopの奇数carry後、helperだけを自身のtreeへ戻す。
  vector<LocalMove> generate_borrow_moves(const LocalState &state, int active_color) const {
    vector<LocalMove> result;
    if (!borrow_neighbor_enabled)
      return result;

    int helper_color = state.borrow_helper_color;
    int helper_size = state.borrow_helper_size;
    int shared_steps = state.borrow_shared_steps;

    // borrow開始前なら、純粋helperへの着地直後の形を検出する。
    if (helper_color < 0) {
      if (!detect_borrow_attach_state(state.board, active_color, state.flush_pos, helper_color, helper_size)) {
        return result;
      }
      shared_steps = 0;
    } else if (!validate_borrow_segment(state, active_color)) {
      return result;
    }

    int segment_size = strategy_flush_size + helper_size;
    bool helper_top = (shared_steps & 1);

    // Carry は空マスへの1歩だけに限定する。
    // mixed segmentの上下関係を正確に追え、既存盤面を不用意に巻き込まない。
    if (shared_steps < BORROW_MAX_SHARED_STEPS) {
      for (int d = 0; d < 4; ++d) {
        int destination = next_cell(state.flush_pos, d);
        if (destination == -1)
          continue;
        if (!is_shortest_step(active_color, state.flush_pos, destination))
          continue;
        if (!state.board[destination].empty())
          continue;

        // nest上では自動帰巣でsegment構造が変わるため、borrow中は避ける。
        int nc = nest_color[destination];
        if (nc == active_color || nc == helper_color)
          continue;

        result.push_back({
            LocalMoveType::BorrowCarry,
            state.flush_pos,
            destination,
            segment_size,
        });
      }
    }

    // helperがtopに来るのは奇数回carry後。そのタイミングだけ切り離せる。
    if (!helper_top)
      return result;

    const Tower &tower = state.board[state.flush_pos];
    int k = tower.size() - helper_size;
    int max_jump = k + 1;

    struct DetachCandidate {
      LocalMove move;
      int gain = INT_MIN;
      int length = 0;
      int merge_height = 0;
    };
    vector<DetachCandidate> candidates;

    int origin_dist = dist_to_nest[helper_color][state.borrow_origin];

    for (int d = 0; d < 4; ++d) {
      int v = state.flush_pos;
      for (int l = 1; l <= max_jump; ++l) {
        int nv = next_cell(v, d);
        if (nv == -1)
          break;
        v = nv;

        if ((int)state.board[v].size() + helper_size > 8)
          continue;

        // 後でhelper色をcolor-fixedで処理できるよう、helper自身のtreeへ戻す。
        if (v != nest[helper_color] && !trees[helper_color].used[v])
          continue;

        // detach後はhelperをpure towerとして残す（巣ならそのまま帰巣）。
        if (v != nest[helper_color] && !state.board[v].empty()) {
          if (top_color(state.board, v) != helper_color)
            continue;
          if (top_block_size(state.board, v) != (int)state.board[v].size())
            continue;
        }

        int after_dist = dist_to_nest[helper_color][v];
        if (after_dist == INT_MAX || origin_dist == INT_MAX)
          continue;
        int gain = origin_dist - after_dist;

        // helperを元より遠くへ捨てる近傍は採らない。
        if (gain < 0)
          continue;

        candidates.push_back({
            {LocalMoveType::BorrowDetach, state.flush_pos, v, helper_size},
            gain,
            l,
            (int)state.board[v].size(),
        });
      }
    }

    sort(candidates.begin(), candidates.end(), [](const DetachCandidate &a, const DetachCandidate &b) {
      if (a.gain != b.gain)
        return a.gain > b.gain;
      if (a.length != b.length)
        return a.length > b.length;
      if (a.merge_height != b.merge_height)
        return a.merge_height > b.merge_height;
      return a.move.destination < b.move.destination;
    });

    if ((int)candidates.size() > BORROW_DETACH_CANDIDATE_LIMIT) {
      candidates.resize(BORROW_DETACH_CANDIDATE_LIMIT);
    }
    for (const auto &candidate : candidates)
      result.push_back(candidate.move);

    return result;
  }

  int borrow_potential(const LocalState &state, int active_color) const {
    if (!borrow_neighbor_enabled)
      return 0;

    if (state.borrow_helper_color >= 0) {
      // borrow途中をbeam後半から落とさない。
      int score = 200000;
      if (state.borrow_shared_steps & 1)
        score += 100000; // detach可能
      return score;
    }

    int helper_color = -1, helper_size = 0;
    if (detect_borrow_attach_state(state.board, active_color, state.flush_pos, helper_color, helper_size)) {
      return 60000 + 5000 * helper_size;
    }
    return 0;
  }

  int evaluate_flush_cost(const Board &board, int color, int source, int packet_size) const {
    if (source == nest[color])
      return 0;

    Board copied = board;
    int action_count = 0;

    if (!flush_packet_to_nest_count_only(copied, action_count, color, source, packet_size)) {
      return INT_MAX;
    }

    return action_count;
  }

  /**
   * beamのtie-break用。
   *
   * ・flush経路上に既に存在する高さ1..4の他色tower
   * ・数手以内にflush経路へ到達できる他色tower
   *
   * を高く評価する。
   * 「最初の数手ではまだflush手数が減らないが、数手後にplatformが完成する」
   * 状態をbeamから落としにくくするための値。
   */
  int platform_potential(const Board &board, int active_color, int flush_pos, int remaining_depth,
                         int reuse_credit) const {
    if (flush_pos == nest[active_color])
      return 0;

    const auto &route = flush_route_cells(active_color, flush_pos);
    const auto &on_route = flush_route_mask(active_color, flush_pos);

    int score = 0;

    // すでに最短路領域上にあるplatform。
    for (int v : route) {
      if (board[v].empty())
        continue;

      int color = top_color(board, v);
      if (color == active_color)
        continue;
      if (top_block_size(board, v) != (int)board[v].size())
        continue;
      if ((int)board[v].size() > 8 - strategy_flush_size)
        continue;

      int h = board[v].size();
      int progress = floor_dist[flush_pos][v];
      int extra_uses = estimated_extra_platform_uses(board, active_color, v);

      score += 100 * h;
      score += max(0, 30 - progress);

      // 後続flushでも使えそうな台は強く残す。
      score += 160 * extra_uses * h;
    }

    if (remaining_depth > 0) {
      // まだrouteに到達していないが、数手でplatformになれるtower。
      for (int v = 0; v < V; ++v) {
        if (board[v].empty())
          continue;
        int color = top_color(board, v);
        if (color < 0 || color == active_color)
          continue;
        if (top_block_size(board, v) != (int)board[v].size())
          continue;
        if ((int)board[v].size() > 8 - strategy_flush_size)
          continue;
        if (on_route[v])
          continue;

        int dist = steps_to_route(color, v, on_route, remaining_depth);
        if (dist <= 0)
          continue;

        score += max(0, remaining_depth - dist + 1) * (int)board[v].size() * 4;
      }
    }

    // route以外も含め、後続flushで再利用されるplatform全体の価値を加える。
    // reuse_credit は LocalState で厳密に差分更新済み。
    score += 80 * reuse_credit;

    return score;
  }

  uint64_t hash_local_state(const LocalState &state) const {
    uint64_t h = state.board_hash;
    auto mix = [&](uint64_t x) {
      h ^= splitmix64(x + 0x9e3779b97f4a7c15ULL + (h << 1));
    };

    mix((uint64_t)(state.flush_pos + 1));
    mix((uint64_t)state.flush_finished << 8);
    mix((uint64_t)state.insert_phase_open << 9);
    mix((uint64_t)(state.insert_color + 2) << 12);
    mix((uint64_t)(state.insert_steps + 1) << 20);
    mix((uint64_t)(state.insert_credit + 1) << 24);
    mix((uint64_t)(state.borrow_helper_color + 2) << 28);
    mix((uint64_t)(state.borrow_helper_size + 1) << 36);
    mix((uint64_t)(state.borrow_origin + 2) << 40);
    mix((uint64_t)(state.borrow_shared_steps + 1) << 50);
    mix((uint64_t)(state.borrow_credit + 1) << 54);
    return h;
  }

  /**
   * 「setupを先に全部行ってからflush」ではなく、
   * flush操作と他色setup操作を同じ探索木に入れて最大5手程度先読みする。
   *
   * 例えば
   *   setup -> setup -> flush -> setup -> flush -> ...
   * のような途中挿入も探索できる。
   *
   * 各探索状態からは、最後に最短路ベースの決定的flushを行った場合の
   * 残り手数を正確に計算する。
   * したがって評価値は
   *
   *   ここまでの局所操作数 + 残りflush手数
   *
   * となる。
   */
  vector<LocalMove> search_platform_plan(const Board &board, int color, int source) const {
    int baseline = evaluate_flush_cost(board, color, source, strategy_flush_size);
    if (baseline == INT_MAX)
      return {};

    vector<LocalMove> best_plan;

    const int baseline_active_remaining = count_color(board, color);
    const int baseline_reuse_credit = reusable_platform_credit(board, color, baseline_active_remaining);
    int best_effective = baseline;
    int best_total = baseline;

    LocalState initial;
    initial.board = board;
    initial.flush_pos = source;
    initial.flush_finished = false;
    initial.finish_cost = baseline;
    initial.total_cost = baseline;
    initial.active_remaining = baseline_active_remaining;
    initial.reuse_credit = baseline_reuse_credit;
    initial.effective_cost = baseline;
    initial.potential = platform_potential(board, color, source, LOCAL_SEARCH_DEPTH, initial.reuse_credit);
    initial.potential += borrow_potential(initial, color);
    initial.board_hash = hash_board(initial.board);
    initial.hash = hash_local_state(initial);

    vector<LocalState> beam;
    beam.push_back(std::move(initial));

    for (int depth = 0; depth < LOCAL_SEARCH_DEPTH; ++depth) {
      vector<LocalState> next;
      next.reserve((LOCAL_BEAM_WIDTH + LOCAL_INSERT_BEAM_WIDTH) * 12);
      unordered_set<uint64_t> seen;
      seen.reserve((LOCAL_BEAM_WIDTH + LOCAL_INSERT_BEAM_WIDTH) * 32);

      for (const auto &state : beam) {
        if (state.flush_finished)
          continue;

        vector<LocalMove> moves;

        int remaining_after_this = LOCAL_SEARCH_DEPTH - (depth + 1);

        if (borrow_neighbor_enabled && state.borrow_helper_color >= 0) {
          // borrowは現在停止中。互換のためコードだけ残す。
          moves = generate_borrow_moves(state, color);
        } else {
          moves = generate_flush_moves(state.board, color, state.flush_pos);

          auto setup_moves = generate_setup_moves(state.board, color, state.flush_pos, remaining_after_this + 1);
          moves.insert(moves.end(), setup_moves.begin(), setup_moves.end());

          // 挿入近傍は局所探索の先頭phaseだけで許す。
          // 1色選んだ後は同じhelper色を最大3手だけ前倒しする。
          if (insert_neighbor_enabled && state.insert_phase_open && state.insert_steps < INSERT_MAX_STEPS) {
            auto insert_moves =
                generate_insert_moves(state.board, color, state.flush_pos, state.insert_color, state.insert_steps);
            moves.insert(moves.end(), insert_moves.begin(), insert_moves.end());
          }

          if (borrow_neighbor_enabled) {
            auto borrow_moves = generate_borrow_moves(state, color);
            moves.insert(moves.end(), borrow_moves.begin(), borrow_moves.end());
          }
        }

        Board scratch = state.board;
        for (const auto &move : moves) {
          Tower old_source = scratch[move.source];
          Tower old_destination = scratch[move.destination];
          uint64_t child_board_hash = state.board_hash;
          int dummy_count = 0;

          if (!execute_action_count_only(scratch, dummy_count, move.source, move.moved_size, move.destination, nullptr,
                                         &child_board_hash)) {
            scratch[move.source] = old_source;
            scratch[move.destination] = old_destination;
            continue;
          }

          const int old_future_use_cap = future_platform_use_cap(state.active_remaining);

          int active_before = count_color_in_tower(old_source, color) + count_color_in_tower(old_destination, color);
          int old_credit = reusable_platform_cell_credit(state.board, color, move.source, old_future_use_cap);
          if (move.destination != move.source) {
            old_credit += reusable_platform_cell_credit(state.board, color, move.destination, old_future_use_cap);
          }

          LocalState child;
          child.moves = state.moves;
          child.move_count = state.move_count;
          child.board_hash = child_board_hash;
          child.flush_pos = state.flush_pos;
          child.flush_finished = state.flush_finished;
          child.active_remaining = state.active_remaining;
          child.insert_phase_open = state.insert_phase_open;
          child.insert_color = state.insert_color;
          child.insert_steps = state.insert_steps;
          child.insert_credit = state.insert_credit;
          child.borrow_helper_color = state.borrow_helper_color;
          child.borrow_helper_size = state.borrow_helper_size;
          child.borrow_origin = state.borrow_origin;
          child.borrow_shared_steps = state.borrow_shared_steps;
          child.borrow_credit = state.borrow_credit;

          int active_after = count_color_in_tower(scratch[move.source], color) +
                             count_color_in_tower(scratch[move.destination], color);
          child.active_remaining += active_after - active_before;

          const int new_future_use_cap = future_platform_use_cap(child.active_remaining);

          if (new_future_use_cap == old_future_use_cap) {
            int new_credit = reusable_platform_cell_credit(scratch, color, move.source, new_future_use_cap);
            if (move.destination != move.source) {
              new_credit += reusable_platform_cell_credit(scratch, color, move.destination, new_future_use_cap);
            }
            child.reuse_credit = state.reuse_credit - old_credit + new_credit;
          } else {
            child.reuse_credit = reusable_platform_credit(scratch, color, child.active_remaining);
          }

          bool valid = child.move_count < LOCAL_SEARCH_DEPTH;
          if (valid)
            child.moves[child.move_count++] = move;

          if (valid && move.type == LocalMoveType::Insert) {
            if (!child.insert_phase_open) {
              valid = false;
            } else {
              if (child.insert_color < 0) {
                child.insert_color = move.color;
              } else if (child.insert_color != move.color) {
                valid = false;
              }
              if (valid) {
                ++child.insert_steps;
                ++child.insert_credit;
              }
            }
          } else if (valid) {
            child.insert_phase_open = false;
          }

          if (valid && move.type == LocalMoveType::Flush) {
            child.flush_pos = move.destination;
            if (move.destination == nest[color]) {
              child.flush_finished = true;
            }
          } else if (valid && move.type == LocalMoveType::BorrowCarry) {
            if (child.borrow_helper_color < 0) {
              int helper_color = -1, helper_size = 0;
              if (!detect_borrow_attach_state(state.board, color, state.flush_pos, helper_color, helper_size)) {
                valid = false;
              } else {
                child.borrow_helper_color = helper_color;
                child.borrow_helper_size = helper_size;
                child.borrow_origin = state.flush_pos;
                child.borrow_shared_steps = 0;
              }
            }
            if (valid) {
              ++child.borrow_shared_steps;
              child.flush_pos = move.destination;
            }
          } else if (valid && move.type == LocalMoveType::BorrowDetach) {
            if (child.borrow_helper_color < 0) {
              valid = false;
            } else {
              int helper_color = child.borrow_helper_color;
              int origin_dist = dist_to_nest[helper_color][child.borrow_origin];
              int after_dist = dist_to_nest[helper_color][move.destination];
              if (origin_dist != INT_MAX && after_dist != INT_MAX) {
                int gain = max(0, origin_dist - after_dist);
                child.borrow_credit += min(BORROW_FUTURE_CREDIT_CAP, gain);
              }
              child.borrow_helper_color = -1;
              child.borrow_helper_size = 0;
              child.borrow_origin = -1;
              child.borrow_shared_steps = 0;
            }
          }

          if (valid) {
            child.hash = hash_local_state(child);
            if (!seen.insert(child.hash).second)
              valid = false;
          }

          if (valid) {
            // 重複排除後にだけ盤面を保存する。
            child.board = scratch;

            if (child.flush_finished) {
              child.finish_cost = 0;
            } else {
              child.finish_cost = evaluate_flush_cost(child.board, color, child.flush_pos, strategy_flush_size);
            }

            if (child.finish_cost == INT_MAX) {
              child.total_cost = INT_MAX;
              child.effective_cost = INT_MAX;
            } else {
              child.total_cost = (int)child.move_count + child.finish_cost;

              int extra_reuse_credit = max(0, child.reuse_credit - baseline_reuse_credit);

              child.effective_cost = child.total_cost - extra_reuse_credit - child.insert_credit - child.borrow_credit;

              if (child.borrow_helper_color < 0 &&
                  (child.effective_cost < best_effective ||
                   (child.effective_cost == best_effective && child.total_cost < best_total))) {
                best_effective = child.effective_cost;
                best_total = child.total_cost;
                best_plan.assign(child.moves.begin(), child.moves.begin() + child.move_count);
              }
            }

            child.potential =
                platform_potential(child.board, color, child.flush_pos, remaining_after_this, child.reuse_credit);
            child.potential += 40 * child.insert_steps;
            child.potential += borrow_potential(child, color);

            next.push_back(std::move(child));
          }

          scratch[move.source] = old_source;
          scratch[move.destination] = old_destination;
        }
      }

      if (next.empty())
        break;

      // 挿入近傍を足しても、従来のpure platform-search 64本を
      // そのまま確保する。追加の32本だけをinsert経験あり状態に使う。
      vector<LocalState> pure_next;
      vector<LocalState> inserted_next;
      pure_next.reserve(next.size());
      inserted_next.reserve(next.size());
      for (auto &state : next) {
        if (state.insert_steps == 0) {
          pure_next.push_back(std::move(state));
        } else {
          inserted_next.push_back(std::move(state));
        }
      }

      auto select_group = [&](vector<LocalState> &group, int limit) -> vector<LocalState> {
        vector<LocalState> selected;
        if (limit <= 0 || group.empty())
          return selected;
        selected.reserve(limit);
        unordered_set<uint64_t> selected_hash;
        selected_hash.reserve(limit * 2);

        sort(group.begin(), group.end(), [](const LocalState &a, const LocalState &b) {
          if (a.effective_cost != b.effective_cost) {
            return a.effective_cost < b.effective_cost;
          }
          if (a.total_cost != b.total_cost)
            return a.total_cost < b.total_cost;
          if (a.finish_cost != b.finish_cost)
            return a.finish_cost < b.finish_cost;
          if (a.reuse_credit != b.reuse_credit)
            return a.reuse_credit > b.reuse_credit;
          if (a.potential != b.potential)
            return a.potential > b.potential;
          return a.move_count < b.move_count;
        });

        int cost_quota = limit / 2;
        for (const auto &state : group) {
          if ((int)selected.size() >= cost_quota)
            break;
          if (!selected_hash.insert(state.hash).second)
            continue;
          selected.push_back(state);
        }

        sort(group.begin(), group.end(), [](const LocalState &a, const LocalState &b) {
          if (a.potential != b.potential)
            return a.potential > b.potential;
          if (a.reuse_credit != b.reuse_credit)
            return a.reuse_credit > b.reuse_credit;
          if (a.effective_cost != b.effective_cost) {
            return a.effective_cost < b.effective_cost;
          }
          if (a.total_cost != b.total_cost)
            return a.total_cost < b.total_cost;
          if (a.finish_cost != b.finish_cost)
            return a.finish_cost < b.finish_cost;
          return a.move_count < b.move_count;
        });

        for (const auto &state : group) {
          if ((int)selected.size() >= limit)
            break;
          if (!selected_hash.insert(state.hash).second)
            continue;
          selected.push_back(state);
        }
        return selected;
      };

      vector<LocalState> selected = select_group(pure_next, LOCAL_BEAM_WIDTH);
      auto inserted_selected = select_group(inserted_next, LOCAL_INSERT_BEAM_WIDTH);
      selected.reserve(selected.size() + inserted_selected.size());
      for (auto &state : inserted_selected) {
        selected.push_back(std::move(state));
      }

      beam = std::move(selected);
    }

    return best_plan;
  }

  /**
   * flush packetをflushする。
   *
   * 最大5手程度、flush自身の移動と他色のsetupを混ぜて局所探索する。
   * 探索で得なprefixが見つかった場合はその操作列を実行し、
   * 残りは最短路ベースの決定的flushで巣まで送る。
   */
  bool flush_with_platform_search(Board &board, vector<Action> &actions, int color, int source,
                                  ColorCounts *counts = nullptr) const {
    if (top_block_size(board, source) < strategy_flush_size)
      return false;

    vector<LocalMove> plan = search_platform_plan(board, color, source);
    int flush_pos = source;

    for (const auto &move : plan) {
      if (!execute_action(board, actions, move.source, move.moved_size, move.destination, counts)) {
        return false;
      }
      if (move.type == LocalMoveType::Insert) {
        ++current_inserted_actions;
      }

      if (move.type == LocalMoveType::Flush || move.type == LocalMoveType::BorrowCarry) {
        flush_pos = move.destination;
        if (flush_pos == nest[color]) {
          return true;
        }
      }
    }

    return flush_packet_to_nest_core(board, actions, color, flush_pos, strategy_flush_size, counts);
  }

  /**
   * active color のtop blockを1段階進める。
   *
   * 基本はGreedy Steiner tree上を通常マージ。
   * tower全体が高さ8になったら、上X段だけをflushして
   * 下側をfill用の集積blockとして残す。
   *
   * また、前方同色towerへfull blockを載せられない場合は、
   * currentの一部をparentへ入れて高さ8までfillした後、
   * parentの上X段をflushする。
   */
  bool advance_one(Board &board, vector<Action> &actions, int color, int source, ColorCounts *counts = nullptr) const {
    if (source == nest[color])
      return false;
    if (board[source].empty())
      return false;
    if (top_color(board, source) != color)
      return false;
    if (!trees[color].used[source])
      return false;

    int block_size = top_block_size(board, source);
    if (block_size <= 0)
      return false;

    // 高さ8のtowerができたら、上X段を先に流す。
    // 下側（または下側の残り）は後続blockを受けるfill地点として残す。
    if ((int)board[source].size() == 8 && block_size >= strategy_flush_size) {
      return flush_with_platform_search(board, actions, color, source, counts);
    }

    // full blockのまま進めるなら通常のSteiner集約を優先する。
    int jump_destination = find_jump_destination(board, color, source, block_size);

    if (jump_destination != -1) {
      return execute_action(board, actions, source, block_size, jump_destination, counts);
    }

    int parent = trees[color].parent[source];
    if (parent < 0)
      return false;

    int capacity = 8 - (int)board[parent].size();

    // 同色の前方towerへfull blockが入らない。
    // currentから必要分をfillして高さ8を作り、上X段をflushする。
    if (!board[parent].empty() && top_color(board, parent) == color && capacity < block_size) {
      int need = capacity;

      if (need > 0) {
        if (!execute_action(board, actions, source, need, parent, counts)) {
          return false;
        }
      }

      if ((int)board[parent].size() != 8)
        return false;
      if (top_block_size(board, parent) < strategy_flush_size)
        return false;

      return flush_with_platform_search(board, actions, color, parent, counts);
    }

    if (capacity <= 0)
      return false;

    // 他色tower上では入る分だけ載せる。
    // 局所探索で作るplatformは高さ4以下なので、
    // 4匹blockなら最大高さ8のジャンプ台になる。
    int moved_size = min(block_size, capacity);
    if (moved_size <= 0)
      return false;

    return execute_action(board, actions, source, moved_size, parent, counts);
  }

  // =========================================================
  // One color
  // =========================================================

  int deepest_active_cell(const Board &board, int color) const {
    int best = -1;
    int best_depth = -1;
    int best_block_size = -1;

    for (int v = 0; v < V; ++v) {
      if (board[v].empty())
        continue;
      if (top_color(board, v) != color)
        continue;
      if (!trees[color].used[v])
        continue;
      if (v == nest[color])
        continue;

      int depth = trees[color].depth[v];
      int block_size = top_block_size(board, v);

      // 葉側から処理して、分岐点で自然にマージさせる。
      if (depth > best_depth || (depth == best_depth && block_size < best_block_size) ||
          (depth == best_depth && block_size == best_block_size && v < best)) {
        best = v;
        best_depth = depth;
        best_block_size = block_size;
      }
    }

    return best;
  }

  /**
   * platform-searchを使わない軽量な color-fixed 1段階進行。
   * interleave beam の評価専用で、実際のactive色の実行は従来の advance_one() を使う。
   * flushに入った場合は count-only でそのpacketを巣まで送り切る。
   */
  bool advance_one_count_only_fast(Board &board, int &action_count, int color, ColorCounts *counts = nullptr,
                                   uint64_t *board_hash = nullptr) const {
    if (counts != nullptr && (*counts)[color] == 0)
      return true;
    if (counts == nullptr && count_color(board, color) == 0)
      return true;

    int source = deepest_active_cell(board, color);
    if (source < 0 || source == nest[color])
      return false;
    if (board[source].empty() || top_color(board, source) != color)
      return false;
    if (!trees[color].used[source])
      return false;

    int block_size = top_block_size(board, source);
    if (block_size <= 0)
      return false;

    if ((int)board[source].size() == 8 && block_size >= strategy_flush_size) {
      return flush_packet_to_nest_count_only(board, action_count, color, source, strategy_flush_size, counts,
                                             board_hash);
    }

    int jump_destination = find_jump_destination(board, color, source, block_size);
    if (jump_destination != -1) {
      return execute_action_count_only(board, action_count, source, block_size, jump_destination, counts, board_hash);
    }

    int parent = trees[color].parent[source];
    if (parent < 0)
      return false;

    int capacity = 8 - (int)board[parent].size();

    if (!board[parent].empty() && top_color(board, parent) == color && capacity < block_size) {
      int need = capacity;
      if (need > 0) {
        if (!execute_action_count_only(board, action_count, source, need, parent, counts, board_hash)) {
          return false;
        }
      }

      if ((int)board[parent].size() != 8)
        return false;
      if (top_block_size(board, parent) < strategy_flush_size)
        return false;

      return flush_packet_to_nest_count_only(board, action_count, color, parent, strategy_flush_size, counts,
                                             board_hash);
    }

    if (capacity <= 0)
      return false;
    int moved_size = min(block_size, capacity);
    if (moved_size <= 0)
      return false;

    return execute_action_count_only(board, action_count, source, moved_size, parent, counts, board_hash);
  }

  int estimate_color_finish_cost_fast(const Board &board, const ColorCounts &base_counts, int color) const {
    if (base_counts[color] == 0)
      return 0;

    Board copied = board;
    ColorCounts counts = base_counts;
    int action_count = 0;
    for (int guard = 0; guard < 4096; ++guard) {
      if (counts[color] == 0)
        return action_count;
      int before = action_count;
      if (!advance_one_count_only_fast(copied, action_count, color, &counts, nullptr)) {
        return INT_MAX / 4;
      }
      if (action_count <= before)
        return INT_MAX / 4;
      if (action_count >= 4096)
        return INT_MAX / 4;
    }
    return INT_MAX / 4;
  }

  int estimate_color_finish_cost_fast(const Board &board, int color) const {
    return estimate_color_finish_cost_fast(board, count_colors(board), color);
  }

  uint64_t finish_cache_key(uint64_t board_hash, int color) const {
    uint64_t x = board_hash;
    x ^= splitmix64((uint64_t)(color + 1) * 0x517cc1b727220a95ULL);
    x ^= splitmix64((uint64_t)(strategy_flush_size + 1) * 0x6eed0e9da4d94a4fULL);
    return x;
  }

  int cached_finish_cost(const Board &board, const ColorCounts &counts, uint64_t board_hash, int color) const {
    uint64_t key = finish_cache_key(board_hash, color);
    auto it = finish_cost_cache.find(key);
    if (it != finish_cost_cache.end()) {
      for (const auto &entry : it->second) {
        if (entry.color == color && entry.flush_size == strategy_flush_size && board_equal(entry.board, board)) {
          return entry.value;
        }
      }
    }

    int value = estimate_color_finish_cost_fast(board, counts, color);

    // メモリ上限だけ設ける。clearしても探索情報や評価値は変わらない。
    if (finish_cost_cache_entries > 20000) {
      finish_cost_cache.clear();
      finish_cost_cache_entries = 0;
    }
    finish_cost_cache[key].push_back(FinishCostCacheEntry{board, color, strategy_flush_size, value});
    ++finish_cost_cache_entries;
    return value;
  }

  struct InterleaveState {
    Board board{};
    ColorCounts counts{};
    uint64_t hash = 0;
    int first_color = -1;
    int physical_cost = 0;

    // beam中はhelper 1操作 = 将来1操作の前倒しとしてだけ扱う。
    // 正確なfinish-cost差分は最後の少数候補にだけ計算する。
    int helper_credit = 0;
    array<uint8_t, 16> helper_actions{};
    uint16_t helper_mask = 0;

    int helper_streak = 0;
    int locked_helper = -1;
    int active_remaining = INT_MAX;

    // 中間beam選抜用の安い近似値。小さいほど良い。
    long long cheap_score = LLONG_MAX;
  };

  // active色を各towerから独立packetとして巣へ運ぶ場合の粗い残り手数。
  // O(V*8) だけで、完全rolloutは行わない。
  int estimate_active_ops_cheap(const Board &board, int color) const {
    long long estimate = 0;
    for (int v = 0; v < V; ++v) {
      if (board[v].empty())
        continue;
      int cnt = count_color_in_tower(board[v], color);
      if (cnt == 0)
        continue;
      int d = dist_to_nest[color][v];
      if (d == INT_MAX)
        return INT_MAX / 4;
      int packets = (cnt + strategy_flush_size - 1) / strategy_flush_size;
      estimate += 1LL * packets * d;
      if (estimate >= INT_MAX / 4)
        return INT_MAX / 4;
    }
    return (int)estimate;
  }

  // activeの今後の最短路領域上にあるpure他色towerを、
  // 既存platformとして少しだけ評価する。
  int interleave_platform_bonus_cheap(const Board &board, int active_color) const {
    int source = deepest_active_cell(board, active_color);
    if (source < 0)
      return 0;
    const auto &route = flush_route_mask(active_color, source);
    int max_base = 8 - strategy_flush_size;
    int bonus = 0;
    for (int v = 0; v < V; ++v) {
      if (!route[v] || board[v].empty())
        continue;
      int c = top_color(board, v);
      if (c < 0 || c == active_color)
        continue;
      if (top_block_size(board, v) != (int)board[v].size())
        continue;
      int h = board[v].size();
      if (h <= 0 || h > max_base)
        continue;
      // 1つの台を過大評価しないよう最大2操作相当まで。
      bonus += min(2, h);
    }
    return bonus;
  }

  long long interleave_cheap_score(const Board &board, int physical_cost, int helper_credit, int active_color) const {
    int active_ops = estimate_active_ops_cheap(board, active_color);
    if (active_ops >= INT_MAX / 8)
      return LLONG_MAX / 4;
    int platform_bonus = interleave_platform_bonus_cheap(board, active_color);
    return 1LL * physical_cost + active_ops - helper_credit - platform_bonus;
  }

  long long interleave_cheap_score(const InterleaveState &state, int active_color) const {
    return interleave_cheap_score(state.board, state.physical_cost, state.helper_credit, active_color);
  }

  // activeの現在位置・今後のflush領域と干渉しそうなhelper色だけを候補にする。
  vector<int> interleave_helper_candidates(const Board &board, const ColorCounts &counts, int active_color,
                                           int locked_helper) const {
    vector<pair<int, int>> scored;

    int active_source = deepest_active_cell(board, active_color);
    if (active_source < 0)
      return {};
    const auto &route = flush_route_mask(active_color, active_source);
    int active_parent = trees[active_color].parent[active_source];

    for (int color = 0; color < P.K; ++color) {
      if (color == active_color)
        continue;
      if (locked_helper >= 0 && color != locked_helper)
        continue;
      if (counts[color] == 0)
        continue;

      auto move = generate_one_insert_action(board, active_color, color);
      if (!move.has_value())
        continue;

      int score = 0;
      if (route[move->destination])
        score += 1200;
      if (trees[active_color].used[move->destination])
        score += 500;
      if (move->destination == active_parent)
        score += 1000;

      int d = floor_dist[active_source][move->destination];
      if (d != INT_MAX)
        score += max(0, 5 - d) * 120;

      // lock中は継続候補として必ず残す。
      if (locked_helper >= 0)
        score += 10000;
      if (score <= 0)
        continue;
      scored.push_back({-score, color});
    }

    sort(scored.begin(), scored.end());
    if ((int)scored.size() > INTERLEAVE_HELPER_COLOR_LIMIT) {
      scored.resize(INTERLEAVE_HELPER_COLOR_LIMIT);
    }

    vector<int> result;
    result.reserve(scored.size());
    for (auto [_, color] : scored)
      result.push_back(color);
    return result;
  }

  /**
   * active色の次の通常操作の前にhelperを1手挿入するかを短いbeamで決める。
   *
   * beam中の各ノードでは完全finish rolloutを行わない。
   * O(V*8)程度のcheap評価だけで幅を絞り、最後に残った上位候補だけ
   * estimate_color_finish_cost_fast() で正確に比較する。
   */
  int choose_interleave_first_color(const Board &start_board, const ColorCounts &start_counts, int active_color,
                                    int actual_locked_helper, int actual_helper_streak) const {
    if (!interleave_neighbor_enabled)
      return active_color;
    if (actual_helper_streak >= INTERLEAVE_MAX_HELPER_STREAK) {
      return active_color;
    }

    auto root_helpers = interleave_helper_candidates(start_board, start_counts, active_color, actual_locked_helper);
    if (root_helpers.empty())
      return active_color;

    InterleaveState initial;
    initial.board = start_board;
    initial.counts = start_counts;
    initial.hash = hash_board(start_board);
    initial.helper_streak = actual_helper_streak;
    initial.locked_helper = actual_locked_helper;
    initial.active_remaining = start_counts[active_color];
    initial.cheap_score = interleave_cheap_score(initial, active_color);
    if (initial.cheap_score >= LLONG_MAX / 8)
      return active_color;

    vector<InterleaveState> beam{initial};
    vector<InterleaveState> rollout_pool;
    rollout_pool.reserve(INTERLEAVE_SEARCH_DEPTH * INTERLEAVE_BEAM_WIDTH);

    for (int depth = 0; depth < INTERLEAVE_SEARCH_DEPTH; ++depth) {
      vector<InterleaveState> next;
      next.reserve(INTERLEAVE_BEAM_WIDTH * 4);
      unordered_set<uint64_t> seen;
      seen.reserve(INTERLEAVE_BEAM_WIDTH * 8);

      for (const auto &state : beam) {
        // 1) active色branch。親盤面を1回だけcopyして複数actionをcount-only実行する。
        {
          Board scratch = state.board;
          ColorCounts scratch_counts = state.counts;
          uint64_t child_hash = state.hash;
          int delta_cost = 0;
          if (advance_one_count_only_fast(scratch, delta_cost, active_color, &scratch_counts, &child_hash) &&
              delta_cost > 0) {
            int physical_cost = state.physical_cost + delta_cost;
            int helper_streak = 0;
            int locked_helper = -1;
            int first_color = state.first_color < 0 ? active_color : state.first_color;
            int active_remaining = scratch_counts[active_color];
            long long cheap_score = interleave_cheap_score(scratch, physical_cost, state.helper_credit, active_color);

            if (cheap_score < LLONG_MAX / 8) {
              uint64_t state_key =
                  child_hash ^ ((uint64_t)(helper_streak + 1) << 57) ^ ((uint64_t)(locked_helper + 2) << 52);
              if (seen.insert(state_key).second) {
                InterleaveState child;
                child.board = scratch;
                child.counts = scratch_counts;
                child.hash = child_hash;
                child.first_color = first_color;
                child.physical_cost = physical_cost;
                child.helper_credit = state.helper_credit;
                child.helper_actions = state.helper_actions;
                child.helper_mask = state.helper_mask;
                child.helper_streak = helper_streak;
                child.locked_helper = locked_helper;
                child.active_remaining = active_remaining;
                child.cheap_score = cheap_score;
                next.push_back(std::move(child));
              }
            }
          }
        }

        // 2) helper色を1物理操作だけ前倒しするbranch。
        if (state.helper_streak < INTERLEAVE_MAX_HELPER_STREAK) {
          auto helpers = interleave_helper_candidates(state.board, state.counts, active_color, state.locked_helper);

          // helper候補間では同じscratch盤面を使い、source/destinationだけundoする。
          Board scratch = state.board;
          for (int helper : helpers) {
            auto move = generate_one_insert_action(state.board, active_color, helper);
            if (!move.has_value())
              continue;

            Tower old_source = scratch[move->source];
            Tower old_destination = scratch[move->destination];
            ColorCounts scratch_counts = state.counts;
            uint64_t child_hash = state.hash;
            int delta_cost = 0;
            bool ok = execute_action_count_only(scratch, delta_cost, move->source, move->moved_size, move->destination,
                                                &scratch_counts, &child_hash);

            if (ok && delta_cost > 0) {
              int physical_cost = state.physical_cost + delta_cost;
              int helper_credit = state.helper_credit + delta_cost;
              int helper_streak = state.helper_streak + 1;
              int locked_helper = helper;
              int first_color = state.first_color < 0 ? helper : state.first_color;
              int active_remaining = scratch_counts[active_color];
              long long cheap_score = interleave_cheap_score(scratch, physical_cost, helper_credit, active_color);

              if (cheap_score < LLONG_MAX / 8) {
                uint64_t state_key =
                    child_hash ^ ((uint64_t)(helper_streak + 1) << 57) ^ ((uint64_t)(locked_helper + 2) << 52);
                if (seen.insert(state_key).second) {
                  InterleaveState child;
                  child.board = scratch;
                  child.counts = scratch_counts;
                  child.hash = child_hash;
                  child.first_color = first_color;
                  child.physical_cost = physical_cost;
                  child.helper_credit = helper_credit;
                  child.helper_actions = state.helper_actions;
                  if (helper < (int)child.helper_actions.size()) {
                    int next_count = (int)child.helper_actions[helper] + delta_cost;
                    child.helper_actions[helper] = (uint8_t)min(255, next_count);
                  }
                  child.helper_mask = state.helper_mask;
                  if (helper < 16) {
                    child.helper_mask |= uint16_t(1u << helper);
                  }
                  child.helper_streak = helper_streak;
                  child.locked_helper = locked_helper;
                  child.active_remaining = active_remaining;
                  child.cheap_score = cheap_score;
                  next.push_back(std::move(child));
                }
              }
            }

            scratch[move->source] = old_source;
            scratch[move->destination] = old_destination;
          }
        }
      }

      if (next.empty())
        break;

      sort(next.begin(), next.end(), [&](const InterleaveState &a, const InterleaveState &b) {
        if (a.cheap_score != b.cheap_score) {
          return a.cheap_score < b.cheap_score;
        }
        bool a_active_first = (a.first_color == active_color);
        bool b_active_first = (b.first_color == active_color);
        if (a_active_first != b_active_first) {
          return a_active_first > b_active_first;
        }
        if (a.active_remaining != b.active_remaining) {
          return a.active_remaining < b.active_remaining;
        }
        if (a.physical_cost != b.physical_cost) {
          return a.physical_cost < b.physical_cost;
        }
        return a.hash < b.hash;
      });

      if ((int)next.size() > INTERLEAVE_BEAM_WIDTH) {
        next.resize(INTERLEAVE_BEAM_WIDTH);
      }

      for (const auto &state : next) {
        if (state.first_color >= 0)
          rollout_pool.push_back(state);
      }
      beam = std::move(next);
    }

    if (rollout_pool.empty())
      return active_color;

    sort(rollout_pool.begin(), rollout_pool.end(), [&](const InterleaveState &a, const InterleaveState &b) {
      if (a.cheap_score != b.cheap_score) {
        return a.cheap_score < b.cheap_score;
      }
      bool a_active_first = (a.first_color == active_color);
      bool b_active_first = (b.first_color == active_color);
      if (a_active_first != b_active_first)
        return a_active_first > b_active_first;
      if (a.physical_cost != b.physical_cost) {
        return a.physical_cost < b.physical_cost;
      }
      return a.hash < b.hash;
    });

    vector<InterleaveState> finalists;
    finalists.reserve(INTERLEAVE_FINAL_ROLLOUT_CANDIDATES);
    unordered_set<uint64_t> finalist_seen;
    finalist_seen.reserve(INTERLEAVE_FINAL_ROLLOUT_CANDIDATES * 2);
    for (const auto &state : rollout_pool) {
      uint64_t key = state.hash ^ (uint64_t)(state.first_color + 2) * 0x9e3779b97f4a7c15ULL;
      if (!finalist_seen.insert(key).second)
        continue;
      finalists.push_back(state);
      if ((int)finalists.size() >= INTERLEAVE_FINAL_ROLLOUT_CANDIDATES) {
        break;
      }
    }

    const uint64_t start_hash = initial.hash;
    long long best_score = LLONG_MAX;
    int best_first_color = active_color;

    for (const auto &state : finalists) {
      int active_finish = cached_finish_cost(state.board, state.counts, state.hash, active_color);
      if (active_finish >= INT_MAX / 8)
        continue;

      int exact_helper_credit = 0;
      for (int helper = 0; helper < P.K && helper < 16; ++helper) {
        if (((state.helper_mask >> helper) & 1u) == 0)
          continue;

        int fallback = state.helper_actions[helper];
        int before = cached_finish_cost(start_board, start_counts, start_hash, helper);
        int after = cached_finish_cost(state.board, state.counts, state.hash, helper);
        int gain = fallback;
        if (before < INT_MAX / 8 && after < INT_MAX / 8) {
          gain = max(gain, before - after);
        }
        exact_helper_credit += max(0, gain);
      }

      long long score = 1LL * state.physical_cost + active_finish - exact_helper_credit;

      if (score < best_score ||
          (score == best_score && state.first_color == active_color && best_first_color != active_color)) {
        best_score = score;
        best_first_color = state.first_color;
      }
    }

    return best_first_color;
  }

  bool solve_one_color(Board &board, vector<Action> &actions, ColorCounts &counts, int color) const {
    int helper_streak = 0;
    int locked_helper = -1;

    while (counts[color] > 0) {
      if ((int)actions.size() >= MAX_ACTIONS)
        return false;

      int chosen_color = choose_interleave_first_color(board, counts, color, locked_helper, helper_streak);

      if (chosen_color != color) {
        auto move = generate_one_insert_action(board, color, chosen_color);
        if (move.has_value() &&
            execute_action(board, actions, move->source, move->moved_size, move->destination, &counts)) {
          ++current_inserted_actions;
          ++helper_streak;
          locked_helper = chosen_color;
          continue;
        }
      }

      helper_streak = 0;
      locked_helper = -1;

      int source = deepest_active_cell(board, color);
      if (source == -1)
        return false;
      if (!advance_one(board, actions, color, source, &counts))
        return false;
    }

    return true;
  }

  // =========================================================
  // One color order
  // =========================================================

  SimulationResult simulate(const vector<int> &color_order) const {
    current_inserted_actions = 0;
    Board board = initial_board();
    ColorCounts counts = initial_color_counts();
    vector<Action> actions;
    actions.reserve(4096);

    for (int color : color_order) {
      if (!solve_one_color(board, actions, counts, color)) {
        return {false, {}};
      }
    }

    if (count_all(counts) != 0) {
      return {false, {}};
    }

    return {true, std::move(actions), current_inserted_actions};
  }

  // small colorsをcolor-mixedで全て帰巣させた盤面から、
  // 残ったlarge colorsだけ既存のcolor-fixed + Greedy Steinerで処理する。
  SimulationResult simulate_from_mixed_prefix(const vector<int> &color_order, const MixedPhaseResult &prefix) const {
    if (!prefix.success)
      return {false, {}};

    Board board = prefix.board;
    ColorCounts counts = count_colors(board);
    vector<Action> actions = prefix.actions;
    actions.reserve(4096);

    for (int color : color_order) {
      if (counts[color] == 0)
        continue;
      if (!solve_one_color(board, actions, counts, color)) {
        return {false, {}};
      }
    }

    if (count_all(counts) != 0)
      return {false, {}};
    return {true, std::move(actions)};
  }

  // =========================================================
  // Color order search
  // =========================================================

  uint64_t mixed_filtered_order_key(const vector<int> &order) const {
    // low 4bit: flush size。以降4bitずつ、mixed prefix後にも残る
    // non-small colorの相対順を詰める。K<=12なので64bitに収まる。
    uint64_t key = (uint64_t)strategy_flush_size;
    int shift = 4;
    for (int color : order) {
      if (is_small_color(color))
        continue;
      if (terminals[color].empty())
        continue;
      key |= (uint64_t)(color + 1) << shift;
      shift += 4;
    }
    return key;
  }

  const SimulationResult &cached_mixed_suffix_result(const vector<int> &order, const MixedPhaseResult &prefix) const {
    uint64_t key = mixed_filtered_order_key(order);
    auto it = mixed_suffix_cache.find(key);
    if (it != mixed_suffix_cache.end())
      return it->second;

    // メモリ上限だけ設ける。clearは再計算が増えるだけで結果は変えない。
    if (mixed_suffix_cache.size() > 4096)
      mixed_suffix_cache.clear();

    vector<int> filtered;
    filtered.reserve(P.K);
    for (int color : order) {
      if (is_small_color(color))
        continue;
      if (terminals[color].empty())
        continue;
      filtered.push_back(color);
    }

    auto result = simulate_from_mixed_prefix(filtered, prefix);
    auto [inserted, _] = mixed_suffix_cache.emplace(key, std::move(result));
    return inserted->second;
  }

  void try_order(const vector<int> &order, int flush_size, const MixedPhaseResult &mixed_prefix, set<vector<int>> &seen,
                 long long &trial_count, long long &mixed_win_count, SimulationResult &current_strategy_best,
                 SimulationResult &mixed_strategy_best, SimulationResult &strategy_best,
                 SimulationResult &global_best) {
    if (!seen.insert(order).second)
      return;
    ++trial_count;

    strategy_flush_size = flush_size;
    borrow_neighbor_enabled = false;
    insert_neighbor_enabled = false;

    // 1) 現在の color-fixed + interleave 戦略。
    interleave_neighbor_enabled = true;
    auto current = simulate(order);

    if (current.success) {
      if (!current_strategy_best.success || current.actions.size() < current_strategy_best.actions.size()) {
        current_strategy_best = current;
      }
      if (!strategy_best.success || current.actions.size() < strategy_best.actions.size()) {
        strategy_best = current;
      }
      if (!global_best.success || current.actions.size() < global_best.actions.size()) {
        global_best = current;
      }
    }

    // 2) この時点の color_mixed 戦略。
    // small colors は mixed prefix で処理し、残りは従来の pure color-fixed。
    // interleave は明示的に切って、mixed 戦略側へ混ぜない。
    const SimulationResult *mixed = nullptr;
    if (mixed_prefix.success) {
      interleave_neighbor_enabled = false;
      mixed = &cached_mixed_suffix_result(order, mixed_prefix);

      if (mixed->success) {
        if (!mixed_strategy_best.success || mixed->actions.size() < mixed_strategy_best.actions.size()) {
          mixed_strategy_best = *mixed;
        }
        if (!strategy_best.success || mixed->actions.size() < strategy_best.actions.size()) {
          strategy_best = *mixed;
        }
        if (!global_best.success || mixed->actions.size() < global_best.actions.size()) {
          global_best = *mixed;
        }
      }
    }

    if (mixed != nullptr && mixed->success && (!current.success || mixed->actions.size() < current.actions.size())) {
      ++mixed_win_count;
    }

    // 次の trial は current 戦略から始めるので戻しておく。
    interleave_neighbor_enabled = true;
  }

  long long permutation_count_cap(int n, long long cap) const {
    long long result = 1;
    for (int i = 2; i <= n; ++i) {
      if (result > cap / i)
        return cap;
      result *= i;
    }
    return result;
  }

  vector<Action> solve() {
    constexpr array<int, 4> FLUSH_SIZES = {7, 6, 5, 4};

    SimulationResult best;
    array<SimulationResult, 4> strategy_best;
    array<SimulationResult, 4> current_strategy_best;
    array<SimulationResult, 4> mixed_strategy_best;
    array<set<vector<int>>, 4> seen;
    array<long long, 4> trial_count{};
    array<long long, 4> mixed_win_count{};

    // color_mixed 側は flush size / color order に依存しない small-color prefix を
    // 一度だけ構築する。失敗した場合も current 戦略はそのまま続行する。
    MixedPhaseResult mixed_prefix = build_small_mixed_prefix();
    cerr << "small mixed prefix: ";
    if (mixed_prefix.success) {
      cerr << mixed_prefix.actions.size() << " actions";
    } else {
      cerr << "failed";
    }
    cerr << ", normal=" << mixed_prefix.normal_moves << ", recovery=" << mixed_prefix.recovery_moves
         << ", recovery_calls=" << mixed_prefix.recovery_calls << '\n';

    vector<vector<int>> seed_orders;

    vector<int> order(P.K);
    iota(order.begin(), order.end(), 0);
    seed_orders.push_back(order);

    reverse(order.begin(), order.end());
    seed_orders.push_back(order);

    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int a, int b) {
      if (terminals[a].size() != terminals[b].size()) {
        return terminals[a].size() > terminals[b].size();
      }
      return a < b;
    });
    seed_orders.push_back(order);

    reverse(order.begin(), order.end());
    seed_orders.push_back(order);

    // まず4つの flush size を最低1回ずつ試す。
    for (int i = 0; i < 4; ++i) {
      try_order(seed_orders[0], FLUSH_SIZES[i], mixed_prefix, seen[i], trial_count[i], mixed_win_count[i],
                current_strategy_best[i], mixed_strategy_best[i], strategy_best[i], best);
    }

    // 代表的な色順も4戦略で公平に試す。
    for (int seed = 1; seed < (int)seed_orders.size(); ++seed) {
      for (int i = 0; i < 4; ++i) {
        if (time_over())
          break;
        try_order(seed_orders[seed], FLUSH_SIZES[i], mixed_prefix, seen[i], trial_count[i], mixed_win_count[i],
                  current_strategy_best[i], mixed_strategy_best[i], strategy_best[i], best);
      }
      if (time_over())
        break;
    }

    if (P.K >= 2) {
      const long long all_orders = permutation_count_cap(P.K, 1000000000LL);

      int strategy_index = 0;
      while (!time_over()) {
        bool all_finished = true;
        for (int i = 0; i < 4; ++i) {
          if ((long long)seen[i].size() < all_orders) {
            all_finished = false;
            break;
          }
        }
        if (all_finished)
          break;

        int i = strategy_index;
        strategy_index = (strategy_index + 1) % 4;
        if ((long long)seen[i].size() == all_orders)
          continue;

        iota(order.begin(), order.end(), 0);
        shuffle(order.begin(), order.end(), mt);

        try_order(order, FLUSH_SIZES[i], mixed_prefix, seen[i], trial_count[i], mixed_win_count[i],
                  current_strategy_best[i], mixed_strategy_best[i], strategy_best[i], best);
      }
    }

    long long total_trials = 0;
    for (int i = 0; i < 4; ++i) {
      total_trials += trial_count[i];
      cerr << "fill8/flush" << FLUSH_SIZES[i] << ": best=";
      if (strategy_best[i].success) {
        cerr << strategy_best[i].actions.size();
      } else {
        cerr << "none";
      }

      cerr << ", current=";
      if (current_strategy_best[i].success) {
        cerr << current_strategy_best[i].actions.size() << "(insert=" << current_strategy_best[i].inserted_actions
             << ')';
      } else {
        cerr << "none";
      }

      cerr << ", mixed=";
      if (mixed_strategy_best[i].success) {
        cerr << mixed_strategy_best[i].actions.size();
      } else {
        cerr << "none";
      }

      cerr << ", trials=" << trial_count[i] << ", mixed_wins=" << mixed_win_count[i] << '\n';
    }
    cerr << "total trials=" << total_trials << '\n';

    if (!best.success) {
      // 念のため時間制限とは無関係に、flush4 + 基本色順を両戦略で1回ずつ試す。
      strategy_flush_size = 4;
      borrow_neighbor_enabled = false;
      insert_neighbor_enabled = false;

      interleave_neighbor_enabled = true;
      auto current = simulate(seed_orders[0]);
      if (current.success)
        best = std::move(current);

      if (mixed_prefix.success) {
        interleave_neighbor_enabled = false;
        auto mixed = simulate_from_mixed_prefix(seed_orders[0], mixed_prefix);
        if (mixed.success && (!best.success || mixed.actions.size() < best.actions.size())) {
          best = std::move(mixed);
        }
      }
    }

    return best.actions;
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  Solver solver;
  auto answer = solver.solve();

  for (const auto &action : answer) {
    cout << action << '\n';
  }

  return 0;
}
