#include <bits/stdc++.h>
using namespace std;

// AHC072 experimental solver:
// 1) build one complete solution by a transport-gain-only greedy policy,
// 2) extract event-backdating proposals from the completed history,
// 3) sample those proposals in random order,
// 4) repair a sampled historical edit with a small local beam,
// 5) accept/reject the resulting complete solution by simulated annealing.
//
// Neutral and mildly worse complete histories may therefore be accepted.
// The best complete history seen during the walk is returned.
//
// No Steiner tree / color-fixed / mixed solver / profit graph.

constexpr int MAX_N = 20;
constexpr int MAX_CELLS = MAX_N * MAX_N;
constexpr int MAX_HEIGHT = 8;
constexpr int MAX_ACTIONS = 100000;
constexpr int TIME_LIMIT_MS = 1900;
constexpr int OFFLINE_TIME_GUARD_MS = 1870;

constexpr int OFFLINE_CHEAP_KEEP = 8000;
constexpr int OFFLINE_EXACT_LIMIT = 96;
constexpr int OFFLINE_MAX_ITERS = 32;
constexpr int RECENT_HASH_LIMIT = 32;

constexpr int LOCAL_BEAM_WIDTH = 48;
constexpr int LOCAL_MAX_DEPTH = 36;
constexpr int LOCAL_GLOBAL_PREBEAM = 512;
constexpr int REPAIR_INF = 1e9;

// Random historical-event SA repair.
//
// The normal neighborhood is now a direct historical edit:
//   event producer -> insert time,
//   replay only the affected history,
//   strengthen the historical use,
//   delete/reroute operations that became redundant,
//   stop as soon as the original history is reached again.
//
// Only a small fraction of failed direct proposals fall back to the old
// local-beam repair.  This is meant to raise the SA trial count by roughly
// one order of magnitude without removing the occasional deep repair.
constexpr int SA_BEAM_WIDTH = 8;
constexpr int SA_GLOBAL_PREBEAM = 72;
constexpr int SA_MAX_DEPTH = 22;
constexpr int SA_MAX_UPHILL = 3;

constexpr int SA_DEEP_BEAM_WIDTH = 28;
constexpr int SA_DEEP_GLOBAL_PREBEAM = 256;
constexpr int SA_DEEP_MAX_DEPTH = 36;

// Percentage of *failed direct proposals* that are rescued by a beam.
constexpr int SA_BEAM_RESCUE_PERCENT = 4;
// Of those beam rescues, percentage that use the deeper configuration.
constexpr int SA_RESCUE_DEEP_PERCENT = 20;

// A direct mutation may keep propagating after the original producer for this
// many historical operations while looking for an exact rejoin.
constexpr int SA_DIRECT_REJOIN_EXTRA = 48;

constexpr double SA_TEMP_START = 2.2;
constexpr double SA_TEMP_END = 0.08;

constexpr array<int, 4> DX = {-1, 1, 0, 0};
constexpr array<int, 4> DY = {0, 0, -1, 1};
constexpr array<char, 4> DIR_CHAR = {'U', 'D', 'L', 'R'};

struct Slime {
  uint16_t id = 0;
  uint8_t color = 0;
  bool operator==(const Slime &) const = default;
};

struct Tower {
  array<Slime, MAX_HEIGHT> a{};
  uint8_t n = 0;

  int size() const {
    return n;
  }
  bool empty() const {
    return n == 0;
  }
  Slime &operator[](int i) {
    return a[i];
  }
  const Slime &operator[](int i) const {
    return a[i];
  }
  Slime &back() {
    return a[n - 1];
  }
  const Slime &back() const {
    return a[n - 1];
  }
  void push_back(Slime s) {
    a[n++] = s;
  }
  void pop_back() {
    --n;
  }

  bool operator==(const Tower &rhs) const {
    if (n != rhs.n)
      return false;
    for (int i = 0; i < n; ++i) {
      if (!(a[i] == rhs.a[i]))
        return false;
    }
    return true;
  }
};

using Board = array<Tower, MAX_CELLS>;

struct MoveSpec {
  int source = -1;
  int destination = -1;
  int moved_size = 0;

  bool operator==(const MoveSpec &) const = default;
};

struct OutputAction {
  int x = 0;
  int y = 0;
  int k = 0;
  int d = 0;
  int l = 0;
};

struct AppliedMove {
  bool ok = false;
  long long gain = LLONG_MIN;
  OutputAction out{};
  array<uint16_t, MAX_HEIGHT> moved_ids{};
  uint8_t moved_count = 0;
};

struct HistoryEntry {
  MoveSpec spec{};
  OutputAction out{};
  long long gain = 0;
  array<uint16_t, MAX_HEIGHT> moved_ids{}; // bottom -> top before the move
  uint8_t moved_count = 0;
};

struct Trace {
  vector<HistoryEntry> history;
  vector<Board> snapshot; // snapshot[t] = board before history[t]
  vector<uint64_t> snapshot_hash;
  vector<uint64_t> snapshot_hash2;
  Board final_board{};
  bool valid = false;
};

struct CandidateMove {
  MoveSpec spec{};
  long long gain = LLONG_MIN;
};

enum class EventType : uint8_t {
  SupportReady,
  BlockReady,
  CapacityReady,
};

struct HistoricalCandidate {
  EventType type = EventType::SupportReady;

  // A future operation producer is moved earlier to make an event available
  // before/use at the earlier operation `use`.
  int producer = -1;
  int use = -1;
  int insert = -1;
  int event_cell = -1;

  // Cheap optimistic estimates used only for candidate filtering.
  // single_gain_delta: gain of the one historical use that generated this
  // candidate. aggregate_gain_delta: sum of all compatible historical uses
  // exposed by backdating the event to insert.
  long long cheap_gain_delta = 0; // kept as aggregate score for logging/ties
  long long single_gain_delta = 0;
  long long aggregate_gain_delta = 0;
  int cheap_strength = 0;
  int cheap_use_count = 1;
  uint8_t selection_bucket = 0; // 1=single, 2=multi, 3=cost, 4=fill
};

struct ExactCandidateResult {
  bool valid = false;
  int action_count = INT_MAX;
  int rejoin_time = INT_MAX;
  vector<MoveSpec> specs;
};

struct Hash128 {
  uint64_t a = 0;
  uint64_t b = 0;

  bool operator==(const Hash128 &) const = default;
};

struct Hash128Hasher {
  size_t operator()(const Hash128 &h) const {
    uint64_t x = h.a ^ std::rotl(h.b, 23);
    x ^= x >> 33;
    x *= 0xff51afd7ed558ccdULL;
    x ^= x >> 33;
    return (size_t)x;
  }
};

struct LocalBeamState {
  Board board{};
  array<MoveSpec, LOCAL_MAX_DEPTH> path{};
  uint8_t path_len = 0;
  uint64_t board_hash = 0;
  uint64_t board_hash2 = 0;
  uint64_t prev_board_hash = 0;
  uint64_t prev_board_hash2 = 0;
  long long heuristic = 0;
  long long score = 0;
};

struct RepairMoveCandidate {
  MoveSpec spec{};
  long long rough = LLONG_MIN;
};

struct LocalExpansionCandidate {
  int parent_index = -1;
  MoveSpec spec{};
  Tower source_after{};
  Tower destination_after{};
  uint64_t board_hash = 0;
  uint64_t board_hash2 = 0;
  long long heuristic = 0;
  long long score = 0;
  long long rough = LLONG_MIN;
};

struct RepairTarget {
  int time = -1;
  vector<int16_t> cell_of_id;
};

struct CausalCone {
  array<uint8_t, MAX_CELLS> mask{};
  vector<int> cells;
  vector<uint8_t> operation_mask;
  vector<int> operations;
  vector<uint16_t> slime_ids;
};

struct CandidateHistoryIndex {
  vector<int> prev_touch_source;
  vector<int> prev_touch_destination;
  vector<int> support_insert;
};

struct PotentialTimeline {
  vector<int> times;
  vector<long long> prefix;
};

struct EventPotentialIndex {
  vector<PotentialTimeline> support;
  vector<PotentialTimeline> block;
  vector<PotentialTimeline> capacity;
};

struct TraceDependencyIndex {
  // Immediate temporal dependencies. Each operation reads the source and
  // destination towers produced by their last writers. Edges therefore form
  // a time-directed DAG instead of an all-shared-cell/all-shared-ID graph.
  vector<array<int, 2>> prev_writer;
  vector<vector<int>> next_dependents;
  vector<vector<int>> ops_by_cell;
};

struct OfflineStats {
  long long generated = 0;
  long long exact_tried = 0;
  long long beam_states = 0;
  long long beam_raw_candidates = 0;
  long long beam_children = 0;
  long long undo_rejects = 0;
  long long transposition_rejects = 0;
  long long rejoin_hits = 0;
  long long causal_cones = 0;
  long long causal_cells = 0;
  long long causal_operations = 0;
  long long causal_ids = 0;
  long long background_actions = 0;
  long long rejoin_materializations = 0;
  long long dynamic_frontier_calls = 0;
  long long dynamic_dirty_cells = 0;
  long long dynamic_source_cells = 0;
  long long dynamic_helper_cells = 0;

  long long direct_trials = 0;
  long long direct_valid = 0;
  long long direct_steps = 0;
  long long direct_reroutes = 0;
  long long direct_use_fail = 0;
  long long direct_event_fail = 0;
  long long direct_rejoin_fail = 0;
  long long beam_rescue_trials = 0;
  long long beam_rescue_valid = 0;
};

struct Solver {
  int N = 0, K = 0, V = 0;
  vector<string> C;
  Board initial_board{};
  array<int, 12> nest{};
  array<int, MAX_CELLS> nest_color{};
  vector<array<int, MAX_CELLS>> dist_to_nest;
  vector<array<uint16_t, MAX_CELLS>> floor_dist;
  // Static floor-only rays used by the local repair generator.
  // ray_dest[v][d][l] is the l-th cell on the ray, or -1 after wall/boundary.
  array<array<array<int16_t, MAX_HEIGHT + 1>, 4>, MAX_CELLS> ray_dest{};
  vector<uint8_t> color_of_id;
  mutable OfflineStats stats;
  mutable uint64_t rng_state = 0x8b8b8b8b1234567dULL;
  chrono::steady_clock::time_point start_time;

  Solver() : start_time(chrono::steady_clock::now()) {
    cin >> N >> K;
    V = N * N;
    C.resize(N);
    for (auto &row : C)
      cin >> row;

    nest.fill(-1);
    nest_color.fill(-1);
    color_of_id.assign(MAX_CELLS + 1, 0);

    uint16_t next_id = 1;
    for (int x = 0; x < N; ++x) {
      for (int y = 0; y < N; ++y) {
        char ch = C[x][y];
        int v = id(x, y);
        if ('A' <= ch && ch <= 'L') {
          int color = ch - 'A';
          nest[color] = v;
          nest_color[v] = color;
        } else if ('a' <= ch && ch <= 'l') {
          int color = ch - 'a';
          initial_board[v].push_back(Slime{next_id, (uint8_t)color});
          if ((int)color_of_id.size() <= next_id) {
            color_of_id.resize(next_id + 1);
          }
          color_of_id[next_id] = (uint8_t)color;
          ++next_id;
        }
      }
    }
    color_of_id.resize(next_id);
    build_distances();
    build_all_pairs_distances();
    build_static_rays();

    // Deterministic per-instance seed.
    uint64_t seed = 0x243f6a8885a308d3ULL ^ ((uint64_t)N << 32) ^ (uint64_t)K;
    for (const auto &row : C) {
      for (unsigned char ch : row) {
        seed = splitmix64(seed ^ ch);
      }
    }
    rng_state ^= seed;
    if (rng_state == 0)
      rng_state = 0x9e3779b97f4a7c15ULL;

    start_time = chrono::steady_clock::now();
  }

  int id(int x, int y) const {
    return x * N + y;
  }
  int x_of(int v) const {
    return v / N;
  }
  int y_of(int v) const {
    return v % N;
  }

  bool in_bounds(int x, int y) const {
    return 0 <= x && x < N && 0 <= y && y < N;
  }

  bool floor_cell(int v) const {
    return C[x_of(v)][y_of(v)] != '#';
  }

