#include <bits/stdc++.h>

template <class T, auto op, auto e>
concept seg_concept = requires(T a, T b) {
  { op(a, b) } -> std::same_as<T>;
  { e() } -> std::same_as<T>;
};

/**
 * @brief 永続セグメント木
 * 過去のすべてのバージョンに対してアクセス・更新が可能 (fully persistent)
 * ポインタを利用したセグメント木をベースとする
 *
 * @tparam データ型 T
 * @tparam 演算 op : T \times T -> T
 * @tparam 単位元 e : x \in T, e \in T, op(x,e) = x
 */
template <class T, auto op, auto e>
  requires seg_concept<T, op, e>
class PersistentSegmentTree {
  struct Node {
    T val;
    int left_child, right_child;
    Node() : val(e()), left_child(-1), right_child(-1) {}
    Node(const T &val) : val(val), left_child(-1), right_child(-1) {}
  };

  int size;                      ///< 葉ノードの数
  std::vector<Node> all_nodes;   ///< すべてのノードを保持
  std::vector<int> version_root; ///< 各バージョンの根のインデックス

  /**
   * @brief 左右子ノードのインデックスをセットする
   */
  void set_children() {
    for (int i = 1; i < size; ++i) {
      all_nodes[i].left_child = i * 2;
      all_nodes[i].right_child = i * 2 + 1;
    }
  }

  /**
   * @brief 初期化時に，各ノードの積をセットする
   */
  void init_merge() {
    for (int i = size - 1; i >= 1; --i) {
      all_nodes[i].val = op(all_nodes[i * 2].val, all_nodes[i * 2 + 1].val);
    }
  }

  /**
   * @brief バージョンverの木におけるpos番目の葉ノードの値を取得
   */
  T get_impl(int pos, int index, int l, int r) {
    if (l + 1 == r) {
      return all_nodes[index].val;
    }
    int m = (l + r) >> 1;
    if (pos < m) {
      return get_impl(pos, all_nodes[index].left_child, l, m);
    } else {
      return get_impl(pos, all_nodes[index].right_child, m, r);
    }
  }

  /**
   * @brief
   * バージョンverの木におけるpos番目の葉ノードの値をxに更新して新たなバージョンの木を作る
   * @return 新しいノードのインデックス
   */
  int set_impl(int pos, T x, int index, int l, int r) {
    if (l + 1 == r) {
      return make_new_node(x);
    }
    int m = (l + r) >> 1;
    if (pos < m) {
      int new_left_child_index =
          set_impl(pos, x, all_nodes[index].left_child, l, m);
      return make_new_node(new_left_child_index, all_nodes[index].right_child);
    } else {
      int new_right_child_index =
          set_impl(pos, x, all_nodes[index].right_child, m, r);
      return make_new_node(all_nodes[index].left_child, new_right_child_index);
    }
  }

  /**
   * @brief 新しいノードを作成
   * @return 新しいノードのインデックス
   */
  int make_new_node(int l, int r) {
    Node node{};
    node.left_child = l;
    node.right_child = r;
    if (l != -1) {
      node.val = op(all_nodes[l].val, all_nodes[r].val);
    }
    all_nodes.emplace_back(node);
    return all_nodes.size() - 1;
  }

  /**
   * @brief 新しいノードを作成
   * @return 新しいノードのインデックス
   */
  int make_new_node(T x) {
    all_nodes.emplace_back(Node(x));
    return all_nodes.size() - 1;
  }

public:
  PersistentSegmentTree(int n_) {
    assert(n_ > 0);
    size = 1;
    while (size < n_) {
      size <<= 1;
    }
    all_nodes.assign(size * 2, Node());
    set_children();
    version_root.emplace_back(1);
  }

  PersistentSegmentTree(const std::vector<T> &vec) {
    int n_ = vec.size();
    assert(n_ > 0);
    size = 1;
    while (size < n_) {
      size <<= 1;
    }
    all_nodes.assign(size * 2, Node());
    for (int i = 0; i < n_; ++i) {
      all_nodes[i + size] = Node(vec[i]);
    }
    set_children();
    init_merge();
    version_root.emplace_back(1);
  }

  /**
   * @brief バージョンverの木におけるpos番目の葉ノードの値を取得
   */
  T get(int ver, int pos) { return get_impl(pos, version_root[ver], 0, size); }

  /**
   * @brief
   * バージョンverの木におけるpos番目の葉ノードの値をxに更新して新たなバージョンの木を作る
   * @return 新しいバージョン
   */
  int set(int ver, int pos, T x) {
    int new_root_index = set_impl(pos, x, version_root[ver], 0, size);
    version_root.emplace_back(new_root_index);
    return version_root.size() - 1;
  }
};

int op(int a, int b) { return a + b; }
int e() { return 0; }

using namespace std;

int main() {
  int N = 8;
  PersistentSegmentTree<int, op, e> seg(N);
  cout << seg.get(0, 1) << endl;
  cout << seg.set(0, 0, 1) << endl;
}