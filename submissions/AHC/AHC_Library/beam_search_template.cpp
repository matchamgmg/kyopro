/**
 * ビームサーチのテンプレート
 */

#include <bits/stdc++.h>

struct Problem {
  void input() {}
};

struct Answer {
  void print() {}
};

struct Legal_action {};

struct Beam_state {
  bool operator<(const Beam_state &rhs) const {}
  bool operator>(const Beam_state &rhs) const {}
};

struct Solver {
  Problem P;
  Solver() {
    P.input();
  }

  Legal_action get_legal_action() {}
  Legal_action get_beam_search_action() {}
  Answer beam_search() {}
};