  long long elapsed_ms() const {
    return chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start_time).count();
  }

  bool time_over() const {
    return elapsed_ms() >= TIME_LIMIT_MS;
  }
  bool offline_time_over() const {
    return elapsed_ms() >= OFFLINE_TIME_GUARD_MS;
  }

  static uint64_t splitmix64(uint64_t x) {
    x += 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    return x ^ (x >> 31);
  }

  uint64_t next_random_u64() const {
    // xorshift64*
    uint64_t x = rng_state;
    x ^= x >> 12;
    x ^= x << 25;
    x ^= x >> 27;
    rng_state = x;
    return x * 2685821657736338717ULL;
  }

  int random_int(int n) const {
    if (n <= 1)
      return 0;
    return (int)(next_random_u64() % (uint64_t)n);
  }

  double random01() const {
    return (next_random_u64() >> 11) * (1.0 / 9007199254740992.0);
  }

  uint64_t hash_board(const Board &b) const {
    uint64_t h = splitmix64((uint64_t)N * 1009 + K);
    for (int v = 0; v < V; ++v) {
      uint64_t z = splitmix64((uint64_t)v + 0x123456789abcdefULL);
      z ^= splitmix64((uint64_t)b[v].size() * 0x9e3779b9ULL + v);
      for (int i = 0; i < b[v].size(); ++i) {
        uint64_t key = ((uint64_t)b[v][i].id << 16) | ((uint64_t)b[v][i].color << 8) | (uint64_t)i;
        z ^= splitmix64(key + (uint64_t)v * 0xbf58476d1ce4e5b9ULL);
      }
      h ^= z;
    }
    return h;
  }

  uint64_t hash_board2(const Board &b) const {
    uint64_t h = splitmix64((uint64_t)N * 0x94d049bb133111ebULL + K + 17);
    for (int v = 0; v < V; ++v) {
      uint64_t z = splitmix64((uint64_t)v + 0xd6e8feb86659fd93ULL);
      z ^= splitmix64((uint64_t)b[v].size() * 0xa0761d6478bd642fULL + 3ULL * v);
      for (int i = 0; i < b[v].size(); ++i) {
        uint64_t key = ((uint64_t)b[v][i].id << 20) | ((uint64_t)b[v][i].color << 12) | (uint64_t)i;
        z ^= splitmix64(key + (uint64_t)v * 0xe7037ed1a0b428dbULL);
      }
      h ^= std::rotl(z, (v % 61) + 1);
    }
    return h;
  }

  void build_static_rays() {
    for (int v = 0; v < MAX_CELLS; ++v) {
      for (int d = 0; d < 4; ++d)
        ray_dest[v][d].fill(-1);
    }
    for (int v = 0; v < V; ++v) {
      if (!floor_cell(v))
        continue;
      int x = x_of(v), y = y_of(v);
      for (int d = 0; d < 4; ++d) {
        int nx = x, ny = y;
        for (int l = 1; l <= MAX_HEIGHT; ++l) {
          nx += DX[d];
          ny += DY[d];
          if (!in_bounds(nx, ny))
            break;
          int nv = id(nx, ny);
          if (!floor_cell(nv))
            break;
          ray_dest[v][d][l] = (int16_t)nv;
        }
      }
    }
  }

  void build_distances() {
    dist_to_nest.assign(K, {});
    for (int color = 0; color < K; ++color) {
      auto &dist = dist_to_nest[color];
      dist.fill(INT_MAX);
      if (nest[color] < 0)
        continue;

      queue<int> q;
      dist[nest[color]] = 0;
      q.push(nest[color]);

      while (!q.empty()) {
        int v = q.front();
        q.pop();
        int x = x_of(v), y = y_of(v);

        for (int d = 0; d < 4; ++d) {
          int nx = x + DX[d], ny = y + DY[d];
          if (!in_bounds(nx, ny))
            continue;
          int nv = id(nx, ny);
          if (!floor_cell(nv) || dist[nv] != INT_MAX)
            continue;
          dist[nv] = dist[v] + 1;
          q.push(nv);
        }
      }
    }
  }

  void build_all_pairs_distances() {
    constexpr uint16_t INF16 = numeric_limits<uint16_t>::max();
    floor_dist.assign(V, {});

    for (int source = 0; source < V; ++source) {
      floor_dist[source].fill(INF16);
      if (!floor_cell(source))
        continue;

      queue<int> q;
      floor_dist[source][source] = 0;
      q.push(source);

      while (!q.empty()) {
        int v = q.front();
        q.pop();
        int x = x_of(v), y = y_of(v);
        for (int d = 0; d < 4; ++d) {
          int nx = x + DX[d], ny = y + DY[d];
          if (!in_bounds(nx, ny))
            continue;
          int nv = id(nx, ny);
          if (!floor_cell(nv))
            continue;
          if (floor_dist[source][nv] != INF16)
            continue;
          floor_dist[source][nv] = floor_dist[source][v] + 1;
          q.push(nv);
        }
      }
    }
  }

  int direction_between(int s, int t) const {
    int sx = x_of(s), sy = y_of(s), tx = x_of(t), ty = y_of(t);
    if (sy == ty) {
      if (tx < sx)
        return 0;
      if (tx > sx)
        return 1;
    }
    if (sx == tx) {
      if (ty < sy)
        return 2;
      if (ty > sy)
        return 3;
    }
    return -1;
  }

  int line_length(int s, int t) const {
    if (direction_between(s, t) < 0)
      return -1;
    return abs(x_of(s) - x_of(t)) + abs(y_of(s) - y_of(t));
  }

  bool line_floor_clear(int s, int t) const {
    int d = direction_between(s, t);
    if (d < 0)
      return false;

    int len = line_length(s, t);
    int x = x_of(s), y = y_of(s);
    for (int l = 1; l <= len; ++l) {
      x += DX[d];
      y += DY[d];
      if (!in_bounds(x, y))
        return false;
      if (!floor_cell(id(x, y)))
        return false;
    }
    return true;
  }

  void home_tower(Tower &tower, int v) const {
    int c = nest_color[v];
    if (c < 0)
      return;
    while (!tower.empty() && tower.back().color == c) {
      tower.pop_back();
    }
  }

  long long transport_gain(const Board &b, const MoveSpec &m) const {
    if (m.source < 0 || m.destination < 0 || m.moved_size <= 0) {
      return LLONG_MIN / 4;
    }
    const Tower &src = b[m.source];
    if (m.moved_size > src.size())
      return LLONG_MIN / 4;

    long long gain = 0;
    for (int i = src.size() - m.moved_size; i < src.size(); ++i) {
      int c = src[i].color;
      int ds = dist_to_nest[c][m.source];
      int dt = dist_to_nest[c][m.destination];
      if (ds == INT_MAX || dt == INT_MAX)
        return LLONG_MIN / 4;
      gain += ds - dt;
    }
    return gain;
  }

  bool legal_move(const Board &b, const MoveSpec &m) const {
    if (m.source < 0 || m.source >= V || m.destination < 0 || m.destination >= V) {
      return false;
    }
    if (m.source == m.destination || m.moved_size <= 0)
      return false;
    if (!floor_cell(m.source) || !floor_cell(m.destination))
      return false;

    const Tower &src = b[m.source];
    const Tower &dst = b[m.destination];
    int h = src.size();

    if (m.moved_size > h)
      return false;
    if (dst.size() + m.moved_size > MAX_HEIGHT)
      return false;

    int d = direction_between(m.source, m.destination);
    if (d < 0)
      return false;

    int l = line_length(m.source, m.destination);
    if (l <= 0 || !line_floor_clear(m.source, m.destination))
      return false;

    int k = h - m.moved_size;
    if (!(0 <= k && k < h))
      return false;
    return l <= k + 1;
  }

  AppliedMove apply_move(Board &b, const MoveSpec &m) const {
    AppliedMove result;
    if (!legal_move(b, m))
      return result;

    int h = b[m.source].size();
    int k = h - m.moved_size;
    int d = direction_between(m.source, m.destination);
    int l = line_length(m.source, m.destination);

    result.ok = true;
    result.gain = transport_gain(b, m);
    result.out = OutputAction{x_of(m.source), y_of(m.source), k, d, l};
    result.moved_count = (uint8_t)m.moved_size;

    for (int i = 0; i < m.moved_size; ++i) {
      result.moved_ids[i] = b[m.source][h - m.moved_size + i].id;
    }

    for (int i = 0; i < m.moved_size; ++i) {
      Slime slime = b[m.source].back();
      b[m.source].pop_back();
      b[m.destination].push_back(slime);
    }

    home_tower(b[m.source], m.source);
    home_tower(b[m.destination], m.destination);
    return result;
  }

  bool simulate_move_towers(const Board &b, const MoveSpec &m, Tower &source_after, Tower &destination_after) const {
    if (!legal_move(b, m))
      return false;

    source_after = b[m.source];
    destination_after = b[m.destination];

    for (int i = 0; i < m.moved_size; ++i) {
      Slime slime = source_after.back();
      source_after.pop_back();
      destination_after.push_back(slime);
    }

    home_tower(source_after, m.source);
    home_tower(destination_after, m.destination);
    return true;
  }

  int total_slimes(const Board &b) const {
    int result = 0;
    for (int v = 0; v < V; ++v)
      result += b[v].size();
    return result;
  }

  bool top_ids_exact(const Board &b, int source, const array<uint16_t, MAX_HEIGHT> &ids, int count) const {
    if (source < 0 || source >= V || count <= 0)
      return false;
    const Tower &t = b[source];
    if (t.size() < count)
      return false;
    int first = t.size() - count;
    for (int i = 0; i < count; ++i) {
      if (t[first + i].id != ids[i])
        return false;
    }
    return true;
  }

  bool top_ids_same_set(const Tower &tower, const array<uint16_t, MAX_HEIGHT> &ids, int count) const {
    if (count <= 0 || count > MAX_HEIGHT || tower.size() < count) {
      return false;
    }

    int first = tower.size() - count;
    array<uint8_t, MAX_HEIGHT> used{};

    for (int i = 0; i < count; ++i) {
      uint16_t target = ids[i];
      bool found = false;
      for (int j = 0; j < count; ++j) {
        if (used[j])
          continue;
        if (tower[first + j].id != target)
          continue;
        used[j] = 1;
        found = true;
        break;
      }
      if (!found)
        return false;
    }
    return true;
  }

  bool segment_contains_all_ids(const Tower &tower, int moved_size, const array<uint16_t, MAX_HEIGHT> &ids,
                                int count) const {
    if (moved_size > tower.size() || count > moved_size)
      return false;
    int first = tower.size() - moved_size;

    for (int j = 0; j < count; ++j) {
      bool found = false;
      for (int i = first; i < tower.size(); ++i) {
        if (tower[i].id == ids[j]) {
          found = true;
          break;
        }
      }
      if (!found)
        return false;
    }
    return true;
  }

  bool all_ids_color(const array<uint16_t, MAX_HEIGHT> &ids, int count, int color) const {
    for (int i = 0; i < count; ++i) {
      if (ids[i] == 0 || ids[i] >= color_of_id.size())
        return false;
      if (color_of_id[ids[i]] != color)
        return false;
    }
    return true;
  }

  int segment_color(const array<uint16_t, MAX_HEIGHT> &ids, int count) const {
    if (count <= 0 || ids[0] == 0 || ids[0] >= color_of_id.size())
      return -1;
    int color = color_of_id[ids[0]];
    return all_ids_color(ids, count, color) ? color : -1;
  }

  vector<CandidateMove> enumerate_legal_moves(const Board &b) const {
    vector<CandidateMove> result;
    result.reserve(4096);

    for (int source = 0; source < V; ++source) {
      int h = b[source].size();
      if (h == 0)
        continue;

      int sx = x_of(source), sy = y_of(source);
      for (int moved = 1; moved <= h; ++moved) {
        int max_jump = h - moved + 1;

        for (int d = 0; d < 4; ++d) {
          int x = sx, y = sy;
          for (int l = 1; l <= max_jump; ++l) {
            x += DX[d];
            y += DY[d];
            if (!in_bounds(x, y))
              break;

            int destination = id(x, y);
            if (!floor_cell(destination))
              break;
            if (b[destination].size() + moved > MAX_HEIGHT)
              continue;

            MoveSpec m{source, destination, moved};
            result.push_back({m, transport_gain(b, m)});
          }
        }
      }
    }

    sort(result.begin(), result.end(), [](const CandidateMove &a, const CandidateMove &b) {
      if (a.gain != b.gain)
        return a.gain > b.gain;
      if (a.spec.source != b.spec.source) {
        return a.spec.source < b.spec.source;
      }
      if (a.spec.destination != b.spec.destination) {
        return a.spec.destination < b.spec.destination;
      }
      return a.spec.moved_size > b.spec.moved_size;
    });
    return result;
  }

  optional<CandidateMove> choose_greedy_move(const Board &b, const deque<uint64_t> &recent_hashes) const {
    auto moves = enumerate_legal_moves(b);
    if (moves.empty())
      return nullopt;

    int probe = min<int>(64, moves.size());
    for (int i = 0; i < probe; ++i) {
      Board child = b;
      if (!apply_move(child, moves[i].spec).ok)
        continue;
      uint64_t h = hash_board(child);
      if (find(recent_hashes.begin(), recent_hashes.end(), h) == recent_hashes.end()) {
        return moves[i];
      }
    }
    return moves.front();
  }

  Trace build_trace_from_specs(const vector<MoveSpec> &specs) const {
    Trace trace;
    trace.history.reserve(specs.size());
    trace.snapshot.reserve(specs.size() + 1);
    trace.snapshot_hash.reserve(specs.size() + 1);
    trace.snapshot_hash2.reserve(specs.size() + 1);

    Board b = initial_board;
    trace.snapshot.push_back(b);
    trace.snapshot_hash.push_back(hash_board(b));
    trace.snapshot_hash2.push_back(hash_board2(b));

    for (const auto &spec : specs) {
      AppliedMove applied = apply_move(b, spec);
      if (!applied.ok)
        return trace;

      HistoryEntry entry;
      entry.spec = spec;
      entry.out = applied.out;
      entry.gain = applied.gain;
      entry.moved_ids = applied.moved_ids;
      entry.moved_count = applied.moved_count;
      trace.history.push_back(entry);
      trace.snapshot.push_back(b);
      trace.snapshot_hash.push_back(hash_board(b));
      trace.snapshot_hash2.push_back(hash_board2(b));
    }

    trace.final_board = b;
    trace.valid = true;
    return trace;
  }

  Trace build_baseline_trace() const {
    Board b = initial_board;
    deque<uint64_t> recent_hashes;
    recent_hashes.push_back(hash_board(b));

    vector<MoveSpec> specs;
    specs.reserve(1024);

    for (int guard = 0; total_slimes(b) > 0 && (int)specs.size() < MAX_ACTIONS && !time_over() && guard < MAX_ACTIONS;
         ++guard) {
      auto best = choose_greedy_move(b, recent_hashes);
      if (!best.has_value())
        break;

      AppliedMove applied = apply_move(b, best->spec);
      if (!applied.ok)
        break;
      specs.push_back(best->spec);

      recent_hashes.push_back(hash_board(b));
      while ((int)recent_hashes.size() > RECENT_HASH_LIMIT) {
        recent_hashes.pop_front();
      }
    }

    return build_trace_from_specs(specs);
  }

  CandidateHistoryIndex build_candidate_history_index(const Trace &trace) const {
    const int T = trace.history.size();
    CandidateHistoryIndex index;
    index.prev_touch_source.assign(T, -1);
    index.prev_touch_destination.assign(T, -1);
    index.support_insert.assign(T, -1);

    array<int, MAX_CELLS> last_touch{};
    last_touch.fill(-1);
    vector<int> last_arrival(color_of_id.size(), -1);

    for (int t = 0; t < T; ++t) {
      const auto &e = trace.history[t];
      index.prev_touch_source[t] = last_touch[e.spec.source];
      index.prev_touch_destination[t] = last_touch[e.spec.destination];

      int insert = t;
      for (int i = 0; i < e.moved_count; ++i) {
        uint16_t sid = e.moved_ids[i];
        if (sid >= last_arrival.size() || last_arrival[sid] < 0) {
          insert = -1;
          break;
        }
        insert = min(insert, last_arrival[sid]);
      }
      index.support_insert[t] = insert;

      last_touch[e.spec.source] = t;
      last_touch[e.spec.destination] = t;
      for (int i = 0; i < e.moved_count; ++i) {
        uint16_t sid = e.moved_ids[i];
        if (sid < last_arrival.size())
          last_arrival[sid] = t;
      }
    }
    return index;
  }

  long long gain_of_top_segment_to(const Board &b, int source, int moved, int destination) const {
    MoveSpec m{source, destination, moved};
    return transport_gain(b, m);
  }

  long long optimistic_support_gain(const Trace &trace, int use, int added_base) const {
    const auto &e = trace.history[use];
    const Board &b = trace.snapshot[use];

    int source = e.spec.source;
    int moved = e.spec.moved_size;
    int h = b[source].size();
    if (h + added_base > MAX_HEIGHT)
      return LLONG_MIN / 4;

    int base = h - moved;
    int max_jump = base + added_base + 1;
    long long best = e.gain;

    int sx = x_of(source), sy = y_of(source);
    for (int d = 0; d < 4; ++d) {
      int x = sx, y = sy;
      for (int l = 1; l <= max_jump; ++l) {
        x += DX[d];
        y += DY[d];
        if (!in_bounds(x, y))
          break;
        int dst = id(x, y);
        if (!floor_cell(dst))
          break;
        if (b[dst].size() + moved > MAX_HEIGHT)
          continue;
        best = max(best, gain_of_top_segment_to(b, source, moved, dst));
      }
    }
    return best;
  }

  long long optimistic_block_gain(const Trace &trace, int use, int added_same_color) const {
    const auto &e = trace.history[use];
    const Board &b = trace.snapshot[use];
    int source = e.spec.source;

    int moved = e.spec.moved_size + added_same_color;
    int h = b[source].size() + added_same_color;
    if (moved > MAX_HEIGHT || h > MAX_HEIGHT)
      return LLONG_MIN / 4;

    int base = h - moved;
    int max_jump = base + 1;
    int color = segment_color(e.moved_ids, e.moved_count);
    if (color < 0)
      return LLONG_MIN / 4;

    long long best = e.gain;
    int sx = x_of(source), sy = y_of(source);

    for (int d = 0; d < 4; ++d) {
      int x = sx, y = sy;
      for (int l = 1; l <= max_jump; ++l) {
        x += DX[d];
        y += DY[d];
        if (!in_bounds(x, y))
          break;
        int dst = id(x, y);
        if (!floor_cell(dst))
          break;
        if (b[dst].size() + moved > MAX_HEIGHT)
          continue;

        int ds = dist_to_nest[color][source];
        int dt = dist_to_nest[color][dst];
        if (ds == INT_MAX || dt == INT_MAX)
          continue;

        long long gain = (long long)moved * (ds - dt);
        best = max(best, gain);
      }
    }
    return best;
  }

  long long optimistic_capacity_gain(const Trace &trace, int use, int freed_capacity) const {
    const auto &e = trace.history[use];
    const Board &b = trace.snapshot[use];
    int source = e.spec.source;
    int destination = e.spec.destination;

    int effective_dst_height = max(0, b[destination].size() - freed_capacity);
    long long best = e.gain;

    for (int moved = e.spec.moved_size + 1; moved <= b[source].size(); ++moved) {
      if (effective_dst_height + moved > MAX_HEIGHT)
        continue;

      int h = b[source].size();
      int max_jump = h - moved + 1;
      int len = line_length(source, destination);
      if (len <= 0 || len > max_jump)
        continue;

      best = max(best, transport_gain(b, MoveSpec{source, destination, moved}));
    }
    return best;
  }

  EventPotentialIndex build_event_potential_index(const Trace &trace) const {
    EventPotentialIndex index;
    index.support.resize(V * (MAX_HEIGHT + 1));
    index.capacity.resize(V * (MAX_HEIGHT + 1));
    index.block.resize(V * K * (MAX_HEIGHT + 1));

    auto append = [](PotentialTimeline &timeline, int t, long long value) {
      if (value <= 0)
        return;
      timeline.times.push_back(t);
      long long pref = value;
      if (!timeline.prefix.empty())
        pref += timeline.prefix.back();
      timeline.prefix.push_back(pref);
    };

    for (int t = 0; t < (int)trace.history.size(); ++t) {
      const auto &e = trace.history[t];
      int color = segment_color(e.moved_ids, e.moved_count);

      for (int amount = 1; amount <= MAX_HEIGHT; ++amount) {
        long long support_best = optimistic_support_gain(trace, t, amount);
        if (support_best > e.gain) {
          append(index.support[e.spec.source * (MAX_HEIGHT + 1) + amount], t, support_best - e.gain);
        }

        long long capacity_best = optimistic_capacity_gain(trace, t, amount);
        if (capacity_best > e.gain) {
          append(index.capacity[e.spec.destination * (MAX_HEIGHT + 1) + amount], t, capacity_best - e.gain);
        }

        if (color >= 0) {
          long long block_best = optimistic_block_gain(trace, t, amount);
          if (block_best > e.gain) {
            int key = (e.spec.source * K + color) * (MAX_HEIGHT + 1) + amount;
            append(index.block[key], t, block_best - e.gain);
          }
        }
      }
    }
    return index;
  }

  pair<long long, int> query_potential_timeline(const PotentialTimeline &timeline, int begin, int end) const {
    if (begin >= end || timeline.times.empty())
      return {0, 0};

    auto lit = lower_bound(timeline.times.begin(), timeline.times.end(), begin);
    auto rit = lower_bound(timeline.times.begin(), timeline.times.end(), end);
    int l = (int)(lit - timeline.times.begin());
    int r = (int)(rit - timeline.times.begin());
    if (l >= r)
      return {0, 0};

    long long sum = timeline.prefix[r - 1];
    if (l > 0)
      sum -= timeline.prefix[l - 1];
    return {sum, r - l};
  }

  pair<long long, int> aggregate_event_potential(const Trace &trace, const EventPotentialIndex &index,
                                                 const HistoricalCandidate &candidate, int insert) const {
    int amount = clamp(candidate.cheap_strength, 1, MAX_HEIGHT);
    int begin = insert;
    int end = candidate.producer;
    if (begin >= end)
      return {candidate.cheap_gain_delta, 1};

    if (candidate.type == EventType::SupportReady) {
      int key = candidate.event_cell * (MAX_HEIGHT + 1) + amount;
      auto result = query_potential_timeline(index.support[key], begin, end);
      if (result.first <= 0)
        return {candidate.cheap_gain_delta, 1};
      return result;
    }

    if (candidate.type == EventType::CapacityReady) {
      int key = candidate.event_cell * (MAX_HEIGHT + 1) + amount;
      auto result = query_potential_timeline(index.capacity[key], begin, end);
      if (result.first <= 0)
        return {candidate.cheap_gain_delta, 1};
      return result;
    }

    int color =
        segment_color(trace.history[candidate.producer].moved_ids, trace.history[candidate.producer].moved_count);
    if (color < 0) {
      color = segment_color(trace.history[candidate.use].moved_ids, trace.history[candidate.use].moved_count);
    }
    if (color < 0)
      return {candidate.cheap_gain_delta, 1};

    int key = (candidate.event_cell * K + color) * (MAX_HEIGHT + 1) + amount;
    auto result = query_potential_timeline(index.block[key], begin, end);
    if (result.first <= 0)
      return {candidate.cheap_gain_delta, 1};
    return result;
  }

  void append_candidate_variants(vector<HistoricalCandidate> &out, const Trace &trace,
                                 const CandidateHistoryIndex &history_index, const EventPotentialIndex &potential_index,
                                 HistoricalCandidate base) const {
    array<int, 7> raw{};
    int count = 0;

    raw[count++] = base.use;
    raw[count++] = max(0, base.use - 4);
    raw[count++] = max(0, base.use - 8);
    raw[count++] = max(0, base.use - 16);
    raw[count++] = max(0, base.use - 32);

    const auto &u = trace.history[base.use];
    int last_touch = -1;
    if (base.event_cell == u.spec.source) {
      last_touch = history_index.prev_touch_source[base.use];
    } else if (base.event_cell == u.spec.destination) {
      last_touch = history_index.prev_touch_destination[base.use];
    }
    if (last_touch >= 0)
      raw[count++] = last_touch;

    if (base.type == EventType::SupportReady) {
      int natural = history_index.support_insert[base.use];
      if (natural >= 0)
        raw[count++] = natural;
    }

    sort(raw.begin(), raw.begin() + count);
    count = unique(raw.begin(), raw.begin() + count) - raw.begin();

    const long long single_gain = base.cheap_gain_delta;
    for (int i = 0; i < count; ++i) {
      int insert = raw[i];
      if (insert < 0 || insert > base.use)
        continue;

      HistoricalCandidate candidate = base;
      candidate.insert = insert;

      // Rank an event by all historical uses that become available when the
      // event is backdated to `insert`, rather than only by the one use that
      // originally generated this candidate.  The range ends at producer:
      // after producer the original history already owns the event.
      auto [aggregate_gain, use_count] = aggregate_event_potential(trace, potential_index, candidate, insert);
      candidate.single_gain_delta = single_gain;
      candidate.aggregate_gain_delta = max(single_gain, aggregate_gain);
      candidate.cheap_gain_delta = candidate.aggregate_gain_delta;
      candidate.cheap_use_count = max(1, use_count);
      out.push_back(candidate);
    }
  }

  vector<HistoricalCandidate> generate_historical_candidates(const Trace &trace) const {
    vector<HistoricalCandidate> result;
    const int T = trace.history.size();
    if (T == 0)
      return result;

    result.reserve(min(300000, T * T * 3));
    const CandidateHistoryIndex index = build_candidate_history_index(trace);
    const EventPotentialIndex potential_index = build_event_potential_index(trace);

    for (int use = 0; use < T; ++use) {
      const auto &u = trace.history[use];
      const Board &before_use = trace.snapshot[use];

      int use_source = u.spec.source;
      int use_destination = u.spec.destination;
      int use_color = segment_color(u.moved_ids, u.moved_count);

      for (int producer = use + 1; producer < T; ++producer) {
        const auto &p = trace.history[producer];
        const Board &before_p = trace.snapshot[producer];
        const Board &after_p = trace.snapshot[producer + 1];

        if (p.spec.destination == use_source) {
          int added = after_p[use_source].size() - before_p[use_source].size();
          if (added > 0 && before_use[use_source].size() + added <= MAX_HEIGHT) {
            long long optimistic = optimistic_support_gain(trace, use, added);
            long long delta = optimistic - u.gain;
            if (delta > 0) {
              append_candidate_variants(result, trace, index, potential_index,
                                        HistoricalCandidate{
                                            EventType::SupportReady,
                                            producer,
                                            use,
                                            use,
                                            use_source,
                                            delta,
                                            added,
                                        });
            }
          }

          if (use_color >= 0 && segment_color(p.moved_ids, p.moved_count) == use_color &&
              before_use[use_source].size() + p.moved_count <= MAX_HEIGHT) {
            long long optimistic = optimistic_block_gain(trace, use, p.moved_count);
            long long delta = optimistic - u.gain;
            if (delta > 0) {
              append_candidate_variants(result, trace, index, potential_index,
                                        HistoricalCandidate{
                                            EventType::BlockReady,
                                            producer,
                                            use,
                                            use,
                                            use_source,
                                            delta,
                                            p.moved_count,
                                        });
            }
          }
        }

        if (p.spec.source == use_destination) {
          int freed = before_p[use_destination].size() - after_p[use_destination].size();
          if (freed > 0) {
            long long optimistic = optimistic_capacity_gain(trace, use, freed);
            long long delta = optimistic - u.gain;
            if (delta > 0) {
              append_candidate_variants(result, trace, index, potential_index,
                                        HistoricalCandidate{
                                            EventType::CapacityReady,
                                            producer,
                                            use,
                                            use,
                                            use_destination,
                                            delta,
                                            freed,
                                        });
            }
          }
        }
      }
    }

    // Merge exact duplicate event/backdating candidates first.  Keeping the
    // best individual fields avoids making the dedup order depend on one
    // particular ranking policy.
    auto identity_less = [](const HistoricalCandidate &a, const HistoricalCandidate &b) {
      return tie(a.type, a.producer, a.use, a.insert, a.event_cell) <
             tie(b.type, b.producer, b.use, b.insert, b.event_cell);
    };
    sort(result.begin(), result.end(), identity_less);

    vector<HistoricalCandidate> deduped;
    deduped.reserve(result.size());
    for (const auto &c : result) {
      if (deduped.empty() ||
          tie(deduped.back().type, deduped.back().producer, deduped.back().use, deduped.back().insert,
              deduped.back().event_cell) != tie(c.type, c.producer, c.use, c.insert, c.event_cell)) {
        deduped.push_back(c);
      } else {
        auto &d = deduped.back();
        d.single_gain_delta = max(d.single_gain_delta, c.single_gain_delta);
        d.aggregate_gain_delta = max(d.aggregate_gain_delta, c.aggregate_gain_delta);
        d.cheap_gain_delta = max(d.cheap_gain_delta, c.cheap_gain_delta);
        d.cheap_use_count = max(d.cheap_use_count, c.cheap_use_count);
        d.cheap_strength = max(d.cheap_strength, c.cheap_strength);
      }
    }
    result.swap(deduped);

    // Do not trust a single cheap score.  The exact-evaluation budget is
    // deliberately diversified:
    //   48: strong one-shot historical benefit,
    //   32: large multi-use aggregate benefit,
    //   16: aggregate benefit discounted by the rewrite span.
    // This prevents long-span multi-use candidates from monopolizing all 96
    // exact slots while still preserving them as a substantial fraction.
    vector<int> by_single(result.size());
    vector<int> by_multi(result.size());
    vector<int> by_cost(result.size());
    iota(by_single.begin(), by_single.end(), 0);
    iota(by_multi.begin(), by_multi.end(), 0);
    iota(by_cost.begin(), by_cost.end(), 0);

    auto deterministic_tie = [&](int ia, int ib) {
      const auto &a = result[ia];
      const auto &b = result[ib];
      if (a.cheap_strength != b.cheap_strength) {
        return a.cheap_strength > b.cheap_strength;
      }
      if (a.cheap_use_count != b.cheap_use_count) {
        return a.cheap_use_count > b.cheap_use_count;
      }
      if (a.insert != b.insert)
        return a.insert > b.insert;
      if (a.use != b.use)
        return a.use < b.use;
      if (a.producer != b.producer)
        return a.producer < b.producer;
      if (a.event_cell != b.event_cell)
        return a.event_cell < b.event_cell;
      return (int)a.type < (int)b.type;
    };

    sort(by_single.begin(), by_single.end(), [&](int ia, int ib) {
      const auto &a = result[ia];
      const auto &b = result[ib];
      if (a.single_gain_delta != b.single_gain_delta) {
        return a.single_gain_delta > b.single_gain_delta;
      }
      if (a.aggregate_gain_delta != b.aggregate_gain_delta) {
        return a.aggregate_gain_delta > b.aggregate_gain_delta;
      }
      return deterministic_tie(ia, ib);
    });

    sort(by_multi.begin(), by_multi.end(), [&](int ia, int ib) {
      const auto &a = result[ia];
      const auto &b = result[ib];
      if (a.aggregate_gain_delta != b.aggregate_gain_delta) {
        return a.aggregate_gain_delta > b.aggregate_gain_delta;
      }
      if (a.cheap_use_count != b.cheap_use_count) {
        return a.cheap_use_count > b.cheap_use_count;
      }
      if (a.single_gain_delta != b.single_gain_delta) {
        return a.single_gain_delta > b.single_gain_delta;
      }
      return deterministic_tie(ia, ib);
    });

    sort(by_cost.begin(), by_cost.end(), [&](int ia, int ib) {
      const auto &a = result[ia];
      const auto &b = result[ib];
      long long span_a = max(0, a.producer - a.insert);
      long long span_b = max(0, b.producer - b.insert);
      // aggregate / (1 + 0.05 * span) == 20*aggregate/(20+span).
      long double lhs = (long double)a.aggregate_gain_delta / (20.0L + span_a);
      long double rhs = (long double)b.aggregate_gain_delta / (20.0L + span_b);
      if (lhs != rhs)
        return lhs > rhs;
      if (a.aggregate_gain_delta != b.aggregate_gain_delta) {
        return a.aggregate_gain_delta > b.aggregate_gain_delta;
      }
      return deterministic_tie(ia, ib);
    });

    vector<uint8_t> selected(result.size(), 0);
    vector<HistoricalCandidate> ordered;
    ordered.reserve(min<int>(OFFLINE_CHEAP_KEEP, result.size()));

    auto take = [&](const vector<int> &order, int quota, uint8_t bucket) {
      int taken = 0;
      for (int idx : order) {
        if (taken >= quota || (int)ordered.size() >= OFFLINE_CHEAP_KEEP) {
          break;
        }
        if (selected[idx])
          continue;
        selected[idx] = 1;
        HistoricalCandidate c = result[idx];
        c.selection_bucket = bucket;
        ordered.push_back(std::move(c));
        ++taken;
      }
    };

    take(by_single, 48, 1);
    take(by_multi, 32, 2);
    take(by_cost, 16, 3);

    // The first 96 entries are the diversified exact-evaluation set. Keep a
    // larger aggregate-ranked tail for compatibility with larger future exact
    // budgets and for diagnostics.
    take(by_multi, OFFLINE_CHEAP_KEEP, 4);
    return ordered;
  }

  uint64_t hash_cell(const Tower &tower, int v) const {
    uint64_t z = splitmix64((uint64_t)v + 0x123456789abcdefULL);
    z ^= splitmix64((uint64_t)tower.size() * 0x9e3779b9ULL + v);
    for (int i = 0; i < tower.size(); ++i) {
      uint64_t key = ((uint64_t)tower[i].id << 16) | ((uint64_t)tower[i].color << 8) | (uint64_t)i;
      z ^= splitmix64(key + (uint64_t)v * 0xbf58476d1ce4e5b9ULL);
    }
    return z;
  }

  uint64_t hash_cell2(const Tower &tower, int v) const {
    uint64_t z = splitmix64((uint64_t)v + 0xd6e8feb86659fd93ULL);
    z ^= splitmix64((uint64_t)tower.size() * 0xa0761d6478bd642fULL + 3ULL * v);
    for (int i = 0; i < tower.size(); ++i) {
      uint64_t key = ((uint64_t)tower[i].id << 20) | ((uint64_t)tower[i].color << 12) | (uint64_t)i;
      z ^= splitmix64(key + (uint64_t)v * 0xe7037ed1a0b428dbULL);
    }
    return std::rotl(z, (v % 61) + 1);
  }

  RepairTarget make_repair_target(const Trace &trace, int time) const {
    RepairTarget target;
    target.time = time;
    target.cell_of_id.assign(color_of_id.size(), -1);

    const Board &board = trace.snapshot[time];
    for (int v = 0; v < V; ++v) {
      for (int i = 0; i < board[v].size(); ++i) {
        uint16_t sid = board[v][i].id;
        if (sid < target.cell_of_id.size()) {
          target.cell_of_id[sid] = (int16_t)v;
        }
      }
    }
    return target;
  }

  int id_target_distance(const RepairTarget &target, uint16_t sid, int current_cell) const {
    constexpr uint16_t INF16 = numeric_limits<uint16_t>::max();
    if (sid == 0 || sid >= color_of_id.size())
      return REPAIR_INF;

    int target_cell = target.cell_of_id[sid];
    if (target_cell >= 0) {
      uint16_t d = floor_dist[current_cell][target_cell];
      return d == INF16 ? REPAIR_INF : (int)d;
    }

    int color = color_of_id[sid];
    int d = dist_to_nest[color][current_cell];
    return d == INT_MAX ? REPAIR_INF : d;
  }

  long long cell_repair_cost(const Tower &current, int v, const Board &target_board, const RepairTarget &target) const {
    long long cost = 0;
    for (int i = 0; i < current.size(); ++i) {
      int d = id_target_distance(target, current[i].id, v);
      if (d >= REPAIR_INF)
        return (long long)REPAIR_INF * 100;
      cost += 24LL * d;
    }

    const Tower &goal = target_board[v];
    if (!(current == goal)) {
      cost += 8;
      cost += 3LL * abs(current.size() - goal.size());
      int common = min(current.size(), goal.size());
      for (int i = 0; i < common; ++i) {
        if (current[i].id != goal[i].id)
          cost += 5;
      }
    }
    return cost;
  }

  long long repair_heuristic(const Board &board, const Board &target_board, const RepairTarget &target) const {
    long long result = 0;
    vector<uint8_t> seen(color_of_id.size(), 0);

    for (int v = 0; v < V; ++v) {
      result += cell_repair_cost(board[v], v, target_board, target);
      if (result >= (long long)REPAIR_INF * 50)
        return result;
      for (int i = 0; i < board[v].size(); ++i) {
        uint16_t sid = board[v][i].id;
        if (sid < seen.size())
          seen[sid] = 1;
      }
    }

    // Homing is irreversible. If the target snapshot still contains an ID
    // that is already absent, this state can never rejoin that target.
    for (uint16_t sid = 1; sid < target.cell_of_id.size(); ++sid) {
      if (target.cell_of_id[sid] >= 0 && !seen[sid]) {
        return (long long)REPAIR_INF * 100;
      }
    }
    return result;
  }

  bool removed_required_target_id(const Tower &before_s, const Tower &before_d, const Tower &after_s,
                                  const Tower &after_d, const RepairTarget &target) const {
    array<uint16_t, MAX_HEIGHT * 2> before_ids{};
    int before_count = 0;
    for (int i = 0; i < before_s.size(); ++i) {
      before_ids[before_count++] = before_s[i].id;
    }
    for (int i = 0; i < before_d.size(); ++i) {
      before_ids[before_count++] = before_d[i].id;
    }

    for (int z = 0; z < before_count; ++z) {
      uint16_t sid = before_ids[z];
      if (sid >= target.cell_of_id.size() || target.cell_of_id[sid] < 0) {
        continue;
      }

      bool found = false;
      for (int i = 0; i < after_s.size() && !found; ++i) {
        found = after_s[i].id == sid;
      }
      for (int i = 0; i < after_d.size() && !found; ++i) {
        found = after_d[i].id == sid;
      }
      if (!found)
        return true;
    }
    return false;
  }

  long long rough_move_toward_target(const Board &board, const MoveSpec &m, const Board &target_board,
                                     const RepairTarget &target, int event_cell) const {
    const Tower &source = board[m.source];
    long long improvement = 0;
    int first = source.size() - m.moved_size;

    for (int i = first; i < source.size(); ++i) {
      uint16_t sid = source[i].id;
      int before = id_target_distance(target, sid, m.source);
      int after = id_target_distance(target, sid, m.destination);
      if (before >= REPAIR_INF || after >= REPAIR_INF)
        continue;
      improvement += 24LL * (before - after);
    }

    if (!(board[m.source] == target_board[m.source]))
      improvement += 4;
    if (!(board[m.destination] == target_board[m.destination]))
      improvement += 6;
    if (m.source == event_cell || m.destination == event_cell)
      improvement += 8;

    long long tg = transport_gain(board, m);
    if (tg > LLONG_MIN / 8)
      improvement += 2 * tg;
    return improvement;
  }

  TraceDependencyIndex build_trace_dependency_index(const Trace &trace) const {
    TraceDependencyIndex index;
    const int T = trace.history.size();
    index.prev_writer.assign(T, array<int, 2>{-1, -1});
    index.next_dependents.resize(T);
    index.ops_by_cell.resize(V);

    array<int, MAX_CELLS> last_writer{};
    last_writer.fill(-1);

    for (int t = 0; t < T; ++t) {
      const auto &e = trace.history[t];
      int ps = last_writer[e.spec.source];
      int pd = last_writer[e.spec.destination];
      index.prev_writer[t] = {ps, pd};

      if (ps >= 0)
        index.next_dependents[ps].push_back(t);
      if (pd >= 0 && pd != ps)
        index.next_dependents[pd].push_back(t);

      index.ops_by_cell[e.spec.source].push_back(t);
      index.ops_by_cell[e.spec.destination].push_back(t);

      last_writer[e.spec.source] = t;
      last_writer[e.spec.destination] = t;
    }
    return index;
  }

  CausalCone build_causal_cone(const Trace &trace, const HistoricalCandidate &candidate, int target_time,
                               const TraceDependencyIndex &dependency) const {
    CausalCone cone;
    const int T = trace.history.size();
    cone.operation_mask.assign(T, 0);

    vector<uint8_t> cell_seen(V, 0);
    vector<uint8_t> id_seen(color_of_id.size(), 0);
    deque<int> op_queue;

    auto add_op = [&](int t) {
      if (t < candidate.insert || t >= target_time)
        return;
      if (cone.operation_mask[t])
        return;
      cone.operation_mask[t] = 1;
      cone.operations.push_back(t);
      op_queue.push_back(t);
    };

    auto add_cell = [&](int v) {
      if (v < 0 || v >= V || !floor_cell(v) || cell_seen[v])
        return;
      cell_seen[v] = 1;
      cone.mask[v] = 1;
      cone.cells.push_back(v);
    };

    auto add_id_stat = [&](uint16_t sid) {
      if (sid == 0 || sid >= id_seen.size() || id_seen[sid])
        return;
      id_seen[sid] = 1;
      cone.slime_ids.push_back(sid);
    };

    // Seed with the historical use and later producer.  Dependencies are
    // propagated only along immediate last-writer edges.  We traverse the DAG
    // in both directions inside [insert,target_time): backwards to the exact
    // tower producers needed by a causal operation and forwards to operations
    // whose inputs would change if that producer is rewritten.  This gives a
    // true temporal dependency component; merely sharing a cell at unrelated
    // times or merely coexisting with the same slime no longer creates an edge.
    add_op(candidate.use);
    add_op(candidate.producer);
    add_cell(candidate.event_cell);

    while (!op_queue.empty()) {
      int t = op_queue.front();
      op_queue.pop_front();
      const auto &e = trace.history[t];

      add_cell(e.spec.source);
      add_cell(e.spec.destination);
      for (int i = 0; i < e.moved_count; ++i)
        add_id_stat(e.moved_ids[i]);

      for (int p : dependency.prev_writer[t])
        add_op(p);
      for (int nxt : dependency.next_dependents[t])
        add_op(nxt);
    }

    sort(cone.operations.begin(), cone.operations.end());

    // External helpers are allowed only from cells untouched by the entire old
    // rewrite interval. They commute with both causal and background actions.
    // Keep a small nearest fringe so the operation generator can still invent
    // support/merge structures absent from the old dependency component.
    vector<int> key_cells;
    auto push_key = [&](int v) {
      if (v < 0 || v >= V || !floor_cell(v))
        return;
      if (find(key_cells.begin(), key_cells.end(), v) == key_cells.end()) {
        key_cells.push_back(v);
      }
    };
    push_key(candidate.event_cell);
    push_key(trace.history[candidate.use].spec.source);
    push_key(trace.history[candidate.use].spec.destination);
    push_key(trace.history[candidate.producer].spec.source);
    push_key(trace.history[candidate.producer].spec.destination);

    const Board &start_board = trace.snapshot[candidate.insert];
    vector<pair<int, int>> helper_sources;
    helper_sources.reserve(V);

    auto touched_in_interval = [&](int v) {
      const auto &ops = dependency.ops_by_cell[v];
      auto it = lower_bound(ops.begin(), ops.end(), candidate.insert);
      return it != ops.end() && *it < target_time;
    };

    for (int v = 0; v < V; ++v) {
      if (cone.mask[v] || start_board[v].empty() || touched_in_interval(v)) {
        continue;
      }

      int best_dist = INT_MAX;
      for (int key : key_cells) {
        MoveSpec probe{v, key, 1};
        if (!legal_move(start_board, probe))
          continue;
        best_dist = min(best_dist, line_length(v, key));
      }
      if (best_dist != INT_MAX)
        helper_sources.push_back({best_dist, v});
    }

    sort(helper_sources.begin(), helper_sources.end());
    constexpr int HELPER_FRINGE_LIMIT = 24;
    for (int i = 0; i < (int)helper_sources.size() && i < HELPER_FRINGE_LIMIT; ++i) {
      int v = helper_sources[i].second;
      cone.mask[v] = 1;
      cone.cells.push_back(v);
    }

    ++stats.causal_cones;
    stats.causal_cells += cone.cells.size();
    stats.causal_operations += cone.operations.size();
    stats.causal_ids += cone.slime_ids.size();
    return cone;
  }

  vector<RepairMoveCandidate> enumerate_repair_moves(const Board &board, const Board &target_board,
                                                     const RepairTarget &target, int event_cell,
                                                     const CausalCone &cone) const {
    array<uint8_t, MAX_CELLS> mismatch{};
    vector<int> dirty_cells;
    dirty_cells.reserve(cone.cells.size());
    for (int v : cone.cells) {
      mismatch[v] = !(board[v] == target_board[v]);
      if (mismatch[v])
        dirty_cells.push_back(v);
    }

    // The last-writer cone is only a static upper bound. At this exact beam
    // state we branch from towers that are actually dirty, from the event cell,
    // and from towers currently carrying an ID already proven causal. A small
    // clean helper frontier preserves platform/setup moves without reopening
    // every clean tower in the static cone at every depth.
    array<uint8_t, MAX_CELLS> source_mask{};
    vector<int> sources;
    sources.reserve(dirty_cells.size() + cone.slime_ids.size() + 16);

    auto add_source = [&](int v) {
      if (v < 0 || v >= V || source_mask[v] || board[v].empty())
        return;
      source_mask[v] = 1;
      sources.push_back(v);
    };

    for (int v : dirty_cells)
      add_source(v);
    add_source(event_cell);

    if (!cone.slime_ids.empty()) {
      vector<uint8_t> causal_id(color_of_id.size(), 0);
      for (uint16_t sid : cone.slime_ids) {
        if (sid < causal_id.size())
          causal_id[sid] = 1;
      }
      for (int v : cone.cells) {
        if (source_mask[v] || board[v].empty())
          continue;
        for (int i = 0; i < board[v].size(); ++i) {
          uint16_t sid = board[v][i].id;
          if (sid < causal_id.size() && causal_id[sid]) {
            add_source(v);
            break;
          }
        }
      }
    }

    vector<int> important_cells = dirty_cells;
    if (0 <= event_cell && event_cell < V &&
        find(important_cells.begin(), important_cells.end(), event_cell) == important_cells.end()) {
      important_cells.push_back(event_cell);
    }

    vector<pair<int, int>> helpers;
    helpers.reserve(cone.cells.size());
    for (int source : cone.cells) {
      if (source_mask[source] || board[source].empty())
        continue;
      int h = board[source].size();
      int best = INT_MAX;
      for (int dst : important_cells) {
        if (dst == source)
          continue;
        int d = direction_between(source, dst);
        if (d < 0)
          continue;
        int l = line_length(source, dst);
        if (l <= 0 || l > h || l > MAX_HEIGHT)
          continue;
        if (ray_dest[source][d][l] != dst)
          continue;
        if (board[dst].size() + 1 > MAX_HEIGHT)
          continue;
        best = min(best, l);
      }
      if (best != INT_MAX)
        helpers.push_back({best, source});
    }
    sort(helpers.begin(), helpers.end());
    constexpr int DYNAMIC_HELPER_LIMIT = 12;
    int helper_added = 0;
    for (auto [dist, v] : helpers) {
      (void)dist;
      if (helper_added >= DYNAMIC_HELPER_LIMIT)
        break;
      add_source(v);
      ++helper_added;
    }

    ++stats.dynamic_frontier_calls;
    stats.dynamic_dirty_cells += dirty_cells.size();
    stats.dynamic_source_cells += sources.size();
    stats.dynamic_helper_cells += helper_added;

    vector<RepairMoveCandidate> result;
    result.reserve(max<size_t>(96, sources.size() * 12));

    for (int source : sources) {
      int h = board[source].size();
      if (h == 0)
        continue;
      bool source_relevant = mismatch[source] || source == event_cell;

      for (int moved = 1; moved <= h; ++moved) {
        int max_jump = h - moved + 1;
        for (int d = 0; d < 4; ++d) {
          for (int l = 1; l <= max_jump; ++l) {
            int destination = ray_dest[source][d][l];
            if (destination < 0)
              break;
            if (!cone.mask[destination])
              continue;
            if (board[destination].size() + moved > MAX_HEIGHT)
              continue;

            if (!source_relevant && !mismatch[destination] && destination != event_cell) {
              continue;
            }

            MoveSpec m{source, destination, moved};
            long long rough = rough_move_toward_target(board, m, target_board, target, event_cell);
            result.push_back({m, rough});
          }
        }
      }
    }
    return result;
  }

  bool boards_equal(const Board &a, const Board &b) const {
    for (int v = 0; v < V; ++v) {
      if (!(a[v] == b[v]))
        return false;
    }
    return true;
  }

  ExactCandidateResult evaluate_candidate_exact(const Trace &trace, const HistoricalCandidate &candidate,
                                                const TraceDependencyIndex &dependency) const {
    ExactCandidateResult result;
    const int T = trace.history.size();

    if (!(0 <= candidate.insert && candidate.insert <= candidate.use && candidate.use < candidate.producer &&
          candidate.producer < T)) {
      return result;
    }

    // Rebuild only the last-writer temporal dependency component that connects
    // the backdated event to its producer. Operations outside this component
    // share no DAG edge with it in the rewrite interval, so they commute as a
    // background and never need to enter the beam.
    int target_time = candidate.producer + 1;
    if (target_time <= candidate.insert || target_time > T)
      return result;

    const CausalCone cone = build_causal_cone(trace, candidate, target_time, dependency);
    if (cone.operations.empty())
      return result;

    Board initial_board = trace.snapshot[candidate.insert];
    vector<MoveSpec> background_specs;
    background_specs.reserve(target_time - candidate.insert);

    int relevant_old_ops = 0;
    for (int t = candidate.insert; t < target_time; ++t) {
      if (cone.operation_mask[t]) {
        ++relevant_old_ops;
        continue;
      }
      AppliedMove a = apply_move(initial_board, trace.history[t].spec);
      if (!a.ok)
        return result;
      background_specs.push_back(trace.history[t].spec);
    }
    stats.background_actions += background_specs.size();

    // A strictly shorter rewrite needs fewer operations than the relevant old
    // component.  Independent background operations cancel from the count.
    if (relevant_old_ops <= 0)
      return result;

    RepairTarget target = make_repair_target(trace, target_time);
    const Board &target_board = trace.snapshot[target_time];
    const uint64_t target_hash = trace.snapshot_hash[target_time];
    const uint64_t target_hash2 = trace.snapshot_hash2[target_time];

    LocalBeamState initial;
    initial.board = std::move(initial_board);
    initial.board_hash = hash_board(initial.board);
    initial.board_hash2 = hash_board2(initial.board);
    initial.heuristic = repair_heuristic(initial.board, target_board, target);
    initial.score = initial.heuristic;
    if (initial.heuristic >= (long long)REPAIR_INF * 50)
      return result;

    int best_saving = 0;
    array<MoveSpec, LOCAL_MAX_DEPTH> best_path{};
    int best_path_len = 0;

    auto try_target = [&](const LocalBeamState &state) {
      if (state.path_len >= relevant_old_ops)
        return;
      if (state.board_hash != target_hash || state.board_hash2 != target_hash2) {
        return;
      }
      if (!boards_equal(state.board, target_board))
        return;

      int saving = relevant_old_ops - state.path_len;
      if (saving <= best_saving)
        return;

      ++stats.rejoin_hits;
      best_saving = saving;
      best_path_len = state.path_len;
      for (int i = 0; i < state.path_len; ++i) {
        best_path[i] = state.path[i];
      }
    };

    vector<LocalBeamState> beam;
    beam.push_back(std::move(initial));
    try_target(beam.front());

    unordered_map<Hash128, int, Hash128Hasher> min_depth;
    min_depth.reserve(LOCAL_BEAM_WIDTH * LOCAL_MAX_DEPTH * 4 + 32);
    min_depth.emplace(Hash128{beam.front().board_hash, beam.front().board_hash2}, 0);

    int max_depth = min(LOCAL_MAX_DEPTH, relevant_old_ops - 1);

    for (int depth = 0; depth < max_depth && !beam.empty() && !offline_time_over(); ++depth) {
      vector<LocalExpansionCandidate> expansions;
      expansions.reserve(4096);
      unordered_map<Hash128, size_t, Hash128Hasher> current_depth_best;
      current_depth_best.reserve(8192);

      auto better_expansion = [](const LocalExpansionCandidate &a, const LocalExpansionCandidate &b) {
        if (a.score != b.score)
          return a.score < b.score;
        if (a.rough != b.rough)
          return a.rough > b.rough;
        if (a.board_hash != b.board_hash) {
          return a.board_hash < b.board_hash;
        }
        return a.board_hash2 < b.board_hash2;
      };

      for (int parent_index = 0; parent_index < (int)beam.size() && !offline_time_over(); ++parent_index) {
        const auto &state = beam[parent_index];
        ++stats.beam_states;

        auto moves = enumerate_repair_moves(state.board, target_board, target, candidate.event_cell, cone);

        for (const auto &rm : moves) {
          if (offline_time_over())
            break;
          ++stats.beam_raw_candidates;

          const MoveSpec &m = rm.spec;
          const Tower &old_source = state.board[m.source];
          const Tower &old_destination = state.board[m.destination];

          Tower source_after;
          Tower destination_after;
          if (!simulate_move_towers(state.board, m, source_after, destination_after)) {
            continue;
          }

          if (removed_required_target_id(old_source, old_destination, source_after, destination_after, target)) {
            continue;
          }

          long long old_cost = cell_repair_cost(old_source, m.source, target_board, target) +
                               cell_repair_cost(old_destination, m.destination, target_board, target);
          long long new_cost = cell_repair_cost(source_after, m.source, target_board, target) +
                               cell_repair_cost(destination_after, m.destination, target_board, target);

          long long new_heuristic = state.heuristic + new_cost - old_cost;
          if (new_heuristic >= (long long)REPAIR_INF * 50)
            continue;

          uint64_t next_hash = state.board_hash;
          next_hash ^= hash_cell(old_source, m.source);
          next_hash ^= hash_cell(old_destination, m.destination);
          next_hash ^= hash_cell(source_after, m.source);
          next_hash ^= hash_cell(destination_after, m.destination);

          uint64_t next_hash2 = state.board_hash2;
          next_hash2 ^= hash_cell2(old_source, m.source);
          next_hash2 ^= hash_cell2(old_destination, m.destination);
          next_hash2 ^= hash_cell2(source_after, m.source);
          next_hash2 ^= hash_cell2(destination_after, m.destination);

          if (state.path_len > 0 && next_hash == state.prev_board_hash && next_hash2 == state.prev_board_hash2) {
            ++stats.undo_rejects;
            continue;
          }

          Hash128 key{next_hash, next_hash2};
          auto seen = min_depth.find(key);
          if (seen != min_depth.end() && seen->second <= depth + 1) {
            ++stats.transposition_rejects;
            continue;
          }

          LocalExpansionCandidate expansion;
          expansion.parent_index = parent_index;
          expansion.spec = m;
          expansion.source_after = source_after;
          expansion.destination_after = destination_after;
          expansion.board_hash = next_hash;
          expansion.board_hash2 = next_hash2;
          expansion.heuristic = new_heuristic;
          expansion.score = new_heuristic;
          expansion.rough = rm.rough;

          auto [it, inserted] = current_depth_best.emplace(key, expansions.size());
          if (inserted) {
            expansions.push_back(std::move(expansion));
          } else {
            auto &old = expansions[it->second];
            if (better_expansion(expansion, old)) {
              old = std::move(expansion);
            }
          }
        }
      }

      if (expansions.empty())
        break;

      if ((int)expansions.size() > LOCAL_GLOBAL_PREBEAM) {
        nth_element(expansions.begin(), expansions.begin() + LOCAL_GLOBAL_PREBEAM, expansions.end(), better_expansion);
        expansions.resize(LOCAL_GLOBAL_PREBEAM);
      }

      auto better_beam_expansion = [](const LocalExpansionCandidate &a, const LocalExpansionCandidate &b) {
        if (a.score != b.score)
          return a.score < b.score;
        if (a.board_hash != b.board_hash) {
          return a.board_hash < b.board_hash;
        }
        return a.board_hash2 < b.board_hash2;
      };
      sort(expansions.begin(), expansions.end(), better_beam_expansion);

      const int beam_keep = min<int>(LOCAL_BEAM_WIDTH, expansions.size());
      vector<LocalBeamState> next;
      next.reserve(beam_keep);

      for (int i = 0; i < (int)expansions.size(); ++i) {
        if (offline_time_over())
          break;
        const auto &expansion = expansions[i];

        bool keep_for_beam = i < beam_keep;
        bool probe_target = expansion.board_hash == target_hash && expansion.board_hash2 == target_hash2 &&
                            depth + 1 < relevant_old_ops && relevant_old_ops - (depth + 1) > best_saving;
        if (!keep_for_beam && !probe_target)
          continue;

        ++stats.beam_children;
        if (!keep_for_beam && probe_target) {
          ++stats.rejoin_materializations;
        }

        const LocalBeamState &parent = beam[expansion.parent_index];
        LocalBeamState child = parent;
        child.board[expansion.spec.source] = expansion.source_after;
        child.board[expansion.spec.destination] = expansion.destination_after;
        child.prev_board_hash = parent.board_hash;
        child.prev_board_hash2 = parent.board_hash2;
        child.board_hash = expansion.board_hash;
        child.board_hash2 = expansion.board_hash2;
        child.heuristic = expansion.heuristic;
        child.score = expansion.score;
        child.path[child.path_len++] = expansion.spec;

        if (probe_target)
          try_target(child);
        if (keep_for_beam)
          next.push_back(std::move(child));
      }

      if (next.empty())
        break;

      for (const auto &state : next) {
        Hash128 key{state.board_hash, state.board_hash2};
        auto [it, inserted] = min_depth.emplace(key, state.path_len);
        if (!inserted && state.path_len < it->second) {
          it->second = state.path_len;
        }
      }

      beam = std::move(next);
    }

    if (best_saving <= 0)
      return result;

    vector<MoveSpec> specs;
    specs.reserve(T - best_saving);
    for (int t = 0; t < candidate.insert; ++t) {
      specs.push_back(trace.history[t].spec);
    }
    // These original actions are independent of the causal component and can
    // be moved before the repaired component without changing either side.
    for (const auto &spec : background_specs)
      specs.push_back(spec);
    for (int i = 0; i < best_path_len; ++i) {
      specs.push_back(best_path[i]);
    }
    for (int t = target_time; t < T; ++t) {
      specs.push_back(trace.history[t].spec);
    }

    result.valid = true;
    result.action_count = specs.size();
    result.rejoin_time = target_time;
    result.specs = std::move(specs);
    return result;
  }

  void update_direct_dirty(array<uint8_t, MAX_CELLS> &dirty, int &dirty_count, int v, const Board &board,
                           const Board &original) const {
    if (v < 0 || v >= V)
      return;
    const bool now_dirty = !(board[v] == original[v]);
    if (now_dirty && !dirty[v]) {
      dirty[v] = 1;
      ++dirty_count;
    } else if (!now_dirty && dirty[v]) {
      dirty[v] = 0;
      --dirty_count;
    }
  }

  bool original_action_semantically_available(const Board &board, const HistoryEntry &entry) const {
    return top_ids_exact(board, entry.spec.source, entry.moved_ids, entry.moved_count) && legal_move(board, entry.spec);
  }

  optional<MoveSpec> find_direct_event_move(const Trace &trace, const HistoricalCandidate &candidate,
                                            const Board &board) const {
    const HistoryEntry &producer = trace.history[candidate.producer];
    const int count = producer.moved_count;
    if (count <= 0)
      return nullopt;

    if (candidate.type == EventType::CapacityReady) {
      const int source = candidate.event_cell;
      if (!top_ids_same_set(board[source], producer.moved_ids, count)) {
        return nullopt;
      }
      MoveSpec move{
          source,
          producer.spec.destination,
          count,
      };
      if (!legal_move(board, move))
        return nullopt;
      return move;
    }

    // SupportReady / BlockReady:
    // move the future producer packet directly to the cell where its later
    // presence became useful.  IDs are unique, so at most one tower can hold
    // the complete top packet.
    for (int source = 0; source < V; ++source) {
      if (source == candidate.event_cell)
        continue;
      if (!top_ids_same_set(board[source], producer.moved_ids, count)) {
        continue;
      }
      MoveSpec move{
          source,
          candidate.event_cell,
          count,
      };
      if (legal_move(board, move))
        return move;
    }
    return nullopt;
  }

  optional<MoveSpec> choose_direct_use_move(const Trace &trace, const HistoricalCandidate &candidate,
                                            const Board &board) const {
    const HistoryEntry &original = trace.history[candidate.use];
    const int source = original.spec.source;
    if (source < 0 || source >= V || board[source].empty())
      return nullopt;

    optional<MoveSpec> best;
    long long best_gain = original.gain;

    if (candidate.type == EventType::SupportReady) {
      if (!top_ids_same_set(board[source], original.moved_ids, original.moved_count)) {
        return nullopt;
      }

      const int moved = original.moved_count;
      const int h = board[source].size();
      const int max_jump = h - moved + 1;

      for (int d = 0; d < 4; ++d) {
        for (int l = 1; l <= max_jump; ++l) {
          int destination = ray_dest[source][d][l];
          if (destination < 0)
            break;
          MoveSpec move{source, destination, moved};
          if (!legal_move(board, move))
            continue;

          long long gain = transport_gain(board, move);
          if (gain > best_gain) {
            best_gain = gain;
            best = move;
          }
        }
      }
      return best;
    }

    if (candidate.type == EventType::BlockReady) {
      int color = segment_color(original.moved_ids, original.moved_count);
      if (color < 0)
        return nullopt;

      const Tower &tower = board[source];
      for (int moved = original.moved_count + 1; moved <= tower.size(); ++moved) {
        if (!segment_contains_all_ids(tower, moved, original.moved_ids, original.moved_count)) {
          continue;
        }

        bool same_color = true;
        for (int i = tower.size() - moved; i < tower.size(); ++i) {
          if (tower[i].color != color) {
            same_color = false;
            break;
          }
        }
        if (!same_color)
          continue;

        const int max_jump = tower.size() - moved + 1;
        for (int d = 0; d < 4; ++d) {
          for (int l = 1; l <= max_jump; ++l) {
            int destination = ray_dest[source][d][l];
            if (destination < 0)
              break;
            MoveSpec move{source, destination, moved};
            if (!legal_move(board, move))
              continue;

            long long gain = transport_gain(board, move);
            if (gain > best_gain) {
              best_gain = gain;
              best = move;
            }
          }
        }
      }
      return best;
    }

    // CapacityReady:
    // use the newly freed landing capacity to move a larger suffix to the
    // same historical destination.
    for (int moved = original.moved_count + 1; moved <= board[source].size(); ++moved) {
      if (!segment_contains_all_ids(board[source], moved, original.moved_ids, original.moved_count)) {
        continue;
      }

      MoveSpec move{
          source,
          original.spec.destination,
          moved,
      };
      if (!legal_move(board, move))
        continue;

      long long gain = transport_gain(board, move);
      if (gain > best_gain) {
        best_gain = gain;
        best = move;
      }
    }
    return best;
  }

  optional<MoveSpec> reroute_original_action(const Board &board, const HistoryEntry &entry) const {
    const int count = entry.moved_count;
    if (count <= 0)
      return nullopt;

    for (int source = 0; source < V; ++source) {
      if (source == entry.spec.source)
        continue;
      if (!top_ids_same_set(board[source], entry.moved_ids, count))
        continue;

      MoveSpec move{
          source,
          entry.spec.destination,
          count,
      };
      if (legal_move(board, move))
        return move;
    }
    return nullopt;
  }

  ExactCandidateResult evaluate_candidate_direct(const Trace &trace, const HistoricalCandidate &candidate) const {
    ExactCandidateResult result;
    ++stats.direct_trials;

    const int T = trace.history.size();
    if (!(0 <= candidate.insert && candidate.insert <= candidate.use && candidate.use < candidate.producer &&
          candidate.producer < T)) {
      ++stats.direct_event_fail;
      return result;
    }

    Board board = trace.snapshot[candidate.insert];

    auto event_move = find_direct_event_move(trace, candidate, board);
    if (!event_move.has_value()) {
      ++stats.direct_event_fail;
      return result;
    }

    vector<MoveSpec> middle;
    middle.reserve(min(T - candidate.insert, candidate.producer - candidate.insert + SA_DIRECT_REJOIN_EXTRA + 4));

    AppliedMove inserted = apply_move(board, *event_move);
    if (!inserted.ok) {
      ++stats.direct_event_fail;
      return result;
    }
    middle.push_back(*event_move);

    // dirty is always relative to trace.snapshot[current_time].  Because both
    // the real and original transition touch at most two towers, advancing the
    // reference time only requires re-checking the union of those towers.
    array<uint8_t, MAX_CELLS> dirty{};
    int dirty_count = 0;
    update_direct_dirty(dirty, dirty_count, event_move->source, board, trace.snapshot[candidate.insert]);
    update_direct_dirty(dirty, dirty_count, event_move->destination, board, trace.snapshot[candidate.insert]);

    bool improved_use_done = false;
    const int last_time = min(T, candidate.producer + 1 + SA_DIRECT_REJOIN_EXTRA);

    auto make_success = [&](int rejoin_time) {
      ExactCandidateResult success;
      success.valid = true;
      success.rejoin_time = rejoin_time;
      success.action_count = candidate.insert + (int)middle.size() + (T - rejoin_time);

      success.specs.reserve(success.action_count);
      for (int t = 0; t < candidate.insert; ++t) {
        success.specs.push_back(trace.history[t].spec);
      }
      for (const auto &move : middle) {
        success.specs.push_back(move);
      }
      for (int t = rejoin_time; t < T; ++t) {
        success.specs.push_back(trace.history[t].spec);
      }
      return success;
    };

    for (int t = candidate.insert; t < last_time && !offline_time_over(); ++t) {
      ++stats.direct_steps;
      const HistoryEntry &original = trace.history[t];

      optional<MoveSpec> actual_move;

      if (t == candidate.use) {
        auto improved = choose_direct_use_move(trace, candidate, board);
        if (!improved.has_value()) {
          ++stats.direct_use_fail;
          return result;
        }
        actual_move = *improved;
        improved_use_done = true;
      } else if (t == candidate.producer) {
        // This future producer has already been executed at insert.
        actual_move.reset();
      } else if (original_action_semantically_available(board, original)) {
        actual_move = original.spec;
      } else {
        auto rerouted = reroute_original_action(board, original);
        if (rerouted.has_value()) {
          actual_move = *rerouted;
          ++stats.direct_reroutes;
        }
        // Otherwise the old operation is simply omitted.  This is safe as a
        // proposal because the edit is accepted only after the exact dirty
        // state has rejoined the original history.
      }

      array<int, 4> touched{
          original.spec.source,
          original.spec.destination,
          -1,
          -1,
      };

      if (actual_move.has_value()) {
        touched[2] = actual_move->source;
        touched[3] = actual_move->destination;

        AppliedMove applied = apply_move(board, *actual_move);
        if (!applied.ok) {
          ++stats.direct_rejoin_fail;
          return result;
        }
        middle.push_back(*actual_move);
      }

      const Board &next_original = trace.snapshot[t + 1];
      for (int i = 0; i < 4; ++i) {
        int v = touched[i];
        if (v < 0)
          continue;

        bool duplicate = false;
        for (int j = 0; j < i; ++j) {
          if (touched[j] == v) {
            duplicate = true;
            break;
          }
        }
        if (duplicate)
          continue;

        update_direct_dirty(dirty, dirty_count, v, board, next_original);
      }

      const int next_time = t + 1;

      // If the edit vanished before the historical use, it cannot realize the
      // sampled event.  Likewise, rejoining before the producer would merely
      // add a useless cycle because the original producer would still remain.
      if (dirty_count == 0) {
        if (!improved_use_done || next_time <= candidate.producer) {
          ++stats.direct_rejoin_fail;
          return result;
        }

        ++stats.direct_valid;
        ++stats.rejoin_hits;
        return make_success(next_time);
      }
    }

    ++stats.direct_rejoin_fail;
    return result;
  }

  ExactCandidateResult evaluate_candidate_sa(const Trace &trace, const HistoricalCandidate &candidate,
                                             const TraceDependencyIndex &dependency, int beam_width, int global_prebeam,
                                             int depth_limit, int max_extra) const {
    ExactCandidateResult result;
    const int T = trace.history.size();

    if (!(0 <= candidate.insert && candidate.insert <= candidate.use && candidate.use < candidate.producer &&
          candidate.producer < T)) {
      return result;
    }

    const int target_time = candidate.producer + 1;
    if (target_time <= candidate.insert || target_time > T)
      return result;

    const CausalCone cone = build_causal_cone(trace, candidate, target_time, dependency);
    if (cone.operations.empty())
      return result;

    Board initial_board = trace.snapshot[candidate.insert];
    vector<MoveSpec> background_specs;
    background_specs.reserve(target_time - candidate.insert);

    int relevant_old_ops = 0;
    for (int t = candidate.insert; t < target_time; ++t) {
      if (cone.operation_mask[t]) {
        ++relevant_old_ops;
        continue;
      }
      AppliedMove a = apply_move(initial_board, trace.history[t].spec);
      if (!a.ok)
        return result;
      background_specs.push_back(trace.history[t].spec);
    }
    stats.background_actions += background_specs.size();

    if (relevant_old_ops <= 0)
      return result;

    RepairTarget target = make_repair_target(trace, target_time);
    const Board &target_board = trace.snapshot[target_time];
    const uint64_t target_hash = trace.snapshot_hash[target_time];
    const uint64_t target_hash2 = trace.snapshot_hash2[target_time];

    LocalBeamState initial;
    initial.board = std::move(initial_board);
    initial.board_hash = hash_board(initial.board);
    initial.board_hash2 = hash_board2(initial.board);
    initial.heuristic = repair_heuristic(initial.board, target_board, target);
    initial.score = initial.heuristic;
    if (initial.heuristic >= (long long)REPAIR_INF * 50)
      return result;

    int best_path_len = INT_MAX;
    array<MoveSpec, LOCAL_MAX_DEPTH> best_path{};

    auto try_target = [&](const LocalBeamState &state) {
      if (state.path_len > relevant_old_ops + max_extra)
        return;
      if (state.path_len >= best_path_len)
        return;
      if (state.board_hash != target_hash || state.board_hash2 != target_hash2) {
        return;
      }
      if (!boards_equal(state.board, target_board))
        return;

      ++stats.rejoin_hits;
      best_path_len = state.path_len;
      for (int i = 0; i < state.path_len; ++i) {
        best_path[i] = state.path[i];
      }
    };

    vector<LocalBeamState> beam;
    beam.push_back(std::move(initial));
    try_target(beam.front());

    unordered_map<Hash128, int, Hash128Hasher> min_depth;
    min_depth.reserve(beam_width * max(4, depth_limit) * 5 + 32);
    min_depth.emplace(Hash128{beam.front().board_hash, beam.front().board_hash2}, 0);

    const int max_depth = min({
        LOCAL_MAX_DEPTH,
        depth_limit,
        relevant_old_ops + max_extra,
    });

    for (int depth = 0; depth < max_depth && !beam.empty() && !offline_time_over(); ++depth) {
      if (best_path_len <= depth + 1)
        break;

      vector<LocalExpansionCandidate> expansions;
      expansions.reserve(max(512, global_prebeam * 3));

      unordered_map<Hash128, size_t, Hash128Hasher> current_depth_best;
      current_depth_best.reserve(max(1024, global_prebeam * 6));

      auto better_expansion = [](const LocalExpansionCandidate &a, const LocalExpansionCandidate &b) {
        if (a.score != b.score)
          return a.score < b.score;
        if (a.rough != b.rough)
          return a.rough > b.rough;
        if (a.board_hash != b.board_hash) {
          return a.board_hash < b.board_hash;
        }
        return a.board_hash2 < b.board_hash2;
      };

      for (int parent_index = 0; parent_index < (int)beam.size() && !offline_time_over(); ++parent_index) {
        const auto &state = beam[parent_index];
        ++stats.beam_states;

        auto moves = enumerate_repair_moves(state.board, target_board, target, candidate.event_cell, cone);

        for (const auto &rm : moves) {
          if (offline_time_over())
            break;
          ++stats.beam_raw_candidates;

          const MoveSpec &m = rm.spec;
          const Tower &old_source = state.board[m.source];
          const Tower &old_destination = state.board[m.destination];

          Tower source_after;
          Tower destination_after;
          if (!simulate_move_towers(state.board, m, source_after, destination_after)) {
            continue;
          }

          if (removed_required_target_id(old_source, old_destination, source_after, destination_after, target)) {
            continue;
          }

          long long old_cost = cell_repair_cost(old_source, m.source, target_board, target) +
                               cell_repair_cost(old_destination, m.destination, target_board, target);
          long long new_cost = cell_repair_cost(source_after, m.source, target_board, target) +
                               cell_repair_cost(destination_after, m.destination, target_board, target);

          long long new_heuristic = state.heuristic + new_cost - old_cost;
          if (new_heuristic >= (long long)REPAIR_INF * 50)
            continue;

          uint64_t next_hash = state.board_hash;
          next_hash ^= hash_cell(old_source, m.source);
          next_hash ^= hash_cell(old_destination, m.destination);
          next_hash ^= hash_cell(source_after, m.source);
          next_hash ^= hash_cell(destination_after, m.destination);

          uint64_t next_hash2 = state.board_hash2;
          next_hash2 ^= hash_cell2(old_source, m.source);
          next_hash2 ^= hash_cell2(old_destination, m.destination);
          next_hash2 ^= hash_cell2(source_after, m.source);
          next_hash2 ^= hash_cell2(destination_after, m.destination);

          if (state.path_len > 0 && next_hash == state.prev_board_hash && next_hash2 == state.prev_board_hash2) {
            ++stats.undo_rejects;
            continue;
          }

          Hash128 key{next_hash, next_hash2};
          auto seen = min_depth.find(key);
          if (seen != min_depth.end() && seen->second <= depth + 1) {
            ++stats.transposition_rejects;
            continue;
          }

          LocalExpansionCandidate expansion;
          expansion.parent_index = parent_index;
          expansion.spec = m;
          expansion.source_after = source_after;
          expansion.destination_after = destination_after;
          expansion.board_hash = next_hash;
          expansion.board_hash2 = next_hash2;
          expansion.heuristic = new_heuristic;

          // Small deterministic jitter from the state hash prevents every
          // sampled event from following exactly the same equal-score beam.
          long long jitter = (long long)((next_hash ^ std::rotl(next_hash2, 17)) & 3ULL);
          expansion.score = new_heuristic * 4 + jitter;
          expansion.rough = rm.rough;

          auto [it, inserted] = current_depth_best.emplace(key, expansions.size());
          if (inserted) {
            expansions.push_back(std::move(expansion));
          } else {
            auto &old = expansions[it->second];
            if (better_expansion(expansion, old)) {
              old = std::move(expansion);
            }
          }
        }
      }

      if (expansions.empty())
        break;

      if ((int)expansions.size() > global_prebeam) {
        nth_element(expansions.begin(), expansions.begin() + global_prebeam, expansions.end(), better_expansion);
        expansions.resize(global_prebeam);
      }

      sort(expansions.begin(), expansions.end(), better_expansion);

      const int beam_keep = min<int>(beam_width, expansions.size());
      vector<LocalBeamState> next;
      next.reserve(beam_keep);

      for (int i = 0; i < (int)expansions.size(); ++i) {
        if (offline_time_over())
          break;

        const auto &expansion = expansions[i];
        const bool keep_for_beam = i < beam_keep;
        const bool probe_target = expansion.board_hash == target_hash && expansion.board_hash2 == target_hash2 &&
                                  depth + 1 <= relevant_old_ops + max_extra && depth + 1 < best_path_len;

        if (!keep_for_beam && !probe_target)
          continue;

        ++stats.beam_children;
        if (!keep_for_beam && probe_target) {
          ++stats.rejoin_materializations;
        }

        const LocalBeamState &parent = beam[expansion.parent_index];
        LocalBeamState child = parent;
        child.board[expansion.spec.source] = expansion.source_after;
        child.board[expansion.spec.destination] = expansion.destination_after;
        child.prev_board_hash = parent.board_hash;
        child.prev_board_hash2 = parent.board_hash2;
        child.board_hash = expansion.board_hash;
        child.board_hash2 = expansion.board_hash2;
        child.heuristic = expansion.heuristic;
        child.score = expansion.score;
        child.path[child.path_len++] = expansion.spec;

        if (probe_target)
          try_target(child);
        if (keep_for_beam)
          next.push_back(std::move(child));
      }

      if (next.empty())
        break;

      for (const auto &state : next) {
        Hash128 key{state.board_hash, state.board_hash2};
        auto [it, inserted] = min_depth.emplace(key, state.path_len);
        if (!inserted && state.path_len < it->second) {
          it->second = state.path_len;
        }
      }

      beam = std::move(next);
    }

    if (best_path_len == INT_MAX)
      return result;

    vector<MoveSpec> specs;
    specs.reserve(T - relevant_old_ops + best_path_len);
    for (int t = 0; t < candidate.insert; ++t) {
      specs.push_back(trace.history[t].spec);
    }
    for (const auto &spec : background_specs)
      specs.push_back(spec);
    for (int i = 0; i < best_path_len; ++i) {
      specs.push_back(best_path[i]);
    }
    for (int t = target_time; t < T; ++t) {
      specs.push_back(trace.history[t].spec);
    }

    result.valid = true;
    result.action_count = specs.size();
    result.rejoin_time = target_time;
    result.specs = std::move(specs);
    return result;
  }

  Trace optimize_offline(Trace trace) const {
    if (!trace.valid || total_slimes(trace.final_board) != 0)
      return trace;

    Trace current = trace;
    Trace best = trace;

    long long trials = 0;
    long long valid_mutations = 0;
    long long accepted = 0;
    long long accepted_improving = 0;
    long long accepted_neutral = 0;
    long long accepted_uphill = 0;
    long long rescue_trials = 0;
    long long rescue_deep_trials = 0;
    long long refreshes = 0;
    long long rebuild_rejects = 0;

    long long states_before = stats.beam_states;
    long long raw_before = stats.beam_raw_candidates;
    long long children_before = stats.beam_children;
    long long rejoins_before = stats.rejoin_hits;
    long long direct_steps_before = stats.direct_steps;
    long long direct_reroutes_before = stats.direct_reroutes;

    vector<HistoricalCandidate> candidates;
    vector<int> random_order;
    size_t random_pos = 0;

    // Building the dependency DAG is unnecessary for the common direct
    // neighborhood.  Construct it lazily only if this history actually needs
    // a beam rescue.
    optional<TraceDependencyIndex> dependency;

    auto refresh_proposals = [&]() {
      candidates = generate_historical_candidates(current);
      stats.generated += candidates.size();
      dependency.reset();

      random_order.resize(candidates.size());
      iota(random_order.begin(), random_order.end(), 0);
      for (int i = (int)random_order.size() - 1; i > 0; --i) {
        int j = random_int(i + 1);
        swap(random_order[i], random_order[j]);
      }
      random_pos = 0;
      ++refreshes;
    };

    auto progress_now = [&]() {
      return min(1.0, max(0.0, (double)elapsed_ms() / (double)OFFLINE_TIME_GUARD_MS));
    };

    auto temperature_now = [&]() {
      const double p = progress_now();
      return SA_TEMP_START * pow(SA_TEMP_END / SA_TEMP_START, p);
    };

    cerr << "sa baseline=" << current.history.size() << '\n';
    refresh_proposals();

    while (!offline_time_over()) {
      if (candidates.empty())
        break;

      if (random_pos >= random_order.size()) {
        // Same current history, another unbiased pass through its events.
        for (int i = (int)random_order.size() - 1; i > 0; --i) {
          int j = random_int(i + 1);
          swap(random_order[i], random_order[j]);
        }
        random_pos = 0;
      }

      const int candidate_index = random_order[random_pos++];
      const HistoricalCandidate candidate = candidates[candidate_index];
      ++trials;
      ++stats.exact_tried;

      // Level 1: cheap direct historical mutation + exact incremental replay.
      ExactCandidateResult proposal = evaluate_candidate_direct(current, candidate);

      bool used_rescue = false;
      bool deep = false;

      // Level 2: only a small random sample of failed direct edits gets the
      // much more expensive local beam.
      if (!proposal.valid && !offline_time_over() && random_int(100) < SA_BEAM_RESCUE_PERCENT) {
        used_rescue = true;
        ++rescue_trials;
        ++stats.beam_rescue_trials;

        if (!dependency.has_value()) {
          dependency = build_trace_dependency_index(current);
        }

        deep = random_int(100) < SA_RESCUE_DEEP_PERCENT;
        if (deep)
          ++rescue_deep_trials;

        const int beam_width = deep ? SA_DEEP_BEAM_WIDTH : SA_BEAM_WIDTH;
        const int global_prebeam = deep ? SA_DEEP_GLOBAL_PREBEAM : SA_GLOBAL_PREBEAM;
        const int depth_limit = deep ? SA_DEEP_MAX_DEPTH : SA_MAX_DEPTH;

        proposal = evaluate_candidate_sa(current, candidate, *dependency, beam_width, global_prebeam, depth_limit,
                                         SA_MAX_UPHILL);

        if (proposal.valid) {
          ++stats.beam_rescue_valid;
        }
      }

      if (!proposal.valid)
        continue;
      ++valid_mutations;

      const int old_actions = current.history.size();
      const int delta = proposal.action_count - old_actions;
      const double temp = temperature_now();

      bool accept = false;
      if (delta <= 0) {
        accept = true;
      } else if (delta <= SA_MAX_UPHILL && temp > 1e-12) {
        const double probability = exp(-(double)delta / temp);
        accept = random01() < probability;
      }

      if (!accept)
        continue;

      Trace rebuilt = build_trace_from_specs(proposal.specs);
      if (!rebuilt.valid || total_slimes(rebuilt.final_board) != 0) {
        ++rebuild_rejects;
        continue;
      }

      const int actual_delta = (int)rebuilt.history.size() - old_actions;

      // The incremental/direct evaluator and the rebuilt trace should agree.
      // If not, trust the rebuilt complete trace and re-check SA acceptance.
      if (actual_delta != delta) {
        bool accept_rebuilt = false;
        if (actual_delta <= 0) {
          accept_rebuilt = true;
        } else if (actual_delta <= SA_MAX_UPHILL && temp > 1e-12) {
          const double probability = exp(-(double)actual_delta / temp);
          accept_rebuilt = random01() < probability;
        }
        if (!accept_rebuilt)
          continue;
      }

      ++accepted;
      if (actual_delta < 0) {
        ++accepted_improving;
      } else if (actual_delta == 0) {
        ++accepted_neutral;
      } else {
        ++accepted_uphill;
      }

      current = std::move(rebuilt);

      if (current.history.size() < best.history.size()) {
        best = current;
        cerr << "sa best=" << best.history.size() << " current=" << current.history.size() << " trial=" << trials
             << " delta=" << actual_delta << " temp=" << fixed << setprecision(3) << temp
             << " type=" << (int)candidate.type << " span=" << (candidate.producer - candidate.insert)
             << " uses=" << candidate.cheap_use_count << " mode=" << (used_rescue ? (deep ? "deep" : "beam") : "direct")
             << defaultfloat << '\n';
      }

      // The accepted complete history is a new SA state.  Its event universe
      // is intentionally regenerated immediately so neutral edits can expose
      // new historical events.
      if (!offline_time_over())
        refresh_proposals();
    }

    cerr << "sa final=" << best.history.size() << " current=" << current.history.size()
         << " elapsed_ms=" << elapsed_ms() << " trials=" << trials << " valid=" << valid_mutations
         << " accepted=" << accepted << " improve=" << accepted_improving << " neutral=" << accepted_neutral
         << " uphill=" << accepted_uphill << " rescue=" << rescue_trials << " rescue_deep=" << rescue_deep_trials
         << " refresh=" << refreshes << " rebuild_reject=" << rebuild_rejects << " generated_total=" << stats.generated
         << " direct_valid=" << stats.direct_valid << " direct_event_fail=" << stats.direct_event_fail
         << " direct_use_fail=" << stats.direct_use_fail << " direct_rejoin_fail=" << stats.direct_rejoin_fail
         << " direct_steps=" << (stats.direct_steps - direct_steps_before)
         << " reroutes=" << (stats.direct_reroutes - direct_reroutes_before)
         << " rescue_valid=" << stats.beam_rescue_valid << " beam_states=" << (stats.beam_states - states_before)
         << " raw=" << (stats.beam_raw_candidates - raw_before)
         << " children=" << (stats.beam_children - children_before)
         << " rejoins=" << (stats.rejoin_hits - rejoins_before) << '\n';

    return best;
  }

  vector<OutputAction> solve() const {
    Trace baseline = build_baseline_trace();
    if (!baseline.valid)
      return {};

    Trace best = baseline;
    if (total_slimes(baseline.final_board) == 0 && !offline_time_over()) {
      best = optimize_offline(std::move(baseline));
    }

    vector<OutputAction> answer;
    answer.reserve(best.history.size());
    for (const auto &entry : best.history) {
      answer.push_back(entry.out);
    }
    return answer;
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  Solver solver;
  auto answer = solver.solve();

  for (const auto &action : answer) {
    cout << action.x << ' ' << action.y << ' ' << action.k << ' ' << DIR_CHAR[action.d] << ' ' << action.l << '\n';
  }

  return 0;
}
